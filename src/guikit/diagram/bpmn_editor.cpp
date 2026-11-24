#include "bpmn_internal.hpp"

namespace guikit {

namespace detail::bpmn_impl {

[[nodiscard]] BpmnPalette ResolveBpmnPalette(BpmnThemePreset preset, const UiTheme& theme) noexcept {
    BpmnThemePreset eff = preset;
    if (eff == BpmnThemePreset::Auto) {
        eff = (theme.mode == ThemeMode::Light) ? BpmnThemePreset::BpmnIoClassic : BpmnThemePreset::CamundaDark;
    }

    BpmnPalette p{};
    if (eff == BpmnThemePreset::BpmnIoClassic) {
        p.canvasBg         = Color(248, 250, 252, 255);
        p.canvasBorder     = Color(203, 213, 225, 255);
        p.gridDot          = Color(148, 163, 184, 110);
        p.gridMajor        = Color(203, 213, 225, 130);
        p.laneBgEven       = Color(255, 255, 255, 242);
        p.laneBgOdd        = Color(244, 247, 251, 242);
        p.poolHeaderBg     = Color(214, 224, 236, 255);
        p.laneHeaderBg     = Color(229, 236, 245, 255);
        p.laneBorder       = Color(148, 163, 184, 230);
        p.laneText         = Color(30, 41, 59, 255);

        p.nodeFill         = Color(255, 255, 255, 255);
        p.nodeStroke       = Color(30, 41, 59, 255);
        p.nodeShadow       = Color(15, 23, 42, 26);
        p.textPrimary      = Color(15, 23, 42, 255);
        p.textSecondary    = Color(51, 65, 85, 255);
        p.textMuted        = Color(100, 116, 139, 235);

        p.startEventFill   = Color(236, 253, 245, 255);
        p.startEventStroke = Color(5, 150, 105, 255);
        p.interEventFill   = Color(255, 251, 235, 255);
        p.interEventStroke = Color(217, 119, 6, 255);
        p.endEventFill     = Color(254, 242, 242, 255);
        p.endEventStroke   = Color(220, 38, 38, 255);
        p.gatewayFill      = Color(255, 251, 235, 255);
        p.gatewayStroke    = Color(180, 83, 9, 255);
        p.taskAccent       = Color(37, 99, 235, 255);
        p.annotationBg     = Color(254, 252, 232, 240);
        p.dataFill         = Color(241, 245, 249, 255);

        p.edgeStroke       = Color(51, 65, 85, 255);
        p.edgeMessage      = Color(37, 99, 235, 235);
        p.edgeLabelBg      = Color(255, 255, 255, 242);

        p.selectAccent     = Color(37, 99, 235, 255);
        p.selectGlow       = Color(59, 130, 246, 55);
        p.hoverAccent      = Color(14, 165, 233, 220);
        p.guideLaser       = Color(236, 72, 153, 220);

        p.hudBg            = Color(255, 255, 255, 246);
        p.hudBorder        = Color(203, 213, 225, 245);
        p.hudHover         = Color(241, 245, 249, 255);
        p.hudActiveBg      = Color(219, 234, 254, 255);
        p.simActiveGreen   = Color(16, 185, 129, 255);
    } else if (eff == BpmnThemePreset::Blueprint) {
        p.canvasBg         = Color(7, 22, 44, 255);
        p.canvasBorder     = Color(30, 78, 138, 255);
        p.gridDot          = Color(56, 189, 248, 70);
        p.gridMajor        = Color(30, 64, 115, 120);
        p.laneBgEven       = Color(10, 31, 60, 225);
        p.laneBgOdd        = Color(13, 38, 72, 225);
        p.poolHeaderBg     = Color(12, 40, 78, 255);
        p.laneHeaderBg     = Color(17, 50, 94, 250);
        p.laneBorder       = Color(56, 189, 248, 155);
        p.laneText         = Color(224, 242, 254, 255);

        p.nodeFill         = Color(12, 38, 72, 252);
        p.nodeStroke       = Color(125, 211, 252, 255);
        p.nodeShadow       = Color(2, 8, 20, 125);
        p.textPrimary      = Color(240, 249, 255, 255);
        p.textSecondary    = Color(186, 230, 253, 240);
        p.textMuted        = Color(125, 211, 252, 195);

        p.startEventFill   = Color(8, 48, 62, 255);
        p.startEventStroke = Color(52, 211, 153, 255);
        p.interEventFill   = Color(45, 38, 22, 255);
        p.interEventStroke = Color(251, 191, 36, 255);
        p.endEventFill     = Color(58, 22, 38, 255);
        p.endEventStroke   = Color(251, 113, 133, 255);
        p.gatewayFill      = Color(16, 44, 82, 255);
        p.gatewayStroke    = Color(250, 204, 21, 255);
        p.taskAccent       = Color(56, 189, 248, 255);
        p.annotationBg     = Color(14, 42, 78, 235);
        p.dataFill         = Color(15, 45, 84, 255);

        p.edgeStroke       = Color(186, 230, 253, 240);
        p.edgeMessage      = Color(56, 189, 248, 240);
        p.edgeLabelBg      = Color(9, 28, 54, 242);

        p.selectAccent     = Color(34, 211, 238, 255);
        p.selectGlow       = Color(34, 211, 238, 70);
        p.hoverAccent      = Color(125, 211, 252, 220);
        p.guideLaser       = Color(244, 114, 182, 230);

        p.hudBg            = Color(9, 28, 54, 246);
        p.hudBorder        = Color(38, 98, 168, 235);
        p.hudHover         = Color(18, 52, 96, 255);
        p.hudActiveBg      = Color(14, 116, 144, 215);
        p.simActiveGreen   = Color(16, 185, 129, 255);
    } else {
        // CamundaDark (default dark theme)
        p.canvasBg         = Color(11, 15, 25, 255);
        p.canvasBorder     = Color(38, 50, 72, 235);
        p.gridDot          = Color(71, 85, 105, 100);
        p.gridMajor        = Color(30, 41, 59, 140);
        p.laneBgEven       = Color(15, 22, 36, 230);
        p.laneBgOdd        = Color(18, 26, 43, 230);
        p.poolHeaderBg     = Color(21, 31, 50, 255);
        p.laneHeaderBg     = Color(26, 38, 60, 252);
        p.laneBorder       = Color(56, 74, 102, 220);
        p.laneText         = Color(226, 232, 240, 255);

        p.nodeFill         = Color(24, 35, 56, 255);
        p.nodeStroke       = Color(164, 180, 204, 245);
        p.nodeShadow       = Color(2, 6, 14, 135);
        p.textPrimary      = Color(248, 250, 252, 255);
        p.textSecondary    = Color(196, 209, 228, 245);
        p.textMuted        = Color(132, 150, 178, 225);

        p.startEventFill   = Color(12, 44, 36, 255);
        p.startEventStroke = Color(16, 185, 129, 255);
        p.interEventFill   = Color(48, 35, 14, 255);
        p.interEventStroke = Color(245, 158, 11, 255);
        p.endEventFill     = Color(54, 19, 30, 255);
        p.endEventStroke   = Color(244, 63, 94, 255);
        p.gatewayFill      = Color(40, 31, 18, 255);
        p.gatewayStroke    = Color(251, 191, 36, 255);
        p.taskAccent       = Color(56, 189, 248, 255);
        p.annotationBg     = Color(28, 40, 62, 235);
        p.dataFill         = Color(28, 39, 60, 255);

        p.edgeStroke       = Color(162, 178, 202, 240);
        p.edgeMessage      = Color(56, 189, 248, 240);
        p.edgeLabelBg      = Color(15, 23, 42, 244);

        p.selectAccent     = Color(34, 211, 238, 255);
        p.selectGlow       = Color(34, 211, 238, 64);
        p.hoverAccent      = Color(56, 189, 248, 215);
        p.guideLaser       = Color(244, 114, 182, 225);

        p.hudBg            = Color(17, 24, 39, 246);
        p.hudBorder        = Color(51, 65, 85, 240);
        p.hudHover         = Color(30, 41, 59, 255);
        p.hudActiveBg      = Color(14, 116, 144, 205);
        p.simActiveGreen   = Color(16, 185, 129, 255);
    }
    return p;
}


// ============================================================================
// Coordinate & Geometry Helpers
// ============================================================================

[[nodiscard]] Vec2 WorldToScreen(Vec2 w, const Rect& viewRect, Vec2 pan, keel::f32 zoom) noexcept {
    return Vec2{
        viewRect.x + pan.x + w.x * zoom,
        viewRect.y + pan.y + w.y * zoom
    };
}

[[nodiscard]] Vec2 ScreenToWorld(Vec2 s, const Rect& viewRect, Vec2 pan, keel::f32 zoom) noexcept {
    const keel::f32 z = keel::max(0.05f, zoom);
    return Vec2{
        (s.x - viewRect.x - pan.x) / z,
        (s.y - viewRect.y - pan.y) / z
    };
}

[[nodiscard]] Rect WorldToScreenRect(const Rect& wr, const Rect& viewRect, Vec2 pan, keel::f32 zoom) noexcept {
    const Vec2 tl = WorldToScreen({wr.x, wr.y}, viewRect, pan, zoom);
    return Rect(tl.x, tl.y, wr.w * zoom, wr.h * zoom);
}

[[nodiscard]] keel::f32 SnapValue(keel::f32 v, keel::f32 step) noexcept {
    if (step <= 1.0f) return v;
    return keel::round(v / step) * step;
}

[[nodiscard]] Rect ComputeDiagramWorldBounds(const BpmnDiagram& diagram) noexcept {
    bool any = false;
    keel::f32 minX = 0.0f, minY = 0.0f, maxX = 1200.0f, maxY = 700.0f;
    auto includeRect = [&](const Rect& r) {
        if (!any) {
            minX = r.x;
            minY = r.y;
            maxX = r.Right();
            maxY = r.Bottom();
            any = true;
        } else {
            minX = keel::min(minX, r.x);
            minY = keel::min(minY, r.y);
            maxX = keel::max(maxX, r.Right());
            maxY = keel::max(maxY, r.Bottom());
        }
    };
    for (const auto& l : diagram.lanes) includeRect(l.rect);
    for (const auto& n : diagram.nodes) includeRect(n.Bounds());
    if (!any) return Rect(0.0f, 0.0f, 1200.0f, 700.0f);
    return Rect(minX, minY, keel::max(200.0f, maxX - minX), keel::max(140.0f, maxY - minY));
}

void FitDiagramToViewport(const BpmnDiagram& diagram, const Rect& viewRect, keel::f32 leftInset, BpmnEditorState& state) noexcept {
    if (viewRect.w <= 80.0f || viewRect.h <= 80.0f) return;
    const Rect wb = ComputeDiagramWorldBounds(diagram);
    const keel::f32 padLeft = leftInset + 24.0f;
    const keel::f32 padRight = 24.0f;
    const keel::f32 padTop = 54.0f;
    const keel::f32 padBottom = 34.0f;
    const keel::f32 availW = keel::max(120.0f, viewRect.w - padLeft - padRight);
    const keel::f32 availH = keel::max(120.0f, viewRect.h - padTop - padBottom);
    const keel::f32 zx = availW / keel::max(100.0f, wb.w);
    const keel::f32 zy = availH / keel::max(100.0f, wb.h);
    state.zoom = keel::clamp(keel::min(zx, zy), 0.35f, 1.25f);
    state.pan.x = padLeft + (availW - wb.w * state.zoom) * 0.5f - wb.x * state.zoom;
    state.pan.y = padTop + (availH - wb.h * state.zoom) * 0.5f - wb.y * state.zoom;
}

[[nodiscard]] Vec2 GetNodePortWorldPos(const BpmnNode& node, BpmnPortSide side) noexcept {
    const Rect b = node.Bounds();
    switch (side) {
        case BpmnPortSide::Left:   return {b.x, b.y + b.h * 0.5f};
        case BpmnPortSide::Right:  return {b.Right(), b.y + b.h * 0.5f};
        case BpmnPortSide::Top:    return {b.x + b.w * 0.5f, b.y};
        case BpmnPortSide::Bottom: return {b.x + b.w * 0.5f, b.Bottom()};
        case BpmnPortSide::Auto:   break;
    }
    return b.Center();
}

[[nodiscard]] Vec2 PortOutwardNormal(BpmnPortSide side) noexcept {
    switch (side) {
        case BpmnPortSide::Left:   return {-1.0f, 0.0f};
        case BpmnPortSide::Right:  return {1.0f, 0.0f};
        case BpmnPortSide::Top:    return {0.0f, -1.0f};
        case BpmnPortSide::Bottom: return {0.0f, 1.0f};
        default:                   return {1.0f, 0.0f};
    }
}

void ResolveAutoPorts(
    const BpmnNode& fromNode,
    const BpmnNode& toNode,
    BpmnPortSide reqFrom,
    BpmnPortSide reqTo,
    BpmnPortSide& outFrom,
    BpmnPortSide& outTo
) noexcept {
    outFrom = reqFrom;
    outTo = reqTo;
    const Vec2 fc = fromNode.Center();
    const Vec2 tc = toNode.Center();
    const keel::f32 dx = tc.x - fc.x;
    const keel::f32 dy = tc.y - fc.y;

    if (fromNode.type == BpmnNodeType::BoundaryEvent && outFrom == BpmnPortSide::Auto) {
        outFrom = BpmnPortSide::Bottom;
    }

    if (outFrom == BpmnPortSide::Auto && outTo == BpmnPortSide::Auto) {
        // Special gateway fork/join ergonomics across lanes
        if (fromNode.type == BpmnNodeType::Gateway && keel::abs(dy) > 58.0f && dx > 16.0f) {
            outFrom = (dy > 0.0f) ? BpmnPortSide::Bottom : BpmnPortSide::Top;
            outTo = BpmnPortSide::Left;
            return;
        }
        if (toNode.type == BpmnNodeType::Gateway && keel::abs(dy) > 58.0f && dx > 16.0f) {
            outFrom = BpmnPortSide::Right;
            outTo = (dy > 0.0f) ? BpmnPortSide::Top : BpmnPortSide::Bottom;
            return;
        }
    }

    if (outFrom == BpmnPortSide::Auto) {
        if (keel::abs(dx) >= keel::abs(dy) * 0.58f) {
            outFrom = (dx >= 0.0f) ? BpmnPortSide::Right : BpmnPortSide::Left;
        } else {
            outFrom = (dy >= 0.0f) ? BpmnPortSide::Bottom : BpmnPortSide::Top;
        }
    }
    if (outTo == BpmnPortSide::Auto) {
        if (fromNode.type == BpmnNodeType::BoundaryEvent) {
            if (dx > 50.0f) {
                outTo = BpmnPortSide::Left;
            } else if (dy < -30.0f) {
                outTo = BpmnPortSide::Bottom;
            } else {
                outTo = BpmnPortSide::Top;
            }
        } else if (keel::abs(dx) >= keel::abs(dy) * 0.58f) {
            outTo = (dx >= 0.0f) ? BpmnPortSide::Left : BpmnPortSide::Right;
        } else {
            outTo = (dy >= 0.0f) ? BpmnPortSide::Top : BpmnPortSide::Bottom;
        }
    }
}

void SimplifyOrthogonalPoints(keel::SmallArray<Vec2, 32>& pts) {
    if (pts.size() < 3) return;
    keel::SmallArray<Vec2, 32> out;
    out.reserve(pts.size());
    out.push_back(pts.front());
    for (keel::usize i = 1; i < pts.size(); ++i) {
        if (keel::hypot(pts[i].x - out.back().x, pts[i].y - out.back().y) > 0.8f) {
            out.push_back(pts[i]);
        }
    }
    if (out.size() < 3) {
        pts = keel::move(out);
        return;
    }
    keel::SmallArray<Vec2, 32> clean;
    clean.reserve(out.size());
    clean.push_back(out.front());
    for (keel::usize i = 1; i + 1 < out.size(); ++i) {
        const Vec2 a = clean.back();
        const Vec2 b = out[i];
        const Vec2 c = out[i + 1];
        const bool sameX = (keel::abs(a.x - b.x) <= 0.8f && keel::abs(b.x - c.x) <= 0.8f);
        const bool sameY = (keel::abs(a.y - b.y) <= 0.8f && keel::abs(b.y - c.y) <= 0.8f);
        if (!sameX && !sameY) {
            clean.push_back(b);
        }
    }
    clean.push_back(out.back());
    pts = keel::move(clean);
}

[[nodiscard]] keel::SmallArray<Vec2, 32> BuildEdgeWorldPoints(
    const BpmnDiagram& diagram,
    const BpmnNode& fromNode,
    const BpmnNode& toNode,
    const BpmnEdge& edge,
    BpmnRoutingMode routingMode
) {
    BpmnPortSide fromSide = BpmnPortSide::Right;
    BpmnPortSide toSide = BpmnPortSide::Left;
    ResolveAutoPorts(fromNode, toNode, edge.fromPort, edge.toPort, fromSide, toSide);

    const Vec2 p0 = GetNodePortWorldPos(fromNode, fromSide);
    const Vec2 p1 = GetNodePortWorldPos(toNode, toSide);

    keel::SmallArray<Vec2, 32> pts;
    pts.push_back(p0);

    if (!edge.waypoints.empty()) {
        keel::SmallArray<Vec2, 32> customWps(edge.waypoints.as_ref());
        if (routingMode == BpmnRoutingMode::Orthogonal) {
            const bool fromH = (fromSide == BpmnPortSide::Left || fromSide == BpmnPortSide::Right);
            const bool toH = (toSide == BpmnPortSide::Left || toSide == BpmnPortSide::Right);
            if (customWps.size() == 1) {
                if (fromH && !toH) customWps[0] = {p1.x, p0.y};
                else if (!fromH && toH) customWps[0] = {p0.x, p1.y};
            } else if (customWps.size() >= 2) {
                if (fromH) customWps.front().y = p0.y;
                else       customWps.front().x = p0.x;
                if (toH)   customWps.back().y = p1.y;
                else       customWps.back().x = p1.x;
            }
        }
        for (const Vec2& wp : customWps) {
            if (keel::hypot(wp.x - pts.back().x, wp.y - pts.back().y) > 0.5f) {
                pts.push_back(wp);
            }
        }
        if (keel::hypot(p1.x - pts.back().x, p1.y - pts.back().y) > 0.5f || pts.size() == 1) {
            pts.push_back(p1);
        }
        return pts;
    }

    // In bpmn-js, Associations (DataObject / DataStore / TextAnnotation) can use direct or 1-bend dotted lines
    if (edge.flowType == BpmnFlowType::Association) {
        if (keel::abs(p0.x - p1.x) > 8.0f && keel::abs(p0.y - p1.y) > 8.0f && routingMode == BpmnRoutingMode::Orthogonal) {
            const bool fromHoriz = (fromSide == BpmnPortSide::Left || fromSide == BpmnPortSide::Right);
            pts.push_back(fromHoriz ? Vec2{p1.x, p0.y} : Vec2{p0.x, p1.y});
        }
        pts.push_back(p1);
        return pts;
    }

    const Vec2 d0 = PortOutwardNormal(fromSide);
    const Vec2 d1 = PortOutwardNormal(toSide);

    if (routingMode == BpmnRoutingMode::Bezier) {
        const keel::f32 dist = keel::hypot(p1.x - p0.x, p1.y - p0.y);
        const keel::f32 tangent = keel::clamp(dist * 0.42f, 28.0f, 150.0f);
        const Vec2 c0{p0.x + d0.x * tangent, p0.y + d0.y * tangent};
        const Vec2 c1{p1.x + d1.x * tangent, p1.y + d1.y * tangent};
        constexpr keel::i32 kSteps = 18;
        for (keel::i32 i = 1; i < kSteps; ++i) {
            const keel::f32 t = keel::as<keel::f32>(i) / keel::as<keel::f32>(kSteps);
            const keel::f32 u = 1.0f - t;
            const keel::f32 b0 = u * u * u;
            const keel::f32 b1 = 3.0f * u * u * t;
            const keel::f32 b2 = 3.0f * u * t * t;
            const keel::f32 b3 = t * t * t;
            pts.push_back({
                b0 * p0.x + b1 * c0.x + b2 * c1.x + b3 * p1.x,
                b0 * p0.y + b1 * c0.y + b2 * c1.y + b3 * p1.y
            });
        }
        pts.push_back(p1);
        return pts;
    }

    // Orthogonal Manhattan Router with Port Lead-Out Stubs & Full Obstacle Clearance (`bpmn-js` ManhattanLayout)
    Rect srcBox = fromNode.Bounds();
    if (fromNode.type == BpmnNodeType::BoundaryEvent && fromNode.attachedToNodeId != 0) {
        if (const BpmnNode* host = diagram.FindNode(fromNode.attachedToNodeId)) {
            srcBox = srcBox.Union(host->Bounds());
        }
    }
    constexpr keel::f32 kBoxPad = 10.0f;
    srcBox = Rect(srcBox.x - kBoxPad, srcBox.y - kBoxPad, srcBox.w + kBoxPad * 2.0f, srcBox.h + kBoxPad * 2.0f);
    const Rect dstBox(toNode.pos.x - kBoxPad, toNode.pos.y - kBoxPad, toNode.size.x + kBoxPad * 2.0f, toNode.size.y + kBoxPad * 2.0f);

    constexpr keel::f32 kStub = 20.0f;
    Vec2 s0{p0.x + d0.x * kStub, p0.y + d0.y * kStub};
    if (fromSide == BpmnPortSide::Left)   s0.x = keel::min(s0.x, srcBox.x - 4.0f);
    if (fromSide == BpmnPortSide::Right)  s0.x = keel::max(s0.x, srcBox.Right() + 4.0f);
    if (fromSide == BpmnPortSide::Top)    s0.y = keel::min(s0.y, srcBox.y - 4.0f);
    if (fromSide == BpmnPortSide::Bottom) s0.y = keel::max(s0.y, srcBox.Bottom() + 4.0f);

    Vec2 s1{p1.x + d1.x * kStub, p1.y + d1.y * kStub};
    if (toSide == BpmnPortSide::Left)   s1.x = keel::min(s1.x, dstBox.x - 4.0f);
    if (toSide == BpmnPortSide::Right)  s1.x = keel::max(s1.x, dstBox.Right() + 4.0f);
    if (toSide == BpmnPortSide::Top)    s1.y = keel::min(s1.y, dstBox.y - 4.0f);
    if (toSide == BpmnPortSide::Bottom) s1.y = keel::max(s1.y, dstBox.Bottom() + 4.0f);

    auto segHitsRect = [](Vec2 a, Vec2 b, const Rect& r) noexcept -> bool {
        const keel::f32 minX = keel::min(a.x, b.x);
        const keel::f32 maxX = keel::max(a.x, b.x);
        const keel::f32 minY = keel::min(a.y, b.y);
        const keel::f32 maxY = keel::max(a.y, b.y);
        return (maxX > r.x + 0.5f && minX < r.Right() - 0.5f &&
                maxY > r.y + 0.5f && minY < r.Bottom() - 0.5f);
    };

    auto isPathClear = [&](keel::ArrayRef<Vec2> chain, bool checkOtherNodes = true) -> bool {
        Vec2 prev{};
        bool first = true;
        for (Vec2 pt : chain) {
            if (!first) {
                if (segHitsRect(prev, pt, srcBox) || segHitsRect(prev, pt, dstBox)) {
                    return false;
                }
                if (checkOtherNodes) {
                    for (const auto& other : diagram.nodes) {
                        if (other.id == fromNode.id || other.id == toNode.id ||
                            other.id == fromNode.attachedToNodeId || other.type == BpmnNodeType::BoundaryEvent) {
                            continue;
                        }
                        const Rect ob(other.pos.x - 6.0f, other.pos.y - 6.0f, other.size.x + 12.0f, other.size.y + 12.0f);
                        if (segHitsRect(prev, pt, ob)) {
                            return false;
                        }
                    }
                }
            }
            prev = pt;
            first = false;
        }
        return true;
    };

    auto adjustMidYClearOfLanes = [&](keel::f32 yVal) -> keel::f32 {
        for (const auto& lane : diagram.lanes) {
            if (keel::abs(yVal - lane.rect.y) < 16.0f) return lane.rect.y - 20.0f;
            if (keel::abs(yVal - lane.rect.Bottom()) < 16.0f) return lane.rect.Bottom() - 20.0f;
        }
        return yVal;
    };

    const bool fromHoriz = (fromSide == BpmnPortSide::Left || fromSide == BpmnPortSide::Right);
    const bool toHoriz = (toSide == BpmnPortSide::Left || toSide == BpmnPortSide::Right);

    if (fromHoriz && toHoriz) {
        const bool facing =
            (fromSide == BpmnPortSide::Right && toSide == BpmnPortSide::Left && s0.x <= s1.x + 4.0f) ||
            (fromSide == BpmnPortSide::Left && toSide == BpmnPortSide::Right && s0.x >= s1.x - 4.0f);

        if (facing && keel::abs(p0.y - p1.y) <= 1.5f && isPathClear({s0, s1})) {
            // Direct horizontal line
        } else {
            bool routed = false;
            if (facing) {
                const keel::StaticArray<keel::f32, 5> candMidX = {
                    (s0.x + s1.x) * 0.5f,
                    s0.x,
                    s1.x,
                    srcBox.Right() + 14.0f,
                    dstBox.x - 14.0f
                };
                for (keel::f32 mx : candMidX) {
                    if ((mx - p0.x) * d0.x >= 10.0f && (mx - p1.x) * d1.x >= 10.0f &&
                        isPathClear({s0, {mx, p0.y}, {mx, p1.y}, s1})) {
                        pts.push_back({mx, p0.y});
                        pts.push_back({mx, p1.y});
                        routed = true;
                        break;
                    }
                }
            }
            if (!routed) {
                const keel::f32 detourY = (p1.y >= p0.y)
                    ? keel::max(srcBox.Bottom(), dstBox.Bottom()) + 16.0f
                    : keel::min(srcBox.y, dstBox.y) - 16.0f;
                pts.push_back(s0);
                pts.push_back({s0.x, detourY});
                pts.push_back({s1.x, detourY});
                pts.push_back(s1);
            }
        }
    } else if (!fromHoriz && !toHoriz) {
        const bool facing =
            (fromSide == BpmnPortSide::Bottom && toSide == BpmnPortSide::Top && s0.y <= s1.y + 4.0f) ||
            (fromSide == BpmnPortSide::Top && toSide == BpmnPortSide::Bottom && s0.y >= s1.y - 4.0f);

        if (facing && keel::abs(p0.x - p1.x) <= 1.5f && isPathClear({s0, s1})) {
            // Direct vertical line
        } else {
            bool routed = false;
            if (facing) {
                const keel::f32 midY = adjustMidYClearOfLanes((s0.y + s1.y) * 0.5f);
                if (isPathClear({s0, {p0.x, midY}, {p1.x, midY}, s1})) {
                    pts.push_back({p0.x, midY});
                    pts.push_back({p1.x, midY});
                    routed = true;
                }
            }
            if (!routed) {
                pts.push_back(s0);
                const keel::StaticArray<keel::f32, 6> candX = {
                    s1.x,
                    s0.x,
                    (srcBox.Right() + dstBox.x) * 0.5f,
                    srcBox.Right() + 16.0f,
                    keel::max(srcBox.Right(), dstBox.Right()) + 16.0f,
                    keel::min(srcBox.x, dstBox.x) - 16.0f
                };
                for (keel::i32 pass = 0; pass < 2 && !routed; ++pass) {
                    for (keel::f32 dx : candX) {
                        if (isPathClear({s0, {dx, s0.y}, {dx, s1.y}, s1}, pass == 0)) {
                            pts.push_back({dx, s0.y});
                            pts.push_back({dx, s1.y});
                            pts.push_back(s1);
                            routed = true;
                            break;
                        }
                    }
                }
                if (!routed) {
                    const keel::f32 detourX = keel::max(srcBox.Right(), dstBox.Right()) + 18.0f;
                    pts.push_back({detourX, s0.y});
                    pts.push_back({detourX, s1.y});
                    pts.push_back(s1);
                }
            }
        }
    } else if (!fromHoriz && toHoriz) {
        // Vertical (Top/Bottom) -> Horizontal (Left/Right)
        const Vec2 corner{p0.x, p1.y};
        const bool forwardFrom = ((corner.y - p0.y) * d0.y >= 12.0f);
        const bool forwardTo = ((corner.x - p1.x) * d1.x >= 12.0f);

        if (forwardFrom && forwardTo && isPathClear({s0, corner, s1})) {
            pts.push_back(corner);
        } else {
            pts.push_back(s0);
            bool routed = false;
            const keel::StaticArray<keel::f32, 6> candMidX = {
                (srcBox.Right() + dstBox.x) * 0.5f,
                srcBox.Right() + 14.0f,
                s1.x,
                srcBox.x - 14.0f,
                keel::max(srcBox.Right(), dstBox.Right()) + 18.0f,
                keel::min(srcBox.x, dstBox.x) - 18.0f
            };
            for (keel::i32 pass = 0; pass < 2 && !routed; ++pass) {
                for (keel::f32 mx : candMidX) {
                    if ((mx - p1.x) * d1.x >= 10.0f &&
                        isPathClear({s0, {mx, s0.y}, {mx, p1.y}, s1}, pass == 0)) {
                        pts.push_back({mx, s0.y});
                        pts.push_back({mx, p1.y});
                        routed = true;
                        break;
                    }
                }
            }
            if (!routed) {
                const keel::f32 midY = adjustMidYClearOfLanes((s0.y + p1.y) * 0.5f);
                pts.push_back({s0.x, midY});
                pts.push_back({s1.x, midY});
                pts.push_back(s1);
            }
        }
    } else {
        // Horizontal (Left/Right) -> Vertical (Top/Bottom)
        const Vec2 corner{p1.x, p0.y};
        const bool forwardFrom = ((corner.x - p0.x) * d0.x >= 12.0f);
        const bool forwardTo = ((corner.y - p1.y) * d1.y >= 12.0f);

        if (forwardFrom && forwardTo && isPathClear({s0, corner, s1})) {
            pts.push_back(corner);
        } else {
            pts.push_back(s0);
            bool routed = false;
            const keel::StaticArray<keel::f32, 5> candMidY = {
                adjustMidYClearOfLanes((s0.y + s1.y) * 0.5f),
                s1.y,
                srcBox.Bottom() + 14.0f,
                srcBox.y - 14.0f,
                keel::max(srcBox.Bottom(), dstBox.Bottom()) + 18.0f
            };
            for (keel::i32 pass = 0; pass < 2 && !routed; ++pass) {
                for (keel::f32 my : candMidY) {
                    if ((my - p1.y) * d1.y >= 10.0f &&
                        isPathClear({s0, {s0.x, my}, {p1.x, my}, s1}, pass == 0)) {
                        pts.push_back({s0.x, my});
                        pts.push_back({p1.x, my});
                        routed = true;
                        break;
                    }
                }
            }
            if (!routed) {
                const keel::f32 midX = (s0.x + s1.x) * 0.5f;
                pts.push_back({midX, s0.y});
                pts.push_back({midX, s1.y});
                pts.push_back(s1);
            }
        }
    }

    pts.push_back(p1);
    SimplifyOrthogonalPoints(pts);
    return pts;
}

[[nodiscard]] keel::SmallArray<Vec2, 64> BuildRoundedPolyline(keel::ArrayRef<Vec2> pts, keel::f32 cornerRadius) {
    if (pts.size() < 3 || cornerRadius <= 1.0f) return keel::SmallArray<Vec2, 64>(pts);
    keel::SmallArray<Vec2, 64> out;
    out.reserve(pts.size() * 4);
    out.push_back(pts.front());

    for (keel::usize i = 1; i + 1 < pts.size(); ++i) {
        const Vec2 prev = pts[i - 1];
        const Vec2 curr = pts[i];
        const Vec2 next = pts[i + 1];

        const keel::f32 dx1 = curr.x - prev.x;
        const keel::f32 dy1 = curr.y - prev.y;
        const keel::f32 len1 = keel::hypot(dx1, dy1);

        const keel::f32 dx2 = next.x - curr.x;
        const keel::f32 dy2 = next.y - curr.y;
        const keel::f32 len2 = keel::hypot(dx2, dy2);

        if (len1 < 3.0f || len2 < 3.0f) {
            out.push_back(curr);
            continue;
        }

        const keel::f32 r = keel::min(cornerRadius, keel::min(len1 * 0.42f, len2 * 0.42f));
        const Vec2 u1{dx1 / len1, dy1 / len1};
        const Vec2 u2{dx2 / len2, dy2 / len2};

        const Vec2 pEnter{curr.x - u1.x * r, curr.y - u1.y * r};
        const Vec2 pExit{curr.x + u2.x * r, curr.y + u2.y * r};

        out.push_back(pEnter);
        constexpr keel::i32 kArcSteps = 4;
        for (keel::i32 s = 1; s < kArcSteps; ++s) {
            const keel::f32 t = keel::as<keel::f32>(s) / keel::as<keel::f32>(kArcSteps);
            const keel::f32 omt = 1.0f - t;
            out.push_back({
                omt * omt * pEnter.x + 2.0f * omt * t * curr.x + t * t * pExit.x,
                omt * omt * pEnter.y + 2.0f * omt * t * curr.y + t * t * pExit.y
            });
        }
        out.push_back(pExit);
    }

    out.push_back(pts.back());
    return out;
}

[[nodiscard]] keel::f32 PointToSegmentDistanceSq(Vec2 p, Vec2 a, Vec2 b) noexcept {
    const keel::f32 vx = b.x - a.x;
    const keel::f32 vy = b.y - a.y;
    const keel::f32 lenSq = vx * vx + vy * vy;
    if (lenSq <= 1e-4f) {
        const keel::f32 dx = p.x - a.x;
        const keel::f32 dy = p.y - a.y;
        return dx * dx + dy * dy;
    }
    const keel::f32 t = keel::clamp(((p.x - a.x) * vx + (p.y - a.y) * vy) / lenSq, 0.0f, 1.0f);
    const keel::f32 projX = a.x + t * vx;
    const keel::f32 projY = a.y + t * vy;
    const keel::f32 dx = p.x - projX;
    const keel::f32 dy = p.y - projY;
    return dx * dx + dy * dy;
}

[[nodiscard]] Vec2 SamplePolylineAtProgress(
    keel::ArrayRef<Vec2> pts,
    keel::f32 progress,
    Vec2* outMidpoint,
    bool* outHorizontalSeg
) noexcept {
    if (pts.empty()) return {0.0f, 0.0f};
    if (pts.size() == 1) {
        if (outMidpoint) *outMidpoint = pts[0];
        if (outHorizontalSeg) *outHorizontalSeg = true;
        return pts[0];
    }
    keel::f32 totalLen = 0.0f;
    keel::usize longestIdx = 0;
    keel::f32 longestSeg = -1.0f;
    for (keel::usize i = 0; i + 1 < pts.size(); ++i) {
        const keel::f32 seg = keel::hypot(pts[i + 1].x - pts[i].x, pts[i + 1].y - pts[i].y);
        totalLen += seg;
        if (seg > longestSeg) {
            longestSeg = seg;
            longestIdx = i;
        }
    }
    if (totalLen <= 1e-4f) {
        if (outMidpoint) *outMidpoint = pts[0];
        if (outHorizontalSeg) *outHorizontalSeg = true;
        return pts[0];
    }

    if (outMidpoint) {
        *outMidpoint = {
            (pts[longestIdx].x + pts[longestIdx + 1].x) * 0.5f,
            (pts[longestIdx].y + pts[longestIdx + 1].y) * 0.5f
        };
    }
    if (outHorizontalSeg) {
        *outHorizontalSeg = keel::abs(pts[longestIdx + 1].x - pts[longestIdx].x) >=
                            keel::abs(pts[longestIdx + 1].y - pts[longestIdx].y);
    }

    const keel::f32 targetDist = keel::clamp(progress, 0.0f, 1.0f) * totalLen;
    keel::f32 accum = 0.0f;
    for (keel::usize i = 0; i + 1 < pts.size(); ++i) {
        const keel::f32 seg = keel::hypot(pts[i + 1].x - pts[i].x, pts[i + 1].y - pts[i].y);
        if (accum + seg >= targetDist) {
            const keel::f32 localT = (seg > 1e-4f) ? (targetDist - accum) / seg : 0.0f;
            return {
                pts[i].x + (pts[i + 1].x - pts[i].x) * localT,
                pts[i].y + (pts[i + 1].y - pts[i].y) * localT
            };
        }
        accum += seg;
    }
    return pts.back();
}


} // namespace detail::bpmn_impl

using namespace detail::bpmn_impl;

Vec2 BpmnEditor::Measure(
    const BpmnEditorOptions& opts,
    UiSize /*size*/,
    ICanvas* /*canvas*/,
    const UiTheme& /*theme*/,
    keel::f32 availableWidth
) {
    keel::f32 defaultW = 880.0f;
    if (opts.width.IsFixed()) {
        defaultW = opts.width.Dp();
    } else if (opts.width.ResolveMode(SizeMode::Stretch) == SizeMode::Stretch && availableWidth > 0.0f) {
        defaultW = availableWidth;
    }
    const keel::f32 defaultH = opts.height.IsFixed()
        ? opts.height.Dp()
        : 520.0f;
    return {defaultW, defaultH};
}

BpmnEditorResponse BpmnEditor::Run(
    Context& ctx,
    BpmnDiagram& diagram,
    const BpmnEditorOptions& opts,
    keel::SourceLocation loc
) {

    const Id id = ctx.MakeChildId(loc);
    const bool isDisabled = opts.disabled || ctx.IsDisabled();
    const UiSize effectiveSize = opts.size;
    constexpr keel::f32 s = 1.0f;
    ICanvas* canvas = ctx.GetCanvas();

    diagram.SyncBoundaryEvents();

    Response baseRes = ctx.Interact(id);
    ctx.RegisterFocusable(id, isDisabled);

    BpmnEditorResponse resp{};
    static_cast<Response&>(resp) = baseRes;

    auto& state = ctx.GetOrCreatePersistentState<BpmnEditorState>(id);

    BpmnEditorOptions effOpts = opts;
    if (opts.tokenSimulationPtr) effOpts.tokenSimulation = *opts.tokenSimulationPtr;
    if (opts.routingModePtr) effOpts.routingMode = *opts.routingModePtr;
    if (opts.selectedNodeIdPtr && *opts.selectedNodeIdPtr != state.primaryNodeId) {
        if (diagram.FindNode(*opts.selectedNodeIdPtr)) {
            state.SelectSingleNode(*opts.selectedNodeIdPtr);
        }
    }

    Rect prevBounds = baseRes.rect;
    if (prevBounds.w <= 1.0f || prevBounds.h <= 1.0f) {
        prevBounds = ctx.GetParentPrevFrameBounds();
    }
    const Rect viewRect = prevBounds;
    resp.canvasRect = viewRect;

    const keel::u32 firstNodeId = diagram.nodes.empty() ? 0 : diagram.nodes.front().id;
    if ((!state.initialized || state.lastDiagramNodeCount != diagram.nodes.size() || state.lastFirstNodeId != firstNodeId) &&
        viewRect.w > 100.0f && viewRect.h > 100.0f) {
        if (!state.initialized || state.lastFirstNodeId != firstNodeId) {
            FitDiagramToViewport(diagram, viewRect, effOpts.showLeftPalette ? 54.0f * s : 0.0f, state);
            state.initialized = true;
        }
        state.lastDiagramNodeCount = diagram.nodes.size();
        state.lastFirstNodeId = firstNodeId;
    }

    for (auto& n : diagram.nodes) {
        if (n.type == BpmnNodeType::Gateway && n.gatewayType == BpmnGatewayType::Exclusive) {
            bool valid = false;
            keel::u32 firstOut = 0;
            for (const auto& e : diagram.edges) {
                if (e.fromNodeId == n.id && e.flowType != BpmnFlowType::Association) {
                    if (firstOut == 0) firstOut = e.id;
                    if (e.id == n.activeOutgoingEdgeId) valid = true;
                }
            }
            if (!valid) n.activeOutgoingEdgeId = firstOut;
        }
    }

    const keel::f64 nowSec = ctx.GetCurrentTime();
    if (effOpts.tokenSimulation) {
        const keel::f32 dtSec = (state.lastSimTickSec > 0.0)
            ? keel::as<keel::f32>(keel::clamp(nowSec - state.lastSimTickSec, 0.004, 0.08))
            : 0.016f;
        state.lastSimTickSec = nowSec;
        StepTokenSimulation(diagram, state, dtSec, effOpts.simulationSpeed, effOpts.autoSpawnTokens);
        ctx.RequestRepaint(2);
    } else {
        state.lastSimTickSec = nowSec;
        if (!diagram.tokens.empty()) {
            diagram.ResetSimulationStats();
        }
    }

    keel::SmallString<256> activeEditStr{};
    keel::usize activeEditCursor = 0;
    const Id editId = Id(id.value ^ 0x9E3779B97F4A7C15ULL);
    if (state.editingNodeId != 0 || state.editingEdgeId != 0 || state.editingLaneId != 0) {
        keel::StringRef initialText{};
        if (const BpmnNode* en = diagram.FindNode(state.editingNodeId)) initialText = en->label;
        else if (const BpmnEdge* ee = diagram.FindEdge(state.editingEdgeId)) initialText = ee->label;
        else if (const BpmnLane* el = diagram.FindLane(state.editingLaneId)) initialText = el->name;

        bool textChanged = false;
        bool textSubmitted = false;
        const keel::StringRef bufView = ctx.GetOrStartTextEdit(editId, initialText, activeEditCursor, textChanged, textSubmitted, false);
        activeEditStr = keel::SmallString<256>::from(bufView);
        if (textChanged) {
            if (BpmnNode* en = diagram.FindNode(state.editingNodeId)) en->label = bufView;
            else if (BpmnEdge* ee = diagram.FindEdge(state.editingEdgeId)) ee->label = bufView;
            else if (BpmnLane* el = diagram.FindLane(state.editingLaneId)) el->name = bufView;
            resp.changed = true;
            resp.diagramModified = true;
        }
        if (textSubmitted || ctx.ConsumeKey(Key::Escape) || !ctx.HasActiveTextEdit(editId)) {
            state.editingNodeId = 0;
            state.editingEdgeId = 0;
            state.editingLaneId = 0;
            ctx.ClearFocus();
        }
    }

    if (viewRect.w > 1.0f && viewRect.h > 1.0f && !isDisabled) {
        const Vec2 mouse = ctx.GetMousePos();
        const bool hovered = ctx.IsRectHovered(id, viewRect);
        resp.hovered = hovered;
        resp.mouseWorldPos = ScreenToWorld(mouse, viewRect, state.pan, state.zoom);

        const bool lmbClicked = hovered && ctx.IsMouseClicked();
        const bool lmbDown = ctx.IsMouseDown();
        const bool lmbReleased = ctx.IsMouseReleased();
        const bool doubleClicked = hovered && baseRes.doubleClicked;
        const KeyModifiers mods = ctx.GetKeyModifiers();

        bool hudConsumedClick = false;

        // A. Top Bar interactions
        if (effOpts.showTopBar && lmbClicked) {
            const Rect topBar = ComputeTopBarRect(viewRect, s);
            if (topBar.Contains(mouse)) {
                hudConsumedClick = true;
                keel::f32 rx = topBar.Right() - 6.0f * s;
                auto hitTopPill = [&](keel::f32 w) -> bool {
                    rx -= w;
                    const Rect pr(rx, topBar.y + 4.0f * s, w, topBar.h - 8.0f * s);
                    rx -= 5.0f * s;
                    return pr.Contains(mouse);
                };
                if (effOpts.tokenSimulation && hitTopPill(96.0f * s)) {
                    for (const auto& n : diagram.nodes) {
                        if (n.type == BpmnNodeType::StartEvent) {
                            SpawnTokenFromStartEvent(diagram, n, state);
                            break;
                        }
                    }
                    ctx.RequestRepaint(2);
                } else if (hitTopPill(106.0f * s)) {
                    effOpts.tokenSimulation = !effOpts.tokenSimulation;
                    if (opts.tokenSimulationPtr) *opts.tokenSimulationPtr = effOpts.tokenSimulation;
                    if (effOpts.tokenSimulation) {
                        state.ClearSelection();
                        for (const auto& n : diagram.nodes) {
                            if (n.type == BpmnNodeType::StartEvent) {
                                SpawnTokenFromStartEvent(diagram, n, state);
                                break;
                            }
                        }
                    } else {
                        diagram.ResetSimulationStats();
                    }
                    resp.changed = true;
                    ctx.RequestRepaint(2);
                } else if (hitTopPill(86.0f * s)) {
                    effOpts.routingMode = (effOpts.routingMode == BpmnRoutingMode::Orthogonal)
                        ? BpmnRoutingMode::Bezier
                        : BpmnRoutingMode::Orthogonal;
                    if (opts.routingModePtr) *opts.routingModePtr = effOpts.routingMode;
                    resp.changed = true;
                    ctx.RequestRepaint(2);
                } else if (hitTopPill(44.0f * s)) {
                    if (state.Redo(diagram)) {
                        resp.changed = true;
                        resp.diagramModified = true;
                    }
                } else if (hitTopPill(44.0f * s)) {
                    if (state.Undo(diagram)) {
                        resp.changed = true;
                        resp.diagramModified = true;
                    }
                } else if (hitTopPill(44.0f * s)) {
                    ctx.SetClipboardText(BpmnEditor::ExportBpmnXml(diagram));
                }
            }
        }

        // B. Left Palette hover, click & drag-to-canvas interactions
        state.hoveredPaletteIdx = -1;
        state.hoveredContextPadBtn = -1;
        state.hoveredReplaceMenuItem = -1;

        const Rect palRect = (effOpts.showLeftPalette && !effOpts.readOnly)
            ? ComputeLeftPaletteRect(viewRect, effOpts.showTopBar, s)
            : Rect(0.0f, 0.0f, 0.0f, 0.0f);

        if (hovered && effOpts.showLeftPalette && !effOpts.readOnly) {
            if (palRect.Contains(mouse)) {
                keel::f32 cy = palRect.y + 5.0f * s;
                const keel::f32 btnH = 25.0f * s;
                for (keel::usize i = 0; i < kPaletteItems.size(); ++i) {
                    const auto& item = kPaletteItems[i];
                    const Rect br(palRect.x + 4.0f * s, cy, palRect.w - 8.0f * s, btnH);
                    if (br.Contains(mouse)) {
                        state.hoveredPaletteIdx = keel::as<keel::i32>(i);
                        if (!hudConsumedClick && lmbClicked) {
                            hudConsumedClick = true;
                            state.activeTool = item.tool;
                            state.paletteDragCreating = (item.tool >= BpmnTool::CreateStartEvent);
                            state.replaceMenuOpen = false;
                            ctx.RequestRepaint(2);
                        }
                        break;
                    }
                    cy += btnH + 2.0f * s;
                    if (item.separatorAfter) cy += 5.0f * s;
                }
                if (lmbClicked) hudConsumedClick = true;
            }
        }
        if (state.paletteDragCreating && lmbReleased && palRect.Contains(mouse)) {
            state.paletteDragCreating = false;
        }

        // C. Bottom-Left Zoom HUD interactions
        if (!hudConsumedClick && effOpts.showZoomControls && lmbClicked) {
            const Rect zHud = ComputeZoomHudRect(viewRect, s);
            if (zHud.Contains(mouse)) {
                hudConsumedClick = true;
                const keel::f32 btnW = 28.0f * s;
                const Rect minusR(zHud.x + 2.0f * s, zHud.y + 2.0f * s, btnW, zHud.h - 4.0f * s);
                const Rect pctR(minusR.Right() + 2.0f * s, zHud.y + 2.0f * s, 58.0f * s, zHud.h - 4.0f * s);
                const Rect plusR(pctR.Right() + 2.0f * s, zHud.y + 2.0f * s, btnW, zHud.h - 4.0f * s);
                const Rect fitR(plusR.Right() + 4.0f * s, zHud.y + 2.0f * s, zHud.Right() - plusR.Right() - 6.0f * s, zHud.h - 4.0f * s);

                const Vec2 centerScreen = viewRect.Center();
                const Vec2 centerWorld = ScreenToWorld(centerScreen, viewRect, state.pan, state.zoom);
                if (minusR.Contains(mouse)) {
                    state.zoom = keel::clamp(state.zoom / 1.18f, 0.25f, 2.5f);
                    state.pan.x = centerScreen.x - viewRect.x - centerWorld.x * state.zoom;
                    state.pan.y = centerScreen.y - viewRect.y - centerWorld.y * state.zoom;
                } else if (plusR.Contains(mouse)) {
                    state.zoom = keel::clamp(state.zoom * 1.18f, 0.25f, 2.5f);
                    state.pan.x = centerScreen.x - viewRect.x - centerWorld.x * state.zoom;
                    state.pan.y = centerScreen.y - viewRect.y - centerWorld.y * state.zoom;
                } else if (pctR.Contains(mouse)) {
                    state.zoom = 1.0f;
                    state.pan.x = centerScreen.x - viewRect.x - centerWorld.x * state.zoom;
                    state.pan.y = centerScreen.y - viewRect.y - centerWorld.y * state.zoom;
                } else if (fitR.Contains(mouse)) {
                    FitDiagramToViewport(diagram, viewRect, effOpts.showLeftPalette ? 54.0f * s : 0.0f, state);
                }
                ctx.RequestRepaint(2);
            }
        }

        // D. Bottom-Right Minimap click/drag navigation
        if (!hudConsumedClick && effOpts.showMinimap && viewRect.w >= 420.0f * s && viewRect.h >= 260.0f * s) {
            const Rect mm = ComputeMinimapRect(viewRect, s);
            if (mm.Contains(mouse) && (lmbClicked || (lmbDown && !state.draggingNodes && !state.panningCanvas && !state.resizingNode && !state.draggingEdgeSegment))) {
                hudConsumedClick = true;
                const Rect wb = ComputeDiagramWorldBounds(diagram);
                const keel::f32 pad = 8.0f * s;
                const keel::f32 sx = (mm.w - pad * 2.0f) / keel::max(100.0f, wb.w);
                const keel::f32 sy = (mm.h - pad * 2.0f) / keel::max(100.0f, wb.h);
                const keel::f32 ms = keel::min(sx, sy);
                const keel::f32 ox = mm.x + (mm.w - wb.w * ms) * 0.5f - wb.x * ms;
                const keel::f32 oy = mm.y + (mm.h - wb.h * ms) * 0.5f - wb.y * ms;
                const keel::f32 targetWorldX = (mouse.x - ox) / keel::max(1e-4f, ms);
                const keel::f32 targetWorldY = (mouse.y - oy) / keel::max(1e-4f, ms);
                state.pan.x = viewRect.w * 0.5f - targetWorldX * state.zoom;
                state.pan.y = viewRect.h * 0.5f - targetWorldY * state.zoom;
                ctx.RequestRepaint(2);
            }
        }

        // E. Context Pad & Replace Menu hover + click interactions
        if (hovered && effOpts.showContextPad && !effOpts.readOnly && !effOpts.tokenSimulation) {
            if (state.primaryNodeId != 0 && state.selectedNodes.size() <= 1) {
                if (BpmnNode* selNode = diagram.FindNode(state.primaryNodeId)) {
                    const Rect nsr = WorldToScreenRect(selNode->Bounds(), viewRect, state.pan, state.zoom);
                    const auto padActions = GetContextPadActionsForNode(*selNode);
                    const keel::i32 numCols = keel::min(4, keel::as<keel::i32>(padActions.size()));
                    const keel::i32 numRows = (keel::as<keel::i32>(padActions.size()) + 3) / 4;
                    const keel::f32 padX = keel::min(viewRect.Right() - 156.0f * s, nsr.Right() + 10.0f * s);
                    const keel::f32 padY = keel::clamp(nsr.y - 4.0f * s, viewRect.y + 48.0f * s, viewRect.Bottom() - 100.0f * s);
                    const keel::f32 btnS = 26.0f * s;
                    const keel::f32 gap = 4.0f * s;
                    const Rect padFrame(
                        padX - 4.0f * s,
                        padY - 4.0f * s,
                        (btnS + gap) * keel::as<keel::f32>(numCols) + 4.0f * s,
                        (btnS + gap) * keel::as<keel::f32>(numRows) + 4.0f * s
                    );

                    if (state.replaceMenuOpen) {
                        const Rect popRect(padFrame.x, padFrame.Bottom() + 6.0f * s, 210.0f * s, 224.0f * s);
                        if (popRect.Contains(mouse)) {
                            const keel::f32 rowH = 23.0f * s;
                            const keel::i32 idx = keel::as<keel::i32>((mouse.y - (popRect.y + 27.0f * s)) / rowH);
                            const keel::i32 maxItems = (selNode->type == BpmnNodeType::Task || selNode->type == BpmnNodeType::SubProcess) ? 8
                                               : (selNode->type == BpmnNodeType::Gateway ? 4 : 7);
                            if (idx >= 0 && idx < maxItems) {
                                state.hoveredReplaceMenuItem = idx;
                                if (!hudConsumedClick && lmbClicked) {
                                    hudConsumedClick = true;
                                    state.PushUndo(diagram);
                                    BpmnNode* mutNode = diagram.FindNode(state.primaryNodeId);
                                    if (mutNode) {
                                        if (mutNode->type == BpmnNodeType::Task || mutNode->type == BpmnNodeType::SubProcess) {
                                            const Vec2 oldCenter = mutNode->Center();
                                            if (idx == 6) {
                                                mutNode->type = BpmnNodeType::SubProcess;
                                                mutNode->isExpanded = false;
                                                mutNode->size = BpmnDiagram::DefaultNodeSize(BpmnNodeType::SubProcess);
                                                mutNode->pos = {oldCenter.x - mutNode->size.x * 0.5f, oldCenter.y - mutNode->size.y * 0.5f};
                                            } else if (idx == 7) {
                                                mutNode->type = BpmnNodeType::SubProcess;
                                                mutNode->isExpanded = true;
                                                mutNode->size = {320.0f, 180.0f};
                                                mutNode->pos = {oldCenter.x - mutNode->size.x * 0.5f, oldCenter.y - mutNode->size.y * 0.5f};
                                            } else {
                                                mutNode->type = BpmnNodeType::Task;
                                                mutNode->isExpanded = false;
                                                mutNode->size = BpmnDiagram::DefaultNodeSize(BpmnNodeType::Task);
                                                mutNode->pos = {oldCenter.x - mutNode->size.x * 0.5f, oldCenter.y - mutNode->size.y * 0.5f};
                                                static constexpr keel::StaticArray<BpmnTaskType, 6> kTaskTypes = {
                                                    BpmnTaskType::User, BpmnTaskType::Service, BpmnTaskType::Send,
                                                    BpmnTaskType::Receive, BpmnTaskType::Script, BpmnTaskType::BusinessRule
                                                };
                                                mutNode->taskType = kTaskTypes[keel::as<keel::usize>(idx)];
                                            }
                                            diagram.SyncBoundaryEvents();
                                        } else if (mutNode->type == BpmnNodeType::Gateway && idx < 4) {
                                            mutNode->gatewayType = keel::from_underlying<BpmnGatewayType>(idx);
                                        } else if (idx < 7) {
                                            mutNode->eventDef = keel::from_underlying<BpmnEventDefinition>(idx);
                                        }
                                        resp.changed = true;
                                        resp.diagramModified = true;
                                    }
                                    state.replaceMenuOpen = false;
                                }
                            }
                            if (lmbClicked) hudConsumedClick = true;
                        }
                    }

                    if (padFrame.Contains(mouse)) {
                        for (keel::usize idx = 0; idx < padActions.size(); ++idx) {
                            const keel::i32 actionId = padActions[idx];
                            const keel::i32 row = keel::as<keel::i32>(idx) / 4;
                            const keel::i32 col = keel::as<keel::i32>(idx) % 4;
                            const Rect br(padX + keel::as<keel::f32>(col) * (btnS + gap), padY + keel::as<keel::f32>(row) * (btnS + gap), btnS, btnS);
                            if (!br.Contains(mouse)) continue;
                            state.hoveredContextPadBtn = actionId;
                            if (!hudConsumedClick && lmbClicked) {
                                hudConsumedClick = true;
                                const keel::u32 srcId = selNode->id;
                                const Vec2 srcPos = selNode->pos;
                                const Vec2 srcSize = selNode->size;
                                const Vec2 srcCenter = selNode->Center();
                                const keel::u32 srcLane = selNode->laneId;

                                auto appendQuickNode = [&](BpmnNodeType newType, keel::StringRef label, BpmnFlowType flow) {
                                    state.PushUndo(diagram);
                                    BpmnNode nn{};
                                    nn.type = newType;
                                    nn.label = label;
                                    nn.size = BpmnDiagram::DefaultNodeSize(newType);
                                    if (newType == BpmnNodeType::TextAnnotation) {
                                        const keel::f32 targetCX = SnapValue(srcCenter.x + srcSize.x * 0.5f + 70.0f, effOpts.gridStep);
                                        const keel::f32 targetCY = SnapValue(srcPos.y - 45.0f, effOpts.gridStep);
                                        nn.pos = {targetCX - nn.size.x * 0.5f, targetCY - nn.size.y * 0.5f};
                                    } else {
                                        const keel::f32 gapX = (newType == BpmnNodeType::Task || newType == BpmnNodeType::SubProcess) ? 60.0f : 50.0f;
                                        const keel::f32 targetCX = SnapValue(srcPos.x + srcSize.x + gapX + nn.size.x * 0.5f, effOpts.gridStep);
                                        nn.pos = {targetCX - nn.size.x * 0.5f, srcCenter.y - nn.size.y * 0.5f};
                                    }
                                    nn.laneId = srcLane;
                                    if (newType == BpmnNodeType::IntermediateEvent) nn.eventDef = BpmnEventDefinition::Timer;
                                    const keel::u32 newId = diagram.AddNode(keel::move(nn));
                                    diagram.AddEdge(srcId, newId, flow);
                                    state.SelectSingleNode(newId);
                                    resp.changed = true;
                                    resp.diagramModified = true;
                                };

                                if (actionId == 0) appendQuickNode(BpmnNodeType::Task, "Next Task", BpmnFlowType::SequenceFlow);
                                else if (actionId == 1) appendQuickNode(BpmnNodeType::Gateway, "Decision?", BpmnFlowType::SequenceFlow);
                                else if (actionId == 2) appendQuickNode(BpmnNodeType::EndEvent, "Completed", BpmnFlowType::SequenceFlow);
                                else if (actionId == 3) appendQuickNode(BpmnNodeType::IntermediateEvent, "Wait Timer", BpmnFlowType::SequenceFlow);
                                else if (actionId == 4) {
                                    state.connectingWire = true;
                                    state.connectFromNodeId = srcId;
                                    state.connectFromPort = BpmnPortSide::Auto;
                                    state.connectEndWorld = resp.mouseWorldPos;
                                } else if (actionId == 5) {
                                    state.replaceMenuOpen = !state.replaceMenuOpen;
                                } else if (actionId == 6) {
                                    appendQuickNode(BpmnNodeType::TextAnnotation, "Text annotation", BpmnFlowType::Association);
                                } else if (actionId == 7) {
                                    state.PushUndo(diagram);
                                    diagram.RemoveNode(srcId);
                                    state.ClearSelection();
                                    resp.changed = true;
                                    resp.diagramModified = true;
                                }
                                ctx.RequestRepaint(2);
                            }
                            break;
                        }
                        if (lmbClicked) hudConsumedClick = true;
                    }
                }
            } else if (!hudConsumedClick && lmbClicked && state.selectedEdgeId != 0) {
                if (BpmnEdge* selEdge = diagram.FindEdge(state.selectedEdgeId)) {
                    const BpmnNode* fn = diagram.FindNode(selEdge->fromNodeId);
                    const BpmnNode* tn = diagram.FindNode(selEdge->toNodeId);
                    if (fn && tn) {
                        const auto wpts = BuildEdgeWorldPoints(diagram, *fn, *tn, *selEdge, effOpts.routingMode);
                        Vec2 wMid{0.0f, 0.0f};
                        (void)SamplePolylineAtProgress(wpts, 0.5f, &wMid);
                        const Vec2 sMid = WorldToScreen(wMid, viewRect, state.pan, state.zoom);
                        const Rect bar(sMid.x - 110.0f * s, sMid.y - 36.0f * s, 220.0f * s, 26.0f * s);
                        if (bar.Contains(mouse)) {
                            hudConsumedClick = true;
                            const keel::f32 bw = (bar.w - 8.0f * s) / 6.0f;
                            const keel::i32 idx = keel::clamp(keel::as<keel::i32>((mouse.x - (bar.x + 4.0f * s)) / bw), 0, 5);
                            state.PushUndo(diagram);
                            if (idx == 0) selEdge->flowType = BpmnFlowType::SequenceFlow;
                            else if (idx == 1) selEdge->flowType = BpmnFlowType::DefaultFlow;
                            else if (idx == 2) selEdge->flowType = BpmnFlowType::ConditionalFlow;
                            else if (idx == 3) selEdge->flowType = BpmnFlowType::MessageFlow;
                            else if (idx == 4) {
                                selEdge->waypoints.clear();
                                selEdge->labelOffset = {0.0f, 0.0f};
                            } else if (idx == 5) {
                                diagram.RemoveEdge(selEdge->id);
                                state.ClearSelection();
                            }
                            resp.changed = true;
                            resp.diagramModified = true;
                            ctx.RequestRepaint(2);
                        }
                    }
                }
            } else if (!hudConsumedClick && lmbClicked && state.selectedLaneId != 0) {
                if (BpmnLane* selLane = diagram.FindLane(state.selectedLaneId)) {
                    const Rect lsr = WorldToScreenRect(selLane->rect, viewRect, state.pan, state.zoom);
                    const Rect bar(keel::max(viewRect.x + 58.0f * s, lsr.x + 44.0f * s), keel::max(viewRect.y + 48.0f * s, lsr.y - 30.0f * s), 232.0f * s, 26.0f * s);
                    if (bar.Contains(mouse)) {
                        hudConsumedClick = true;
                        const keel::f32 bw = (bar.w - 8.0f * s) / 4.0f;
                        const keel::i32 idx = keel::clamp(keel::as<keel::i32>((mouse.x - (bar.x + 4.0f * s)) / bw), 0, 3);
                        state.PushUndo(diagram);
                        const Rect curR = selLane->rect;
                        const auto poolName = selLane->poolName;
                        if (idx == 0) {
                            const keel::u32 newId = diagram.AddLane(poolName, "Upper Lane", Rect(curR.x, curR.y - 180.0f, curR.w, 180.0f));
                            state.selectedLaneId = newId;
                        } else if (idx == 1) {
                            const keel::u32 newId = diagram.AddLane(poolName, "Lower Lane", Rect(curR.x, curR.Bottom(), curR.w, 180.0f));
                            state.selectedLaneId = newId;
                        } else if (idx == 2 && curR.h >= 140.0f) {
                            const keel::f32 halfH = curR.h * 0.5f;
                            selLane->rect.h = halfH;
                            keel::SmallString<128> splitName = keel::SmallString<128>::from(selLane->name);
                            splitName.append(" B");
                            diagram.AddLane(poolName, splitName, Rect(curR.x, curR.y + halfH, curR.w, halfH));
                        } else if (idx == 3) {
                            diagram.RemoveLane(selLane->id);
                            state.ClearSelection();
                        }
                        resp.changed = true;
                        resp.diagramModified = true;
                        ctx.RequestRepaint(2);
                    }
                }
            }
        }

        // F. Mouse Wheel Zoom (centered at cursor)
        if (hovered) {
            const Vec2 wheel = ctx.GetMouseWheel();
            if (keel::abs(wheel.y) > 0.01f) {
                const Vec2 worldBefore = ScreenToWorld(mouse, viewRect, state.pan, state.zoom);
                const keel::f32 factor = (wheel.y > 0.0f) ? 1.12f : (1.0f / 1.12f);
                state.zoom = keel::clamp(state.zoom * factor, 0.25f, 2.5f);
                state.pan.x = mouse.x - viewRect.x - worldBefore.x * state.zoom;
                state.pan.y = mouse.y - viewRect.y - worldBefore.y * state.zoom;
                ctx.ConsumeMouseWheel();
                ctx.RequestRepaint(2);
            }
        }

        // G. Hit-Testing World Elements (Resize Handles, Edge Endpoints/Segments, External Labels, Nodes, Edges, Lanes)
        state.hoveredNodeId = 0;
        state.hoveredEdgeId = 0;
        state.hoveredLaneId = 0;
        state.hoveredLabelNodeId = 0;
        state.hoveredLabelEdgeId = 0;
        state.hoveredResizeCorner = -1;
        state.hoveredEdgeSegIdx = -1;
        state.hoveredEdgeEndpoint = 0;
        state.dragBoundaryHostId = 0;

        if (hovered && !hudConsumedClick) {
            const Vec2 wp = resp.mouseWorldPos;

            // G1. Check 4-Corner Resize Handles on Selected Resizable Node
            if (!effOpts.readOnly && !effOpts.tokenSimulation && state.primaryNodeId != 0 && state.selectedNodes.size() == 1) {
                if (const BpmnNode* selNode = diagram.FindNode(state.primaryNodeId)) {
                    if (IsResizableNode(selNode->type)) {
                        const Rect nsr = WorldToScreenRect(selNode->Bounds(), viewRect, state.pan, state.zoom);
                        const auto corners = GetNodeResizeHandleRects(nsr, state.zoom, s);
                        for (keel::i32 c = 0; c < 4; ++c) {
                            const Rect& cr = corners[keel::as<keel::usize>(c)];
                            const Rect hitR(cr.x - 3.0f * s, cr.y - 3.0f * s, cr.w + 6.0f * s, cr.h + 6.0f * s);
                            if (hitR.Contains(mouse)) {
                                state.hoveredResizeCorner = c;
                                break;
                            }
                        }
                    }
                }
            }

            // G2. Check Selected Edge Endpoint Reconnect Circles & Orthogonal Segment Handles
            if (!effOpts.readOnly && !effOpts.tokenSimulation && state.hoveredResizeCorner < 0 && state.selectedEdgeId != 0) {
                if (const BpmnEdge* selEdge = diagram.FindEdge(state.selectedEdgeId)) {
                    const BpmnNode* fn = diagram.FindNode(selEdge->fromNodeId);
                    const BpmnNode* tn = diagram.FindNode(selEdge->toNodeId);
                    if (fn && tn) {
                        const auto wpts = BuildEdgeWorldPoints(diagram, *fn, *tn, *selEdge, effOpts.routingMode);
                        if (wpts.size() >= 2) {
                            const Vec2 pStart = WorldToScreen(wpts.front(), viewRect, state.pan, state.zoom);
                            const Vec2 pEnd = WorldToScreen(wpts.back(), viewRect, state.pan, state.zoom);
                            const keel::f32 endR = 8.5f * s;
                            if ((mouse.x - pStart.x) * (mouse.x - pStart.x) + (mouse.y - pStart.y) * (mouse.y - pStart.y) <= endR * endR) {
                                state.hoveredEdgeEndpoint = 1;
                                state.hoveredEdgeId = selEdge->id;
                            } else if ((mouse.x - pEnd.x) * (mouse.x - pEnd.x) + (mouse.y - pEnd.y) * (mouse.y - pEnd.y) <= endR * endR) {
                                state.hoveredEdgeEndpoint = 2;
                                state.hoveredEdgeId = selEdge->id;
                            } else if (effOpts.routingMode == BpmnRoutingMode::Orthogonal) {
                                const keel::f32 segTolSq = (7.0f * s) * (7.0f * s);
                                for (keel::usize i = 0; i + 1 < wpts.size(); ++i) {
                                    const Vec2 sp0 = WorldToScreen(wpts[i], viewRect, state.pan, state.zoom);
                                    const Vec2 sp1 = WorldToScreen(wpts[i + 1], viewRect, state.pan, state.zoom);
                                    const keel::f32 segLen = keel::hypot(sp1.x - sp0.x, sp1.y - sp0.y);
                                    if (segLen >= 18.0f * s && PointToSegmentDistanceSq(mouse, sp0, sp1) <= segTolSq) {
                                        state.hoveredEdgeSegIdx = keel::as<keel::i32>(i);
                                        state.hoveredEdgeId = selEdge->id;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // G3. Check External Labels (Events, Gateways, DataObjects, DataStores, and Edges)
            if (state.hoveredResizeCorner < 0 && state.hoveredEdgeEndpoint == 0 && state.hoveredEdgeSegIdx < 0) {
                for (keel::usize revIdx = diagram.nodes.size(); revIdx > 0; --revIdx) {
                    const auto& nd = diagram.nodes[revIdx - 1];
                    if (!IsExternalLabelNode(nd.type) || nd.label.empty()) continue;
                    const Rect nsr = WorldToScreenRect(nd.Bounds(), viewRect, state.pan, state.zoom);
                    const Rect lsr = ComputeExternalNodeLabelScreenRect(diagram, nd, nsr, state.zoom, s);
                    if (lsr.Contains(mouse)) {
                        state.hoveredLabelNodeId = nd.id;
                        state.hoveredNodeId = nd.id;
                        break;
                    }
                }
                if (state.hoveredLabelNodeId == 0) {
                    for (keel::usize revIdx = diagram.edges.size(); revIdx > 0; --revIdx) {
                        const auto& ed = diagram.edges[revIdx - 1];
                        if (ed.label.empty()) continue;
                        const BpmnNode* fn = diagram.FindNode(ed.fromNodeId);
                        const BpmnNode* tn = diagram.FindNode(ed.toNodeId);
                        if (!fn || !tn) continue;
                        const auto wpts = BuildEdgeWorldPoints(diagram, *fn, *tn, ed, effOpts.routingMode);
                        keel::SmallArray<Vec2, 32> spts;
                        spts.reserve(wpts.size());
                        for (const auto& pt : wpts) spts.push_back(WorldToScreen(pt, viewRect, state.pan, state.zoom));
                        const Rect elr = ComputeEdgeLabelScreenRect(canvas, spts, ed, state.zoom, s);
                        if (elr.w > 0.0f && elr.Contains(mouse)) {
                            state.hoveredLabelEdgeId = ed.id;
                            state.hoveredEdgeId = ed.id;
                            break;
                        }
                    }
                }
            }

            // G4. Check Node Bounds (Pass 0: BoundaryEvents, Pass 1: Standard Nodes, Pass 2: Expanded SubProcess Containers)
            if (state.hoveredResizeCorner < 0 && state.hoveredEdgeEndpoint == 0 && state.hoveredEdgeSegIdx < 0 &&
                state.hoveredLabelNodeId == 0 && state.hoveredLabelEdgeId == 0) {
                for (keel::i32 pass = 0; pass < 3 && state.hoveredNodeId == 0; ++pass) {
                    for (keel::usize revIdx = diagram.nodes.size(); revIdx > 0; --revIdx) {
                        const auto& nd = diagram.nodes[revIdx - 1];
                        const bool isBoundary = (nd.type == BpmnNodeType::BoundaryEvent);
                        const bool isExpandedSub = (nd.type == BpmnNodeType::SubProcess && nd.isExpanded);
                        if (pass == 0 && !isBoundary) continue;
                        if (pass == 1 && (isBoundary || isExpandedSub)) continue;
                        if (pass == 2 && !isExpandedSub) continue;
                        if (nd.Bounds().Contains(wp)) {
                            state.hoveredNodeId = nd.id;
                            break;
                        }
                    }
                }
            }

            if (state.activeTool == BpmnTool::CreateIntermediateEvent) {
                for (const auto& n : diagram.nodes) {
                    if (n.type == BpmnNodeType::Task || n.type == BpmnNodeType::SubProcess) {
                        const Rect b = n.Bounds();
                        if (wp.x >= b.x && wp.x <= b.Right() && keel::abs(wp.y - b.Bottom()) <= 18.0f) {
                            state.dragBoundaryHostId = n.id;
                            break;
                        }
                    }
                }
            }

            if (state.hoveredNodeId == 0 && state.hoveredEdgeId == 0) {
                const keel::f32 tolWorld = 7.5f / keel::max(0.25f, state.zoom);
                const keel::f32 tolSq = tolWorld * tolWorld;
                for (const auto& edge : diagram.edges) {
                    const BpmnNode* fn = diagram.FindNode(edge.fromNodeId);
                    const BpmnNode* tn = diagram.FindNode(edge.toNodeId);
                    if (!fn || !tn) continue;
                    const auto wpts = BuildEdgeWorldPoints(diagram, *fn, *tn, edge, effOpts.routingMode);
                    for (keel::usize i = 0; i + 1 < wpts.size(); ++i) {
                        if (PointToSegmentDistanceSq(wp, wpts[i], wpts[i + 1]) <= tolSq) {
                            state.hoveredEdgeId = edge.id;
                            break;
                        }
                    }
                    if (state.hoveredEdgeId != 0) break;
                }
            }

            for (keel::usize revIdx = diagram.lanes.size(); revIdx > 0; --revIdx) {
                const auto& ln = diagram.lanes[revIdx - 1];
                if (ln.rect.Contains(wp)) {
                    state.hoveredLaneId = ln.id;
                    break;
                }
            }
        }

        // G5. Live Ghost Preview Snapping & Smart Alignment Guides during Creation Tools
        if (hovered && !effOpts.readOnly && !effOpts.tokenSimulation &&
            state.activeTool >= BpmnTool::CreateStartEvent && state.activeTool != BpmnTool::CreateLane) {
            BpmnNodeType gType = BpmnNodeType::Task;
            switch (state.activeTool) {
                case BpmnTool::CreateStartEvent:        gType = BpmnNodeType::StartEvent; break;
                case BpmnTool::CreateIntermediateEvent: gType = (state.dragBoundaryHostId != 0) ? BpmnNodeType::BoundaryEvent : BpmnNodeType::IntermediateEvent; break;
                case BpmnTool::CreateEndEvent:          gType = BpmnNodeType::EndEvent; break;
                case BpmnTool::CreateGateway:           gType = BpmnNodeType::Gateway; break;
                case BpmnTool::CreateTask:              gType = BpmnNodeType::Task; break;
                case BpmnTool::CreateSubProcess:        gType = BpmnNodeType::SubProcess; break;
                case BpmnTool::CreateDataObject:        gType = BpmnNodeType::DataObject; break;
                case BpmnTool::CreateDataStore:         gType = BpmnNodeType::DataStore; break;
                case BpmnTool::CreateAnnotation:        gType = BpmnNodeType::TextAnnotation; break;
                default: break;
            }
            const Vec2 gSize = BpmnDiagram::DefaultNodeSize(gType);
            Vec2 gCenter = resp.mouseWorldPos;
            if (effOpts.snapToGrid && !mods.alt) {
                gCenter.x = SnapValue(gCenter.x, effOpts.gridStep);
                gCenter.y = SnapValue(gCenter.y, effOpts.gridStep);
            }
            Vec2 gPos{gCenter.x - gSize.x * 0.5f, gCenter.y - gSize.y * 0.5f};
            state.activeGuides.clear();
            if (effOpts.smartGuides && state.dragBoundaryHostId == 0) {
                const keel::f32 snapTol = 6.0f;
                for (const auto& other : diagram.nodes) {
                    if (other.type == BpmnNodeType::BoundaryEvent) continue;
                    const Vec2 oc = other.Center();
                    if (keel::abs((gPos.x + gSize.x * 0.5f) - oc.x) <= snapTol) {
                        gPos.x = oc.x - gSize.x * 0.5f;
                        state.activeGuides.push_back(AlignmentGuide{
                            true, oc.x,
                            keel::min(gPos.y, other.pos.y) - 20.0f,
                            keel::max(gPos.y + gSize.y, other.pos.y + other.size.y) + 20.0f
                        });
                    }
                    if (keel::abs((gPos.y + gSize.y * 0.5f) - oc.y) <= snapTol) {
                        gPos.y = oc.y - gSize.y * 0.5f;
                        state.activeGuides.push_back(AlignmentGuide{
                            false, oc.y,
                            keel::min(gPos.x, other.pos.x) - 20.0f,
                            keel::max(gPos.x + gSize.x, other.pos.x + other.size.x) + 20.0f
                        });
                    }
                }
            }
            state.ghostSnappedPos = gPos;
            ctx.RequestRepaint(2);
        } else if (!state.draggingNodes) {
            state.activeGuides.clear();
        }

        // H. Token Simulation Click Triggers on StartEvent / BoundaryEvent / Exclusive Gateway
        if (!hudConsumedClick && effOpts.tokenSimulation && lmbClicked) {
            for (auto& n : diagram.nodes) {
                const Rect sr = WorldToScreenRect(n.Bounds(), viewRect, state.pan, state.zoom);
                if (n.type == BpmnNodeType::StartEvent || n.type == BpmnNodeType::BoundaryEvent) {
                    const keel::f32 btnW = 22.0f * s;
                    const keel::f32 btnH = 18.0f * s;
                    const Rect trigRect(sr.x - btnW - 4.0f * s, sr.Center().y - btnH * 0.5f, btnW, btnH);
                    if (trigRect.Contains(mouse) || sr.Contains(mouse)) {
                        SpawnTokenFromStartEvent(diagram, n, state);
                        hudConsumedClick = true;
                        ctx.RequestRepaint(2);
                        break;
                    }
                } else if (n.type == BpmnNodeType::Gateway && n.gatewayType == BpmnGatewayType::Exclusive && sr.Contains(mouse)) {
                    keel::SmallArray<keel::u32, 8> outIds;
                    for (const auto& e : diagram.edges) {
                        if (e.fromNodeId == n.id && e.flowType != BpmnFlowType::Association) {
                            outIds.push_back(e.id);
                        }
                    }
                    if (!outIds.empty()) {
                        keel::usize curIdx = outIds.size();
                        for (keel::usize oi = 0; oi < outIds.size(); ++oi) {
                            if (outIds[oi] == n.activeOutgoingEdgeId) {
                                curIdx = oi;
                                break;
                            }
                        }
                        if (curIdx >= outIds.size() || curIdx + 1 >= outIds.size()) {
                            n.activeOutgoingEdgeId = outIds.front();
                        } else {
                            n.activeOutgoingEdgeId = outIds[curIdx + 1];
                        }
                        hudConsumedClick = true;
                        ctx.RequestRepaint(2);
                        break;
                    }
                }
            }
        }

        // I. Double-Click Inline Label Editing
        if (!hudConsumedClick && !effOpts.readOnly && !effOpts.tokenSimulation && doubleClicked) {
            state.editingNodeId = 0;
            state.editingEdgeId = 0;
            state.editingLaneId = 0;
            if (state.hoveredNodeId != 0) {
                state.editingNodeId = state.hoveredNodeId;
                if (const BpmnNode* n = diagram.FindNode(state.editingNodeId)) {
                    (void)ctx.EnsureTextEdit(editId, n->label, false, false);
                }
            } else if (state.hoveredEdgeId != 0) {
                state.editingEdgeId = state.hoveredEdgeId;
                if (const BpmnEdge* e = diagram.FindEdge(state.editingEdgeId)) {
                    (void)ctx.EnsureTextEdit(editId, e->label, false, false);
                }
            } else if (state.hoveredLaneId != 0) {
                state.editingLaneId = state.hoveredLaneId;
                if (const BpmnLane* l = diagram.FindLane(state.editingLaneId)) {
                    (void)ctx.EnsureTextEdit(editId, l->name, false, false);
                }
            }
        }

        // J. Primary Canvas Tool Interactions
        if (!hudConsumedClick && hovered && (ctx.IsMiddleMouseClicked() || ctx.IsRightMouseClicked() || (lmbClicked && state.activeTool == BpmnTool::Hand))) {
            state.panningCanvas = true;
            state.panStartMouse = mouse;
            state.panStartOffset = state.pan;
        }

        if (state.panningCanvas) {
            if (ctx.IsMiddleMouseDown() || ctx.IsRightMouseDown() || (lmbDown && state.activeTool == BpmnTool::Hand)) {
                state.pan.x = state.panStartOffset.x + (mouse.x - state.panStartMouse.x);
                state.pan.y = state.panStartOffset.y + (mouse.y - state.panStartMouse.y);
                ctx.RequestRepaint(2);
            } else {
                state.panningCanvas = false;
            }
        }

        const bool triggerCreateOnCanvas =
            (!hudConsumedClick && !effOpts.readOnly && !effOpts.tokenSimulation &&
             state.activeTool >= BpmnTool::CreateStartEvent &&
             (lmbClicked || (state.paletteDragCreating && lmbReleased && hovered && !palRect.Contains(mouse))));

        if (triggerCreateOnCanvas) {
            ctx.RequestFocus(id);
            state.PushUndo(diagram);
            if (state.activeTool == BpmnTool::CreateLane) {
                keel::f32 newY = SnapValue(resp.mouseWorldPos.y, effOpts.gridStep);
                keel::f32 newX = SnapValue(resp.mouseWorldPos.x - 120.0f, effOpts.gridStep);
                keel::f32 newW = 1280.0f;
                if (!diagram.lanes.empty()) {
                    newX = diagram.lanes.back().rect.x;
                    newY = diagram.lanes.back().rect.Bottom();
                    newW = diagram.lanes.back().rect.w;
                }
                const keel::u32 lid = diagram.AddLane("Order Fulfillment Pool", "New Swimlane", Rect(newX, newY, newW, 190.0f));
                state.ClearSelection();
                state.selectedLaneId = lid;
            } else {
                BpmnNode nn{};
                switch (state.activeTool) {
                    case BpmnTool::CreateStartEvent:        nn.type = BpmnNodeType::StartEvent; nn.label = "Start"; break;
                    case BpmnTool::CreateIntermediateEvent:
                        if (state.dragBoundaryHostId != 0) {
                            nn.type = BpmnNodeType::BoundaryEvent;
                            nn.attachedToNodeId = state.dragBoundaryHostId;
                            nn.eventDef = BpmnEventDefinition::Error;
                            nn.label = "Exception";
                            if (const BpmnNode* host = diagram.FindNode(state.dragBoundaryHostId)) {
                                nn.boundaryRatioX = keel::clamp((resp.mouseWorldPos.x - host->pos.x) / keel::max(1.0f, host->size.x), 0.15f, 0.85f);
                            }
                        } else {
                            nn.type = BpmnNodeType::IntermediateEvent;
                            nn.eventDef = BpmnEventDefinition::Timer;
                            nn.label = "Wait Timer";
                        }
                        break;
                    case BpmnTool::CreateEndEvent:          nn.type = BpmnNodeType::EndEvent; nn.label = "End"; break;
                    case BpmnTool::CreateGateway:           nn.type = BpmnNodeType::Gateway; nn.label = "Gateway?"; break;
                    case BpmnTool::CreateTask:              nn.type = BpmnNodeType::Task; nn.label = "Task"; break;
                    case BpmnTool::CreateSubProcess:        nn.type = BpmnNodeType::SubProcess; nn.label = "Sub-Process"; break;
                    case BpmnTool::CreateDataObject:        nn.type = BpmnNodeType::DataObject; nn.label = "Payload"; break;
                    case BpmnTool::CreateDataStore:         nn.type = BpmnNodeType::DataStore; nn.label = "Database"; break;
                    case BpmnTool::CreateAnnotation:        nn.type = BpmnNodeType::TextAnnotation; nn.label = "Note / SLA"; break;
                    default: break;
                }
                nn.size = BpmnDiagram::DefaultNodeSize(nn.type);
                nn.pos = state.ghostSnappedPos;
                nn.laneId = state.hoveredLaneId;
                const keel::u32 newId = diagram.AddNode(keel::move(nn));
                state.SelectSingleNode(newId);
            }
            state.activeTool = BpmnTool::Select;
            state.paletteDragCreating = false;
            state.activeGuides.clear();
            resp.changed = true;
            resp.diagramModified = true;
            ctx.RequestRepaint(2);
        } else if (!hudConsumedClick && lmbClicked && state.activeTool == BpmnTool::GlobalConnect) {
            ctx.RequestFocus(id);
            if (state.hoveredNodeId != 0) {
                state.connectingWire = true;
                state.connectFromNodeId = state.hoveredNodeId;
                state.connectFromPort = BpmnPortSide::Auto;
                state.connectEndWorld = resp.mouseWorldPos;
            }
        } else if (!hudConsumedClick && lmbClicked && state.activeTool == BpmnTool::Space && !effOpts.readOnly) {
            ctx.RequestFocus(id);
            state.PushUndo(diagram);
            state.spaceToolDragging = true;
            state.spaceToolAxisLocked = false;
            state.spaceStartWorld = resp.mouseWorldPos;
            state.spaceCurrentWorld = resp.mouseWorldPos;
            state.spaceInitialNodes.clear();
            state.spaceInitialLanes.clear();
            for (const auto& n : diagram.nodes) state.spaceInitialNodes.push_back(NodePosEntry{n.id, n.pos});
            for (const auto& l : diagram.lanes) state.spaceInitialLanes.push_back(LaneRectEntry{l.id, l.rect});
        } else if (!hudConsumedClick && lmbClicked && state.activeTool == BpmnTool::Lasso) {
            ctx.RequestFocus(id);
            state.lassoActive = true;
            state.lassoStartWorld = resp.mouseWorldPos;
            state.lassoEndWorld = resp.mouseWorldPos;
            if (!mods.shift && !mods.ctrl) state.ClearSelection();
        } else if (!hudConsumedClick && lmbClicked && state.activeTool == BpmnTool::Select) {
            ctx.RequestFocus(id);
            // 1. Check 4-Corner Node Resize Handle Click
            if (!effOpts.readOnly && !effOpts.tokenSimulation && state.hoveredResizeCorner >= 0 && state.primaryNodeId != 0) {
                if (const BpmnNode* rn = diagram.FindNode(state.primaryNodeId)) {
                    state.PushUndo(diagram);
                    state.resizingNode = true;
                    state.resizeCorner = state.hoveredResizeCorner;
                    state.resizeStartWorld = resp.mouseWorldPos;
                    state.resizeInitialBounds = rn->Bounds();
                }
            }
            // 2. Check Edge Endpoint Reconnect Handle Click
            else if (!effOpts.readOnly && !effOpts.tokenSimulation && state.hoveredEdgeEndpoint > 0 && state.selectedEdgeId != 0) {
                state.reconnectingEdge = true;
                state.reconnectEdgeId = state.selectedEdgeId;
                state.reconnectEndpoint = state.hoveredEdgeEndpoint;
                state.reconnectEndWorld = resp.mouseWorldPos;
                state.reconnectSnapNodeId = 0;
            }
            // 3. Check Orthogonal Edge Segment Drag Handle Click
            else if (!effOpts.readOnly && !effOpts.tokenSimulation && state.hoveredEdgeSegIdx >= 0 && state.selectedEdgeId != 0) {
                if (BpmnEdge* se = diagram.FindEdge(state.selectedEdgeId)) {
                    const BpmnNode* fn = diagram.FindNode(se->fromNodeId);
                    const BpmnNode* tn = diagram.FindNode(se->toNodeId);
                    if (fn && tn) {
                        state.PushUndo(diagram);
                        const auto fullPts = BuildEdgeWorldPoints(diagram, *fn, *tn, *se, effOpts.routingMode);
                        if (fullPts.size() >= 2) {
                            keel::i32 segIdx = state.hoveredEdgeSegIdx;
                            const bool isHoriz = keel::abs(fullPts[keel::as<keel::usize>(segIdx) + 1].y - fullPts[keel::as<keel::usize>(segIdx)].y) <=
                                                 keel::abs(fullPts[keel::as<keel::usize>(segIdx) + 1].x - fullPts[keel::as<keel::usize>(segIdx)].x);
                            keel::Array<Vec2> inner;
                            if (fullPts.size() <= 2) {
                                const Vec2 mid{(fullPts.front().x + fullPts.back().x) * 0.5f, (fullPts.front().y + fullPts.back().y) * 0.5f};
                                inner.push_back(mid);
                                inner.push_back(mid);
                                segIdx = 1;
                            } else {
                                if (segIdx == 0) {
                                    inner.push_back(fullPts[1]);
                                    segIdx = 1;
                                }
                                for (keel::usize k = 1; k + 1 < fullPts.size(); ++k) inner.push_back(fullPts[k]);
                                if (state.hoveredEdgeSegIdx >= keel::as<keel::i32>(fullPts.size()) - 2) {
                                    inner.push_back(inner.back());
                                    segIdx = keel::as<keel::i32>(inner.size()) - 1;
                                }
                            }
                            se->waypoints = keel::move(inner);
                            state.draggingEdgeSegment = true;
                            state.dragEdgeId = se->id;
                            state.dragEdgeSegIdx = segIdx;
                            state.dragEdgeSegHorizontal = isHoriz;
                            state.dragEdgeStartWorld = resp.mouseWorldPos;
                            state.dragEdgeInitWaypoints = se->waypoints.clone();
                        }
                    }
                }
            }
            // 4. Check External Label Drag Click (Node or Edge)
            else if (!effOpts.readOnly && !effOpts.tokenSimulation && (state.hoveredLabelNodeId != 0 || state.hoveredLabelEdgeId != 0)) {
                state.PushUndo(diagram);
                state.draggingLabel = true;
                state.dragLabelNodeId = state.hoveredLabelNodeId;
                state.dragLabelEdgeId = state.hoveredLabelEdgeId;
                state.dragLabelStartWorld = resp.mouseWorldPos;
                if (state.dragLabelNodeId != 0) {
                    state.SelectSingleNode(state.dragLabelNodeId);
                    if (const BpmnNode* ln = diagram.FindNode(state.dragLabelNodeId)) {
                        state.dragLabelInitOffset = ln->labelOffset;
                    }
                } else if (state.dragLabelEdgeId != 0) {
                    state.ClearSelection();
                    state.selectedEdgeId = state.dragLabelEdgeId;
                    if (const BpmnEdge* le = diagram.FindEdge(state.dragLabelEdgeId)) {
                        state.dragLabelInitOffset = le->labelOffset;
                    }
                }
                resp.changed = true;
            }
            // 5. Check Node Click (including SubProcess [+]/[-] collapse/expand marker)
            else if (state.hoveredNodeId != 0) {
                BpmnNode* clickedNode = diagram.FindNode(state.hoveredNodeId);
                bool toggledExpand = false;
                if (!effOpts.readOnly && !effOpts.tokenSimulation && clickedNode && clickedNode->type == BpmnNodeType::SubProcess) {
                    const Rect nsr = WorldToScreenRect(clickedNode->Bounds(), viewRect, state.pan, state.zoom);
                    const Rect markerR(nsr.Center().x - 10.0f * s, nsr.Bottom() - 18.0f * s, 20.0f * s, 17.0f * s);
                    if (markerR.Contains(mouse)) {
                        state.PushUndo(diagram);
                        const Vec2 oldC = clickedNode->Center();
                        clickedNode->isExpanded = !clickedNode->isExpanded;
                        clickedNode->size = clickedNode->isExpanded ? Vec2{320.0f, 180.0f} : BpmnDiagram::DefaultNodeSize(BpmnNodeType::SubProcess);
                        clickedNode->pos = {oldC.x - clickedNode->size.x * 0.5f, oldC.y - clickedNode->size.y * 0.5f};
                        diagram.SyncBoundaryEvents();
                        toggledExpand = true;
                        resp.changed = true;
                        resp.diagramModified = true;
                        ctx.RequestRepaint(2);
                    }
                }
                if (!toggledExpand) {
                    if (mods.shift || mods.ctrl) {
                        state.ToggleNodeSelection(state.hoveredNodeId);
                    } else if (!state.IsNodeSelected(state.hoveredNodeId)) {
                        state.SelectSingleNode(state.hoveredNodeId);
                    }
                    if (!effOpts.readOnly && !effOpts.tokenSimulation) {
                        state.draggingNodes = true;
                        state.dragUndoRecorded = false;
                        state.dragStartWorld = resp.mouseWorldPos;
                        state.dragInitialNodePos.clear();
                        for (keel::u32 selId : state.selectedNodes) {
                            if (const BpmnNode* sn = diagram.FindNode(selId)) {
                                state.dragInitialNodePos.push_back(NodePosEntry{selId, sn->pos});
                                if (sn->type == BpmnNodeType::SubProcess && sn->isExpanded) {
                                    const Rect sb = sn->Bounds();
                                    for (const auto& child : diagram.nodes) {
                                        if (child.id != sn->id && !state.IsNodeSelected(child.id) && sb.Contains(child.Center())) {
                                            state.dragInitialNodePos.push_back(NodePosEntry{child.id, child.pos});
                                        }
                                    }
                                }
                            }
                        }
                    }
                    resp.changed = true;
                }
            } else if (state.hoveredEdgeId != 0) {
                state.ClearSelection();
                state.selectedEdgeId = state.hoveredEdgeId;
                resp.changed = true;
            } else if (state.hoveredLaneId != 0) {
                state.ClearSelection();
                state.selectedLaneId = state.hoveredLaneId;
                if (!effOpts.readOnly && !effOpts.tokenSimulation) {
                    if (const BpmnLane* l = diagram.FindLane(state.hoveredLaneId)) {
                        const keel::f32 headerWorldW = 64.0f;
                        const keel::f32 edgeTol = 12.0f / keel::max(0.25f, state.zoom);
                        if (keel::abs(resp.mouseWorldPos.x - l->rect.Right()) <= edgeTol) {
                            state.draggingLane = true;
                            state.laneResizeEdge = 1;
                        } else if (keel::abs(resp.mouseWorldPos.y - l->rect.Bottom()) <= edgeTol) {
                            state.draggingLane = true;
                            state.laneResizeEdge = 2;
                        } else if (resp.mouseWorldPos.x <= l->rect.x + headerWorldW) {
                            state.draggingLane = true;
                            state.laneResizeEdge = 0;
                        }
                        if (state.draggingLane) {
                            state.PushUndo(diagram);
                            state.laneDragStartWorld = resp.mouseWorldPos;
                            state.laneInitialRect = l->rect;
                            state.laneContainedNodeOffsets.clear();
                            for (const auto& n : diagram.nodes) {
                                if (l->rect.Contains(n.Center())) {
                                    state.laneContainedNodeOffsets.push_back(NodePosEntry{n.id, n.pos});
                                }
                            }
                        } else {
                            if (mods.shift) {
                                state.lassoActive = true;
                                state.lassoStartWorld = resp.mouseWorldPos;
                                state.lassoEndWorld = resp.mouseWorldPos;
                            } else {
                                state.panningCanvas = true;
                                state.panStartMouse = mouse;
                                state.panStartOffset = state.pan;
                            }
                        }
                    }
                }
                resp.changed = true;
            } else {
                state.ClearSelection();
                if (mods.shift) {
                    state.lassoActive = true;
                    state.lassoStartWorld = resp.mouseWorldPos;
                    state.lassoEndWorld = resp.mouseWorldPos;
                } else {
                    state.panningCanvas = true;
                    state.panStartMouse = mouse;
                    state.panStartOffset = state.pan;
                }
            }
        }

        // Ongoing 4-Corner Node Resize (`Resize`)
        if (state.resizingNode) {
            if (lmbDown) {
                if (BpmnNode* rn = diagram.FindNode(state.primaryNodeId)) {
                    const Vec2 delta{
                        resp.mouseWorldPos.x - state.resizeStartWorld.x,
                        resp.mouseWorldPos.y - state.resizeStartWorld.y
                    };
                    const keel::f32 minW = (rn->type == BpmnNodeType::SubProcess && rn->isExpanded) ? 200.0f : 80.0f;
                    const keel::f32 minH = (rn->type == BpmnNodeType::SubProcess && rn->isExpanded) ? 120.0f : 50.0f;
                    keel::f32 left = state.resizeInitialBounds.x;
                    keel::f32 top = state.resizeInitialBounds.y;
                    keel::f32 right = state.resizeInitialBounds.Right();
                    keel::f32 bottom = state.resizeInitialBounds.Bottom();

                    if (state.resizeCorner == 0 || state.resizeCorner == 3) {
                        left = keel::min(right - minW, left + delta.x);
                        if (effOpts.snapToGrid && !mods.alt) left = keel::min(right - minW, SnapValue(left, effOpts.gridStep));
                    } else {
                        right = keel::max(left + minW, right + delta.x);
                        if (effOpts.snapToGrid && !mods.alt) right = keel::max(left + minW, SnapValue(right, effOpts.gridStep));
                    }
                    if (state.resizeCorner == 0 || state.resizeCorner == 1) {
                        top = keel::min(bottom - minH, top + delta.y);
                        if (effOpts.snapToGrid && !mods.alt) top = keel::min(bottom - minH, SnapValue(top, effOpts.gridStep));
                    } else {
                        bottom = keel::max(top + minH, bottom + delta.y);
                        if (effOpts.snapToGrid && !mods.alt) bottom = keel::max(top + minH, SnapValue(bottom, effOpts.gridStep));
                    }
                    rn->pos = {left, top};
                    rn->size = {right - left, bottom - top};
                    diagram.SyncBoundaryEvents();
                    resp.changed = true;
                    resp.diagramModified = true;
                    ctx.RequestRepaint(2);
                }
            } else {
                state.resizingNode = false;
                state.resizeCorner = -1;
            }
        }

        // Ongoing Orthogonal Edge Segment Drag (`ConnectionSegmentMove` + `BendpointMove`)
        if (state.draggingEdgeSegment) {
            if (lmbDown) {
                if (BpmnEdge* se = diagram.FindEdge(state.dragEdgeId)) {
                    const Vec2 delta{
                        resp.mouseWorldPos.x - state.dragEdgeStartWorld.x,
                        resp.mouseWorldPos.y - state.dragEdgeStartWorld.y
                    };
                    se->waypoints = state.dragEdgeInitWaypoints.clone();
                    const keel::i32 wpIdx0 = state.dragEdgeSegIdx - 1;
                    const keel::i32 wpIdx1 = state.dragEdgeSegIdx;
                    if (state.dragEdgeSegHorizontal) {
                        keel::f32 newY = ((wpIdx0 >= 0 && wpIdx0 < keel::as<keel::i32>(se->waypoints.size())) ? se->waypoints[keel::as<keel::usize>(wpIdx0)].y : resp.mouseWorldPos.y) + delta.y;
                        if (effOpts.snapToGrid && !mods.alt) newY = SnapValue(newY, effOpts.gridStep);
                        if (wpIdx0 >= 0 && wpIdx0 < keel::as<keel::i32>(se->waypoints.size())) se->waypoints[keel::as<keel::usize>(wpIdx0)].y = newY;
                        if (wpIdx1 >= 0 && wpIdx1 < keel::as<keel::i32>(se->waypoints.size())) se->waypoints[keel::as<keel::usize>(wpIdx1)].y = newY;
                    } else {
                        keel::f32 newX = ((wpIdx0 >= 0 && wpIdx0 < keel::as<keel::i32>(se->waypoints.size())) ? se->waypoints[keel::as<keel::usize>(wpIdx0)].x : resp.mouseWorldPos.x) + delta.x;
                        if (effOpts.snapToGrid && !mods.alt) newX = SnapValue(newX, effOpts.gridStep);
                        if (wpIdx0 >= 0 && wpIdx0 < keel::as<keel::i32>(se->waypoints.size())) se->waypoints[keel::as<keel::usize>(wpIdx0)].x = newX;
                        if (wpIdx1 >= 0 && wpIdx1 < keel::as<keel::i32>(se->waypoints.size())) se->waypoints[keel::as<keel::usize>(wpIdx1)].x = newX;
                    }
                    resp.changed = true;
                    resp.diagramModified = true;
                    ctx.RequestRepaint(2);
                }
            } else {
                state.draggingEdgeSegment = false;
                state.dragEdgeId = 0;
                state.dragEdgeSegIdx = -1;
            }
        }

        // Ongoing Edge Endpoint Reconnect (`Reconnect`)
        if (state.reconnectingEdge) {
            state.reconnectEndWorld = resp.mouseWorldPos;
            state.reconnectSnapNodeId = state.hoveredNodeId;
            if (state.reconnectSnapNodeId != 0) {
                if (const BpmnNode* target = diagram.FindNode(state.reconnectSnapNodeId)) {
                    state.reconnectEndWorld = target->Center();
                }
            }
            if (lmbDown) {
                ctx.RequestRepaint(2);
            } else {
                if (BpmnEdge* re = diagram.FindEdge(state.reconnectEdgeId)) {
                    if (state.reconnectSnapNodeId != 0) {
                        const keel::u32 otherId = (state.reconnectEndpoint == 1) ? re->toNodeId : re->fromNodeId;
                        if (state.reconnectSnapNodeId != otherId) {
                            state.PushUndo(diagram);
                            if (state.reconnectEndpoint == 1) {
                                re->fromNodeId = state.reconnectSnapNodeId;
                                re->fromPort = BpmnPortSide::Auto;
                            } else {
                                re->toNodeId = state.reconnectSnapNodeId;
                                re->toPort = BpmnPortSide::Auto;
                            }
                            re->waypoints.clear();
                            resp.changed = true;
                            resp.diagramModified = true;
                        }
                    }
                }
                state.reconnectingEdge = false;
                state.reconnectEdgeId = 0;
                state.reconnectEndpoint = 0;
                state.reconnectSnapNodeId = 0;
                ctx.RequestRepaint(2);
            }
        }

        // Ongoing External Label Dragging (`ExternalLabelMove`)
        if (state.draggingLabel) {
            if (lmbDown) {
                const Vec2 delta{
                    resp.mouseWorldPos.x - state.dragLabelStartWorld.x,
                    resp.mouseWorldPos.y - state.dragLabelStartWorld.y
                };
                if (state.dragLabelNodeId != 0) {
                    if (BpmnNode* ln = diagram.FindNode(state.dragLabelNodeId)) {
                        ln->labelOffset = {state.dragLabelInitOffset.x + delta.x, state.dragLabelInitOffset.y + delta.y};
                        resp.changed = true;
                        resp.diagramModified = true;
                        ctx.RequestRepaint(2);
                    }
                } else if (state.dragLabelEdgeId != 0) {
                    if (BpmnEdge* le = diagram.FindEdge(state.dragLabelEdgeId)) {
                        le->labelOffset = {state.dragLabelInitOffset.x + delta.x, state.dragLabelInitOffset.y + delta.y};
                        resp.changed = true;
                        resp.diagramModified = true;
                        ctx.RequestRepaint(2);
                    }
                }
            } else {
                state.draggingLabel = false;
                state.dragLabelNodeId = 0;
                state.dragLabelEdgeId = 0;
            }
        }

        // Ongoing Node Dragging + Smart Alignment Guides
        if (state.draggingNodes) {
            if (lmbDown) {
                const Vec2 delta{
                    resp.mouseWorldPos.x - state.dragStartWorld.x,
                    resp.mouseWorldPos.y - state.dragStartWorld.y
                };
                if (!state.dragUndoRecorded && (keel::abs(delta.x) > 2.0f || keel::abs(delta.y) > 2.0f)) {
                    BpmnDiagram beforeSnap = diagram.clone();
                    for (const auto& [nid, initPos] : state.dragInitialNodePos) {
                        if (BpmnNode* n = beforeSnap.FindNode(nid)) n->pos = initPos;
                    }
                    state.PushUndo(beforeSnap);
                    state.dragUndoRecorded = true;
                }

                state.activeGuides.clear();
                for (const auto& [nid, initPos] : state.dragInitialNodePos) {
                    BpmnNode* n = diagram.FindNode(nid);
                    if (!n) continue;
                    if (n->type == BpmnNodeType::BoundaryEvent && n->attachedToNodeId != 0) {
                        if (const BpmnNode* host = diagram.FindNode(n->attachedToNodeId)) {
                            const keel::f32 newCenterX = initPos.x + n->size.x * 0.5f + delta.x;
                            n->boundaryRatioX = keel::clamp((newCenterX - host->pos.x) / keel::max(1.0f, host->size.x), 0.15f, 0.85f);
                        }
                        continue;
                    }
                    Vec2 targetPos{initPos.x + delta.x, initPos.y + delta.y};
                    if (effOpts.snapToGrid && !mods.alt) {
                        const keel::f32 cx = SnapValue(targetPos.x + n->size.x * 0.5f, effOpts.gridStep);
                        const keel::f32 cy = SnapValue(targetPos.y + n->size.y * 0.5f, effOpts.gridStep);
                        targetPos.x = cx - n->size.x * 0.5f;
                        targetPos.y = cy - n->size.y * 0.5f;
                    }

                    if (effOpts.smartGuides && nid == state.primaryNodeId) {
                        const keel::f32 snapTol = 6.0f;
                        const keel::f32 myCX = targetPos.x + n->size.x * 0.5f;
                        const keel::f32 myCY = targetPos.y + n->size.y * 0.5f;
                        for (const auto& other : diagram.nodes) {
                            if (state.IsNodeSelected(other.id) || other.type == BpmnNodeType::BoundaryEvent) continue;
                            const Vec2 oc = other.Center();
                            if (keel::abs(myCX - oc.x) <= snapTol) {
                                targetPos.x = oc.x - n->size.x * 0.5f;
                                state.activeGuides.push_back(AlignmentGuide{
                                    true, oc.x,
                                    keel::min(targetPos.y, other.pos.y) - 20.0f,
                                    keel::max(targetPos.y + n->size.y, other.pos.y + other.size.y) + 20.0f
                                });
                            }
                            if (keel::abs(myCY - oc.y) <= snapTol) {
                                targetPos.y = oc.y - n->size.y * 0.5f;
                                state.activeGuides.push_back(AlignmentGuide{
                                    false, oc.y,
                                    keel::min(targetPos.x, other.pos.x) - 20.0f,
                                    keel::max(targetPos.x + n->size.x, other.pos.x + other.size.x) + 20.0f
                                });
                            }
                        }
                    }

                    n->pos = targetPos;
                }
                diagram.SyncBoundaryEvents();
                resp.changed = true;
                resp.diagramModified = true;
                ctx.RequestRepaint(2);
            } else {
                state.draggingNodes = false;
                state.activeGuides.clear();
            }
        }

        // Ongoing Lane Move / Resize
        if (state.draggingLane) {
            if (lmbDown) {
                if (BpmnLane* l = diagram.FindLane(state.selectedLaneId)) {
                    const Vec2 delta{
                        resp.mouseWorldPos.x - state.laneDragStartWorld.x,
                        resp.mouseWorldPos.y - state.laneDragStartWorld.y
                    };
                    if (state.laneResizeEdge == 1) {
                        l->rect.w = keel::max(320.0f, SnapValue(state.laneInitialRect.w + delta.x, effOpts.gridStep));
                    } else if (state.laneResizeEdge == 2) {
                        l->rect.h = keel::max(110.0f, SnapValue(state.laneInitialRect.h + delta.y, effOpts.gridStep));
                    } else {
                        l->rect.x = SnapValue(state.laneInitialRect.x + delta.x, effOpts.gridStep);
                        l->rect.y = SnapValue(state.laneInitialRect.y + delta.y, effOpts.gridStep);
                        const Vec2 appliedDelta{l->rect.x - state.laneInitialRect.x, l->rect.y - state.laneInitialRect.y};
                        for (const auto& [nid, initPos] : state.laneContainedNodeOffsets) {
                            if (BpmnNode* cn = diagram.FindNode(nid)) {
                                cn->pos = {initPos.x + appliedDelta.x, initPos.y + appliedDelta.y};
                            }
                        }
                        diagram.SyncBoundaryEvents();
                    }
                    resp.changed = true;
                    resp.diagramModified = true;
                    ctx.RequestRepaint(2);
                }
            } else {
                state.draggingLane = false;
            }
        }

        // Ongoing Space Tool (`S`) Dragging
        if (state.spaceToolDragging) {
            if (lmbDown) {
                state.spaceCurrentWorld = resp.mouseWorldPos;
                const keel::f32 dx = state.spaceCurrentWorld.x - state.spaceStartWorld.x;
                const keel::f32 dy = state.spaceCurrentWorld.y - state.spaceStartWorld.y;
                if (!state.spaceToolAxisLocked && (keel::abs(dx) > 6.0f || keel::abs(dy) > 6.0f)) {
                    state.spaceToolAxisLocked = true;
                    state.spaceToolHorizontal = (keel::abs(dx) >= keel::abs(dy));
                }
                if (state.spaceToolAxisLocked) {
                    const keel::f32 shift = effOpts.snapToGrid
                        ? SnapValue(state.spaceToolHorizontal ? dx : dy, effOpts.gridStep)
                        : (state.spaceToolHorizontal ? dx : dy);
                    for (const auto& [nid, initPos] : state.spaceInitialNodes) {
                        BpmnNode* n = diagram.FindNode(nid);
                        if (!n || n->type == BpmnNodeType::BoundaryEvent) continue;
                        if (state.spaceToolHorizontal) {
                            n->pos.x = (initPos.x >= state.spaceStartWorld.x) ? (initPos.x + shift) : initPos.x;
                        } else {
                            n->pos.y = (initPos.y >= state.spaceStartWorld.y) ? (initPos.y + shift) : initPos.y;
                        }
                    }
                    for (const auto& [lid, initR] : state.spaceInitialLanes) {
                        BpmnLane* l = diagram.FindLane(lid);
                        if (!l) continue;
                        if (state.spaceToolHorizontal) {
                            if (initR.x < state.spaceStartWorld.x && initR.Right() > state.spaceStartWorld.x) {
                                l->rect.w = keel::max(240.0f, initR.w + shift);
                            } else if (initR.x >= state.spaceStartWorld.x) {
                                l->rect.x = initR.x + shift;
                            }
                        } else {
                            if (initR.y < state.spaceStartWorld.y && initR.Bottom() > state.spaceStartWorld.y) {
                                l->rect.h = keel::max(100.0f, initR.h + shift);
                            } else if (initR.y >= state.spaceStartWorld.y) {
                                l->rect.y = initR.y + shift;
                            }
                        }
                    }
                    diagram.SyncBoundaryEvents();
                    resp.changed = true;
                    resp.diagramModified = true;
                    ctx.RequestRepaint(2);
                }
            } else {
                state.spaceToolDragging = false;
                state.activeTool = BpmnTool::Select;
            }
        }

        // Ongoing Lasso Selection
        if (state.lassoActive) {
            if (lmbDown) {
                state.lassoEndWorld = resp.mouseWorldPos;
                const Rect box(
                    keel::min(state.lassoStartWorld.x, state.lassoEndWorld.x),
                    keel::min(state.lassoStartWorld.y, state.lassoEndWorld.y),
                    keel::abs(state.lassoEndWorld.x - state.lassoStartWorld.x),
                    keel::abs(state.lassoEndWorld.y - state.lassoStartWorld.y)
                );
                state.selectedNodes.clear();
                for (const auto& n : diagram.nodes) {
                    if (box.Intersects(n.Bounds())) {
                        state.selectedNodes.push_back(n.id);
                    }
                }
                state.primaryNodeId = state.selectedNodes.empty() ? 0 : state.selectedNodes.back();
                ctx.RequestRepaint(2);
            } else {
                state.lassoActive = false;
                if (state.activeTool == BpmnTool::Lasso) {
                    state.activeTool = BpmnTool::Select;
                }
            }
        }

        // Ongoing Wire Connection
        if (state.connectingWire) {
            state.connectEndWorld = resp.mouseWorldPos;
            state.connectSnapNodeId = (state.hoveredNodeId != 0 && state.hoveredNodeId != state.connectFromNodeId)
                ? state.hoveredNodeId
                : 0;
            if (state.connectSnapNodeId != 0) {
                if (const BpmnNode* target = diagram.FindNode(state.connectSnapNodeId)) {
                    state.connectEndWorld = target->Center();
                }
            }
            if (lmbReleased || (lmbClicked && state.connectSnapNodeId != 0)) {
                if (state.connectSnapNodeId != 0) {
                    state.PushUndo(diagram);
                    const BpmnNode* src = diagram.FindNode(state.connectFromNodeId);
                    const BpmnNode* dst = diagram.FindNode(state.connectSnapNodeId);
                    BpmnFlowType flow = BpmnFlowType::SequenceFlow;
                    if (src && dst) {
                        if (src->type == BpmnNodeType::DataObject || src->type == BpmnNodeType::DataStore || src->type == BpmnNodeType::TextAnnotation ||
                            dst->type == BpmnNodeType::DataObject || dst->type == BpmnNodeType::DataStore || dst->type == BpmnNodeType::TextAnnotation) {
                            flow = BpmnFlowType::Association;
                        } else if (src->laneId != 0 && dst->laneId != 0 && src->laneId != dst->laneId) {
                            const BpmnLane* l1 = diagram.FindLane(src->laneId);
                            const BpmnLane* l2 = diagram.FindLane(dst->laneId);
                            if (l1 && l2 && l1->poolName != l2->poolName) {
                                flow = BpmnFlowType::MessageFlow;
                            }
                        }
                    }
                    const keel::u32 eid = diagram.AddEdge(state.connectFromNodeId, state.connectSnapNodeId, flow);
                    state.ClearSelection();
                    state.selectedEdgeId = eid;
                    resp.changed = true;
                    resp.diagramModified = true;
                }
                state.connectingWire = false;
                state.connectFromNodeId = 0;
                state.connectSnapNodeId = 0;
                if (state.activeTool == BpmnTool::GlobalConnect) {
                    state.activeTool = BpmnTool::Select;
                }
                ctx.RequestRepaint(2);
            }
        }

        // K. Keyboard Shortcuts when focused or hovered
        if ((ctx.IsFocused(id) || hovered) && state.editingNodeId == 0 && state.editingEdgeId == 0 && state.editingLaneId == 0 && !effOpts.readOnly) {
            if (ctx.ConsumeShortcut(Key::Z, true, false)) {
                if (state.Undo(diagram)) { resp.changed = true; resp.diagramModified = true; }
            } else if (ctx.ConsumeShortcut(Key::Y, true, false) || ctx.ConsumeShortcut(Key::Z, true, true)) {
                if (state.Redo(diagram)) { resp.changed = true; resp.diagramModified = true; }
            } else if (ctx.ConsumeShortcut(Key::A, true, false)) {
                state.selectedNodes.clear();
                for (const auto& n : diagram.nodes) state.selectedNodes.push_back(n.id);
                state.primaryNodeId = state.selectedNodes.empty() ? 0 : state.selectedNodes.back();
                resp.changed = true;
            } else if (ctx.ConsumeShortcut(Key::C, true, false)) {
                state.clipboardNodes.clear();
                state.clipboardEdges.clear();
                for (keel::u32 nid : state.selectedNodes) {
                    if (const BpmnNode* n = diagram.FindNode(nid)) state.clipboardNodes.push_back(*n);
                }
                for (const auto& e : diagram.edges) {
                    if (state.IsNodeSelected(e.fromNodeId) && state.IsNodeSelected(e.toNodeId)) {
                        state.clipboardEdges.push_back(e.clone());
                    }
                }
            } else if (ctx.ConsumeShortcut(Key::V, true, false) && !state.clipboardNodes.empty()) {
                state.PushUndo(diagram);
                keel::HashMap<keel::u32, keel::u32> idMap;
                state.selectedNodes.clear();
                for (BpmnNode copy : state.clipboardNodes) {
                    const keel::u32 oldId = copy.id;
                    copy.id = 0;
                    copy.pos.x += 40.0f;
                    copy.pos.y += 40.0f;
                    const keel::u32 newId = diagram.AddNode(keel::move(copy));
                    idMap.put(oldId, newId);
                    state.selectedNodes.push_back(newId);
                }
                for (const auto& e : state.clipboardEdges) {
                    const keel::u32* fromNew = idMap.get(e.fromNodeId);
                    const keel::u32* toNew = idMap.get(e.toNodeId);
                    if (fromNew && toNew) {
                        diagram.AddEdge(*fromNew, *toNew, e.flowType, e.label, e.fromPort, e.toPort);
                    }
                }
                state.primaryNodeId = state.selectedNodes.empty() ? 0 : state.selectedNodes.back();
                resp.changed = true;
                resp.diagramModified = true;
            } else if (ctx.ConsumeShortcut(Key::D, true, false) && !state.selectedNodes.empty()) {
                state.PushUndo(diagram);
                keel::Array<keel::u32> newSel;
                for (keel::u32 nid : state.selectedNodes) {
                    if (const BpmnNode* n = diagram.FindNode(nid)) {
                        BpmnNode copy = *n;
                        copy.id = 0;
                        copy.pos.x += 40.0f;
                        copy.pos.y += 40.0f;
                        newSel.push_back(diagram.AddNode(keel::move(copy)));
                    }
                }
                state.selectedNodes = keel::move(newSel);
                state.primaryNodeId = state.selectedNodes.empty() ? 0 : state.selectedNodes.back();
                resp.changed = true;
                resp.diagramModified = true;
            } else if (ctx.ConsumeKey(Key::Delete) || ctx.ConsumeKey(Key::Backspace)) {
                if (!state.selectedNodes.empty()) {
                    state.PushUndo(diagram);
                    const auto toDel = state.selectedNodes.clone();
                    for (keel::u32 nid : toDel) diagram.RemoveNode(nid);
                    state.ClearSelection();
                    resp.changed = true;
                    resp.diagramModified = true;
                } else if (state.selectedEdgeId != 0) {
                    state.PushUndo(diagram);
                    diagram.RemoveEdge(state.selectedEdgeId);
                    state.ClearSelection();
                    resp.changed = true;
                    resp.diagramModified = true;
                } else if (state.selectedLaneId != 0) {
                    state.PushUndo(diagram);
                    diagram.RemoveLane(state.selectedLaneId);
                    state.ClearSelection();
                    resp.changed = true;
                    resp.diagramModified = true;
                }
            } else if (ctx.ConsumeKey(Key::Escape)) {
                state.activeTool = BpmnTool::Select;
                state.connectingWire = false;
                state.reconnectingEdge = false;
                state.resizingNode = false;
                state.draggingEdgeSegment = false;
                state.draggingLabel = false;
                state.paletteDragCreating = false;
                state.replaceMenuOpen = false;
                state.ClearSelection();
            } else if (!mods.ctrl && !mods.alt) {
                if (ctx.ConsumeKey(Key::V)) state.activeTool = BpmnTool::Select;
                else if (ctx.ConsumeKey(Key::H)) state.activeTool = BpmnTool::Hand;
                else if (ctx.ConsumeKey(Key::C)) state.activeTool = BpmnTool::GlobalConnect;
                const auto typed = ctx.ConsumePendingTextInput();
                for (const auto& sIn : typed) {
                    if (sIn == "v" || sIn == "V") state.activeTool = BpmnTool::Select;
                    else if (sIn == "h" || sIn == "H") state.activeTool = BpmnTool::Hand;
                    else if (sIn == "l" || sIn == "L") state.activeTool = BpmnTool::Lasso;
                    else if (sIn == "s" || sIn == "S") state.activeTool = BpmnTool::Space;
                    else if (sIn == "c" || sIn == "C") state.activeTool = BpmnTool::GlobalConnect;
                }
            }
        }
    }

    resp.pan = state.pan;
    resp.zoom = state.zoom;
    resp.activeTool = state.activeTool;
    resp.selectedNodeId = state.primaryNodeId;
    resp.selectedEdgeId = state.selectedEdgeId;
    resp.selectedLaneId = state.selectedLaneId;
    resp.selectedNodeCount = state.selectedNodes.size();
    resp.hoveredNodeId = state.hoveredNodeId;
    resp.hoveredEdgeId = state.hoveredEdgeId;
    resp.simulationActive = effOpts.tokenSimulation;
    resp.activeTokenCount = diagram.tokens.size();
    resp.undoDepth = state.undoStack.size();
    resp.redoDepth = state.redoStack.size();

    if (opts.selectedNodeIdPtr) *opts.selectedNodeIdPtr = state.primaryNodeId;
    if (opts.selectedEdgeIdPtr) *opts.selectedEdgeIdPtr = state.selectedEdgeId;
    if (opts.selectedLaneIdPtr) *opts.selectedLaneIdPtr = state.selectedLaneId;

    if (resp.hovered && !opts.tooltip.empty()) {
        ctx.SetTooltip(opts.tooltip, baseRes.rect);
    }

    state.renderSnapshot = diagram.clone();

    Node* node = ctx.CreateNode(keel::from_underlying<NodeType>(keel::as<keel::u16>(NodeType::User) + 20u), id);
    auto* payload = ctx.AllocPayload<BpmnEditorPayload>();
    payload->opts = effOpts;
    payload->opts.tooltip = ctx.AllocText(opts.tooltip);
    payload->resp = resp;
    payload->diagramSnapshot = &state.renderSnapshot;
    payload->stateSnapshot = &state;
    payload->activeEditBuffer = ctx.AllocString(activeEditStr);
    payload->activeEditCursor = activeEditCursor;
    payload->cursorBlinkOn = ctx.IsCursorVisible();
    payload->animPhase = keel::as<keel::f32>(keel::fmod(nowSec * 0.85, 1.0));

    node->text = "BpmnEditor";
    node->SetPayload(payload);

    keel::u64 graphHash = detail::HashArgs(
        diagram.name,
        diagram.lanes.size(),
        diagram.nodes.size(),
        diagram.edges.size(),
        diagram.tokens.size()
    );
    for (const auto& l : diagram.lanes) {
        graphHash = detail::HashArgs(graphHash, l.id, l.name, l.poolName, l.rect.x, l.rect.y, l.rect.w, l.rect.h);
    }
    for (const auto& n : diagram.nodes) {
        graphHash = detail::HashArgs(
            graphHash, n.id, keel::as<keel::u8>(n.type), n.label, n.documentation,
            n.pos.x, n.pos.y, n.size.x, n.size.y,
            keel::as<keel::u8>(n.taskType), keel::as<keel::u8>(n.gatewayType), keel::as<keel::u8>(n.eventDef),
            n.nonInterrupting, n.isExpanded, n.labelOffset.x, n.labelOffset.y,
            n.activeOutgoingEdgeId, n.tokenExecutionCount, n.activeTokensInside,
            n.slaDurationMs, n.loopMarker, n.parallelMultiInstance
        );
    }
    for (const auto& e : diagram.edges) {
        graphHash = detail::HashArgs(
            graphHash, e.id, e.fromNodeId, e.toNodeId, keel::as<keel::u8>(e.flowType), e.label,
            e.labelOffset.x, e.labelOffset.y, e.waypoints.size()
        );
        for (const auto& wp : e.waypoints) {
            graphHash = detail::HashArgs(graphHash, wp.x, wp.y);
        }
    }
    for (const auto& t : diagram.tokens) {
        graphHash = detail::HashArgs(graphHash, t.id, t.edgeId, t.nodeId, keel::as<keel::i32>(t.progress * 200.0f));
    }

    node->visualHash = detail::HashArgs(
        graphHash,
        keel::as<keel::u8>(payload->opts.routingMode),
        keel::as<keel::u8>(payload->opts.themePreset),
        payload->opts.showLeftPalette,
        payload->opts.showTopBar,
        payload->opts.showContextPad,
        payload->opts.showMinimap,
        payload->opts.showZoomControls,
        payload->opts.showGrid,
        payload->opts.gridStep,
        payload->opts.tokenSimulation,
        payload->opts.showSlaHeatmap,
        payload->opts.background,
        payload->opts.border,
        state.pan.x,
        state.pan.y,
        state.zoom,
        keel::as<keel::u8>(state.activeTool),
        state.primaryNodeId,
        state.selectedEdgeId,
        state.selectedLaneId,
        state.selectedNodes.size(),
        state.hoveredNodeId,
        state.hoveredEdgeId,
        state.hoveredLabelNodeId,
        state.hoveredLabelEdgeId,
        state.hoveredResizeCorner,
        state.hoveredEdgeSegIdx,
        state.hoveredEdgeEndpoint,
        state.hoveredPaletteIdx,
        state.hoveredContextPadBtn,
        state.hoveredReplaceMenuItem,
        state.replaceMenuOpen,
        state.connectingWire,
        state.reconnectingEdge,
        state.connectEndWorld.x,
        state.connectEndWorld.y,
        state.ghostSnappedPos.x,
        state.ghostSnappedPos.y,
        state.lassoActive,
        state.lassoEndWorld.x,
        state.lassoEndWorld.y,
        state.spaceToolDragging,
        state.spaceCurrentWorld.x,
        state.spaceCurrentWorld.y,
        state.editingNodeId,
        state.editingEdgeId,
        state.editingLaneId,
        payload->activeEditBuffer,
        payload->cursorBlinkOn,
        effOpts.tokenSimulation ? keel::as<keel::i32>(payload->animPhase * 60.0f) : 0,
        viewRect.w,
        viewRect.h,
        effectiveSize,
        isDisabled
    );

    node->measureFn = [](const Node* n, keel::f32 availableWidth, ICanvas* c, const UiTheme& th) noexcept -> Vec2 {
        const auto* p = n->As<BpmnEditorPayload>();
        if (!p) return {0.0f, 0.0f};
        BpmnEditorOptions effOpts = p->opts;
        if (n->widthMode == SizeMode::Stretch && !effOpts.width.IsFixed()) {
            effOpts.width = SizeMode::Stretch;
        }
        return BpmnEditor::Measure(effOpts, n->uiSize, c, th, availableWidth);
    };
    node->renderFn = &RenderBpmnEditorNode;

    node->uiSize = effectiveSize;
    node->disabled = isDisabled;
    node->hovered = resp.hovered;
    node->pressed = baseRes.pressed;

    ControlSpec ctrlSpec{};
    ctrlSpec.width = opts.width;
    ctrlSpec.height = opts.height;
    ctrlSpec.defaultWidthMode = SizeMode::Stretch;
    ctrlSpec.defaultHeightMode = SizeMode::Stretch;
    ctrlSpec.minHeight = opts.height.IsFixed() ? opts.height.Dp() : 320.0f;
    Control::SetupNode(ctx, node, ctrlSpec);

    return resp;
}


} // namespace guikit
