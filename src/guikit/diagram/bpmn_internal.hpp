#pragma once

#include "guikit/diagram/bpmn_editor.hpp"
#include "guikit/context.hpp"
#include "guikit/core/control.hpp"
#include "keel/algo.h"
#include "keel/array.h"
#include "keel/format.h"
#include "keel/hash_map.h"
#include "keel/math.h"
#include "keel/number.h"
#include "keel/small_array.h"
#include "keel/static_array.h"
#include "keel/string.h"


namespace guikit::detail::bpmn_impl {

struct BpmnPalette {
    Color canvasBg{};
    Color canvasBorder{};
    Color gridDot{};
    Color gridMajor{};
    Color laneBgEven{};
    Color laneBgOdd{};
    Color poolHeaderBg{};
    Color laneHeaderBg{};
    Color laneBorder{};
    Color laneText{};

    Color nodeFill{};
    Color nodeStroke{};
    Color nodeShadow{};
    Color textPrimary{};
    Color textSecondary{};
    Color textMuted{};

    Color startEventFill{};
    Color startEventStroke{};
    Color interEventFill{};
    Color interEventStroke{};
    Color endEventFill{};
    Color endEventStroke{};
    Color gatewayFill{};
    Color gatewayStroke{};
    Color taskAccent{};
    Color annotationBg{};
    Color dataFill{};

    Color edgeStroke{};
    Color edgeMessage{};
    Color edgeLabelBg{};

    Color selectAccent{};
    Color selectGlow{};
    Color hoverAccent{};
    Color guideLaser{};

    Color hudBg{};
    Color hudBorder{};
    Color hudHover{};
    Color hudActiveBg{};
    Color simActiveGreen{};
};


[[nodiscard]] BpmnPalette ResolveBpmnPalette(BpmnThemePreset preset, const UiTheme& theme) noexcept;

struct AlignmentGuide {
    bool vertical{true};
    keel::f32 worldCoord{0.0f};
    keel::f32 worldSpanMin{0.0f};
    keel::f32 worldSpanMax{0.0f};
};

struct NodePosEntry {
    keel::u32 id{0};
    Vec2 pos{0.0f, 0.0f};
};

struct LaneRectEntry {
    keel::u32 id{0};
    Rect rect{};
};

struct BpmnEditorState {
    bool initialized{false};
    keel::usize lastDiagramNodeCount{0};
    keel::u32 lastFirstNodeId{0};

    Vec2 pan{60.0f, 44.0f};
    keel::f32 zoom{1.0f};

    BpmnTool activeTool{BpmnTool::Select};

    // Selection state
    keel::Array<keel::u32> selectedNodes{};
    keel::u32 primaryNodeId{0};
    keel::u32 selectedEdgeId{0};
    keel::u32 selectedLaneId{0};

    keel::u32 hoveredNodeId{0};
    keel::u32 hoveredEdgeId{0};
    keel::u32 hoveredLaneId{0};
    keel::u32 hoveredLabelNodeId{0};
    keel::u32 hoveredLabelEdgeId{0};

    keel::i32 hoveredPaletteIdx{-1};
    keel::i32 hoveredContextPadBtn{-1};
    keel::i32 hoveredReplaceMenuItem{-1};
    keel::i32 hoveredResizeCorner{-1};   // 0=NW, 1=NE, 2=SE, 3=SW
    keel::i32 hoveredEdgeSegIdx{-1};     // Hovered orthogonal segment index on selected/hovered edge
    keel::i32 hoveredEdgeEndpoint{0};    // 0=none, 1=source, 2=target

    // Replace Menu (`🔧 Change Type`) popover
    bool replaceMenuOpen{false};

    // Dragging state
    bool panningCanvas{false};
    Vec2 panStartMouse{0.0f, 0.0f};
    Vec2 panStartOffset{0.0f, 0.0f};

    bool draggingNodes{false};
    bool dragUndoRecorded{false};
    Vec2 dragStartWorld{0.0f, 0.0f};
    keel::Array<NodePosEntry> dragInitialNodePos{};
    keel::u32 dragBoundaryHostId{0};

    // Node corner resize (`Task`, `SubProcess`, `TextAnnotation`)
    bool resizingNode{false};
    keel::i32 resizeCorner{-1};
    Vec2 resizeStartWorld{0.0f, 0.0f};
    Rect resizeInitialBounds{};

