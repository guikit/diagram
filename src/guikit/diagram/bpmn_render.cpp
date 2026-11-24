#include "bpmn_internal.hpp"

namespace guikit::detail::bpmn_impl {

// ============================================================================
// Multi-Line Word-Wrapping & Rotated Swimlane Typography Helpers
// ============================================================================

void DrawWrappedTextBox(
    ICanvas& canvas,
    keel::StringRef text,
    Rect bounds,
    keel::f32 fontSize,
    Color color,
    FontStyle style = FontStyle::Regular,
    keel::f32 alignX = 0.5f,
    keel::f32 alignY = 0.5f,
    keel::usize maxLines = 3
) {
    if (text.empty() || color.a == 0 || bounds.w <= 4.0f || bounds.h <= 4.0f || maxLines == 0) return;

    keel::SmallArray<keel::SmallString<128>, 8> lines;

    keel::usize pos = 0;
    while (pos <= text.size() && lines.size() < maxLines) {
        const keel::usize nl = text.find('\n', pos);
        const keel::StringRef para = (nl == keel::kInvalidIndex)
            ? text.substr(pos)
            : text.substr(pos, nl - pos);

        // Word-wrap `para` into `bounds.w`
        keel::usize wPos = 0;
        keel::SmallString<128> currentLine;
        while (wPos < para.size() && lines.size() < maxLines) {
            while (wPos < para.size() && para[wPos] == ' ') ++wPos;
            if (wPos >= para.size()) break;
            const keel::usize nextSpace = para.find(' ', wPos);
            const keel::StringRef word = (nextSpace == keel::kInvalidIndex)
                ? para.substr(wPos)
                : para.substr(wPos, nextSpace - wPos);

            keel::SmallString<128> candidate;
            if (currentLine.empty()) {
                candidate.append(word);
            } else {
                candidate.append(currentLine);
                candidate.push_back(' ');
                candidate.append(word);
            }

            const keel::f32 candW = canvas.MeasureTextStyled(candidate, fontSize, style).x;
            if (!currentLine.empty() && candW > bounds.w && lines.size() + 1 < maxLines) {
                lines.push_back(keel::move(currentLine));
                currentLine = keel::SmallString<128>::from(word);
            } else {
                currentLine = keel::move(candidate);
            }
            if (nextSpace == keel::kInvalidIndex) break;
            wPos = nextSpace + 1;
        }
        if (!currentLine.empty() && lines.size() < maxLines) {
            lines.push_back(keel::move(currentLine));
        }

        if (nl == keel::kInvalidIndex) break;
        pos = nl + 1;
    }

    if (lines.empty()) return;

    const keel::f32 lineH = keel::max(fontSize * 1.18f, canvas.GetLineHeight(fontSize) * 0.94f);
    const keel::f32 totalH = keel::as<keel::f32>(lines.size()) * lineH;
    keel::f32 startY = bounds.y + (bounds.h - totalH) * alignY;

    for (const auto& line : lines) {
        const Rect lr(bounds.x, startY, bounds.w, lineH);
        canvas.DrawTextBox(line, lr, fontSize, color, alignX, 0.5f);
        startY += lineH;
    }
}

void DrawVerticalRotatedHeaderText(
    ICanvas& canvas,
    keel::StringRef text,
    Rect stripRect,
    keel::f32 fontSize,
    Color color,
    FontStyle style = FontStyle::Bold
) {
    if (text.empty() || stripRect.w <= 4.0f || stripRect.h <= 16.0f) return;
    canvas.PushClip(stripRect);
    const Vec2 tm = canvas.MeasureTextStyled(text, fontSize, style);
    const keel::f32 tx = stripRect.x + (stripRect.w - tm.y) * 0.5f;
    const keel::f32 ty = stripRect.y + (stripRect.h + keel::min(tm.x, stripRect.h - 8.0f)) * 0.5f;
    canvas.DrawTextStyled(text, {tx, ty}, fontSize, color, style, -90.0f);
    canvas.PopClip();
}


// ============================================================================
// Vector Drawing Primitives for BPMN 2.0 Symbols
// ============================================================================

void DrawDashedLine(ICanvas& canvas, Vec2 a, Vec2 b, Color col, keel::f32 width, keel::f32 dashLen, keel::f32 gapLen) {
    const keel::f32 dx = b.x - a.x;
    const keel::f32 dy = b.y - a.y;
    const keel::f32 len = keel::hypot(dx, dy);
    if (len <= 0.5f) return;
    const keel::f32 ux = dx / len;
    const keel::f32 uy = dy / len;
    const keel::f32 step = keel::max(2.0f, dashLen + gapLen);
    for (keel::f32 d = 0.0f; d < len; d += step) {
        const keel::f32 dEnd = keel::min(len, d + dashLen);
        canvas.DrawLine(
            {a.x + ux * d, a.y + uy * d},
            {a.x + ux * dEnd, a.y + uy * dEnd},
            col,
            width
        );
    }
}

void DrawDashedCircle(ICanvas& canvas, Vec2 center, keel::f32 r, Color stroke, keel::f32 strokeWidth, keel::i32 segments = 16) {
    for (keel::i32 i = 0; i < segments; ++i) {
        const keel::f32 a0 = (keel::as<keel::f32>(i) * keel::kTauF) / keel::as<keel::f32>(segments);
        const keel::f32 a1 = ((keel::as<keel::f32>(i) + 0.58f) * keel::kTauF) / keel::as<keel::f32>(segments);
        canvas.DrawLine(
            {center.x + keel::cos(a0) * r, center.y + keel::sin(a0) * r},
            {center.x + keel::cos(a1) * r, center.y + keel::sin(a1) * r},
            stroke,
            strokeWidth
        );
    }
}

void DrawDiamond(ICanvas& canvas, Vec2 center, keel::f32 halfW, keel::f32 halfH, Color fill, Color stroke, keel::f32 strokeWidth) {
    const Vec2 top{center.x, center.y - halfH};
    const Vec2 right{center.x + halfW, center.y};
    const Vec2 bottom{center.x, center.y + halfH};
    const Vec2 left{center.x - halfW, center.y};

    if (fill.a > 0) {
        canvas.DrawTriangle(top, right, bottom, fill);
        canvas.DrawTriangle(top, bottom, left, fill);
    }
    if (stroke.a > 0 && strokeWidth > 0.0f) {
        canvas.DrawLine(top, right, stroke, strokeWidth);
        canvas.DrawLine(right, bottom, stroke, strokeWidth);
        canvas.DrawLine(bottom, left, stroke, strokeWidth);
        canvas.DrawLine(left, top, stroke, strokeWidth);
    }
}

void DrawEventDefinitionSymbol(
    ICanvas& canvas,
    Vec2 center,
    keel::f32 radius,
    BpmnEventDefinition def,
    bool filledThrow,
    Color strokeColor,
    Color bgColor,
    keel::f32 s
) {
    const keel::f32 sw = keel::max(1.2f, 1.4f * s);
    switch (def) {
        case BpmnEventDefinition::None:
            break;
        case BpmnEventDefinition::Message: {
            const keel::f32 ew = radius * 1.02f;
            const keel::f32 eh = radius * 0.70f;
            const Rect envRect(center.x - ew * 0.5f, center.y - eh * 0.5f, ew, eh);
            canvas.DrawRoundRect(
                envRect,
                1.5f * s,
                filledThrow ? strokeColor : Color(0, 0, 0, 0),
                strokeColor,
                sw
            );
            const Color flapCol = filledThrow ? bgColor : strokeColor;
            canvas.DrawLine({envRect.x, envRect.y}, {center.x, center.y + eh * 0.06f}, flapCol, sw);
            canvas.DrawLine({envRect.Right(), envRect.y}, {center.x, center.y + eh * 0.06f}, flapCol, sw);
            break;
        }
        case BpmnEventDefinition::Timer: {
            const keel::f32 cr = radius * 0.54f;
            canvas.DrawRoundRect(
                Rect(center.x - cr, center.y - cr, cr * 2.0f, cr * 2.0f),
                cr,
                Color(0, 0, 0, 0),
                strokeColor,
                sw
            );
            for (keel::i32 i = 0; i < 4; ++i) {
                const keel::f32 ang = keel::as<keel::f32>(i) * keel::kHalfPiF;
                canvas.DrawLine(
                    {center.x + keel::cos(ang) * (cr * 0.72f), center.y + keel::sin(ang) * (cr * 0.72f)},
                    {center.x + keel::cos(ang) * cr, center.y + keel::sin(ang) * cr},
                    strokeColor,
                    sw
                );
            }
            canvas.DrawLine(center, {center.x, center.y - cr * 0.58f}, strokeColor, sw);
            canvas.DrawLine(center, {center.x + cr * 0.45f, center.y + cr * 0.12f}, strokeColor, sw);
            break;
        }
        case BpmnEventDefinition::Error: {
            const keel::f32 r = radius * 0.50f;
            const Vec2 p0{center.x - r * 0.65f, center.y + r * 0.65f};
            const Vec2 p1{center.x - r * 0.15f, center.y - r * 0.65f};
            const Vec2 p2{center.x + r * 0.20f, center.y + r * 0.18f};
            const Vec2 p3{center.x + r * 0.70f, center.y - r * 0.65f};
            canvas.DrawLine(p0, p1, strokeColor, sw * 1.25f);
            canvas.DrawLine(p1, p2, strokeColor, sw * 1.25f);
            canvas.DrawLine(p2, p3, strokeColor, sw * 1.25f);
            break;
        }
        case BpmnEventDefinition::Signal: {
            const keel::f32 r = radius * 0.50f;
            const Vec2 p0{center.x, center.y - r};
            const Vec2 p1{center.x + r * 0.92f, center.y + r * 0.72f};
            const Vec2 p2{center.x - r * 0.92f, center.y + r * 0.72f};
            canvas.DrawTriangle(
                p0, p1, p2,
                filledThrow ? strokeColor : Color(0, 0, 0, 0),
                strokeColor,
                sw
            );
            break;
        }
        case BpmnEventDefinition::Escalation: {
            const keel::f32 r = radius * 0.48f;
            const Vec2 top{center.x, center.y - r};
            const Vec2 right{center.x + r * 0.75f, center.y + r * 0.68f};
            const Vec2 mid{center.x, center.y + r * 0.15f};
            const Vec2 left{center.x - r * 0.75f, center.y + r * 0.68f};
            canvas.DrawLine(top, right, strokeColor, sw * 1.2f);
            canvas.DrawLine(right, mid, strokeColor, sw * 1.2f);
            canvas.DrawLine(mid, left, strokeColor, sw * 1.2f);
            canvas.DrawLine(left, top, strokeColor, sw * 1.2f);
            break;
        }
        case BpmnEventDefinition::Terminate: {
            const keel::f32 tr = radius * 0.54f;
            canvas.DrawRoundRect(
                Rect(center.x - tr, center.y - tr, tr * 2.0f, tr * 2.0f),
                tr,
                strokeColor
            );
            break;
        }
    }
}

void DrawCogwheel(ICanvas& canvas, Vec2 c, keel::f32 r, Color col, Color bgCol, keel::f32 sw) {
    for (keel::i32 i = 0; i < 4; ++i) {
        const keel::f32 ang = keel::as<keel::f32>(i) * (keel::kPiF * 0.25f);
        canvas.DrawLine(
            {c.x - keel::cos(ang) * r, c.y - keel::sin(ang) * r},
            {c.x + keel::cos(ang) * r, c.y + keel::sin(ang) * r},
            col,
            sw * 1.3f
        );
    }
    const keel::f32 outerR = r * 0.68f;
    const keel::f32 innerR = r * 0.30f;
    canvas.DrawRoundRect(Rect(c.x - outerR, c.y - outerR, outerR * 2.0f, outerR * 2.0f), outerR, bgCol, col, sw);
    canvas.DrawRoundRect(Rect(c.x - innerR, c.y - innerR, innerR * 2.0f, innerR * 2.0f), innerR, bgCol, col, sw * 0.85f);
}

void DrawTaskTypeMarker(ICanvas& canvas, Rect iconRect, BpmnTaskType taskType, Color col, Color bgCol, keel::f32 s) {
    const Vec2 c = iconRect.Center();
    const keel::f32 r = keel::min(iconRect.w, iconRect.h) * 0.46f;
    const keel::f32 sw = keel::max(1.15f, 1.35f * s);

    switch (taskType) {
        case BpmnTaskType::None:
        case BpmnTaskType::CallActivity:
            break;
        case BpmnTaskType::User: {
            const keel::f32 hr = r * 0.42f;
            canvas.DrawRoundRect(
                Rect(c.x - hr, c.y - r * 0.82f, hr * 2.0f, hr * 2.0f),
                hr,
                bgCol,
                col,
                sw
            );
            const Rect bust(c.x - r * 0.84f, c.y + r * 0.08f, r * 1.68f, r * 0.80f);
            canvas.DrawRoundRect(bust, r * 0.38f, bgCol, col, sw);
            canvas.DrawLine({c.x - r * 0.32f, bust.y + bust.h * 0.35f}, {c.x - r * 0.32f, bust.Bottom()}, col, sw * 0.85f);
            canvas.DrawLine({c.x + r * 0.32f, bust.y + bust.h * 0.35f}, {c.x + r * 0.32f, bust.Bottom()}, col, sw * 0.85f);
            break;
        }
        case BpmnTaskType::Service: {
            // Signature `bpmn-js` two interlocking gears
            DrawCogwheel(canvas, {c.x - r * 0.28f, c.y - r * 0.28f}, r * 0.58f, col, bgCol, sw * 0.9f);
            DrawCogwheel(canvas, {c.x + r * 0.22f, c.y + r * 0.22f}, r * 0.72f, col, bgCol, sw);
            break;
        }
        case BpmnTaskType::Send:
        case BpmnTaskType::Receive: {
            const bool filled = (taskType == BpmnTaskType::Send);
            const Rect er(c.x - r * 0.92f, c.y - r * 0.62f, r * 1.84f, r * 1.24f);
            canvas.DrawRoundRect(er, 1.5f * s, filled ? col : bgCol, col, sw);
            const Color flap = filled ? bgCol : col;
            canvas.DrawLine({er.x, er.y}, {c.x, c.y + r * 0.08f}, flap, sw);
            canvas.DrawLine({er.Right(), er.y}, {c.x, c.y + r * 0.08f}, flap, sw);
            break;
        }
        case BpmnTaskType::Script: {
            const Rect sr(c.x - r * 0.72f, c.y - r * 0.85f, r * 1.44f, r * 1.70f);
            canvas.DrawRoundRect(sr, 2.5f * s, bgCol, col, sw);
            for (keel::i32 i = 0; i < 3; ++i) {
                const keel::f32 ly = sr.y + sr.h * (0.28f + keel::as<keel::f32>(i) * 0.22f);
                canvas.DrawLine({sr.x + sr.w * 0.22f, ly}, {sr.Right() - sr.w * 0.22f, ly}, col, sw);
            }
            break;
        }
        case BpmnTaskType::Manual: {
            const Rect palm(c.x - r * 0.78f, c.y - r * 0.45f, r * 1.56f, r * 0.95f);
            canvas.DrawRoundRect(palm, 2.5f * s, bgCol, col, sw);
            canvas.DrawLine({palm.x + palm.w * 0.33f, palm.y}, {palm.x + palm.w * 0.33f, palm.Bottom()}, col, sw);
            canvas.DrawLine({palm.x + palm.w * 0.66f, palm.y}, {palm.x + palm.w * 0.66f, palm.Bottom()}, col, sw);
            break;
        }
        case BpmnTaskType::BusinessRule: {
            const Rect tbl(c.x - r * 0.88f, c.y - r * 0.68f, r * 1.76f, r * 1.36f);
            canvas.DrawRoundRect(tbl, 1.5f * s, bgCol, col, sw);
            canvas.DrawRect(Rect(tbl.x, tbl.y, tbl.w, tbl.h * 0.34f), col.WithAlpha(85));
            canvas.DrawLine({tbl.x, tbl.y + tbl.h * 0.34f}, {tbl.Right(), tbl.y + tbl.h * 0.34f}, col, sw);
            canvas.DrawLine({tbl.x, tbl.y + tbl.h * 0.67f}, {tbl.Right(), tbl.y + tbl.h * 0.67f}, col, sw);
            canvas.DrawLine({tbl.x + tbl.w * 0.38f, tbl.y + tbl.h * 0.34f}, {tbl.x + tbl.w * 0.38f, tbl.Bottom()}, col, sw);
            break;
        }
    }
}

void DrawGatewayMarker(ICanvas& canvas, Vec2 center, keel::f32 halfSize, BpmnGatewayType gwType, Color col, keel::f32 s) {
    const keel::f32 sw = keel::max(1.9f, 2.4f * s);
    switch (gwType) {
        case BpmnGatewayType::Exclusive: {
            const keel::f32 d = halfSize * 0.34f;
            canvas.DrawLine({center.x - d, center.y - d}, {center.x + d, center.y + d}, col, sw);
            canvas.DrawLine({center.x + d, center.y - d}, {center.x - d, center.y + d}, col, sw);
            break;
        }
        case BpmnGatewayType::Parallel: {
            const keel::f32 d = halfSize * 0.42f;
            canvas.DrawLine({center.x - d, center.y}, {center.x + d, center.y}, col, sw);
            canvas.DrawLine({center.x, center.y - d}, {center.x, center.y + d}, col, sw);
            break;
        }
        case BpmnGatewayType::Inclusive: {
            const keel::f32 r = halfSize * 0.40f;
            canvas.DrawRoundRect(
                Rect(center.x - r, center.y - r, r * 2.0f, r * 2.0f),
                r,
                Color(0, 0, 0, 0),
                col,
                sw
            );
            break;
        }
        case BpmnGatewayType::EventBased: {
            const keel::f32 r1 = halfSize * 0.50f;
            const keel::f32 r2 = halfSize * 0.39f;
            const keel::f32 swThin = keel::max(1.1f, 1.3f * s);
            canvas.DrawRoundRect(Rect(center.x - r1, center.y - r1, r1 * 2.0f, r1 * 2.0f), r1, Color(0, 0, 0, 0), col, swThin);
            canvas.DrawRoundRect(Rect(center.x - r2, center.y - r2, r2 * 2.0f, r2 * 2.0f), r2, Color(0, 0, 0, 0), col, swThin);
            const keel::f32 pr = halfSize * 0.24f;
            for (keel::i32 i = 0; i < 5; ++i) {
                const keel::f32 a0 = -keel::kHalfPiF + keel::as<keel::f32>(i) * (keel::kTauF / 5.0f);
                const keel::f32 a1 = -keel::kHalfPiF + keel::as<keel::f32>((i + 1) % 5) * (keel::kTauF / 5.0f);
                canvas.DrawLine(
                    {center.x + keel::cos(a0) * pr, center.y + keel::sin(a0) * pr},
                    {center.x + keel::cos(a1) * pr, center.y + keel::sin(a1) * pr},
                    col,
                    swThin
                );
            }
            break;
        }
    }
}

[[nodiscard]] bool HasPortConnection(const BpmnDiagram& diagram, const BpmnNode& node, BpmnPortSide targetSide) noexcept {
    for (const auto& e : diagram.edges) {
        if (e.fromNodeId != node.id && e.toNodeId != node.id) continue;
        const BpmnNode* fn = diagram.FindNode(e.fromNodeId);
        const BpmnNode* tn = diagram.FindNode(e.toNodeId);
        if (!fn || !tn) continue;
        BpmnPortSide fs = BpmnPortSide::Right;
        BpmnPortSide ts = BpmnPortSide::Left;
        ResolveAutoPorts(*fn, *tn, e.fromPort, e.toPort, fs, ts);
        if (e.fromNodeId == node.id && fs == targetSide) return true;
        if (e.toNodeId == node.id && ts == targetSide) return true;
    }
    return false;
}

[[nodiscard]] Rect ComputeExternalNodeLabelScreenRect(
    const BpmnDiagram& diagram,
    const BpmnNode& node,
    const Rect& sr,
    keel::f32 zoom,
    keel::f32 s,
    keel::f32* outAlignX,
    keel::f32* outAlignY
) noexcept {
    const keel::f32 zs = keel::clamp(zoom, 0.42f, 2.0f) * s;
    const Vec2 center = sr.Center();
    const keel::f32 offX = node.labelOffset.x * zoom;
    const keel::f32 offY = node.labelOffset.y * zoom;

    if (node.type == BpmnNodeType::BoundaryEvent) {
        if (outAlignX) *outAlignX = 0.5f;
        if (outAlignY) *outAlignY = 0.5f;
        return Rect(sr.Right() + 4.0f * zs + offX, center.y + 1.0f * zs + offY, 84.0f * zs, 18.0f * zs);
    }

    const bool bottomBusy = HasPortConnection(diagram, node, BpmnPortSide::Bottom);
    const bool topBusy = HasPortConnection(diagram, node, BpmnPortSide::Top);

    Rect lblRect(center.x - 64.0f * zs, sr.Bottom() + 4.0f * zs, 128.0f * zs, 28.0f * zs);
    keel::f32 alignX = 0.5f;
    keel::f32 alignY = 0.0f;

    if (bottomBusy && !topBusy) {
        lblRect = Rect(center.x - 64.0f * zs, sr.y - 30.0f * zs, 128.0f * zs, 26.0f * zs);
        alignY = 1.0f;
    } else if (bottomBusy && topBusy) {
        lblRect = Rect(sr.x - 94.0f * zs, sr.Bottom() - 2.0f * zs, 88.0f * zs, 28.0f * zs);
        alignX = 1.0f;
        alignY = 0.0f;
    }

    lblRect.x += offX;
    lblRect.y += offY;
    if (outAlignX) *outAlignX = alignX;
    if (outAlignY) *outAlignY = alignY;
    return lblRect;
}

[[nodiscard]] bool IsExternalLabelNode(BpmnNodeType type) noexcept {
    return type == BpmnNodeType::StartEvent ||
           type == BpmnNodeType::IntermediateEvent ||
           type == BpmnNodeType::BoundaryEvent ||
           type == BpmnNodeType::EndEvent ||
           type == BpmnNodeType::Gateway ||
           type == BpmnNodeType::DataObject ||
           type == BpmnNodeType::DataStore;
}

[[nodiscard]] bool IsResizableNode(BpmnNodeType type) noexcept {
    return type == BpmnNodeType::Task ||
           type == BpmnNodeType::SubProcess ||
           type == BpmnNodeType::TextAnnotation;
}

[[nodiscard]] keel::StaticArray<Rect, 4> GetNodeResizeHandleRects(const Rect& sr, keel::f32 zoom, keel::f32 s) noexcept {
    const keel::f32 zs = keel::clamp(zoom, 0.35f, 2.2f) * s;
    const keel::f32 pad = 5.0f * zs;
    const Rect outline(sr.x - pad, sr.y - pad, sr.w + pad * 2.0f, sr.h + pad * 2.0f);
    const keel::f32 hs = 6.5f * s;
    const keel::f32 hh = hs * 0.5f;
    return {{
        Rect(outline.x - hh, outline.y - hh, hs, hs),             // 0: NW
        Rect(outline.Right() - hh, outline.y - hh, hs, hs),       // 1: NE
        Rect(outline.Right() - hh, outline.Bottom() - hh, hs, hs),// 2: SE
        Rect(outline.x - hh, outline.Bottom() - hh, hs, hs)       // 3: SW
    }};
}

void DrawExternalNodeLabel(
    ICanvas& canvas,
    const BpmnDiagram& diagram,
    const BpmnNode& node,
    const Rect& sr,
    keel::f32 zoom,
    keel::f32 s,
    const BpmnPalette& pal,
    bool isNodeSelected = false,
    bool isLabelHovered = false
) {
    if (node.label.empty() || zoom < 0.42f) return;
    const keel::f32 zs = keel::clamp(zoom, 0.42f, 2.0f) * s;
    const keel::f32 fs = keel::clamp(11.0f * zs, 8.5f * s, 13.5f * s);
    keel::f32 alignX = 0.5f;
    keel::f32 alignY = 0.0f;
    Rect lblRect = ComputeExternalNodeLabelScreenRect(diagram, node, sr, zoom, s, &alignX, &alignY);

    if (node.type == BpmnNodeType::BoundaryEvent) {
        const Vec2 tm = canvas.MeasureText(node.label, fs);
        lblRect.w = tm.x + 8.0f * zs;
        lblRect.h = tm.y + 4.0f * zs;
        canvas.DrawRoundRect(lblRect, 3.0f * zs, pal.canvasBg.WithAlpha(225));
        canvas.DrawTextBox(node.label, lblRect, fs, pal.textPrimary, 0.5f, 0.5f);
    } else {
        DrawWrappedTextBox(canvas, node.label, lblRect, fs, pal.textPrimary, FontStyle::Regular, alignX, alignY, 2);
    }

    if (isLabelHovered || (isNodeSelected && (keel::abs(node.labelOffset.x) > 0.5f || keel::abs(node.labelOffset.y) > 0.5f))) {
        const Color c = isLabelHovered ? pal.selectAccent : pal.hoverAccent.WithAlpha(160);
        const keel::f32 sw = keel::max(1.0f, 1.15f * s);
        DrawDashedLine(canvas, {lblRect.x, lblRect.y}, {lblRect.Right(), lblRect.y}, c, sw, 3.5f * s, 2.5f * s);
        DrawDashedLine(canvas, {lblRect.Right(), lblRect.y}, {lblRect.Right(), lblRect.Bottom()}, c, sw, 3.5f * s, 2.5f * s);
        DrawDashedLine(canvas, {lblRect.Right(), lblRect.Bottom()}, {lblRect.x, lblRect.Bottom()}, c, sw, 3.5f * s, 2.5f * s);
        DrawDashedLine(canvas, {lblRect.x, lblRect.Bottom()}, {lblRect.x, lblRect.y}, c, sw, 3.5f * s, 2.5f * s);
    }
}

void DrawNodeVisual(
    ICanvas& canvas,
    const BpmnDiagram& diagram,
    const BpmnNode& node,
    const Rect& sr,
    keel::f32 zoom,
    keel::f32 s,
    const BpmnPalette& pal,
    bool isSelected,
    bool isHovered,
    bool simulationActive,
    bool showSlaHeatmap,
    keel::f32 animPhase,
    bool isLabelHovered = false,
    bool showResizeHandles = false,
    keel::i32 hoveredResizeCorner = -1
) {
    const keel::f32 zs = keel::clamp(zoom, 0.35f, 2.2f) * s;
    const Vec2 center = sr.Center();

    // Optional SLA Heatmap halo on activities
    if (showSlaHeatmap && (node.type == BpmnNodeType::Task || node.type == BpmnNodeType::SubProcess)) {
        const keel::f32 slaNorm = keel::clamp((node.slaDurationMs - 100.0f) / 1400.0f, 0.0f, 1.0f);
        const Color heatCol = (slaNorm > 0.66f)
            ? Color(244, 63, 94, 58)
            : ((slaNorm > 0.33f) ? Color(245, 158, 11, 52) : Color(16, 185, 129, 44));
        const keel::f32 exp = 6.0f * zs;
        canvas.DrawRoundRect(
            Rect(sr.x - exp, sr.y - exp, sr.w + exp * 2.0f, sr.h + exp * 2.0f),
            12.0f * zs,
            heatCol
        );
    }

    // Active Token simulation pulse
    if (node.activeTokensInside > 0) {
        const keel::f32 pad = (4.0f + 2.5f * keel::sin(animPhase * keel::kTauF)) * zs;
        const keel::f32 rad = (node.type == BpmnNodeType::StartEvent || node.type == BpmnNodeType::IntermediateEvent ||
                           node.type == BpmnNodeType::BoundaryEvent || node.type == BpmnNodeType::EndEvent)
            ? (sr.w * 0.5f + pad)
            : 10.0f * zs;
        canvas.DrawRoundRect(
            Rect(sr.x - pad, sr.y - pad, sr.w + pad * 2.0f, sr.h + pad * 2.0f),
            rad,
            pal.simActiveGreen.WithAlpha(80),
            pal.simActiveGreen,
            keel::max(1.5f, 1.8f * s)
        );
    }

    switch (node.type) {
        case BpmnNodeType::StartEvent:
        case BpmnNodeType::IntermediateEvent:
        case BpmnNodeType::BoundaryEvent:
        case BpmnNodeType::EndEvent: {
            const keel::f32 r = keel::min(sr.w, sr.h) * 0.5f;
            Color fill = pal.startEventFill;
            Color stroke = pal.startEventStroke;
            keel::f32 strokeW = keel::max(1.6f, 2.0f * zs);

            if (node.type == BpmnNodeType::IntermediateEvent || node.type == BpmnNodeType::BoundaryEvent) {
                fill = pal.interEventFill;
                stroke = pal.interEventStroke;
                strokeW = keel::max(1.4f, 1.75f * zs);
            } else if (node.type == BpmnNodeType::EndEvent) {
                fill = pal.endEventFill;
                stroke = pal.endEventStroke;
                strokeW = keel::max(2.8f, 3.6f * zs);
            }
            if (node.customColor.a > 0) {
                stroke = node.customColor;
            }

            canvas.DrawRoundRect(
                Rect(center.x - r, center.y - r + 1.8f * zs, r * 2.0f, r * 2.0f),
                r,
                pal.nodeShadow
            );

            if (node.nonInterrupting && (node.type == BpmnNodeType::IntermediateEvent || node.type == BpmnNodeType::BoundaryEvent)) {
                canvas.DrawRoundRect(Rect(center.x - r, center.y - r, r * 2.0f, r * 2.0f), r, fill);
                DrawDashedCircle(canvas, center, r, stroke, strokeW, 14);
                DrawDashedCircle(canvas, center, r * 0.78f, stroke, keel::max(1.1f, 1.3f * zs), 12);
            } else {
                canvas.DrawRoundRect(Rect(center.x - r, center.y - r, r * 2.0f, r * 2.0f), r, fill, stroke, strokeW);
                if (node.type == BpmnNodeType::IntermediateEvent || node.type == BpmnNodeType::BoundaryEvent) {
                    const keel::f32 rInner = r * 0.78f;
                    canvas.DrawRoundRect(
                        Rect(center.x - rInner, center.y - rInner, rInner * 2.0f, rInner * 2.0f),
                        rInner,
                        Color(0, 0, 0, 0),
                        stroke,
                        keel::max(1.1f, 1.35f * zs)
                    );
                }
            }

            const bool filledIcon = (node.type == BpmnNodeType::EndEvent);
            DrawEventDefinitionSymbol(canvas, center, r, node.eventDef, filledIcon, stroke, fill, zs);
            DrawExternalNodeLabel(canvas, diagram, node, sr, zoom, s, pal, isSelected, isLabelHovered);
            break;
        }

        case BpmnNodeType::Gateway: {
            const keel::f32 hw = sr.w * 0.5f;
            const keel::f32 hh = sr.h * 0.5f;
            const Color stroke = (node.customColor.a > 0) ? node.customColor : pal.gatewayStroke;
            DrawDiamond(canvas, {center.x, center.y + 1.8f * zs}, hw, hh, pal.nodeShadow, Color(0, 0, 0, 0), 0.0f);
            DrawDiamond(canvas, center, hw, hh, pal.gatewayFill, stroke, keel::max(1.6f, 2.0f * zs));
            DrawGatewayMarker(canvas, center, keel::min(hw, hh), node.gatewayType, stroke, zs);
            DrawExternalNodeLabel(canvas, diagram, node, sr, zoom, s, pal, isSelected, isLabelHovered);
            break;
        }

        case BpmnNodeType::Task:
        case BpmnNodeType::SubProcess: {
            const keel::f32 rad = 10.0f * zs;
            const bool isCall = (node.type == BpmnNodeType::Task && node.taskType == BpmnTaskType::CallActivity);
            const bool isExpSub = (node.type == BpmnNodeType::SubProcess && node.isExpanded);
            const keel::f32 strokeW = isCall ? keel::max(2.8f, 3.6f * zs) : keel::max(1.5f, 1.9f * zs);
            const Color strokeCol = (node.customColor.a > 0) ? node.customColor : pal.nodeStroke;
            const Color fillCol = isExpSub ? pal.nodeFill.WithAlpha(220) : pal.nodeFill;

            // Crisp `bpmn-js` Task / SubProcess rounded rectangle
            canvas.DrawRoundRect(Rect(sr.x, sr.y + 2.0f * zs, sr.w, sr.h), rad, pal.nodeShadow);
            canvas.DrawRoundRect(sr, rad, fillCol, strokeCol, strokeW);

            if (isExpSub) {
                const keel::f32 hdrH = 26.0f * zs;
                canvas.DrawLine({sr.x, sr.y + hdrH}, {sr.Right(), sr.y + hdrH}, strokeCol.WithAlpha(90), 1.0f * s);
                if (!node.label.empty() && zoom >= 0.35f) {
                    const keel::f32 fs = keel::clamp(11.5f * zs, 8.5f * s, 13.5f * s);
                    canvas.DrawTextBox(node.label, Rect(sr.x + 10.0f * zs, sr.y + 3.0f * zs, sr.w - 20.0f * zs, hdrH - 4.0f * zs), fs, pal.textPrimary, 0.5f, 0.5f);
                }
            } else {
                // Top-left BPMN 2.0 Task Type Marker Icon
                if (node.type == BpmnNodeType::Task && node.taskType != BpmnTaskType::None) {
                    const Rect iconRect(sr.x + 6.5f * zs, sr.y + 6.0f * zs, 16.0f * zs, 16.0f * zs);
                    DrawTaskTypeMarker(canvas, iconRect, node.taskType, strokeCol, pal.nodeFill, zs);
                }

                // Centered multi-line wrapped Task label (`bpmn-js` style, up to 3 lines)
                if (!node.label.empty() && zoom >= 0.35f) {
                    const keel::f32 fs = keel::clamp(11.5f * zs, 8.5f * s, 13.5f * s);
                    const keel::f32 topPad = 14.0f * zs;
                    const keel::f32 botPad = (node.type == BpmnNodeType::SubProcess || node.parallelMultiInstance || node.loopMarker ||
                                          (showSlaHeatmap && !node.documentation.empty()))
                        ? 18.0f * zs
                        : 10.0f * zs;
                    const Rect textRect(
                        sr.x + 8.0f * zs,
                        sr.y + topPad,
                        keel::max(16.0f, sr.w - 16.0f * zs),
                        keel::max(14.0f, sr.h - topPad - botPad)
                    );
                    DrawWrappedTextBox(canvas, node.label, textRect, fs, pal.textPrimary, FontStyle::Regular, 0.5f, 0.5f, 3);
                }
            }

            // Active token progress bar or optional SLA telemetry when heatmap is enabled
            if (node.activeTokensInside > 0) {
                const Rect progBg(sr.x + 12.0f * zs, sr.Bottom() - 10.0f * zs, sr.w - 24.0f * zs, 4.0f * zs);
                canvas.DrawRoundRect(progBg, 2.0f * zs, pal.simActiveGreen.WithAlpha(55));
                const keel::f32 fillW = progBg.w * (0.25f + 0.75f * keel::abs(keel::sin(animPhase * keel::kPiF)));
                canvas.DrawRoundRect(Rect(progBg.x, progBg.y, fillW, progBg.h), 2.0f * zs, pal.simActiveGreen);
            } else if (showSlaHeatmap && !node.documentation.empty() && zoom >= 0.55f && node.type == BpmnNodeType::Task && !node.parallelMultiInstance && !node.loopMarker) {
                const keel::f32 subFs = keel::clamp(9.0f * zs, 7.5f * s, 10.5f * s);
                const Rect subRect(sr.x + 8.0f * zs, sr.Bottom() - 15.0f * zs, sr.w - 16.0f * zs, 12.0f * zs);
                canvas.DrawTextBox(node.documentation, subRect, subFs, pal.textMuted, 0.5f, 0.5f);
            }

            // SubProcess [+]/[-] marker or Loop / Parallel Multi-Instance markers at bottom center
            if (node.type == BpmnNodeType::SubProcess) {
                const keel::f32 ms = 13.0f * zs;
                const Rect mr(center.x - ms * 0.5f, sr.Bottom() - ms - 3.5f * zs, ms, ms);
                canvas.DrawRoundRect(mr, 2.0f * zs, pal.nodeFill, strokeCol, keel::max(1.1f, 1.3f * zs));
                const Vec2 mc = mr.Center();
                canvas.DrawLine({mc.x - ms * 0.28f, mc.y}, {mc.x + ms * 0.28f, mc.y}, strokeCol, keel::max(1.1f, 1.35f * zs));
                if (!isExpSub) {
                    canvas.DrawLine({mc.x, mc.y - ms * 0.28f}, {mc.x, mc.y + ms * 0.28f}, strokeCol, keel::max(1.1f, 1.35f * zs));
                }
            } else if (node.parallelMultiInstance) {
                const keel::f32 mh = 9.0f * zs;
                const keel::f32 my = sr.Bottom() - mh - 4.0f * zs;
                for (keel::i32 i = -1; i <= 1; ++i) {
                    const keel::f32 mx = center.x + keel::as<keel::f32>(i) * 3.5f * zs;
                    canvas.DrawLine({mx, my}, {mx, my + mh}, strokeCol, keel::max(1.2f, 1.45f * zs));
                }
            } else if (node.loopMarker) {
                const keel::f32 lr = 4.5f * zs;
                const Vec2 lc{center.x, sr.Bottom() - lr - 4.0f * zs};
                canvas.DrawRoundRect(Rect(lc.x - lr, lc.y - lr, lr * 2.0f, lr * 2.0f), lr, Color(0, 0, 0, 0), strokeCol, keel::max(1.1f, 1.35f * zs));
            }
            break;
        }

        case BpmnNodeType::DataObject: {
            const keel::f32 fold = keel::min(sr.w, sr.h) * 0.26f;
            const keel::f32 sw = keel::max(1.3f, 1.6f * zs);
            canvas.DrawRoundRect(sr, 2.0f * zs, pal.dataFill, pal.nodeStroke, sw);
            canvas.DrawLine({sr.Right() - fold, sr.y}, {sr.Right() - fold, sr.y + fold}, pal.nodeStroke, sw);
            canvas.DrawLine({sr.Right() - fold, sr.y + fold}, {sr.Right(), sr.y + fold}, pal.nodeStroke, sw);
            DrawExternalNodeLabel(canvas, diagram, node, sr, zoom, s, pal, isSelected, isLabelHovered);
            break;
        }

        case BpmnNodeType::DataStore: {
            const keel::f32 sw = keel::max(1.3f, 1.6f * zs);
            const keel::f32 capH = sr.h * 0.28f;
            canvas.DrawRoundRect(sr, 6.0f * zs, pal.dataFill, pal.nodeStroke, sw);
            canvas.DrawRoundRect(Rect(sr.x, sr.y, sr.w, capH), capH * 0.5f, pal.laneHeaderBg, pal.nodeStroke, sw);
            canvas.DrawLine({sr.x, sr.y + capH * 1.45f}, {sr.Right(), sr.y + capH * 1.45f}, pal.nodeStroke.WithAlpha(160), sw * 0.85f);
            DrawExternalNodeLabel(canvas, diagram, node, sr, zoom, s, pal, isSelected, isLabelHovered);
            break;
        }

        case BpmnNodeType::TextAnnotation: {
            const keel::f32 sw = keel::max(1.4f, 1.8f * zs);
            const keel::f32 bw = 10.0f * zs;
            canvas.DrawLine({sr.x + bw, sr.y}, {sr.x, sr.y}, pal.nodeStroke, sw);
            canvas.DrawLine({sr.x, sr.y}, {sr.x, sr.Bottom()}, pal.nodeStroke, sw);
            canvas.DrawLine({sr.x, sr.Bottom()}, {sr.x + bw, sr.Bottom()}, pal.nodeStroke, sw);
            if (!node.label.empty() && zoom >= 0.38f) {
                const keel::f32 fs = keel::clamp(10.5f * zs, 8.0f * s, 12.5f * s);
                const Rect tr(sr.x + 8.0f * zs, sr.y + 2.0f * zs, keel::max(8.0f, sr.w - 10.0f * zs), keel::max(8.0f, sr.h - 4.0f * zs));
                DrawWrappedTextBox(canvas, node.label, tr, fs, pal.textSecondary, FontStyle::Regular, 0.0f, 0.5f, 3);
            }
            break;
        }
    }

    // Authentic `bpmn-js` / `diagram-js` rectangular `djs-outline` bounding box on selection or hover
    if (isSelected || isHovered) {
        const keel::f32 pad = 5.0f * zs;
        const Rect outlineRect(sr.x - pad, sr.y - pad, sr.w + pad * 2.0f, sr.h + pad * 2.0f);
        if (isSelected) {
            canvas.DrawRoundRect(
                outlineRect,
                3.0f * s,
                pal.selectGlow.WithAlpha(24),
                pal.selectAccent,
                keel::max(1.4f, 1.7f * s)
            );
            if (showResizeHandles && IsResizableNode(node.type)) {
                const auto handles = GetNodeResizeHandleRects(sr, zoom, s);
                for (keel::i32 c = 0; c < 4; ++c) {
                    const bool isCornerHov = (hoveredResizeCorner == c);
                    canvas.DrawRoundRect(
                        handles[keel::as<keel::usize>(c)],
                        1.5f * s,
                        isCornerHov ? pal.selectAccent : pal.nodeFill,
                        pal.selectAccent,
                        1.4f * s
                    );
                }
            }
        } else {
            const keel::f32 osw = keel::max(1.1f, 1.3f * s);
            DrawDashedLine(canvas, {outlineRect.x, outlineRect.y}, {outlineRect.Right(), outlineRect.y}, pal.hoverAccent, osw, 4.0f * s, 3.0f * s);
            DrawDashedLine(canvas, {outlineRect.Right(), outlineRect.y}, {outlineRect.Right(), outlineRect.Bottom()}, pal.hoverAccent, osw, 4.0f * s, 3.0f * s);
            DrawDashedLine(canvas, {outlineRect.Right(), outlineRect.Bottom()}, {outlineRect.x, outlineRect.Bottom()}, pal.hoverAccent, osw, 4.0f * s, 3.0f * s);
            DrawDashedLine(canvas, {outlineRect.x, outlineRect.Bottom()}, {outlineRect.x, outlineRect.y}, pal.hoverAccent, osw, 4.0f * s, 3.0f * s);
        }
    }

    // Token execution counter badge in Token Simulation mode
    if (simulationActive && (node.tokenExecutionCount > 0 || node.activeTokensInside > 0) && zoom >= 0.45f) {
        char buf[32];
        const keel::StringRef badgeStr = keel::format_buf(buf, "%u", node.tokenExecutionCount + node.activeTokensInside);
        const keel::f32 bFs = keel::clamp(9.5f * zs, 8.0f * s, 11.5f * s);
        const keel::f32 bw = keel::max(18.0f * zs, canvas.MeasureText(badgeStr, bFs).x + 8.0f * zs);
        const keel::f32 bh = 16.0f * zs;
        const Rect badgeRect(sr.Right() - bw * 0.65f, sr.y - bh * 0.45f, bw, bh);
        const Color badgeBg = (node.activeTokensInside > 0) ? pal.simActiveGreen : pal.selectAccent;
        canvas.DrawRoundRect(badgeRect, bh * 0.5f, badgeBg, pal.canvasBg, 1.2f * s);
        canvas.DrawTextBox(badgeStr, badgeRect, bFs, Color(255, 255, 255, 255), 0.5f, 0.5f);
    }
}

[[nodiscard]] Rect ComputeEdgeLabelScreenRect(
    ICanvas* canvas,
    keel::ArrayRef<Vec2> rawScreenPts,
    const BpmnEdge& edge,
    keel::f32 zoom,
    keel::f32 s
) noexcept {
    if (edge.label.empty() || rawScreenPts.size() < 2) return Rect{};
    const keel::f32 zs = keel::clamp(zoom, 0.4f, 2.0f) * s;
    Vec2 mid{0.0f, 0.0f};
    bool isHorizSeg = true;
    (void)SamplePolylineAtProgress(rawScreenPts, 0.5f, &mid, &isHorizSeg);
    const keel::f32 fs = keel::clamp(10.2f * zs, 8.0f * s, 12.5f * s);
    const Vec2 tm = canvas
        ? canvas->MeasureText(edge.label, fs)
        : Vec2{keel::as<keel::f32>(edge.label.size()) * fs * 0.56f, fs * 1.2f};
    const keel::f32 labelX = mid.x + edge.labelOffset.x * zoom;
    const keel::f32 labelY = (isHorizSeg ? (mid.y - tm.y * 0.5f - 10.5f * zs) : mid.y) + edge.labelOffset.y * zoom;
    return Rect(
        labelX - tm.x * 0.5f - 4.5f * zs,
        labelY - tm.y * 0.5f - 2.0f * zs,
        tm.x + 9.0f * zs,
        tm.y + 4.0f * zs
    );
}

void DrawEdgeVisual(
    ICanvas& canvas,
    keel::ArrayRef<Vec2> rawScreenPts,
    const BpmnEdge& edge,
    BpmnRoutingMode routingMode,
    keel::f32 zoom,
    keel::f32 s,
    const BpmnPalette& pal,
    bool isSelected,
    bool isHovered,
    bool isSimActiveBranch,
    keel::i32 hoveredSegIdx = -1,
    keel::i32 hoveredEndpoint = 0,
    bool isLabelHovered = false
) {
    if (rawScreenPts.size() < 2) return;
    const keel::f32 zs = keel::clamp(zoom, 0.4f, 2.0f) * s;
    Color col = (edge.flowType == BpmnFlowType::MessageFlow) ? pal.edgeMessage : pal.edgeStroke;
    if (isSimActiveBranch) col = pal.simActiveGreen;
    if (isHovered) col = pal.hoverAccent;
    if (isSelected) col = pal.selectAccent;

    const keel::f32 sw = (isSelected || isHovered || isSimActiveBranch)
        ? keel::max(2.0f, 2.3f * zs)
        : keel::max(1.5f, 1.8f * zs);

    const bool useRoundedCorners =
        (routingMode == BpmnRoutingMode::Orthogonal) &&
        (edge.flowType != BpmnFlowType::MessageFlow && edge.flowType != BpmnFlowType::Association);

    keel::SmallArray<Vec2, 64> roundedPts;
    if (useRoundedCorners) {
        roundedPts = BuildRoundedPolyline(rawScreenPts, 6.0f * zs);
    }
    const keel::ArrayRef<Vec2> drawPts = useRoundedCorners
        ? roundedPts.as_ref()
        : rawScreenPts;

    if (isSelected) {
        for (keel::usize i = 0; i + 1 < drawPts.size(); ++i) {
            canvas.DrawLine(drawPts[i], drawPts[i + 1], pal.selectGlow, sw + 4.5f * zs);
        }
    }

    for (keel::usize i = 0; i + 1 < drawPts.size(); ++i) {
        const Vec2 a = drawPts[i];
        const Vec2 b = drawPts[i + 1];
        if (edge.flowType == BpmnFlowType::MessageFlow) {
            DrawDashedLine(canvas, a, b, col, sw, 6.0f * zs, 4.0f * zs);
        } else if (edge.flowType == BpmnFlowType::Association) {
            DrawDashedLine(canvas, a, b, col, sw, 2.5f * zs, 3.5f * zs);
        } else {
            canvas.DrawLine(a, b, col, sw);
        }
    }

    // Source decorators (Default slash `\`, Conditional mini-diamond, Message open circle)
    const Vec2 p0 = rawScreenPts.front();
    const Vec2 pNext = rawScreenPts[1];
    const keel::f32 sdx = pNext.x - p0.x;
    const keel::f32 sdy = pNext.y - p0.y;
    const keel::f32 slen = keel::hypot(sdx, sdy);
    if (slen > 4.0f) {
        const keel::f32 ux = sdx / slen;
        const keel::f32 uy = sdy / slen;
        const keel::f32 nx = -uy;
        const keel::f32 ny = ux;
        if (edge.flowType == BpmnFlowType::DefaultFlow) {
            const Vec2 tc{p0.x + ux * 10.5f * zs, p0.y + uy * 10.5f * zs};
            canvas.DrawLine(
                {tc.x - ux * 3.5f * zs - nx * 5.0f * zs, tc.y - uy * 3.5f * zs - ny * 5.0f * zs},
                {tc.x + ux * 3.5f * zs + nx * 5.0f * zs, tc.y + uy * 3.5f * zs + ny * 5.0f * zs},
                col,
                sw * 1.15f
            );
        } else if (edge.flowType == BpmnFlowType::ConditionalFlow) {
            const Vec2 dc{p0.x + ux * 7.5f * zs, p0.y + uy * 7.5f * zs};
            DrawDiamond(canvas, dc, 5.5f * zs, 4.0f * zs, pal.nodeFill, col, sw);
        } else if (edge.flowType == BpmnFlowType::MessageFlow) {
            const keel::f32 mr = 4.0f * zs;
            canvas.DrawRoundRect(Rect(p0.x - mr, p0.y - mr, mr * 2.0f, mr * 2.0f), mr, pal.nodeFill, col, sw);
        }
    }

    // Target arrowhead (for Sequence, Default, Conditional, Message flows)
    if (edge.flowType != BpmnFlowType::Association) {
        const Vec2 tip = rawScreenPts.back();
        const Vec2 prev = rawScreenPts[rawScreenPts.size() - 2];
        const keel::f32 dx = tip.x - prev.x;
        const keel::f32 dy = tip.y - prev.y;
        const keel::f32 len = keel::hypot(dx, dy);
        if (len > 1.0f) {
            const keel::f32 ux = dx / len;
            const keel::f32 uy = dy / len;
            const keel::f32 nx = -uy;
            const keel::f32 ny = ux;
            const keel::f32 al = 9.0f * zs;
            const keel::f32 aw = 4.6f * zs;
            const Vec2 b1{tip.x - ux * al + nx * aw, tip.y - uy * al + ny * aw};
            const Vec2 b2{tip.x - ux * al - nx * aw, tip.y - uy * al - ny * aw};
            canvas.DrawTriangle(
                tip, b1, b2,
                (edge.flowType == BpmnFlowType::MessageFlow) ? pal.nodeFill : col,
                col,
                1.0f * s
            );
        }
    }

    // Interactive `bpmn-js` Segment Drag Handles (`ConnectionSegmentMove`) & Endpoint Reconnect Handles (`BendpointMove`)
    if ((isSelected || isHovered) && routingMode == BpmnRoutingMode::Orthogonal) {
        for (keel::usize i = 0; i + 1 < rawScreenPts.size(); ++i) {
            const Vec2 a = rawScreenPts[i];
            const Vec2 b = rawScreenPts[i + 1];
            const keel::f32 segLen = keel::hypot(b.x - a.x, b.y - a.y);
            if (segLen < 22.0f * s) continue;
            const bool isHoriz = keel::abs(b.x - a.x) >= keel::abs(b.y - a.y);
            const Vec2 mid{(a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f};
            const keel::f32 hw = isHoriz ? 7.0f * s : 3.2f * s;
            const keel::f32 hh = isHoriz ? 3.2f * s : 7.0f * s;
            const bool isSegHov = (hoveredSegIdx == keel::as<keel::i32>(i));
            const Rect pillR(mid.x - hw, mid.y - hh, hw * 2.0f, hh * 2.0f);
            canvas.DrawRoundRect(
                pillR,
                3.0f * s,
                isSegHov ? pal.selectAccent : pal.nodeFill,
                pal.selectAccent,
                1.3f * s
            );
        }
    }
    if (isSelected) {
        const keel::f32 er = 4.5f * s;
        const Vec2 ep0 = rawScreenPts.front();
        const Vec2 ep1 = rawScreenPts.back();
        canvas.DrawRoundRect(
            Rect(ep0.x - er, ep0.y - er, er * 2.0f, er * 2.0f),
            er,
            (hoveredEndpoint == 1) ? pal.selectAccent : pal.nodeFill,
            pal.selectAccent,
            1.5f * s
        );
        canvas.DrawRoundRect(
            Rect(ep1.x - er, ep1.y - er, er * 2.0f, er * 2.0f),
            er,
            (hoveredEndpoint == 2) ? pal.selectAccent : pal.nodeFill,
            pal.selectAccent,
            1.5f * s
        );
    }

    // Clean `bpmn-js` Edge Label placement (above horizontal segments so the wire stays unbroken, or knockout halo on vertical segments)
    if (!edge.label.empty() && zoom >= 0.45f) {
        const keel::f32 fs = keel::clamp(10.2f * zs, 8.0f * s, 12.5f * s);
        const Rect pill = ComputeEdgeLabelScreenRect(&canvas, rawScreenPts, edge, zoom, s);
        canvas.DrawRoundRect(
            pill,
            3.0f * zs,
            pal.canvasBg.WithAlpha(232),
            (isSelected || isLabelHovered) ? pal.selectAccent : Color(0, 0, 0, 0),
            (isSelected || isLabelHovered) ? 1.1f * s : 0.0f
        );
        canvas.DrawTextBox(edge.label, pill, fs, (isSelected || isLabelHovered) ? pal.selectAccent : pal.textPrimary, 0.5f, 0.5f);
    }
}


void DrawContextPadButtonIcon(ICanvas& canvas, Rect r, keel::i32 btnIdx, Color col, const BpmnPalette& pal, keel::f32 s) {
    const Vec2 c = r.Center();
    const keel::f32 sw = keel::max(1.25f, 1.45f * s);
    switch (btnIdx) {
        case 0: { // Append Task (rounded rectangle)
            canvas.DrawRoundRect(Rect(c.x - 6.8f * s, c.y - 4.8f * s, 13.6f * s, 9.6f * s), 2.4f * s, Color(0, 0, 0, 0), col, sw);
            break;
        }
        case 1: { // Append Gateway (diamond)
            DrawDiamond(canvas, c, 6.4f * s, 6.4f * s, Color(0, 0, 0, 0), pal.gatewayStroke, sw * 1.1f);
            break;
        }
        case 2: { // Append EndEvent (thick circle)
            const keel::f32 rad = 5.8f * s;
            canvas.DrawRoundRect(Rect(c.x - rad, c.y - rad, rad * 2.0f, rad * 2.0f), rad, Color(0, 0, 0, 0), pal.endEventStroke, sw * 1.85f);
            break;
        }
        case 3: { // Append IntermediateEvent (concentric double circle)
            const keel::f32 r1 = 6.0f * s;
            const keel::f32 r2 = 4.0f * s;
            canvas.DrawRoundRect(Rect(c.x - r1, c.y - r1, r1 * 2.0f, r1 * 2.0f), r1, Color(0, 0, 0, 0), pal.interEventStroke, sw);
            canvas.DrawRoundRect(Rect(c.x - r2, c.y - r2, r2 * 2.0f, r2 * 2.0f), r2, Color(0, 0, 0, 0), pal.interEventStroke, sw * 0.85f);
            break;
        }
        case 4: { // Connect using Sequence/MessageFlow (diagonal arrow)
            const Vec2 a{c.x - 5.2f * s, c.y + 5.2f * s};
            const Vec2 b{c.x + 5.2f * s, c.y - 5.2f * s};
            canvas.DrawLine(a, b, col, sw);
            canvas.DrawTriangle(
                b,
                {b.x - 5.0f * s, b.y + 1.2f * s},
                {b.x - 1.2f * s, b.y + 5.0f * s},
                col
            );
            canvas.DrawRoundRect(Rect(a.x - 2.0f * s, a.y - 2.0f * s, 4.0f * s, 4.0f * s), 2.0f * s, col);
            break;
        }
        case 5: { // Replace Element Type (Wrench `🔧` icon)
            const Vec2 head{c.x + 2.5f * s, c.y - 2.5f * s};
            const Vec2 tail{c.x - 5.2f * s, c.y + 5.2f * s};
            canvas.DrawLine({c.x + 0.5f * s, c.y - 0.5f * s}, tail, col, sw * 1.45f);
            const keel::f32 hr = 3.6f * s;
            canvas.DrawRoundRect(Rect(head.x - hr, head.y - hr, hr * 2.0f, hr * 2.0f), hr, Color(0, 0, 0, 0), col, sw * 1.15f);
            canvas.DrawRoundRect(Rect(tail.x - 1.6f * s, tail.y - 1.6f * s, 3.2f * s, 3.2f * s), 1.6f * s, col);
            break;
        }
        case 6: { // Append TextAnnotation (`[` bracket with text lines)
            canvas.DrawLine({c.x - 1.5f * s, c.y - 5.5f * s}, {c.x - 5.5f * s, c.y - 5.5f * s}, col, sw);
            canvas.DrawLine({c.x - 5.5f * s, c.y - 5.5f * s}, {c.x - 5.5f * s, c.y + 5.5f * s}, col, sw);
            canvas.DrawLine({c.x - 5.5f * s, c.y + 5.5f * s}, {c.x - 1.5f * s, c.y + 5.5f * s}, col, sw);
            canvas.DrawLine({c.x - 2.0f * s, c.y - 1.8f * s}, {c.x + 5.5f * s, c.y - 1.8f * s}, col, sw);
            canvas.DrawLine({c.x - 2.0f * s, c.y + 1.8f * s}, {c.x + 4.0f * s, c.y + 1.8f * s}, col, sw);
            break;
        }
        case 7: { // Delete (Trash bin `🗑` icon)
            const Rect bin(c.x - 4.4f * s, c.y - 3.0f * s, 8.8f * s, 9.0f * s);
            canvas.DrawRoundRect(bin, 1.5f * s, Color(0, 0, 0, 0), col, sw);
            canvas.DrawLine({c.x - 6.0f * s, c.y - 3.2f * s}, {c.x + 6.0f * s, c.y - 3.2f * s}, col, sw * 1.15f);
            canvas.DrawLine({c.x - 2.2f * s, c.y - 5.2f * s}, {c.x + 2.2f * s, c.y - 5.2f * s}, col, sw);
            canvas.DrawLine({c.x - 1.6f * s, bin.y + 2.2f * s}, {c.x - 1.6f * s, bin.Bottom() - 2.0f * s}, col, sw * 0.85f);
            canvas.DrawLine({c.x + 1.6f * s, bin.y + 2.2f * s}, {c.x + 1.6f * s, bin.Bottom() - 2.0f * s}, col, sw * 0.85f);
            break;
        }
    }
}

void DrawPaletteButtonIcon(ICanvas& canvas, Rect r, BpmnTool tool, Color col, const BpmnPalette& pal, keel::f32 s) {
    const Vec2 c = r.Center();
    const keel::f32 sw = keel::max(1.25f, 1.45f * s);
    switch (tool) {
        case BpmnTool::Select: {
            const Vec2 p0{c.x - 4.5f * s, c.y - 6.0f * s};
            const Vec2 p1{c.x - 4.5f * s, c.y + 5.5f * s};
            const Vec2 p2{c.x + 5.0f * s, c.y + 1.0f * s};
            canvas.DrawTriangle(p0, p1, p2, col);
            break;
        }
        case BpmnTool::Hand: {
            const Rect palm(c.x - 5.0f * s, c.y - 2.0f * s, 10.0f * s, 8.0f * s);
            canvas.DrawRoundRect(palm, 2.5f * s, Color(0, 0, 0, 0), col, sw);
            for (keel::i32 i = -1; i <= 1; ++i) {
                const keel::f32 fx = c.x + keel::as<keel::f32>(i) * 3.0f * s;
                canvas.DrawLine({fx, c.y - 6.5f * s}, {fx, c.y - 2.0f * s}, col, sw);
            }
            break;
        }
        case BpmnTool::Lasso: {
            const Rect box(c.x - 6.5f * s, c.y - 5.5f * s, 13.0f * s, 11.0f * s);
            DrawDashedLine(canvas, {box.x, box.y}, {box.Right(), box.y}, col, sw, 3.0f * s, 2.0f * s);
            DrawDashedLine(canvas, {box.Right(), box.y}, {box.Right(), box.Bottom()}, col, sw, 3.0f * s, 2.0f * s);
            DrawDashedLine(canvas, {box.Right(), box.Bottom()}, {box.x, box.Bottom()}, col, sw, 3.0f * s, 2.0f * s);
            DrawDashedLine(canvas, {box.x, box.Bottom()}, {box.x, box.y}, col, sw, 3.0f * s, 2.0f * s);
            break;
        }
        case BpmnTool::Space: {
            canvas.DrawLine({c.x, c.y - 7.0f * s}, {c.x, c.y + 7.0f * s}, col, sw);
            canvas.DrawLine({c.x - 6.5f * s, c.y}, {c.x + 6.5f * s, c.y}, col, sw);
            canvas.DrawTriangle({c.x - 7.5f * s, c.y}, {c.x - 3.5f * s, c.y - 3.0f * s}, {c.x - 3.5f * s, c.y + 3.0f * s}, col);
            canvas.DrawTriangle({c.x + 7.5f * s, c.y}, {c.x + 3.5f * s, c.y - 3.0f * s}, {c.x + 3.5f * s, c.y + 3.0f * s}, col);
            break;
        }
        case BpmnTool::GlobalConnect: {
            const Vec2 a{c.x - 5.5f * s, c.y + 4.5f * s};
            const Vec2 b{c.x + 5.5f * s, c.y - 4.5f * s};
            canvas.DrawLine(a, b, col, sw);
            canvas.DrawRoundRect(Rect(a.x - 2.5f * s, a.y - 2.5f * s, 5.0f * s, 5.0f * s), 2.5f * s, col);
            canvas.DrawRoundRect(Rect(b.x - 2.5f * s, b.y - 2.5f * s, 5.0f * s, 5.0f * s), 2.5f * s, col);
            break;
        }
        case BpmnTool::CreateStartEvent: {
            const keel::f32 rad = 6.5f * s;
            canvas.DrawRoundRect(Rect(c.x - rad, c.y - rad, rad * 2.0f, rad * 2.0f), rad, Color(0, 0, 0, 0), pal.startEventStroke, sw * 1.15f);
            break;
        }
        case BpmnTool::CreateIntermediateEvent: {
            const keel::f32 r1 = 6.8f * s;
            const keel::f32 r2 = 4.6f * s;
            canvas.DrawRoundRect(Rect(c.x - r1, c.y - r1, r1 * 2.0f, r1 * 2.0f), r1, Color(0, 0, 0, 0), pal.interEventStroke, sw);
            canvas.DrawRoundRect(Rect(c.x - r2, c.y - r2, r2 * 2.0f, r2 * 2.0f), r2, Color(0, 0, 0, 0), pal.interEventStroke, sw * 0.85f);
            break;
        }
        case BpmnTool::CreateEndEvent: {
            const keel::f32 rad = 6.5f * s;
            canvas.DrawRoundRect(Rect(c.x - rad, c.y - rad, rad * 2.0f, rad * 2.0f), rad, Color(0, 0, 0, 0), pal.endEventStroke, sw * 1.9f);
            break;
        }
        case BpmnTool::CreateGateway: {
            DrawDiamond(canvas, c, 7.2f * s, 7.2f * s, Color(0, 0, 0, 0), pal.gatewayStroke, sw * 1.15f);
            break;
        }
        case BpmnTool::CreateTask: {
            canvas.DrawRoundRect(Rect(c.x - 7.5f * s, c.y - 5.2f * s, 15.0f * s, 10.4f * s), 2.5f * s, Color(0, 0, 0, 0), col, sw);
            break;
        }
        case BpmnTool::CreateSubProcess: {
            const Rect br(c.x - 7.5f * s, c.y - 5.5f * s, 15.0f * s, 11.0f * s);
            canvas.DrawRoundRect(br, 2.5f * s, Color(0, 0, 0, 0), col, sw);
            canvas.DrawLine({c.x - 2.5f * s, c.y + 1.5f * s}, {c.x + 2.5f * s, c.y + 1.5f * s}, col, sw);
            canvas.DrawLine({c.x, c.y - 1.0f * s}, {c.x, c.y + 4.0f * s}, col, sw);
            break;
        }
        case BpmnTool::CreateDataObject: {
            const Rect dr(c.x - 5.2f * s, c.y - 6.5f * s, 10.4f * s, 13.0f * s);
            canvas.DrawRoundRect(dr, 1.5f * s, Color(0, 0, 0, 0), col, sw);
            canvas.DrawLine({dr.Right() - 3.5f * s, dr.y}, {dr.Right(), dr.y + 3.5f * s}, col, sw);
            break;
        }
        case BpmnTool::CreateDataStore: {
            const Rect dr(c.x - 6.2f * s, c.y - 6.0f * s, 12.4f * s, 12.0f * s);
            canvas.DrawRoundRect(dr, 3.0f * s, Color(0, 0, 0, 0), col, sw);
            canvas.DrawLine({dr.x, dr.y + 4.0f * s}, {dr.Right(), dr.y + 4.0f * s}, col, sw);
            break;
        }
        case BpmnTool::CreateAnnotation: {
            canvas.DrawLine({c.x - 2.0f * s, c.y - 6.0f * s}, {c.x - 6.5f * s, c.y - 6.0f * s}, col, sw);
            canvas.DrawLine({c.x - 6.5f * s, c.y - 6.0f * s}, {c.x - 6.5f * s, c.y + 6.0f * s}, col, sw);
            canvas.DrawLine({c.x - 6.5f * s, c.y + 6.0f * s}, {c.x - 2.0f * s, c.y + 6.0f * s}, col, sw);
            canvas.DrawLine({c.x - 2.5f * s, c.y - 2.0f * s}, {c.x + 5.5f * s, c.y - 2.0f * s}, col, sw);
            canvas.DrawLine({c.x - 2.5f * s, c.y + 2.0f * s}, {c.x + 4.0f * s, c.y + 2.0f * s}, col, sw);
            break;
        }
        case BpmnTool::CreateLane: {
            const Rect lr(c.x - 7.5f * s, c.y - 5.5f * s, 15.0f * s, 11.0f * s);
            canvas.DrawRoundRect(lr, 1.5f * s, Color(0, 0, 0, 0), col, sw);
            canvas.DrawLine({lr.x + 4.0f * s, lr.y}, {lr.x + 4.0f * s, lr.Bottom()}, col, sw);
            canvas.DrawLine({lr.x, c.y}, {lr.Right(), c.y}, col, sw);
            break;
        }
    }
}

[[nodiscard]] Rect ComputeLeftPaletteRect(const Rect& viewRect, bool showTopBar, keel::f32 s) noexcept {
    const keel::f32 x = viewRect.x + 12.0f * s;
    const keel::f32 y = viewRect.y + (showTopBar ? 50.0f : 12.0f) * s;
    const keel::f32 w = 42.0f * s;
    const keel::f32 h = keel::min(keel::max(120.0f, viewRect.h - (y - viewRect.y) - 52.0f * s), 462.0f * s);
    return Rect(x, y, w, h);
}

[[nodiscard]] Rect ComputeTopBarRect(const Rect& viewRect, keel::f32 s) noexcept {
    return Rect(viewRect.x + 12.0f * s, viewRect.y + 10.0f * s, keel::max(200.0f, viewRect.w - 24.0f * s), 32.0f * s);
}

[[nodiscard]] Rect ComputeMinimapRect(const Rect& viewRect, keel::f32 s) noexcept {
    const keel::f32 mw = 188.0f * s;
    const keel::f32 mh = 116.0f * s;
    return Rect(viewRect.Right() - mw - 12.0f * s, viewRect.Bottom() - mh - 12.0f * s, mw, mh);
}

[[nodiscard]] Rect ComputeZoomHudRect(const Rect& viewRect, keel::f32 s) noexcept {
    const keel::f32 zw = 178.0f * s;
    const keel::f32 zh = 28.0f * s;
    return Rect(viewRect.x + 12.0f * s, viewRect.Bottom() - zh - 12.0f * s, zw, zh);
}


// ============================================================================
// Render Node Function (`RenderBpmnEditorNode`)
// ============================================================================

void RenderBpmnEditorNode(const Node* node, ICanvas& canvas, const UiTheme& theme) {
    const auto* payload = node->As<BpmnEditorPayload>();
    if (!payload || !payload->diagramSnapshot || !payload->stateSnapshot) return;

    const BpmnEditorOptions& opts = payload->opts;
    const BpmnDiagram& diagram = *payload->diagramSnapshot;
    const BpmnEditorState& state = *payload->stateSnapshot;
    constexpr keel::f32 s = 1.0f;

    const Rect viewRect = node->computedRect;
    if (viewRect.w <= 2.0f || viewRect.h <= 2.0f) return;

    const BpmnPalette pal = ResolveBpmnPalette(opts.themePreset, theme);
    const keel::f32 radius = opts.border.radius.value_or(8.0f) * s;
    const Color canvasBg = opts.background.Resolve(node->hovered, node->pressed, node->disabled, pal.canvasBg);
    const Color canvasBorder = opts.border.color.Resolve(node->hovered, node->pressed, node->disabled, pal.canvasBorder);
    const keel::f32 borderW = (opts.border.width > 0.0f) ? keel::max(1.0f, opts.border.width * s) : (1.0f * s);

    // 1. Canvas Background & Border
    canvas.DrawRoundRect(viewRect, radius, canvasBg, canvasBorder, borderW);
    canvas.PushClip(viewRect);

    const Vec2 pan = state.pan;
    const keel::f32 zoom = state.zoom;
    const keel::f32 zs = keel::clamp(zoom, 0.35f, 2.2f) * s;

    // 2. World Grid
    if (opts.showGrid && opts.gridStep >= 8.0f) {
        const keel::f32 screenStep = opts.gridStep * zoom;
        if (screenStep >= 9.0f) {
            const Vec2 wTL = ScreenToWorld({viewRect.x, viewRect.y}, viewRect, pan, zoom);
            const Vec2 wBR = ScreenToWorld({viewRect.Right(), viewRect.Bottom()}, viewRect, pan, zoom);
            const keel::f32 startX = keel::floor(wTL.x / opts.gridStep) * opts.gridStep;
            const keel::f32 startY = keel::floor(wTL.y / opts.gridStep) * opts.gridStep;
            const keel::i32 maxCols = keel::as<keel::i32>((wBR.x - startX) / opts.gridStep) + 2;
            const keel::i32 maxRows = keel::as<keel::i32>((wBR.y - startY) / opts.gridStep) + 2;
            if (maxCols * maxRows <= 6500) {
                for (keel::i32 r = 0; r < maxRows; ++r) {
                    const keel::f32 wy = startY + keel::as<keel::f32>(r) * opts.gridStep;
                    const keel::f32 sy = viewRect.y + pan.y + wy * zoom;
                    for (keel::i32 c = 0; c < maxCols; ++c) {
                        const keel::f32 wx = startX + keel::as<keel::f32>(c) * opts.gridStep;
                        const keel::f32 sx = viewRect.x + pan.x + wx * zoom;
                        canvas.DrawRect(Rect(sx - 0.8f * s, sy - 0.8f * s, 1.6f * s, 1.6f * s), pal.gridDot);
                    }
                }
            }
        }
    }

    // 3. Swimlanes & Participant Pools (`BpmnLane`) - Authentic `bpmn.io` Two-Column Vertical Header Strip
    const keel::f32 poolStripW = keel::clamp(28.0f * zs, 18.0f * s, 36.0f * s);
    const keel::f32 laneStripW = keel::clamp(28.0f * zs, 18.0f * s, 36.0f * s);

    // Draw individual lane bands and vertical lane headers first
    for (keel::usize i = 0; i < diagram.lanes.size(); ++i) {
        const BpmnLane& lane = diagram.lanes[i];
        const Rect sr = WorldToScreenRect(lane.rect, viewRect, pan, zoom);
        if (sr.Right() < viewRect.x || sr.x > viewRect.Right() || sr.Bottom() < viewRect.y || sr.y > viewRect.Bottom()) {
            continue;
        }
        const bool hasPool = !lane.poolName.empty();
        const keel::f32 leftOffset = hasPool ? poolStripW : 0.0f;

        const bool isLaneSel = (state.selectedLaneId == lane.id);
        const bool isLaneHov = (state.hoveredLaneId == lane.id);
        const Color laneBg = (i % 2 == 0) ? pal.laneBgEven : pal.laneBgOdd;
        const Color borderCol = isLaneSel ? pal.selectAccent : (isLaneHov ? pal.hoverAccent : pal.laneBorder);
        const keel::f32 bw = isLaneSel ? keel::max(1.8f, 2.2f * s) : keel::max(1.0f, 1.2f * s);

        canvas.DrawRect(sr, laneBg);
        canvas.DrawLine({sr.x + leftOffset, sr.y}, {sr.Right(), sr.y}, borderCol, bw);
        canvas.DrawLine({sr.x + leftOffset, sr.Bottom()}, {sr.Right(), sr.Bottom()}, borderCol, bw);
        canvas.DrawLine({sr.Right(), sr.y}, {sr.Right(), sr.Bottom()}, borderCol, bw);

        const Rect laneHdrRect(sr.x + leftOffset, sr.y, keel::min(laneStripW, keel::max(0.0f, sr.w - leftOffset)), sr.h);
        const Color hdrAccent = (lane.accentColor.a > 0) ? lane.accentColor : pal.laneHeaderBg;
        canvas.DrawRect(laneHdrRect, hdrAccent);
        canvas.DrawLine({laneHdrRect.x, laneHdrRect.y}, {laneHdrRect.x, laneHdrRect.Bottom()}, borderCol, bw);
        canvas.DrawLine({laneHdrRect.Right(), laneHdrRect.y}, {laneHdrRect.Right(), laneHdrRect.Bottom()}, borderCol, bw);

        if (zoom >= 0.35f) {
            const keel::f32 fs = keel::clamp(11.0f * zs, 8.0f * s, 13.0f * s);
            DrawVerticalRotatedHeaderText(canvas, lane.name, laneHdrRect, fs, pal.laneText, FontStyle::Bold);
        }
    }

    // Draw unified outer Participant Pool vertical header strip for contiguous lanes sharing `poolName`
    keel::usize groupStart = 0;
    while (groupStart < diagram.lanes.size()) {
        const keel::StringRef pName = diagram.lanes[groupStart].poolName;
        keel::usize groupEnd = groupStart + 1;
        Rect unionWorld = diagram.lanes[groupStart].rect;
        while (groupEnd < diagram.lanes.size() && diagram.lanes[groupEnd].poolName == pName) {
            unionWorld = unionWorld.Union(diagram.lanes[groupEnd].rect);
            ++groupEnd;
        }
        if (!pName.empty()) {
            const Rect poolSr = WorldToScreenRect(unionWorld, viewRect, pan, zoom);
            const Rect poolHdrRect(poolSr.x, poolSr.y, keel::min(poolStripW, poolSr.w), poolSr.h);
            canvas.DrawRect(poolHdrRect, pal.poolHeaderBg);
            canvas.DrawRoundRect(poolSr, 3.0f * s, Color(0, 0, 0, 0), pal.laneBorder, keel::max(1.2f, 1.5f * s));
            canvas.DrawLine({poolHdrRect.Right(), poolHdrRect.y}, {poolHdrRect.Right(), poolHdrRect.Bottom()}, pal.laneBorder, keel::max(1.2f, 1.4f * s));
            if (zoom >= 0.35f) {
                const keel::f32 fs = keel::clamp(11.5f * zs, 8.5f * s, 13.5f * s);
                DrawVerticalRotatedHeaderText(canvas, pName, poolHdrRect, fs, pal.laneText, FontStyle::Bold);
            }
        }
        groupStart = groupEnd;
    }

    // 4. Edges / Flows (`BpmnEdge`)
    for (const auto& edge : diagram.edges) {
        const BpmnNode* fromNode = diagram.FindNode(edge.fromNodeId);
        const BpmnNode* toNode = diagram.FindNode(edge.toNodeId);
        if (!fromNode || !toNode) continue;

        const auto worldPts = BuildEdgeWorldPoints(diagram, *fromNode, *toNode, edge, opts.routingMode);
        keel::SmallArray<Vec2, 32> screenPts;
        screenPts.reserve(worldPts.size());
        for (const Vec2& wp : worldPts) {
            screenPts.push_back(WorldToScreen(wp, viewRect, pan, zoom));
        }

        bool isSimActiveBranch = false;
        if (opts.tokenSimulation && fromNode->type == BpmnNodeType::Gateway &&
            fromNode->gatewayType == BpmnGatewayType::Exclusive) {
            if (fromNode->activeOutgoingEdgeId == edge.id) {
                isSimActiveBranch = true;
            }
        }

        const bool isEdgeSel = (state.selectedEdgeId == edge.id);
        const bool isEdgeHov = (state.hoveredEdgeId == edge.id);
        DrawEdgeVisual(
            canvas,
            screenPts,
            edge,
            opts.routingMode,
            zoom,
            s,
            pal,
            isEdgeSel,
            isEdgeHov,
            isSimActiveBranch,
            (isEdgeSel || isEdgeHov) ? state.hoveredEdgeSegIdx : -1,
            isEdgeSel ? state.hoveredEdgeEndpoint : 0,
            state.hoveredLabelEdgeId == edge.id
        );
    }

    // Interactive wire connection or endpoint reconnect preview
    if (state.connectingWire && state.connectFromNodeId != 0) {
        if (const BpmnNode* src = diagram.FindNode(state.connectFromNodeId)) {
            const Vec2 w0 = (state.connectFromPort == BpmnPortSide::Auto)
                ? src->Center()
                : GetNodePortWorldPos(*src, state.connectFromPort);
            const Vec2 s0 = WorldToScreen(w0, viewRect, pan, zoom);
            const Vec2 s1 = WorldToScreen(state.connectEndWorld, viewRect, pan, zoom);
            DrawDashedLine(canvas, s0, s1, pal.selectAccent, 2.2f * s, 6.0f * s, 4.0f * s);
            canvas.DrawRoundRect(Rect(s1.x - 4.5f * s, s1.y - 4.5f * s, 9.0f * s, 9.0f * s), 4.5f * s, pal.selectAccent);
        }
    } else if (state.reconnectingEdge && state.reconnectEdgeId != 0) {
        if (const BpmnEdge* re = diagram.FindEdge(state.reconnectEdgeId)) {
            const BpmnNode* fixedNode = diagram.FindNode(state.reconnectEndpoint == 1 ? re->toNodeId : re->fromNodeId);
            if (fixedNode) {
                const Vec2 s0 = WorldToScreen(fixedNode->Center(), viewRect, pan, zoom);
                const Vec2 s1 = WorldToScreen(state.reconnectEndWorld, viewRect, pan, zoom);
                DrawDashedLine(canvas, s0, s1, pal.selectAccent, 2.2f * s, 6.0f * s, 4.0f * s);
                canvas.DrawRoundRect(Rect(s1.x - 4.5f * s, s1.y - 4.5f * s, 9.0f * s, 9.0f * s), 4.5f * s, pal.selectAccent);
            }
        }
    }

    // 5. Nodes (`BpmnNode`) - Pass 0: Expanded SubProcess containers, Pass 1: Standard nodes, Pass 2: BoundaryEvents
    for (keel::i32 pass = 0; pass < 3; ++pass) {
        for (const auto& n : diagram.nodes) {
            const bool isBoundary = (n.type == BpmnNodeType::BoundaryEvent);
            const bool isExpSub = (n.type == BpmnNodeType::SubProcess && n.isExpanded);
            const keel::i32 nodePass = isExpSub ? 0 : (isBoundary ? 2 : 1);
            if (nodePass != pass) continue;

            const Rect sr = WorldToScreenRect(n.Bounds(), viewRect, pan, zoom);
            if (sr.Right() < viewRect.x - 100.0f || sr.x > viewRect.Right() + 100.0f ||
                sr.Bottom() < viewRect.y - 100.0f || sr.y > viewRect.Bottom() + 100.0f) {
                continue;
            }
            const bool isSel = state.IsNodeSelected(n.id);
            const bool isHov = (state.hoveredNodeId == n.id || state.connectSnapNodeId == n.id || state.reconnectSnapNodeId == n.id);
            const bool showResize = (isSel && state.selectedNodes.size() <= 1 && !opts.readOnly && !opts.tokenSimulation);
            DrawNodeVisual(
                canvas,
                diagram,
                n,
                sr,
                zoom,
                s,
                pal,
                isSel,
                isHov,
                opts.tokenSimulation,
                opts.showSlaHeatmap,
                payload->animPhase,
                state.hoveredLabelNodeId == n.id,
                showResize,
                showResize ? state.hoveredResizeCorner : -1
            );

            // Token Simulation Triggers on StartEvent / BoundaryEvent / Exclusive Gateway
            if (opts.tokenSimulation && zoom >= 0.45f) {
                if (n.type == BpmnNodeType::StartEvent || n.type == BpmnNodeType::BoundaryEvent) {
                    const keel::f32 btnW = 22.0f * s;
                    const keel::f32 btnH = 18.0f * s;
                    const Rect trigRect(sr.x - btnW - 4.0f * s, sr.Center().y - btnH * 0.5f, btnW, btnH);
                    const Color trigCol = (n.type == BpmnNodeType::StartEvent) ? pal.simActiveGreen : pal.interEventStroke;
                    canvas.DrawRoundRect(trigRect, 4.0f * s, trigCol, pal.canvasBg, 1.2f * s);
                    const Vec2 tc = trigRect.Center();
                    canvas.DrawTriangle(
                        {tc.x - 3.0f * s, tc.y - 4.2f * s},
                        {tc.x - 3.0f * s, tc.y + 4.2f * s},
                        {tc.x + 4.5f * s, tc.y},
                        Color(255, 255, 255, 255)
                    );
                }
            }
        }
    }

    // 6. Live Animated Process Tokens (`BpmnToken`)
    if (opts.tokenSimulation) {
        for (const auto& tok : diagram.tokens) {
            if (tok.edgeId != 0) {
                const BpmnEdge* edge = diagram.FindEdge(tok.edgeId);
                if (!edge) continue;
                const BpmnNode* fromNode = diagram.FindNode(edge->fromNodeId);
                const BpmnNode* toNode = diagram.FindNode(edge->toNodeId);
                if (!fromNode || !toNode) continue;

                const auto worldPts = BuildEdgeWorldPoints(diagram, *fromNode, *toNode, *edge, opts.routingMode);
                const Vec2 wPos = SamplePolylineAtProgress(worldPts, tok.progress);
                const Vec2 sPos = WorldToScreen(wPos, viewRect, pan, zoom);
                const keel::f32 tr = keel::clamp(8.5f * zoom * s, 6.0f * s, 12.0f * s);
                canvas.DrawRoundRect(Rect(sPos.x - tr * 1.4f, sPos.y - tr * 1.4f, tr * 2.8f, tr * 2.8f), tr * 1.4f, tok.color.WithAlpha(75));
                canvas.DrawRoundRect(Rect(sPos.x - tr, sPos.y - tr, tr * 2.0f, tr * 2.0f), tr, tok.color, Color(255, 255, 255, 240), 1.5f * s);
                char ibuf[16];
                const keel::StringRef instStr = keel::format_buf(ibuf, "%u", tok.instanceId);
                canvas.DrawTextBox(instStr, Rect(sPos.x - tr, sPos.y - tr, tr * 2.0f, tr * 2.0f), tr * 1.05f, Color(255, 255, 255, 255), 0.5f, 0.5f);
            }
        }
    }

    // 7. Smart Alignment Laser Guides (active during node drag AND during Create Tool Ghost Preview!)
    if (opts.smartGuides && (state.draggingNodes || state.activeTool >= BpmnTool::CreateStartEvent) && !state.activeGuides.empty()) {
        for (const auto& g : state.activeGuides) {
            if (g.vertical) {
                const Vec2 a = WorldToScreen({g.worldCoord, g.worldSpanMin}, viewRect, pan, zoom);
                const Vec2 b = WorldToScreen({g.worldCoord, g.worldSpanMax}, viewRect, pan, zoom);
                DrawDashedLine(canvas, a, b, pal.guideLaser, 1.4f * s, 5.0f * s, 3.0f * s);
            } else {
                const Vec2 a = WorldToScreen({g.worldSpanMin, g.worldCoord}, viewRect, pan, zoom);
                const Vec2 b = WorldToScreen({g.worldSpanMax, g.worldCoord}, viewRect, pan, zoom);
                DrawDashedLine(canvas, a, b, pal.guideLaser, 1.4f * s, 5.0f * s, 3.0f * s);
            }
        }
    }

    // 8. Space Tool (`S`) Split Axis Line
    if (state.spaceToolDragging && state.spaceToolAxisLocked) {
        const Vec2 sSplit = WorldToScreen(state.spaceStartWorld, viewRect, pan, zoom);
        const Vec2 sCurr = WorldToScreen(state.spaceCurrentWorld, viewRect, pan, zoom);
        if (state.spaceToolHorizontal) {
            DrawDashedLine(canvas, {sSplit.x, viewRect.y}, {sSplit.x, viewRect.Bottom()}, pal.selectAccent, 1.8f * s, 6.0f * s, 4.0f * s);
            DrawDashedLine(canvas, {sCurr.x, viewRect.y}, {sCurr.x, viewRect.Bottom()}, pal.guideLaser, 1.8f * s, 6.0f * s, 4.0f * s);
            canvas.DrawRect(Rect(keel::min(sSplit.x, sCurr.x), viewRect.y, keel::abs(sCurr.x - sSplit.x), viewRect.h), pal.selectGlow);
        } else {
            DrawDashedLine(canvas, {viewRect.x, sSplit.y}, {viewRect.Right(), sSplit.y}, pal.selectAccent, 1.8f * s, 6.0f * s, 4.0f * s);
            DrawDashedLine(canvas, {viewRect.x, sCurr.y}, {viewRect.Right(), sCurr.y}, pal.guideLaser, 1.8f * s, 6.0f * s, 4.0f * s);
            canvas.DrawRect(Rect(viewRect.x, keel::min(sSplit.y, sCurr.y), viewRect.w, keel::abs(sCurr.y - sSplit.y)), pal.selectGlow);
        }
    }

    // 9. Lasso Marquee Box
    if (state.lassoActive) {
        const Vec2 a = WorldToScreen(state.lassoStartWorld, viewRect, pan, zoom);
        const Vec2 b = WorldToScreen(state.lassoEndWorld, viewRect, pan, zoom);
        const Rect lr(keel::min(a.x, b.x), keel::min(a.y, b.y), keel::abs(b.x - a.x), keel::abs(b.y - a.y));
        canvas.DrawRoundRect(lr, 3.0f * s, pal.selectGlow, pal.selectAccent, 1.4f * s);
    }

    // 10. Authentic `bpmn-js` CreatePreview (Full Vector Node Silhouette under Cursor)
    if (!opts.readOnly && !opts.tokenSimulation && payload->resp.hovered && state.activeTool >= BpmnTool::CreateStartEvent && state.hoveredPaletteIdx < 0) {
        if (state.activeTool == BpmnTool::CreateLane) {
            Rect laneGhost(state.ghostSnappedPos.x, state.ghostSnappedPos.y, 1280.0f, 190.0f);
            if (!diagram.lanes.empty()) {
                laneGhost.x = diagram.lanes.back().rect.x;
                laneGhost.y = diagram.lanes.back().rect.Bottom();
                laneGhost.w = diagram.lanes.back().rect.w;
            }
            const Rect gsr = WorldToScreenRect(laneGhost, viewRect, pan, zoom);
            canvas.DrawRoundRect(gsr, 4.0f * s, pal.selectGlow, pal.selectAccent, 1.6f * s);
        } else {
            BpmnNode ghost{};
            switch (state.activeTool) {
                case BpmnTool::CreateStartEvent:        ghost.type = BpmnNodeType::StartEvent; ghost.label = "Start"; break;
                case BpmnTool::CreateIntermediateEvent: ghost.type = (state.dragBoundaryHostId != 0) ? BpmnNodeType::BoundaryEvent : BpmnNodeType::IntermediateEvent; ghost.eventDef = BpmnEventDefinition::Timer; ghost.label = (state.dragBoundaryHostId != 0) ? "Boundary" : "Timer"; break;
                case BpmnTool::CreateEndEvent:          ghost.type = BpmnNodeType::EndEvent; ghost.label = "End"; break;
                case BpmnTool::CreateGateway:           ghost.type = BpmnNodeType::Gateway; ghost.label = "Gateway?"; break;
                case BpmnTool::CreateTask:              ghost.type = BpmnNodeType::Task; ghost.label = "Task"; break;
                case BpmnTool::CreateSubProcess:        ghost.type = BpmnNodeType::SubProcess; ghost.label = "Sub-Process"; break;
                case BpmnTool::CreateDataObject:        ghost.type = BpmnNodeType::DataObject; ghost.label = "Payload"; break;
                case BpmnTool::CreateDataStore:         ghost.type = BpmnNodeType::DataStore; ghost.label = "Database"; break;
                case BpmnTool::CreateAnnotation:        ghost.type = BpmnNodeType::TextAnnotation; ghost.label = "Text annotation"; break;
                default: break;
            }
            ghost.size = BpmnDiagram::DefaultNodeSize(ghost.type);
            ghost.pos = state.ghostSnappedPos;
            const Rect gsr = WorldToScreenRect(ghost.Bounds(), viewRect, pan, zoom);
            DrawNodeVisual(canvas, diagram, ghost, gsr, zoom, s, pal, false, true, false, false, 0.0f);
        }
    }

    // 11. Context-Sensitive `bpmn.io` Context Pad (Vector Icons) + `🔧 Replace Menu` Popover
    if (opts.showContextPad && !opts.readOnly && !opts.tokenSimulation && state.editingNodeId == 0 && state.editingEdgeId == 0) {
        if (state.primaryNodeId != 0 && state.selectedNodes.size() <= 1) {
            if (const BpmnNode* selNode = diagram.FindNode(state.primaryNodeId)) {
                const keel::ArrayRef<keel::i32> actions = GetContextPadActionsForNode(*selNode);
                const keel::i32 cols = keel::min(4, keel::as<keel::i32>(actions.size()));
                const keel::i32 rows = (keel::as<keel::i32>(actions.size()) + 3) / 4;
                const Rect nsr = WorldToScreenRect(selNode->Bounds(), viewRect, pan, zoom);
                const keel::f32 btnS = 26.0f * s;
                const keel::f32 gap = 4.0f * s;
                const keel::f32 padW = (btnS + gap) * keel::as<keel::f32>(cols) + 4.0f * s;
                const keel::f32 padH = (btnS + gap) * keel::as<keel::f32>(rows) + 4.0f * s;
                const keel::f32 padX = keel::min(viewRect.Right() - padW - 12.0f * s, nsr.Right() + 10.0f * s);
                const keel::f32 padY = keel::clamp(nsr.y - 4.0f * s, viewRect.y + 48.0f * s, viewRect.Bottom() - padH - 12.0f * s);
                const Rect padFrame(padX - 4.0f * s, padY - 4.0f * s, padW, padH);
                canvas.DrawRoundRect(padFrame, 6.0f * s, pal.hudBg, pal.hudBorder, 1.2f * s);

                for (keel::usize slot = 0; slot < actions.size(); ++slot) {
                    const keel::i32 actIdx = actions[slot];
                    const keel::i32 row = keel::as<keel::i32>(slot) / 4;
                    const keel::i32 col = keel::as<keel::i32>(slot) % 4;
                    const Rect br(padX + keel::as<keel::f32>(col) * (btnS + gap), padY + keel::as<keel::f32>(row) * (btnS + gap), btnS, btnS);
                    const bool isActiveBtn = (actIdx == 5 && state.replaceMenuOpen);
                    const bool isHovBtn = (state.hoveredContextPadBtn == actIdx);
                    const Color bBg = isActiveBtn
                        ? pal.hudActiveBg
                        : (isHovBtn ? ((actIdx == 7) ? pal.endEventFill : pal.hudActiveBg) : ((actIdx == 7) ? pal.endEventFill.WithAlpha(160) : pal.hudHover));
                    const Color bFg = (actIdx == 7)
                        ? pal.endEventStroke
                        : ((isActiveBtn || isHovBtn) ? pal.selectAccent : pal.textPrimary);
                    canvas.DrawRoundRect(br, 4.0f * s, bBg, isHovBtn ? pal.selectAccent : pal.hudBorder, 1.0f * s);
                    DrawContextPadButtonIcon(canvas, br, actIdx, bFg, pal, s);
                }

                // Floating tooltip for hovered Context Pad button
                if (state.hoveredContextPadBtn >= 0 && state.hoveredContextPadBtn < 8 && !state.replaceMenuOpen) {
                    const keel::StringRef tip = kContextPadTooltips[keel::as<keel::usize>(state.hoveredContextPadBtn)];
                    const keel::f32 tfs = 10.0f * s;
                    const Vec2 tm = canvas.MeasureText(tip, tfs);
                    const Rect tipRect(padFrame.x, padFrame.y - tm.y - 10.0f * s, tm.x + 12.0f * s, tm.y + 6.0f * s);
                    canvas.DrawRoundRect(tipRect, 4.0f * s, pal.hudBg, pal.selectAccent, 1.0f * s);
                    canvas.DrawTextBox(tip, tipRect, tfs, pal.textPrimary, 0.5f, 0.5f);
                }

                if (state.replaceMenuOpen) {
                    keel::StaticArray<keel::StringRef, 8> items{};
                    keel::usize itemCount = 0;
                    if (selNode->type == BpmnNodeType::Task || selNode->type == BpmnNodeType::SubProcess) {
                        items = {
                            "User Task", "Service Task", "Send Task", "Receive Task",
                            "Script Task", "Business Rule Task", "Sub-Process (Collapsed)", "Sub-Process (Expanded)"
                        };
                        itemCount = 8;
                    } else if (selNode->type == BpmnNodeType::Gateway) {
                        items = {"Exclusive Gateway", "Parallel Gateway", "Inclusive Gateway", "Event-Based Gateway", "", "", "", ""};
                        itemCount = 4;
                    } else {
                        items = {"Blank Event", "Message Event", "Timer Event", "Error Event", "Signal Event", "Escalation Event", "Terminate End Event", ""};
                        itemCount = 7;
                    }
                    const keel::f32 rowH = 24.0f * s;
                    const keel::f32 popH = 30.0f * s + keel::as<keel::f32>(itemCount) * rowH;
                    const Rect popRect(padFrame.x, padFrame.Bottom() + 6.0f * s, 204.0f * s, popH);
                    canvas.DrawRoundRect(popRect, 7.0f * s, pal.hudBg, pal.selectAccent, 1.4f * s);
                    canvas.DrawText("CHANGE ELEMENT TYPE", {popRect.x + 10.0f * s, popRect.y + 7.0f * s}, 9.5f * s, pal.selectAccent);
                    canvas.DrawLine({popRect.x + 6.0f * s, popRect.y + 23.0f * s}, {popRect.Right() - 6.0f * s, popRect.y + 23.0f * s}, pal.hudBorder, 1.0f * s);

                    for (keel::usize k = 0; k < itemCount; ++k) {
                        const Rect ir(popRect.x + 6.0f * s, popRect.y + 27.0f * s + keel::as<keel::f32>(k) * rowH, popRect.w - 12.0f * s, rowH - 2.0f * s);
                        const bool isRowHov = (state.hoveredReplaceMenuItem == keel::as<keel::i32>(k));
                        canvas.DrawRoundRect(ir, 4.0f * s, isRowHov ? pal.hudActiveBg : pal.hudHover);

                        const Rect iconBox(ir.x + 5.0f * s, ir.y + 3.0f * s, 16.0f * s, 16.0f * s);
                        if (selNode->type == BpmnNodeType::Task || selNode->type == BpmnNodeType::SubProcess) {
                            if (k >= 6) {
                                DrawPaletteButtonIcon(canvas, iconBox, BpmnTool::CreateSubProcess, pal.textPrimary, pal, s * 0.85f);
                            } else {
                                static constexpr keel::StaticArray<BpmnTaskType, 6> kRowTasks = {
                                    BpmnTaskType::User, BpmnTaskType::Service, BpmnTaskType::Send,
                                    BpmnTaskType::Receive, BpmnTaskType::Script, BpmnTaskType::BusinessRule
                                };
                                DrawTaskTypeMarker(canvas, iconBox, kRowTasks[k], pal.textPrimary, pal.hudHover, s * 0.9f);
                            }
                        } else if (selNode->type == BpmnNodeType::Gateway) {
                            DrawGatewayMarker(canvas, iconBox.Center(), 7.0f * s, keel::from_underlying<BpmnGatewayType>(k), pal.gatewayStroke, s * 0.8f);
                        } else {
                            DrawEventDefinitionSymbol(canvas, iconBox.Center(), 7.0f * s, keel::from_underlying<BpmnEventDefinition>(k), selNode->type == BpmnNodeType::EndEvent, pal.textPrimary, pal.hudHover, s * 0.85f);
                        }

                        canvas.DrawTextBox(items[k], Rect(ir.x + 26.0f * s, ir.y, ir.w - 30.0f * s, ir.h), 10.0f * s, isRowHov ? pal.selectAccent : pal.textPrimary, 0.0f, 0.5f);
                    }
                }
            }
        } else if (state.selectedEdgeId != 0) {
            if (const BpmnEdge* selEdge = diagram.FindEdge(state.selectedEdgeId)) {
                const BpmnNode* fn = diagram.FindNode(selEdge->fromNodeId);
                const BpmnNode* tn = diagram.FindNode(selEdge->toNodeId);
                if (fn && tn) {
                    const auto wpts = BuildEdgeWorldPoints(diagram, *fn, *tn, *selEdge, opts.routingMode);
                    Vec2 wMid{0.0f, 0.0f};
                    (void)SamplePolylineAtProgress(wpts, 0.5f, &wMid);
                    const Vec2 sMid = WorldToScreen(wMid, viewRect, pan, zoom);
                    const Rect bar(sMid.x - 110.0f * s, sMid.y - 38.0f * s, 220.0f * s, 26.0f * s);
                    canvas.DrawRoundRect(bar, 5.0f * s, pal.hudBg, pal.selectAccent, 1.2f * s);
                    static constexpr keel::StaticArray<keel::StringRef, 6> kEdgeActions = {"Seq", "Def \\", "Cond", "Msg", "Auto", "Del"};
                    const keel::f32 bw = (bar.w - 8.0f * s) / 6.0f;
                    for (keel::i32 i = 0; i < 6; ++i) {
                        const Rect br(bar.x + 4.0f * s + keel::as<keel::f32>(i) * bw, bar.y + 3.0f * s, bw - 2.0f * s, bar.h - 6.0f * s);
                        canvas.DrawRoundRect(br, 3.0f * s, (i == 5) ? pal.endEventFill : pal.hudHover);
                        canvas.DrawTextBox(kEdgeActions[keel::as<keel::usize>(i)], br, 8.6f * s, (i == 5) ? pal.endEventStroke : pal.textPrimary, 0.5f, 0.5f);
                    }
                }
            }
        } else if (state.selectedLaneId != 0) {
            if (const BpmnLane* selLane = diagram.FindLane(state.selectedLaneId)) {
                const Rect lsr = WorldToScreenRect(selLane->rect, viewRect, pan, zoom);
                const Rect bar(keel::max(viewRect.x + 58.0f * s, lsr.x + 44.0f * s), keel::max(viewRect.y + 48.0f * s, lsr.y - 30.0f * s), 232.0f * s, 26.0f * s);
                canvas.DrawRoundRect(bar, 5.0f * s, pal.hudBg, pal.selectAccent, 1.2f * s);
                static constexpr keel::StaticArray<keel::StringRef, 4> kLaneActions = {"+Above", "+Below", "Split /2", "Del Lane"};
                const keel::f32 bw = (bar.w - 8.0f * s) / 4.0f;
                for (keel::i32 i = 0; i < 4; ++i) {
                    const Rect br(bar.x + 4.0f * s + keel::as<keel::f32>(i) * bw, bar.y + 3.0f * s, bw - 2.0f * s, bar.h - 6.0f * s);
                    canvas.DrawRoundRect(br, 3.0f * s, (i == 3) ? pal.endEventFill : pal.hudHover);
                    canvas.DrawTextBox(kLaneActions[keel::as<keel::usize>(i)], br, 9.0f * s, (i == 3) ? pal.endEventStroke : pal.textPrimary, 0.5f, 0.5f);
                }
            }
        }
    }

    // 12. Inline Label Editor Overlay (`Double-Click` / `F2`)
    if (state.editingNodeId != 0 || state.editingEdgeId != 0 || state.editingLaneId != 0) {
        Rect editBox(viewRect.Center().x - 90.0f * s, viewRect.Center().y - 16.0f * s, 180.0f * s, 30.0f * s);
        if (const BpmnNode* en = diagram.FindNode(state.editingNodeId)) {
            const Rect nsr = WorldToScreenRect(en->Bounds(), viewRect, pan, zoom);
            editBox = Rect(nsr.Center().x - 86.0f * s, nsr.Center().y - 15.0f * s, 172.0f * s, 30.0f * s);
        } else if (const BpmnEdge* ee = diagram.FindEdge(state.editingEdgeId)) {
            const BpmnNode* fn = diagram.FindNode(ee->fromNodeId);
            const BpmnNode* tn = diagram.FindNode(ee->toNodeId);
            if (fn && tn) {
                const auto wpts = BuildEdgeWorldPoints(diagram, *fn, *tn, *ee, opts.routingMode);
                Vec2 wMid{0.0f, 0.0f};
                (void)SamplePolylineAtProgress(wpts, 0.5f, &wMid);
                const Vec2 sMid = WorldToScreen(wMid, viewRect, pan, zoom);
                editBox = Rect(sMid.x - 80.0f * s, sMid.y - 15.0f * s, 160.0f * s, 30.0f * s);
            }
        } else if (const BpmnLane* el = diagram.FindLane(state.editingLaneId)) {
            const Rect lsr = WorldToScreenRect(el->rect, viewRect, pan, zoom);
            editBox = Rect(lsr.x + 48.0f * s, lsr.y + 6.0f * s, 180.0f * s, 28.0f * s);
        }
        canvas.DrawRoundRect(editBox, 5.0f * s, pal.hudBg, pal.selectAccent, 2.0f * s);
        keel::SmallString<256> displayStr = keel::SmallString<256>::from(payload->activeEditBuffer);
        if (payload->cursorBlinkOn) {
            displayStr.push_back('|');
        }
        canvas.DrawTextBox(displayStr, Rect(editBox.x + 8.0f * s, editBox.y, editBox.w - 16.0f * s, editBox.h), 11.5f * s, pal.textPrimary, 0.0f, 0.5f);
    }

    // 13. Left Floating `bpmn.io` Tool Palette
    if (opts.showLeftPalette && !opts.readOnly) {
        const Rect palRect = ComputeLeftPaletteRect(viewRect, opts.showTopBar, s);
        canvas.DrawRoundRect(palRect, 7.0f * s, pal.hudBg, pal.hudBorder, 1.2f * s);
        keel::f32 cy = palRect.y + 5.0f * s;
        const keel::f32 btnH = 25.0f * s;
        for (keel::usize idx = 0; idx < kPaletteItems.size(); ++idx) {
            const auto& item = kPaletteItems[idx];
            if (cy + btnH > palRect.Bottom() - 2.0f * s) break;
            const Rect br(palRect.x + 4.0f * s, cy, palRect.w - 8.0f * s, btnH);
            const bool isActive = (state.activeTool == item.tool);
            const bool isHov = (state.hoveredPaletteIdx == keel::as<keel::i32>(idx));
            if (isActive || isHov) {
                canvas.DrawRoundRect(br, 4.0f * s, isActive ? pal.hudActiveBg : pal.hudHover, isActive ? pal.selectAccent : pal.hudBorder, 1.1f * s);
            }
            DrawPaletteButtonIcon(canvas, br, item.tool, (isActive || isHov) ? pal.selectAccent : pal.textPrimary, pal, s);
            if (isHov) {
                const keel::f32 tfs = 10.0f * s;
                const Vec2 tm = canvas.MeasureText(item.tooltip, tfs);
                const Rect tipRect(palRect.Right() + 8.0f * s, br.Center().y - tm.y * 0.5f - 3.5f * s, tm.x + 12.0f * s, tm.y + 7.0f * s);
                canvas.DrawRoundRect(tipRect, 4.0f * s, pal.hudBg, pal.selectAccent, 1.0f * s);
                canvas.DrawTextBox(item.tooltip, tipRect, tfs, pal.textPrimary, 0.5f, 0.5f);
            }
            cy += btnH + 2.0f * s;
            if (item.separatorAfter) {
                canvas.DrawLine({palRect.x + 6.0f * s, cy + 1.5f * s}, {palRect.Right() - 6.0f * s, cy + 1.5f * s}, pal.hudBorder, 1.0f * s);
                cy += 5.0f * s;
            }
        }
    }

    // 14. Top Floating BPMN Status & Token Simulation Bar
    if (opts.showTopBar) {
        const Rect topBar = ComputeTopBarRect(viewRect, s);
        canvas.DrawRoundRect(topBar, 6.0f * s, pal.hudBg, pal.hudBorder, 1.2f * s);

        char headerBuf[256];
        const auto headerStr = keel::format_buf(
            headerBuf,
            "BPMN 2.0  |  %s   (%zu nodes, %zu flows, %zu lanes)",
            diagram.name,
            diagram.nodes.size(),
            diagram.edges.size(),
            diagram.lanes.size()
        );
        canvas.DrawTextBox(
            headerStr,
            Rect(topBar.x + 12.0f * s, topBar.y, topBar.w * 0.48f, topBar.h),
            11.0f * s,
            pal.textPrimary,
            0.0f,
            0.5f
        );

        keel::f32 rx = topBar.Right() - 6.0f * s;
        auto drawTopPill = [&](keel::StringRef text, keel::f32 w, bool active, Color activeCol) -> Rect {
            rx -= w;
            const Rect pr(rx, topBar.y + 4.0f * s, w, topBar.h - 8.0f * s);
            rx -= 5.0f * s;
            canvas.DrawRoundRect(
                pr,
                4.0f * s,
                active ? activeCol.WithAlpha(42) : pal.hudHover,
                active ? activeCol : pal.hudBorder,
                1.1f * s
            );
            canvas.DrawTextBox(text, pr, 10.0f * s, active ? activeCol : pal.textPrimary, 0.5f, 0.5f);
            return pr;
        };

        if (opts.tokenSimulation) {
            (void)drawTopPill("+ Trigger Token", 96.0f * s, true, pal.simActiveGreen);
        }
        (void)drawTopPill(
            opts.tokenSimulation ? "● Token Sim: ON" : "▶ Token Sim",
            106.0f * s,
            opts.tokenSimulation,
            pal.simActiveGreen
        );
        (void)drawTopPill(
            (opts.routingMode == BpmnRoutingMode::Orthogonal) ? "Route: Ortho" : "Route: Bezier",
            86.0f * s,
            false,
            pal.selectAccent
        );
        (void)drawTopPill("Redo", 44.0f * s, !state.redoStack.empty(), pal.selectAccent);
        (void)drawTopPill("Undo", 44.0f * s, !state.undoStack.empty(), pal.selectAccent);
        (void)drawTopPill("XML", 44.0f * s, false, pal.selectAccent);
    }

    // 15. Bottom-Left Zoom & Fit HUD
    if (opts.showZoomControls) {
        const Rect zHud = ComputeZoomHudRect(viewRect, s);
        canvas.DrawRoundRect(zHud, 5.0f * s, pal.hudBg, pal.hudBorder, 1.2f * s);
        const keel::f32 btnW = 28.0f * s;
        const Rect minusR(zHud.x + 2.0f * s, zHud.y + 2.0f * s, btnW, zHud.h - 4.0f * s);
        const Rect pctR(minusR.Right() + 2.0f * s, zHud.y + 2.0f * s, 58.0f * s, zHud.h - 4.0f * s);
        const Rect plusR(pctR.Right() + 2.0f * s, zHud.y + 2.0f * s, btnW, zHud.h - 4.0f * s);
        const Rect fitR(plusR.Right() + 4.0f * s, zHud.y + 2.0f * s, zHud.Right() - plusR.Right() - 6.0f * s, zHud.h - 4.0f * s);

        canvas.DrawRoundRect(minusR, 3.0f * s, pal.hudHover);
        canvas.DrawTextBox("-", minusR, 12.0f * s, pal.textPrimary, 0.5f, 0.5f);
        char zBuf[24];
        const keel::StringRef zStr = keel::format_buf(zBuf, "%.0f%%", zoom * 100.0f);
        canvas.DrawTextBox(zStr, pctR, 10.5f * s, pal.textSecondary, 0.5f, 0.5f);
        canvas.DrawRoundRect(plusR, 3.0f * s, pal.hudHover);
        canvas.DrawTextBox("+", plusR, 12.0f * s, pal.textPrimary, 0.5f, 0.5f);
        canvas.DrawRoundRect(fitR, 3.0f * s, pal.hudHover);
        canvas.DrawTextBox("Fit", fitR, 10.0f * s, pal.selectAccent, 0.5f, 0.5f);
    }

    // 16. Bottom-Right Interactive Minimap
    if (opts.showMinimap && viewRect.w >= 420.0f * s && viewRect.h >= 260.0f * s) {
        const Rect mm = ComputeMinimapRect(viewRect, s);
        canvas.DrawRoundRect(mm, 6.0f * s, pal.hudBg, pal.hudBorder, 1.2f * s);
        canvas.PushClip(mm);

        const Rect wb = ComputeDiagramWorldBounds(diagram);
        const keel::f32 pad = 8.0f * s;
        const keel::f32 sx = (mm.w - pad * 2.0f) / keel::max(100.0f, wb.w);
        const keel::f32 sy = (mm.h - pad * 2.0f) / keel::max(100.0f, wb.h);
        const keel::f32 ms = keel::min(sx, sy);
        const keel::f32 ox = mm.x + (mm.w - wb.w * ms) * 0.5f - wb.x * ms;
        const keel::f32 oy = mm.y + (mm.h - wb.h * ms) * 0.5f - wb.y * ms;

        for (const auto& l : diagram.lanes) {
            canvas.DrawRect(Rect(ox + l.rect.x * ms, oy + l.rect.y * ms, l.rect.w * ms, l.rect.h * ms), pal.laneHeaderBg.WithAlpha(110));
        }
        for (const auto& n : diagram.nodes) {
            Color nc = pal.taskAccent;
            if (n.type == BpmnNodeType::StartEvent) nc = pal.startEventStroke;
            else if (n.type == BpmnNodeType::EndEvent) nc = pal.endEventStroke;
            else if (n.type == BpmnNodeType::Gateway) nc = pal.gatewayStroke;
            canvas.DrawRect(Rect(ox + n.pos.x * ms, oy + n.pos.y * ms, keel::max(2.5f, n.size.x * ms), keel::max(2.5f, n.size.y * ms)), nc);
        }

        const Vec2 camTL = ScreenToWorld({viewRect.x, viewRect.y}, viewRect, pan, zoom);
        const Vec2 camBR = ScreenToWorld({viewRect.Right(), viewRect.Bottom()}, viewRect, pan, zoom);
        const Rect camRect(
            ox + camTL.x * ms,
            oy + camTL.y * ms,
            (camBR.x - camTL.x) * ms,
            (camBR.y - camTL.y) * ms
        );
        canvas.DrawRoundRect(camRect, 2.0f * s, pal.selectGlow, pal.selectAccent, 1.2f * s);
        canvas.PopClip();
    }

    canvas.PopClip();
}


} // namespace guikit::detail::bpmn_impl
