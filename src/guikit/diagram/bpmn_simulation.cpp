#include "bpmn_internal.hpp"

namespace guikit {

namespace detail::bpmn_impl {

// ============================================================================
// Token Simulation Step Engine (`bpmn-js-token-simulation`)
// ============================================================================

void SpawnTokenFromStartEvent(BpmnDiagram& diagram, const BpmnNode& startNode, BpmnEditorState& state) {
    static constexpr keel::StaticArray<Color, 5> kTokenColors = {{
        Color(16, 185, 129, 255),
        Color(56, 189, 248, 255),
        Color(245, 158, 11, 255),
        Color(168, 85, 247, 255),
        Color(244, 63, 94, 255)
    }};
    const keel::u32 instId = state.nextInstanceId++;
    const Color col = kTokenColors[(instId - 1) % kTokenColors.size()];
    if (BpmnNode* mutStart = diagram.FindNode(startNode.id)) {
        mutStart->tokenExecutionCount++;
    }
    for (const auto& edge : diagram.edges) {
        if (edge.fromNodeId == startNode.id && edge.flowType != BpmnFlowType::Association) {
            BpmnToken tok{};
            tok.id = state.nextTokenId++;
            tok.edgeId = edge.id;
            tok.nodeId = 0;
            tok.progress = 0.0f;
            tok.color = col;
            tok.instanceId = instId;
            diagram.tokens.push_back(tok);
        }
    }
}

void StepTokenSimulation(BpmnDiagram& diagram, BpmnEditorState& state, keel::f32 dtSec, keel::f32 speedMultiplier, bool autoSpawn) {
    if (state.simPaused) return;
    const keel::f32 dt = keel::clamp(dtSec, 0.001f, 0.12f) * keel::clamp(speedMultiplier, 0.2f, 4.0f);

    if (autoSpawn && diagram.tokens.size() < 24) {
        state.spawnCooldownSec -= dt;
        if (state.spawnCooldownSec <= 0.0) {
            for (const auto& n : diagram.nodes) {
                if (n.type == BpmnNodeType::StartEvent) {
                    SpawnTokenFromStartEvent(diagram, n, state);
                    break;
                }
            }
            state.spawnCooldownSec = 2.6;
        }
    }

    for (auto& n : diagram.nodes) n.activeTokensInside = 0;

    keel::SmallArray<BpmnToken, 32> spawnedNext{};
    keel::SmallArray<keel::usize, 32> removeIndices{};

    for (keel::usize i = 0; i < diagram.tokens.size(); ++i) {
        BpmnToken& tok = diagram.tokens[i];
        if (tok.edgeId != 0) {
            tok.progress += dt * 0.95f;
            if (tok.progress >= 1.0f) {
                const BpmnEdge* edge = diagram.FindEdge(tok.edgeId);
                BpmnNode* target = edge ? diagram.FindNode(edge->toNodeId) : nullptr;
                removeIndices.push_back(i);
                if (!target) continue;

                if (target->type == BpmnNodeType::EndEvent) {
                    target->tokenExecutionCount++;
                    if (target->eventDef == BpmnEventDefinition::Terminate) {
                        const keel::u32 termInst = tok.instanceId;
                        for (keel::usize k = 0; k < diagram.tokens.size(); ++k) {
                            if (diagram.tokens[k].instanceId == termInst) {
                                removeIndices.push_back(k);
                            }
                        }
                    }
                } else if (target->type == BpmnNodeType::Task || target->type == BpmnNodeType::SubProcess ||
                           target->type == BpmnNodeType::IntermediateEvent) {
                    BpmnToken dwellTok = tok;
                    dwellTok.edgeId = 0;
                    dwellTok.nodeId = target->id;
                    dwellTok.progress = 0.0f;
                    spawnedNext.push_back(dwellTok);
                } else if (target->type == BpmnNodeType::Gateway) {
                    target->tokenExecutionCount++;
                    keel::SmallArray<const BpmnEdge*, 8> outEdges;
                    for (const auto& e : diagram.edges) {
                        if (e.fromNodeId == target->id && e.flowType != BpmnFlowType::Association) {
                            outEdges.push_back(&e);
                        }
                    }
                    if (!outEdges.empty()) {
                        if (target->gatewayType == BpmnGatewayType::Exclusive || target->gatewayType == BpmnGatewayType::EventBased) {
                            const BpmnEdge* chosen = outEdges.front();
                            for (const BpmnEdge* oe : outEdges) {
                                if (oe->id == target->activeOutgoingEdgeId) {
                                    chosen = oe;
                                    break;
                                }
                            }
                            BpmnToken nextTok = tok;
                            nextTok.id = state.nextTokenId++;
                            nextTok.edgeId = chosen->id;
                            nextTok.nodeId = 0;
                            nextTok.progress = 0.0f;
                            spawnedNext.push_back(nextTok);
                        } else {
                            for (const BpmnEdge* oe : outEdges) {
                                BpmnToken nextTok = tok;
                                nextTok.id = state.nextTokenId++;
                                nextTok.edgeId = oe->id;
                                nextTok.nodeId = 0;
                                nextTok.progress = 0.0f;
                                spawnedNext.push_back(nextTok);
                            }
                        }
                    }
                }
            }
        } else if (tok.nodeId != 0) {
            BpmnNode* host = diagram.FindNode(tok.nodeId);
            if (!host) {
                removeIndices.push_back(i);
                continue;
            }
            const keel::f32 dwellSec = keel::clamp(host->slaDurationMs / 1000.0f, 0.25f, 2.0f);
            tok.progress += dt / dwellSec;
            if (tok.progress >= 1.0f) {
                host->tokenExecutionCount++;
                removeIndices.push_back(i);
                for (const auto& e : diagram.edges) {
                    if (e.fromNodeId == host->id && e.flowType != BpmnFlowType::Association) {
                        BpmnToken nextTok = tok;
                        nextTok.id = state.nextTokenId++;
                        nextTok.edgeId = e.id;
                        nextTok.nodeId = 0;
                        nextTok.progress = 0.0f;
                        spawnedNext.push_back(nextTok);
                    }
                }
            } else {
                host->activeTokensInside++;
            }
        }
    }

    if (!removeIndices.empty()) {
        keel::sort(removeIndices.as_mut_ref());
        for (keel::usize k = removeIndices.size(); k > 0; --k) {
            const keel::usize remIdx = removeIndices[k - 1];
            if (k < removeIndices.size() && remIdx == removeIndices[k]) {
                continue;
            }
            if (remIdx < diagram.tokens.size()) {
                diagram.tokens.remove_at(remIdx);
            }
        }
    }
    for (const auto& st : spawnedNext) {
        if (diagram.tokens.size() < 48) {
            diagram.tokens.push_back(st);
            if (st.nodeId != 0) {
                if (BpmnNode* n = diagram.FindNode(st.nodeId)) {
                    n->activeTokensInside++;
                }
            }
        }
    }
}


} // namespace detail::bpmn_impl

using namespace detail::bpmn_impl;

// ============================================================================
// Standard BPMN 2.0 XML Serialization & Round-Trip Deserialization
// ============================================================================
namespace {

keel::String EscapeXmlText(keel::StringRef s) {
    keel::String out = keel::String::with_capacity(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '&':  out.append("&amp;"); break;
            case '<':  out.append("&lt;"); break;
            case '>':  out.append("&gt;"); break;
            case '"':  out.append("&quot;"); break;
            case '\'': out.append("&apos;"); break;
            case '\n': out.append("&#10;"); break;
            default:   out.push_back(c); break;
        }
    }
    return out;
}

keel::String UnescapeXmlText(keel::StringRef s) {
    keel::String out = keel::String::with_capacity(s.size());
    for (keel::usize i = 0; i < s.size(); ++i) {
        if (s[i] == '&') {
            if (s.substr(i, 5) == "&amp;")  { out.push_back('&');  i += 4; continue; }
            if (s.substr(i, 4) == "&lt;")   { out.push_back('<');  i += 3; continue; }
            if (s.substr(i, 4) == "&gt;")   { out.push_back('>');  i += 3; continue; }
            if (s.substr(i, 6) == "&quot;") { out.push_back('"');  i += 5; continue; }
            if (s.substr(i, 6) == "&apos;") { out.push_back('\''); i += 5; continue; }
            if (s.substr(i, 5) == "&#10;")  { out.push_back('\n'); i += 4; continue; }
        }
        out.push_back(s[i]);
    }
    return out;
}

keel::String ExtractXmlAttr(keel::StringRef tag, keel::StringRef attrName) {
    keel::SmallString<64> needle = keel::SmallString<64>::from(attrName);
    needle.append("=\"");
    const keel::usize pos = tag.find(needle);
    if (pos == keel::kInvalidIndex) return keel::String{};
    const keel::usize valStart = pos + needle.size();
    const keel::usize valEnd = tag.find('"', valStart);
    if (valEnd == keel::kInvalidIndex) return keel::String{};
    return UnescapeXmlText(tag.substr(valStart, valEnd - valStart));
}

keel::f32 ExtractXmlFloatAttr(keel::StringRef tag, keel::StringRef attrName, keel::f32 fallback = 0.0f) {
    const keel::String s = ExtractXmlAttr(tag, attrName);
    if (s.empty()) return fallback;
    const auto parsed = keel::parse_f32(s.as_ref().trim());
    return parsed.is_ok() ? parsed.value() : fallback;
}

keel::u32 ParseTrailingIdNumber(keel::StringRef xmlId) {
    keel::usize i = xmlId.size();
    while (i > 0 && xmlId[i - 1] >= '0' && xmlId[i - 1] <= '9') --i;
    if (i == xmlId.size()) return 0;
    const auto parsed = keel::parse_u64(xmlId.substr(i));
    return parsed.is_ok() ? keel::as<keel::u32>(parsed.value()) : 0;
}

struct PendingBoundaryAttach {
    keel::u32 nodeId{0};
    keel::String hostXmlId{};
};

} // namespace