    // External label dragging (`Event`, `Gateway`, `DataObject`, `DataStore`, `BpmnEdge`)
    bool draggingLabel{false};
    keel::u32 dragLabelNodeId{0};
    keel::u32 dragLabelEdgeId{0};
    Vec2 dragLabelStartWorld{0.0f, 0.0f};
    Vec2 dragLabelInitOffset{0.0f, 0.0f};

    // Orthogonal Edge Segment & Endpoint Reconnect dragging (`ConnectionSegmentMove` / `BendpointMove`)
    bool draggingEdgeSegment{false};
    keel::u32 dragEdgeId{0};
    keel::i32 dragEdgeSegIdx{-1};
    bool dragEdgeSegHorizontal{true};
    Vec2 dragEdgeStartWorld{0.0f, 0.0f};
    keel::Array<Vec2> dragEdgeInitWaypoints{};

    bool reconnectingEdge{false};
    keel::u32 reconnectEdgeId{0};
    keel::i32 reconnectEndpoint{0}; // 1=fromNodeId, 2=toNodeId
    Vec2 reconnectEndWorld{0.0f, 0.0f};
    keel::u32 reconnectSnapNodeId{0};

    // Palette Drag-to-Create + Live Ghost Preview Smart Alignment
    bool paletteDragCreating{false};
    Vec2 ghostSnappedPos{0.0f, 0.0f};

    bool draggingLane{false};
    keel::i32 laneResizeEdge{0}; // 0 = move, 1 = right edge, 2 = bottom edge
    Vec2 laneDragStartWorld{0.0f, 0.0f};
    Rect laneInitialRect{};
    keel::Array<NodePosEntry> laneContainedNodeOffsets{};

    // Lasso box selection
    bool lassoActive{false};
    Vec2 lassoStartWorld{0.0f, 0.0f};
    Vec2 lassoEndWorld{0.0f, 0.0f};

    // Space Tool (`S`)
    bool spaceToolDragging{false};
    bool spaceToolAxisLocked{false};
    bool spaceToolHorizontal{true};
    Vec2 spaceStartWorld{0.0f, 0.0f};
    Vec2 spaceCurrentWorld{0.0f, 0.0f};
    keel::Array<NodePosEntry> spaceInitialNodes{};
    keel::Array<LaneRectEntry> spaceInitialLanes{};

    // Interactive wire connection
    bool connectingWire{false};
    keel::u32 connectFromNodeId{0};
    BpmnPortSide connectFromPort{BpmnPortSide::Auto};
    Vec2 connectEndWorld{0.0f, 0.0f};
    keel::u32 connectSnapNodeId{0};

    // Inline text editing (double-click)
    keel::u32 editingNodeId{0};
    keel::u32 editingEdgeId{0};
    keel::u32 editingLaneId{0};

    // Smart alignment guides active during node drag or ghost creation preview
    keel::Array<AlignmentGuide> activeGuides{};

    // Undo / Redo stacks
    keel::Array<BpmnDiagram> undoStack{};
    keel::Array<BpmnDiagram> redoStack{};
    BpmnDiagram renderSnapshot{};

    // Clipboard for Copy / Paste
    keel::Array<BpmnNode> clipboardNodes{};
    keel::Array<BpmnEdge> clipboardEdges{};

    // Token simulation timing
    keel::f64 lastSimTickSec{0.0};
    keel::f64 spawnCooldownSec{0.0};
    keel::u32 nextTokenId{1};
    keel::u32 nextInstanceId{1};
    bool simPaused{false};

    [[nodiscard]] bool IsNodeSelected(keel::u32 id) const noexcept {
        if (id == 0) return false;
        if (primaryNodeId == id) return true;
        for (keel::u32 selId : selectedNodes) {
            if (selId == id) return true;
        }
        return false;
    }

    void SelectSingleNode(keel::u32 id) {
        selectedNodes.clear();
        if (id != 0) selectedNodes.push_back(id);
        primaryNodeId = id;
        selectedEdgeId = 0;
        selectedLaneId = 0;
        replaceMenuOpen = false;
    }

    void ToggleNodeSelection(keel::u32 id) {
        if (id == 0) return;
        bool removed = false;
        for (keel::usize i = 0; i < selectedNodes.size(); ++i) {
            if (selectedNodes[i] == id) {
                selectedNodes.remove_at(i);
                primaryNodeId = selectedNodes.empty() ? 0 : selectedNodes.back();
                removed = true;
                break;
            }
        }
        if (!removed) {
            selectedNodes.push_back(id);
            primaryNodeId = id;
        }
        selectedEdgeId = 0;
        selectedLaneId = 0;
        replaceMenuOpen = false;
    }

