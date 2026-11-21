#pragma once

// ==============================================================================
// GuiKit Diagram Module - Interactive BPMN 2.0 Modeler & Token Simulator
// ==============================================================================

#include "guikit/context.hpp"
#include "guikit/diagram/bpmn_editor.hpp"

namespace guikit::diagram {

/// Renders an interactive BPMN 2.0 Diagram Modeler & Token Simulation editor widget.
inline BpmnEditorResponse BpmnEditor(
    Context& ui,
    BpmnDiagram& diagram,
    const BpmnEditorOptions& opts = {},
    keel::SourceLocation loc = keel::SourceLocation::current()
) {
    return ui.Run<guikit::BpmnEditor>(diagram, opts, loc);
}

} // namespace guikit::diagram
