#include "guikit/guikit.hpp"
#include "guikit/diagram.hpp"
#include "guikit/backend/thor_canvas.hpp"
#include "keel/keel.h"
#include "keel/math.h"

static keel::i32 g_failures = 0;

#define CHECK(cond)                                                                  \
    do {                                                                             \
        if (!(cond)) {                                                               \
            keel::eprintln("[FAIL] {} ({}:{})", #cond, __FILE__, __LINE__);          \
            ++g_failures;                                                            \
        }                                                                            \
    } while (0)

#define CHECK_NEAR(a, b, eps)                                                        \
    do {                                                                             \
        if (keel::abs((a) - (b)) > (eps)) {                                           \
            keel::eprintln("[FAIL] {} ({}) != {} ({}) [{}:{}]",                      \
                           #a, (a), #b, (b), __FILE__, __LINE__);                    \
            ++g_failures;                                                            \
        }                                                                            \
    } while (0)

using namespace guikit;

void TestBpmnEditorWidgetWorkflow() {
    keel::println("  -> TestBpmnEditorWidgetWorkflow");
    Context ui;
    ThorCanvas canvas;
    keel::Array<keel::u32> pixels(960 * 600);
    canvas.SetTarget(pixels.data(), 960, 600, 960);

    BpmnDiagram diagram;
    diagram.name = "Unit Test BPMN Process";
    const keel::u32 laneId = diagram.AddLane("Test Pool", "Main Lane", Rect(40.0f, 40.0f, 800.0f, 220.0f));
    const keel::u32 nStart = diagram.AddNode({
        .type = BpmnNodeType::StartEvent,
        .label = "Start",
        .pos = {100.0f, 120.0f},
        .eventDef = BpmnEventDefinition::Message,
        .laneId = laneId
    });
    const keel::u32 nTask = diagram.AddNode({
        .type = BpmnNodeType::Task,
        .label = "Process Order",
        .pos = {220.0f, 106.0f},
        .taskType = BpmnTaskType::Service,
        .laneId = laneId,
        .slaDurationMs = 300.0f
    });
    const keel::u32 nBoundary = diagram.AddNode({
        .type = BpmnNodeType::BoundaryEvent,
        .label = "Timeout",
        .eventDef = BpmnEventDefinition::Timer,
        .attachedToNodeId = nTask,
        .boundaryRatioX = 0.70f,
        .laneId = laneId
    });
    const keel::u32 nEnd = diagram.AddNode({
        .type = BpmnNodeType::EndEvent,
        .label = "Done",
        .pos = {440.0f, 120.0f},
        .laneId = laneId
    });
    const keel::u32 e1 = diagram.AddEdge(nStart, nTask, BpmnFlowType::SequenceFlow);
    const keel::u32 e2 = diagram.AddEdge(nTask, nEnd, BpmnFlowType::DefaultFlow, "OK");
    (void)nBoundary;
    CHECK(e1 != 0);
    CHECK(e2 != 0);
    CHECK(diagram.nodes.size() == 4);
    CHECK(diagram.edges.size() == 2);

    // Verify BoundaryEvent snapped to bottom border of nTask
    const BpmnNode* boundPtr = diagram.FindNode(nBoundary);
    const BpmnNode* taskPtr = diagram.FindNode(nTask);
    CHECK(boundPtr != nullptr);
    CHECK(taskPtr != nullptr);
    CHECK_NEAR(boundPtr->pos.y + boundPtr->size.y * 0.5f, taskPtr->pos.y + taskPtr->size.y, 0.5f);

    bool tokenSim = false;
    BpmnRoutingMode routing = BpmnRoutingMode::Orthogonal;
    keel::u32 selNode = 0;
    BpmnEditorResponse resp{};

    auto renderFrame = [&]() {
        ui.BeginFrame({960.0f, 600.0f});
        resp = guikit::diagram::BpmnEditor(ui, diagram, {
            .routingMode = routing,
            .themePreset = BpmnThemePreset::BpmnIoClassic,
            .tokenSimulation = tokenSim,
            .autoSpawnTokens = true,
            .simulationSpeed = 2.0f,
            .showSlaHeatmap = true,
            .tokenSimulationPtr = &tokenSim,
            .routingModePtr = &routing,
            .selectedNodeIdPtr = &selNode
        });
        ui.EndFrame(canvas);
    };

    ui.SetCurrentTime(1.0);
    renderFrame();
    renderFrame();
    CHECK(resp.canvasRect.w > 700.0f);
    CHECK(resp.canvasRect.h > 400.0f);

    // Programmatically select nTask via two-way binding and verify response
    selNode = nTask;
    renderFrame();
    CHECK(resp.selectedNodeId == nTask);

    // Enable Token Simulation and step time so autoSpawnTokens spawns a token
    tokenSim = true;
    ui.AdvanceTime(0.05);
    renderFrame();
    CHECK(resp.simulationActive);
    CHECK(resp.activeTokenCount >= 1);

    // Step simulation forward and switch to Bezier routing
    routing = BpmnRoutingMode::Bezier;
    ui.AdvanceTime(0.5);
    renderFrame();
    CHECK(diagram.FindNode(nStart)->tokenExecutionCount >= 1);
}

int main() {
    keel::println("[RUN] Testing GuiKit Diagram module...");
    TestBpmnEditorWidgetWorkflow();
    if (g_failures == 0) {
        keel::println("[PASS] All GuiKit Diagram tests passed successfully!");
        return 0;
    }
    keel::eprintln("[FAIL] Total failures: {}", g_failures);
    return 1;
}