    void ClearSelection() noexcept {
        selectedNodes.clear();
        primaryNodeId = 0;
        selectedEdgeId = 0;
        selectedLaneId = 0;
        replaceMenuOpen = false;
    }

    void PushUndo(const BpmnDiagram& d) {
        if (undoStack.size() >= 24) {
            undoStack.remove_at(0);
        }
        undoStack.push_back(d.clone());
        redoStack.clear();
    }

    void TrimInactiveCaches() noexcept {
        renderSnapshot = BpmnDiagram{};
        dragInitialNodePos = {};
        dragEdgeInitWaypoints = {};
        laneContainedNodeOffsets = {};
        spaceInitialNodes = {};
        spaceInitialLanes = {};
        activeGuides = {};
    }

    bool Undo(BpmnDiagram& d) {
        if (undoStack.empty()) return false;
        redoStack.push_back(d.clone());
        d = keel::move(undoStack.back());
        undoStack.pop_back();
        ClearSelection();
        return true;
    }

    bool Redo(BpmnDiagram& d) {
        if (redoStack.empty()) return false;
        undoStack.push_back(d.clone());
        d = keel::move(redoStack.back());
        redoStack.pop_back();
        ClearSelection();
        return true;
    }
};

struct BpmnEditorPayload {
    BpmnEditorOptions opts{};
    BpmnEditorResponse resp{};
    const BpmnDiagram* diagramSnapshot{nullptr};
    const BpmnEditorState* stateSnapshot{nullptr};
    keel::StringRef activeEditBuffer{};
    keel::usize activeEditCursor{0};
    bool cursorBlinkOn{true};
    keel::f32 animPhase{0.0f};
};


struct PaletteItemDesc {
    BpmnTool tool;
    keel::StringRef shortTag;
    keel::StringRef tooltip;
    bool separatorAfter{false};
};

inline constexpr keel::StaticArray<PaletteItemDesc, 15> kPaletteItems = {{
    {BpmnTool::Select,                  "V",   "Activate the select / move tool (V)"},
    {BpmnTool::Hand,                    "H",   "Activate the hand tool (H)"},
    {BpmnTool::Lasso,                   "L",   "Activate the lasso tool (L)"},
    {BpmnTool::Space,                   "S",   "Activate the create/remove space tool (S)"},
    {BpmnTool::GlobalConnect,           "C",   "Activate the global connect tool (C)", true},
    {BpmnTool::CreateStartEvent,        "○",   "Create StartEvent"},
    {BpmnTool::CreateIntermediateEvent, "◎",   "Create Intermediate/Boundary Event"},
    {BpmnTool::CreateEndEvent,          "●",   "Create EndEvent", true},
    {BpmnTool::CreateGateway,           "◇",   "Create Gateway"},
    {BpmnTool::CreateTask,              "▭",   "Create Task"},
    {BpmnTool::CreateSubProcess,        "⊞",   "Create expanded/collapsed Sub-Process", true},
    {BpmnTool::CreateDataObject,        "▯",   "Create DataObjectReference"},
    {BpmnTool::CreateDataStore,         "⛁",   "Create DataStoreReference"},
    {BpmnTool::CreateAnnotation,        "[T",  "Create TextAnnotation"},
    {BpmnTool::CreateLane,              "☰",   "Create Pool/Participant"}
}};

inline constexpr keel::StaticArray<keel::StringRef, 8> kContextPadTooltips = {{
    "Append Task",
    "Append Gateway",
    "Append EndEvent",
    "Append Intermediate/Boundary Event",
    "Connect using Sequence/Message/Association",
    "Change element type (Replace Menu)",
    "Append TextAnnotation",
    "Remove element (Del)"
}};

[[nodiscard]] inline keel::ArrayRef<keel::i32> GetContextPadActionsForNode(const BpmnNode& node) noexcept {
    static constexpr keel::i32 kEndActions[] = {4, 5, 6, 7};
    static constexpr keel::i32 kArtifactActions[] = {4, 6, 7};
    static constexpr keel::i32 kDefaultActions[] = {0, 1, 2, 3, 4, 5, 6, 7};
    switch (node.type) {
        case BpmnNodeType::EndEvent:
            // In BPMN 2.0, EndEvent cannot have outgoing SequenceFlow
            return kEndActions;
        case BpmnNodeType::DataObject:
        case BpmnNodeType::DataStore:
        case BpmnNodeType::TextAnnotation:
            // Artifacts only support Association connect, Annotation append, and Delete
            return kArtifactActions;
        default:
            return kDefaultActions;
    }
}


// Layout rect helpers
[[nodiscard]] Rect ComputeLeftPaletteRect(const Rect& viewRect, bool showTopBar, keel::f32 s) noexcept;
[[nodiscard]] Rect ComputeTopBarRect(const Rect& viewRect, keel::f32 s) noexcept;
[[nodiscard]] Rect ComputeMinimapRect(const Rect& viewRect, keel::f32 s) noexcept;
[[nodiscard]] Rect ComputeZoomHudRect(const Rect& viewRect, keel::f32 s) noexcept;

// Coordinate & orthogonal routing helpers
[[nodiscard]] Vec2 WorldToScreen(Vec2 w, const Rect& viewRect, Vec2 pan, keel::f32 zoom) noexcept;
[[nodiscard]] Vec2 ScreenToWorld(Vec2 s, const Rect& viewRect, Vec2 pan, keel::f32 zoom) noexcept;
[[nodiscard]] Rect WorldToScreenRect(const Rect& wr, const Rect& viewRect, Vec2 pan, keel::f32 zoom) noexcept;
[[nodiscard]] keel::f32 SnapValue(keel::f32 v, keel::f32 step) noexcept;
[[nodiscard]] Rect ComputeDiagramWorldBounds(const BpmnDiagram& diagram) noexcept;
void FitDiagramToViewport(const BpmnDiagram& diagram, const Rect& viewRect, keel::f32 leftInset, BpmnEditorState& state) noexcept;
[[nodiscard]] Vec2 GetNodePortWorldPos(const BpmnNode& node, BpmnPortSide side) noexcept;
[[nodiscard]] Vec2 PortOutwardNormal(BpmnPortSide side) noexcept;
void ResolveAutoPorts(
    const BpmnNode& fromNode,
    const BpmnNode& toNode,
    BpmnPortSide reqFrom,
    BpmnPortSide reqTo,
    BpmnPortSide& outFrom,
    BpmnPortSide& outTo
) noexcept;
void SimplifyOrthogonalPoints(keel::SmallArray<Vec2, 32>& pts);
[[nodiscard]] keel::SmallArray<Vec2, 32> BuildEdgeWorldPoints(
    const BpmnDiagram& diagram,
    const BpmnNode& fromNode,
    const BpmnNode& toNode,
    const BpmnEdge& edge,
    BpmnRoutingMode routingMode
);
[[nodiscard]] keel::SmallArray<Vec2, 64> BuildRoundedPolyline(keel::ArrayRef<Vec2> pts, keel::f32 cornerRadius);
[[nodiscard]] keel::f32 PointToSegmentDistanceSq(Vec2 p, Vec2 a, Vec2 b) noexcept;
[[nodiscard]] Vec2 SamplePolylineAtProgress(
    keel::ArrayRef<Vec2> pts,
    keel::f32 progress,
    Vec2* outMidpoint = nullptr,
    bool* outHorizontalSeg = nullptr
) noexcept;

// Token simulation helpers
void SpawnTokenFromStartEvent(BpmnDiagram& diagram, const BpmnNode& startNode, BpmnEditorState& state);
void StepTokenSimulation(BpmnDiagram& diagram, BpmnEditorState& state, keel::f32 dtSec, keel::f32 speedMultiplier, bool autoSpawn);

// Rendering & label/handle geometry helpers
[[nodiscard]] Rect ComputeExternalNodeLabelScreenRect(
    const BpmnDiagram& diagram,
    const BpmnNode& node,
    const Rect& sr,
    keel::f32 zoom,
    keel::f32 s,
    keel::f32* outAlignX = nullptr,
    keel::f32* outAlignY = nullptr
) noexcept;
[[nodiscard]] bool IsExternalLabelNode(BpmnNodeType type) noexcept;
[[nodiscard]] bool IsResizableNode(BpmnNodeType type) noexcept;
[[nodiscard]] keel::StaticArray<Rect, 4> GetNodeResizeHandleRects(const Rect& sr, keel::f32 zoom, keel::f32 s) noexcept;
[[nodiscard]] Rect ComputeEdgeLabelScreenRect(
    ICanvas* canvas,
    keel::ArrayRef<Vec2> rawScreenPts,
    const BpmnEdge& edge,
    keel::f32 zoom,
    keel::f32 s
) noexcept;
void RenderBpmnEditorNode(const Node* node, ICanvas& canvas, const UiTheme& theme);

} // namespace guikit::detail::bpmn_impl