keel::String BpmnEditor::ExportBpmnXml(const BpmnDiagram& diagram) {
    keel::String xml = keel::String::with_capacity(8192);
    xml.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    xml.append("<bpmn:definitions xmlns:bpmn=\"http://www.omg.org/spec/BPMN/20100524/MODEL\"\n");
    xml.append("                  xmlns:bpmndi=\"http://www.omg.org/spec/BPMN/20100524/DI\"\n");
    xml.append("                  xmlns:dc=\"http://www.omg.org/spec/DD/20100524/DC\"\n");
    xml.append("                  xmlns:di=\"http://www.omg.org/spec/DD/20100524/DI\"\n");
    xml.append("                  id=\"Definitions_1\" targetNamespace=\"http://bpmn.io/schema/bpmn\">\n");
    const keel::String procNameEsc = EscapeXmlText(diagram.name);
    keel::format_into(xml, "  <bpmn:process id=\"Process_1\" name=\"{}\" isExecutable=\"true\">\n", procNameEsc);

    if (!diagram.lanes.empty()) {
        xml.append("    <bpmn:laneSet id=\"LaneSet_1\">\n");
        for (const auto& l : diagram.lanes) {
            const keel::String laneNameEsc = EscapeXmlText(l.name);
            const keel::String poolNameEsc = EscapeXmlText(l.poolName);
            keel::format_into(
                xml,
                "      <bpmn:lane id=\"Lane_{}\" name=\"{}\" poolName=\"{}\" />\n",
                l.id,
                laneNameEsc,
                poolNameEsc
            );
        }
        xml.append("    </bpmn:laneSet>\n");
    }

    for (const auto& n : diagram.nodes) {
        const char* tag = "bpmn:task";
        switch (n.type) {
            case BpmnNodeType::StartEvent:        tag = "bpmn:startEvent"; break;
            case BpmnNodeType::IntermediateEvent: tag = "bpmn:intermediateCatchEvent"; break;
            case BpmnNodeType::BoundaryEvent:     tag = "bpmn:boundaryEvent"; break;
            case BpmnNodeType::EndEvent:          tag = "bpmn:endEvent"; break;
            case BpmnNodeType::Task:
                switch (n.taskType) {
                    case BpmnTaskType::None:         tag = "bpmn:task"; break;
                    case BpmnTaskType::User:         tag = "bpmn:userTask"; break;
                    case BpmnTaskType::Service:      tag = "bpmn:serviceTask"; break;
                    case BpmnTaskType::Send:         tag = "bpmn:sendTask"; break;
                    case BpmnTaskType::Receive:      tag = "bpmn:receiveTask"; break;
                    case BpmnTaskType::Script:       tag = "bpmn:scriptTask"; break;
                    case BpmnTaskType::Manual:       tag = "bpmn:manualTask"; break;
                    case BpmnTaskType::BusinessRule: tag = "bpmn:businessRuleTask"; break;
                    case BpmnTaskType::CallActivity: tag = "bpmn:callActivity"; break;
                }
                break;
            case BpmnNodeType::SubProcess:        tag = "bpmn:subProcess"; break;
            case BpmnNodeType::Gateway:
                switch (n.gatewayType) {
                    case BpmnGatewayType::Exclusive:  tag = "bpmn:exclusiveGateway"; break;
                    case BpmnGatewayType::Parallel:   tag = "bpmn:parallelGateway"; break;
                    case BpmnGatewayType::Inclusive:  tag = "bpmn:inclusiveGateway"; break;
                    case BpmnGatewayType::EventBased: tag = "bpmn:eventBasedGateway"; break;
                }
                break;
            case BpmnNodeType::DataObject:        tag = "bpmn:dataObjectReference"; break;
            case BpmnNodeType::DataStore:         tag = "bpmn:dataStoreReference"; break;
            case BpmnNodeType::TextAnnotation:    tag = "bpmn:textAnnotation"; break;
        }
        const keel::String labelEsc = EscapeXmlText(n.label);
        keel::format_into(xml, "    <{} id=\"FlowNode_{}\" name=\"{}\"", tag, n.id, labelEsc);
        if (n.type == BpmnNodeType::BoundaryEvent && n.attachedToNodeId != 0) {
            keel::format_into(
                xml,
                " attachedToRef=\"FlowNode_{}\" cancelActivity=\"{}\"",
                n.attachedToNodeId,
                n.nonInterrupting ? "false" : "true"
            );
        }
        if (n.eventDef != BpmnEventDefinition::None &&
            (n.type == BpmnNodeType::StartEvent || n.type == BpmnNodeType::IntermediateEvent ||
             n.type == BpmnNodeType::BoundaryEvent || n.type == BpmnNodeType::EndEvent)) {
            const char* evTag = "bpmn:timerEventDefinition";
            switch (n.eventDef) {
                case BpmnEventDefinition::Message:    evTag = "bpmn:messageEventDefinition"; break;
                case BpmnEventDefinition::Timer:      evTag = "bpmn:timerEventDefinition"; break;
                case BpmnEventDefinition::Error:      evTag = "bpmn:errorEventDefinition"; break;
                case BpmnEventDefinition::Signal:     evTag = "bpmn:signalEventDefinition"; break;
                case BpmnEventDefinition::Escalation: evTag = "bpmn:escalationEventDefinition"; break;
                case BpmnEventDefinition::Terminate:  evTag = "bpmn:terminateEventDefinition"; break;
                case BpmnEventDefinition::None:       break;
            }
            keel::format_into(xml, ">\n      <{} />\n    </{}>\n", evTag, tag);
        } else {
            xml.append(" />\n");
        }
    }

    for (const auto& e : diagram.edges) {
        const char* ftag = "bpmn:sequenceFlow";
        if (e.flowType == BpmnFlowType::MessageFlow) ftag = "bpmn:messageFlow";
        else if (e.flowType == BpmnFlowType::Association) ftag = "bpmn:association";
        keel::format_into(
            xml,
            "    <{} id=\"Flow_{}\" sourceRef=\"FlowNode_{}\" targetRef=\"FlowNode_{}\"",
            ftag,
            e.id,
            e.fromNodeId,
            e.toNodeId
        );
        if (e.flowType == BpmnFlowType::DefaultFlow) xml.append(" isDefault=\"true\"");
        else if (e.flowType == BpmnFlowType::ConditionalFlow) xml.append(" isConditional=\"true\"");
        if (!e.label.empty()) {
            const keel::String edgeLabelEsc = EscapeXmlText(e.label);
            keel::format_into(xml, " name=\"{}\"", edgeLabelEsc);
        }
        xml.append(" />\n");
    }

    xml.append("  </bpmn:process>\n");
    xml.append("  <bpmndi:BPMNDiagram id=\"BPMNDiagram_1\">\n");
    xml.append("    <bpmndi:BPMNPlane id=\"BPMNPlane_1\" bpmnElement=\"Process_1\">\n");

    for (const auto& l : diagram.lanes) {
        keel::format_into(
            xml,
            "      <bpmndi:BPMNShape id=\"Lane_{}_di\" bpmnElement=\"Lane_{}\" isHorizontal=\"true\">\n"
            "        <dc:Bounds x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" />\n"
            "      </bpmndi:BPMNShape>\n",
            l.id,
            l.id,
            keel::as<keel::i32>(l.rect.x),
            keel::as<keel::i32>(l.rect.y),
            keel::as<keel::i32>(l.rect.w),
            keel::as<keel::i32>(l.rect.h)
        );
    }

    for (const auto& n : diagram.nodes) {
        keel::format_into(xml, "      <bpmndi:BPMNShape id=\"FlowNode_{}_di\" bpmnElement=\"FlowNode_{}\"", n.id, n.id);
        if (n.type == BpmnNodeType::SubProcess) {
            keel::format_into(xml, " isExpanded=\"{}\"", n.isExpanded ? "true" : "false");
        }
        xml.append(">\n");
        keel::format_into(
            xml,
            "        <dc:Bounds x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" />\n",
            keel::as<keel::i32>(n.pos.x),
            keel::as<keel::i32>(n.pos.y),
            keel::as<keel::i32>(n.size.x),
            keel::as<keel::i32>(n.size.y)
        );
        if (keel::abs(n.labelOffset.x) > 0.5f || keel::abs(n.labelOffset.y) > 0.5f) {
            keel::format_into(
                xml,
                "        <bpmndi:BPMNLabel>\n"
                "          <dc:Bounds x=\"{}\" y=\"{}\" width=\"90\" height=\"20\" />\n"
                "        </bpmndi:BPMNLabel>\n",
                keel::as<keel::i32>(n.pos.x + n.labelOffset.x),
                keel::as<keel::i32>(n.pos.y + n.labelOffset.y)
            );
        }
        xml.append("      </bpmndi:BPMNShape>\n");
    }

    for (const auto& e : diagram.edges) {
        const BpmnNode* fn = diagram.FindNode(e.fromNodeId);
        const BpmnNode* tn = diagram.FindNode(e.toNodeId);
        if (!fn || !tn) continue;
        const auto wpts = BuildEdgeWorldPoints(diagram, *fn, *tn, e, BpmnRoutingMode::Orthogonal);
        keel::format_into(xml, "      <bpmndi:BPMNEdge id=\"Flow_{}_di\" bpmnElement=\"Flow_{}\">\n", e.id, e.id);
        for (const auto& pt : wpts) {
            keel::format_into(
                xml,
                "        <di:waypoint x=\"{}\" y=\"{}\" />\n",
                keel::as<keel::i32>(pt.x),
                keel::as<keel::i32>(pt.y)
            );
        }
        if (keel::abs(e.labelOffset.x) > 0.5f || keel::abs(e.labelOffset.y) > 0.5f) {
            keel::format_into(
                xml,
                "        <bpmndi:BPMNLabel>\n"
                "          <dc:Bounds x=\"{}\" y=\"{}\" width=\"80\" height=\"20\" />\n"
                "        </bpmndi:BPMNLabel>\n",
                keel::as<keel::i32>(e.labelOffset.x),
                keel::as<keel::i32>(e.labelOffset.y)
            );
        }
        xml.append("      </bpmndi:BPMNEdge>\n");
    }

    xml.append("    </bpmndi:BPMNPlane>\n");
    xml.append("  </bpmndi:BPMNDiagram>\n");
    xml.append("</bpmn:definitions>\n");
    return xml;
}

bool BpmnEditor::ImportBpmnXml(keel::StringRef xml, BpmnDiagram& outDiagram) {
    if (xml.find("<bpmn:") == keel::kInvalidIndex && xml.find("<definitions") == keel::kInvalidIndex) {
        return false;
    }

    BpmnDiagram parsed;
    parsed.Clear();
    parsed.name = "Imported BPMN 2.0 Process";

    keel::HashMap<keel::String, keel::u32> nodeIdMap;
    keel::HashMap<keel::String, keel::u32> laneIdMap;
    keel::HashMap<keel::String, keel::u32> edgeIdMap;
    keel::Array<PendingBoundaryAttach> pendingBoundaryAttach;

    keel::usize pos = 0;
    while (pos < xml.size()) {
        const keel::usize lt = xml.find('<', pos);
        if (lt == keel::kInvalidIndex) break;
        const keel::usize gt = xml.find('>', lt + 1);
        if (gt == keel::kInvalidIndex) break;

        const keel::StringRef tagContent = xml.substr(lt + 1, gt - lt - 1);
        pos = gt + 1;
        if (tagContent.empty() || tagContent[0] == '?' || tagContent[0] == '!' || tagContent[0] == '/') continue;

        keel::usize nameEnd = 0;
        while (nameEnd < tagContent.size() && tagContent[nameEnd] != ' ' && tagContent[nameEnd] != '\t' &&
               tagContent[nameEnd] != '\r' && tagContent[nameEnd] != '\n' && tagContent[nameEnd] != '/') {
            ++nameEnd;
        }
        const keel::StringRef tagName = tagContent.substr(0, nameEnd);
        const keel::usize colon = tagName.find(':');
        const keel::StringRef localName = (colon != keel::kInvalidIndex) ? tagName.substr(colon + 1) : tagName;

        if (localName == "process") {
            const keel::String procName = ExtractXmlAttr(tagContent, "name");
            if (!procName.empty()) parsed.name = procName;
        } else if (localName == "lane") {
            keel::String xmlId = ExtractXmlAttr(tagContent, "id");
            const keel::String name = ExtractXmlAttr(tagContent, "name");
            const keel::String poolNameAttr = ExtractXmlAttr(tagContent, "poolName");
            BpmnLane l{};
            l.id = ParseTrailingIdNumber(xmlId);
            if (l.id == 0) l.id = parsed.nextId++;
            else parsed.nextId = keel::max(parsed.nextId, l.id + 1);
            l.name = name.empty() ? keel::StringRef("Lane") : name.as_ref();
            l.poolName = poolNameAttr.empty() ? parsed.name.as_ref() : poolNameAttr.as_ref();
            l.rect = Rect(80.0f, 80.0f + keel::as<keel::f32>(parsed.lanes.size()) * 190.0f, 1280.0f, 190.0f);
            laneIdMap.put(keel::move(xmlId), l.id);
            parsed.lanes.push_back(keel::move(l));
        } else if (localName == "startEvent" || localName == "intermediateCatchEvent" || localName == "intermediateThrowEvent" ||
                   localName == "boundaryEvent" || localName == "endEvent" ||
                   localName == "task" || localName == "userTask" || localName == "serviceTask" ||
                   localName == "sendTask" || localName == "receiveTask" || localName == "scriptTask" ||
                   localName == "businessRuleTask" || localName == "manualTask" || localName == "callActivity" ||
                   localName == "subProcess" ||
                   localName == "exclusiveGateway" || localName == "parallelGateway" ||
                   localName == "inclusiveGateway" || localName == "eventBasedGateway" ||
                   localName == "dataObjectReference" || localName == "dataStoreReference" || localName == "textAnnotation") {
            keel::String xmlId = ExtractXmlAttr(tagContent, "id");
            if (xmlId.empty()) continue;

            BpmnNode n{};
            n.id = ParseTrailingIdNumber(xmlId);
            if (n.id == 0) n.id = parsed.nextId++;
            else parsed.nextId = keel::max(parsed.nextId, n.id + 1);
            const keel::String nodeLabel = ExtractXmlAttr(tagContent, "name");
            n.label = nodeLabel;

            if (localName == "startEvent") n.type = BpmnNodeType::StartEvent;
            else if (localName == "intermediateCatchEvent" || localName == "intermediateThrowEvent") n.type = BpmnNodeType::IntermediateEvent;
            else if (localName == "boundaryEvent") {
                n.type = BpmnNodeType::BoundaryEvent;
                n.nonInterrupting = (ExtractXmlAttr(tagContent, "cancelActivity") == "false");
                keel::String attachedRef = ExtractXmlAttr(tagContent, "attachedToRef");
                if (!attachedRef.empty()) {
                    pendingBoundaryAttach.push_back(PendingBoundaryAttach{n.id, keel::move(attachedRef)});
                }
            } else if (localName == "endEvent") n.type = BpmnNodeType::EndEvent;
            else if (localName == "subProcess") n.type = BpmnNodeType::SubProcess;
            else if (localName == "exclusiveGateway") { n.type = BpmnNodeType::Gateway; n.gatewayType = BpmnGatewayType::Exclusive; }
            else if (localName == "parallelGateway")  { n.type = BpmnNodeType::Gateway; n.gatewayType = BpmnGatewayType::Parallel; }
            else if (localName == "inclusiveGateway") { n.type = BpmnNodeType::Gateway; n.gatewayType = BpmnGatewayType::Inclusive; }
            else if (localName == "eventBasedGateway"){ n.type = BpmnNodeType::Gateway; n.gatewayType = BpmnGatewayType::EventBased; }
            else if (localName == "dataObjectReference") n.type = BpmnNodeType::DataObject;
            else if (localName == "dataStoreReference")  n.type = BpmnNodeType::DataStore;
            else if (localName == "textAnnotation")      n.type = BpmnNodeType::TextAnnotation;
            else {
                n.type = BpmnNodeType::Task;
                if (localName == "serviceTask")           n.taskType = BpmnTaskType::Service;
                else if (localName == "sendTask")         n.taskType = BpmnTaskType::Send;
                else if (localName == "receiveTask")      n.taskType = BpmnTaskType::Receive;
                else if (localName == "scriptTask")       n.taskType = BpmnTaskType::Script;
                else if (localName == "businessRuleTask") n.taskType = BpmnTaskType::BusinessRule;
                else if (localName == "manualTask")       n.taskType = BpmnTaskType::Manual;
                else if (localName == "callActivity")     n.taskType = BpmnTaskType::CallActivity;
                else                                      n.taskType = BpmnTaskType::User;
            }
            n.size = BpmnDiagram::DefaultNodeSize(n.type);
            n.pos = {140.0f + keel::as<keel::f32>(parsed.nodes.size()) * 160.0f, 180.0f};

            if (tagContent.back() != '/') {
                keel::SmallString<64> closeNeedle = keel::SmallString<64>::from("</");
                closeNeedle.append(tagName);
                closeNeedle.push_back('>');
                const keel::usize closePos = xml.find(closeNeedle, pos);
                if (closePos != keel::kInvalidIndex) {
                    const keel::StringRef body = xml.substr(pos, closePos - pos);
                    if (body.find("messageEventDefinition") != keel::kInvalidIndex)        n.eventDef = BpmnEventDefinition::Message;
                    else if (body.find("timerEventDefinition") != keel::kInvalidIndex)     n.eventDef = BpmnEventDefinition::Timer;
                    else if (body.find("errorEventDefinition") != keel::kInvalidIndex)     n.eventDef = BpmnEventDefinition::Error;
                    else if (body.find("signalEventDefinition") != keel::kInvalidIndex)    n.eventDef = BpmnEventDefinition::Signal;
                    else if (body.find("escalationEventDefinition") != keel::kInvalidIndex) n.eventDef = BpmnEventDefinition::Escalation;
                    else if (body.find("terminateEventDefinition") != keel::kInvalidIndex)  n.eventDef = BpmnEventDefinition::Terminate;
                    pos = closePos + closeNeedle.size();
                }
            }

            nodeIdMap.put(keel::move(xmlId), n.id);
            parsed.nodes.push_back(keel::move(n));
        } else if (localName == "sequenceFlow" || localName == "messageFlow" || localName == "association") {
            keel::String xmlId = ExtractXmlAttr(tagContent, "id");
            const keel::String srcRef = ExtractXmlAttr(tagContent, "sourceRef");
            const keel::String dstRef = ExtractXmlAttr(tagContent, "targetRef");
            const keel::u32* srcIdPtr = nodeIdMap.get(srcRef.as_ref());
            const keel::u32* dstIdPtr = nodeIdMap.get(dstRef.as_ref());
            if (!srcIdPtr || !dstIdPtr) continue;

            BpmnEdge e{};
            e.id = ParseTrailingIdNumber(xmlId);
            if (e.id == 0) e.id = parsed.nextId++;
            else parsed.nextId = keel::max(parsed.nextId, e.id + 1);
            e.fromNodeId = *srcIdPtr;
            e.toNodeId = *dstIdPtr;
            const keel::String edgeLabel = ExtractXmlAttr(tagContent, "name");
            e.label = edgeLabel;
            if (localName == "messageFlow") e.flowType = BpmnFlowType::MessageFlow;
            else if (localName == "association") e.flowType = BpmnFlowType::Association;
            else if (ExtractXmlAttr(tagContent, "isDefault") == "true") e.flowType = BpmnFlowType::DefaultFlow;
            else if (ExtractXmlAttr(tagContent, "isConditional") == "true") e.flowType = BpmnFlowType::ConditionalFlow;
            else e.flowType = BpmnFlowType::SequenceFlow;

            if (!xmlId.empty()) edgeIdMap.put(keel::move(xmlId), e.id);
            parsed.edges.push_back(keel::move(e));
        } else if (localName == "BPMNShape") {
            const keel::String elemRef = ExtractXmlAttr(tagContent, "bpmnElement");
            const keel::String expandedStr = ExtractXmlAttr(tagContent, "isExpanded");
            const keel::usize shapeEnd = xml.find("</bpmndi:BPMNShape>", pos);
            const keel::usize altEnd = (shapeEnd == keel::kInvalidIndex) ? xml.find("</BPMNShape>", pos) : shapeEnd;
            if (altEnd != keel::kInvalidIndex) {
                const keel::StringRef shapeBody = xml.substr(pos, altEnd - pos);
                const keel::usize bPos = shapeBody.find("Bounds");
                if (bPos != keel::kInvalidIndex) {
                    const keel::usize bEnd = shapeBody.find('>', bPos);
                    if (bEnd != keel::kInvalidIndex) {
                        const keel::StringRef bTag = shapeBody.substr(bPos, bEnd - bPos);
                        const keel::f32 x = ExtractXmlFloatAttr(bTag, "x", 0.0f);
                        const keel::f32 y = ExtractXmlFloatAttr(bTag, "y", 0.0f);
                        const keel::f32 w = ExtractXmlFloatAttr(bTag, "width", 100.0f);
                        const keel::f32 h = ExtractXmlFloatAttr(bTag, "height", 80.0f);
                        if (const keel::u32* nidPtr = nodeIdMap.get(elemRef.as_ref())) {
                            if (BpmnNode* n = parsed.FindNode(*nidPtr)) {
                                n->pos = {x, y};
                                n->size = {w, h};
                                if (!expandedStr.empty()) n->isExpanded = (expandedStr == "true");
                            }
                        } else if (const keel::u32* lidPtr = laneIdMap.get(elemRef.as_ref())) {
                            if (BpmnLane* l = parsed.FindLane(*lidPtr)) {
                                l->rect = Rect(x, y, w, h);
                            }
                        }
                    }
                }
                pos = altEnd;
            }
        } else if (localName == "BPMNEdge") {
            const keel::String elemRef = ExtractXmlAttr(tagContent, "bpmnElement");
            const keel::usize edgeEnd = xml.find("</bpmndi:BPMNEdge>", pos);
            const keel::usize altEnd = (edgeEnd == keel::kInvalidIndex) ? xml.find("</BPMNEdge>", pos) : edgeEnd;
            const keel::u32* eidPtr = edgeIdMap.get(elemRef.as_ref());
            if (altEnd != keel::kInvalidIndex && eidPtr) {
                const keel::StringRef edgeBody = xml.substr(pos, altEnd - pos);
                keel::SmallArray<Vec2, 16> allPts;
                keel::usize wpScan = 0;
                while (wpScan < edgeBody.size()) {
                    const keel::usize wPos = edgeBody.find("waypoint", wpScan);
                    if (wPos == keel::kInvalidIndex) break;
                    const keel::usize wEnd = edgeBody.find('>', wPos);
                    if (wEnd == keel::kInvalidIndex) break;
                    const keel::StringRef wTag = edgeBody.substr(wPos, wEnd - wPos);
                    allPts.push_back(Vec2{ExtractXmlFloatAttr(wTag, "x", 0.0f), ExtractXmlFloatAttr(wTag, "y", 0.0f)});
                    wpScan = wEnd + 1;
                }
                if (allPts.size() > 2) {
                    if (BpmnEdge* e = parsed.FindEdge(*eidPtr)) {
                        e->waypoints.assign(allPts.slice(1, allPts.size() - 2));
                    }
                }
                pos = altEnd;
            }
        }
    }

    for (const auto& [bid, hostXmlId] : pendingBoundaryAttach) {
        if (const keel::u32* hostIdPtr = nodeIdMap.get(hostXmlId.as_ref())) {
            BpmnNode* bn = parsed.FindNode(bid);
            const BpmnNode* host = parsed.FindNode(*hostIdPtr);
            if (bn && host) {
                bn->attachedToNodeId = host->id;
                bn->boundaryRatioX = keel::clamp((bn->Center().x - host->pos.x) / keel::max(1.0f, host->size.x), 0.15f, 0.85f);
            }
        }
    }

    for (auto& n : parsed.nodes) {
        for (const auto& l : parsed.lanes) {
            if (l.rect.Contains(n.Center())) {
                n.laneId = l.id;
                break;
            }
        }
    }
    parsed.SyncBoundaryEvents();

    if (parsed.nodes.empty() && parsed.lanes.empty()) {
        return false;
    }

    outDiagram = keel::move(parsed);
    return true;
}

} // namespace guikit
