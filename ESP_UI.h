#pragma once
// ============================================================================
// ESP_UI.h — nuevo diseño del menú (barra lateral + tarjetas + sliders custom)
// SOLO VISUAL: usa las mismas variables/funciones que ya existen en ESP.h.
// Se incluye en ESP.h en el lugar donde estaban ESP_ApplyStyle() y ESP_DrawMenu().
// ============================================================================
#include <cstdio>
#include <cmath>
#include <cfloat>

// Iconos Font Awesome (por si tu Icon.h no los trae)
#ifndef ICON_FA_USER
#define ICON_FA_USER "\xef\x80\x87"
#endif
#ifndef ICON_FA_MOON
#define ICON_FA_MOON "\xef\x86\x86"
#endif
#ifndef ICON_FA_SUN
#define ICON_FA_SUN "\xef\x86\x85"
#endif
#ifndef ICON_FA_TIMES
#define ICON_FA_TIMES "\xef\x80\x8d"
#endif
#ifndef ICON_FA_PAPER_PLANE
#define ICON_FA_PAPER_PLANE "\xef\x87\x98"
#endif
#ifndef ICON_FA_CUBE
#define ICON_FA_CUBE "\xef\x86\xb2"
#endif

// ============================================================================
// Estado del tema (se puede guardar en ESP_SaveSettings, ver instrucciones)
// ============================================================================
static int  g_uiAccent  = 1;     // 0 Red, 1 Blue, 2 Green, 3 Pink
static bool g_uiRounded = true;  // Rounded / Square
static bool g_uiDark    = true;  // Dark / Light
static int  g_uiTab     = 0;

static bool g_uiSliderDrag     = false;
static bool g_uiSliderDragPrev = false;

struct UIPal {
    ImU32 bg, panel, panelHi, text, dim, track, accent, accentSoft, onAccent, border;
};
static UIPal g_pal;

static inline ImU32 UI_U32(const ImVec4 &v) { return ImGui::ColorConvertFloat4ToU32(v); }
static inline float UI_Clamp01(float v) { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); }
static inline float UI_CardRad() { return g_uiRounded ? 14.f : 4.f; }
static inline float UI_PillRad(float h) { return g_uiRounded ? h * 0.5f : 6.f; }

// Evita que un arrastre (scroll táctil) cuente como click
static inline bool UI_ClickOK() {
    return ImGui::GetIO().MouseDragMaxDistanceSqr[0] < 64.f;
}

// ============================================================================
// Tema
// ============================================================================
static void ESP_ApplyTheme(void) {
    static const ImVec4 kAccent[4] = {
        ImVec4(0.93f, 0.27f, 0.31f, 1.f),   // Red
        ImVec4(0.18f, 0.44f, 1.00f, 1.f),   // Blue
        ImVec4(0.14f, 0.78f, 0.46f, 1.f),   // Green
        ImVec4(0.96f, 0.33f, 0.66f, 1.f),   // Pink
    };
    const ImVec4 acc = kAccent[g_uiAccent & 3];
    const bool d = g_uiDark;

    const ImVec4 bg      = d ? ImVec4(0.06f, 0.06f, 0.08f, 0.98f) : ImVec4(0.95f, 0.95f, 0.97f, 0.98f);
    const ImVec4 panel   = d ? ImVec4(0.11f, 0.11f, 0.14f, 1.f)   : ImVec4(1.00f, 1.00f, 1.00f, 1.f);
    const ImVec4 panelHi = d ? ImVec4(0.16f, 0.16f, 0.20f, 1.f)   : ImVec4(0.90f, 0.91f, 0.95f, 1.f);
    const ImVec4 text    = d ? ImVec4(0.95f, 0.95f, 0.97f, 1.f)   : ImVec4(0.08f, 0.08f, 0.11f, 1.f);
    const ImVec4 dim     = d ? ImVec4(0.55f, 0.55f, 0.62f, 1.f)   : ImVec4(0.42f, 0.42f, 0.50f, 1.f);
    const ImVec4 track   = d ? ImVec4(0.32f, 0.32f, 0.36f, 1.f)   : ImVec4(0.78f, 0.78f, 0.83f, 1.f);
    const ImVec4 accHi   = ImVec4(fminf(acc.x * 1.10f, 1.f), fminf(acc.y * 1.10f, 1.f), fminf(acc.z * 1.10f, 1.f), 1.f);
    const ImVec4 accLo   = ImVec4(acc.x * 0.80f, acc.y * 0.80f, acc.z * 0.80f, 1.f);

    g_pal.bg         = UI_U32(bg);
    g_pal.panel      = UI_U32(panel);
    g_pal.panelHi    = UI_U32(panelHi);
    g_pal.text       = UI_U32(text);
    g_pal.dim        = UI_U32(dim);
    g_pal.track      = UI_U32(track);
    g_pal.accent     = UI_U32(acc);
    g_pal.accentSoft = UI_U32(ImVec4(acc.x, acc.y, acc.z, 0.18f));
    g_pal.onAccent   = IM_COL32(255, 255, 255, 255);
    g_pal.border     = UI_U32(ImVec4(acc.x, acc.y, acc.z, 0.35f));

    ImGuiStyle &s = ImGui::GetStyle();
    const bool r = g_uiRounded;
    s.WindowPadding     = ImVec2(14, 14);
    s.FramePadding      = ImVec2(10, 7);
    s.ItemSpacing       = ImVec2(10, 9);
    s.ItemInnerSpacing  = ImVec2(8, 6);
    s.ScrollbarSize     = 6.f;
    s.GrabMinSize       = 18.f;
    s.WindowBorderSize  = 1.f;
    s.ChildBorderSize   = 0.f;
    s.FrameBorderSize   = 0.f;
    s.TabBorderSize     = 0.f;
    s.WindowRounding    = r ? 22.f : 6.f;
    s.ChildRounding     = r ? 16.f : 4.f;
    s.FrameRounding     = r ? 12.f : 3.f;
    s.GrabRounding      = r ? 12.f : 3.f;
    s.ScrollbarRounding = r ? 8.f  : 2.f;
    s.TabRounding       = r ? 10.f : 3.f;
    s.Alpha             = 1.0f;

    ImVec4 *c = s.Colors;
    c[ImGuiCol_Text]                 = text;
    c[ImGuiCol_TextDisabled]         = dim;
    c[ImGuiCol_WindowBg]             = bg;
    c[ImGuiCol_ChildBg]              = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_PopupBg]              = bg;
    c[ImGuiCol_Border]               = ImVec4(acc.x, acc.y, acc.z, 0.35f);
    c[ImGuiCol_BorderShadow]         = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_TitleBg]              = panel;
    c[ImGuiCol_TitleBgActive]        = panel;
    c[ImGuiCol_TitleBgCollapsed]     = panel;
    c[ImGuiCol_FrameBg]              = panel;
    c[ImGuiCol_FrameBgHovered]       = panelHi;
    c[ImGuiCol_FrameBgActive]        = panelHi;
    c[ImGuiCol_CheckMark]            = acc;
    c[ImGuiCol_SliderGrab]           = acc;
    c[ImGuiCol_SliderGrabActive]     = accHi;
    c[ImGuiCol_Button]               = acc;
    c[ImGuiCol_ButtonHovered]        = accHi;
    c[ImGuiCol_ButtonActive]         = accLo;
    c[ImGuiCol_Header]               = panel;
    c[ImGuiCol_HeaderHovered]        = panelHi;
    c[ImGuiCol_HeaderActive]         = panelHi;
    c[ImGuiCol_Separator]            = ImVec4(dim.x, dim.y, dim.z, 0.25f);
    c[ImGuiCol_SeparatorHovered]     = acc;
    c[ImGuiCol_SeparatorActive]      = accHi;
    c[ImGuiCol_ScrollbarBg]          = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_ScrollbarGrab]        = track;
    c[ImGuiCol_ScrollbarGrabHovered] = acc;
    c[ImGuiCol_ScrollbarGrabActive]  = accHi;
    c[ImGuiCol_Tab]                  = panel;
    c[ImGuiCol_TabHovered]           = accHi;
    c[ImGuiCol_TabActive]            = acc;
    c[ImGuiCol_TabUnfocused]         = panel;
    c[ImGuiCol_TabUnfocusedActive]   = acc;
    c[ImGuiCol_ResizeGrip]           = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_ResizeGripHovered]    = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_ResizeGripActive]     = ImVec4(0, 0, 0, 0);
}

// ESP_ApplyStyle() se sigue llamando desde ImGuiDrawView.mm (init)
static void ESP_ApplyStyle(void) { ESP_ApplyTheme(); }

// ============================================================================
// Abrir enlaces (Telegram)
// ============================================================================
static void ESP_OpenURL(const char *url) {
    NSString *s = [NSString stringWithUTF8String:url];
    dispatch_async(dispatch_get_main_queue(), ^{
        NSURL *u = [NSURL URLWithString:s];
        if (u) [[UIApplication sharedApplication] openURL:u options:@{} completionHandler:nil];
    });
}

// ============================================================================
// Widgets custom
// ============================================================================

// Botón redondo con icono (barra lateral / cabecera)
static bool UI_IconButton(const char *id, const char *icon, float size, bool active, ImU32 fg) {
    ImVec2 p = ImGui::GetCursorScreenPos();
    bool clicked = ImGui::InvisibleButton(id, ImVec2(size, size)) && UI_ClickOK();
    bool hov = ImGui::IsItemHovered();
    ImDrawList *dl = ImGui::GetWindowDrawList();
    float rad = g_uiRounded ? size * 0.5f : 10.f;
    dl->AddRectFilled(p, ImVec2(p.x + size, p.y + size), (active || hov) ? g_pal.panelHi : g_pal.panel, rad);
    if (active)
        dl->AddRect(p, ImVec2(p.x + size, p.y + size), g_pal.border, rad, 0, 1.5f);
    ImVec2 ts = ImGui::CalcTextSize(icon);
    dl->AddText(ImVec2(p.x + (size - ts.x) * 0.5f, p.y + (size - ts.y) * 0.5f), fg, icon);
    return clicked;
}

// Título de sección en mayúsculas, color tenue
static void UI_Label(const char *t) {
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(g_pal.dim));
    ImGui::TextUnformatted(t);
    ImGui::PopStyleColor();
}

// Fila con interruptor (círculo gris / círculo de acento con check)
static bool UI_Toggle(const char *label, bool *v) {
    ImGui::PushID(label);
    const float fs = ImGui::GetFontSize();
    const float w = ImGui::GetContentRegionAvail().x;
    const float h = fs * 2.6f;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    bool clicked = ImGui::InvisibleButton("##t", ImVec2(w, h)) && UI_ClickOK();
    if (clicked) *v = !*v;
    bool hov = ImGui::IsItemHovered();

    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), hov ? g_pal.panelHi : g_pal.panel, UI_CardRad());
    dl->AddText(ImVec2(p.x + fs, p.y + (h - fs) * 0.5f), g_pal.text, label);

    const float r = fs * 0.85f;
    const ImVec2 c(p.x + w - fs - r, p.y + h * 0.5f);
    const ImU32 fill = *v ? g_pal.accent : g_pal.track;
    if (g_uiRounded) dl->AddCircleFilled(c, r, fill, 32);
    else             dl->AddRectFilled(ImVec2(c.x - r, c.y - r), ImVec2(c.x + r, c.y + r), fill, 5.f);
    if (*v) {
        ImVec2 pts[3] = {
            ImVec2(c.x - r * 0.42f, c.y + r * 0.02f),
            ImVec2(c.x - r * 0.10f, c.y + r * 0.36f),
            ImVec2(c.x + r * 0.45f, c.y - r * 0.32f)
        };
        dl->AddPolyline(pts, 3, g_pal.onAccent, ImDrawFlags_None, 2.2f);
    }
    ImGui::PopID();
    return clicked;
}

// Tarjeta con slider (etiqueta + valor en acento + barra con perilla blanca)
static bool UI_Slider(const char *label, float *v, float mn, float mx, const char *fmt) {
    ImGui::PushID(label);
    ImGuiIO &io = ImGui::GetIO();
    const float fs = ImGui::GetFontSize();
    const float w = ImGui::GetContentRegionAvail().x;
    const float h = fs * 4.0f;
    const ImVec2 p = ImGui::GetCursorScreenPos();

    ImGui::Dummy(ImVec2(w, h));
    // Solo la zona de la barra captura el arrastre (así el resto de la tarjeta sirve para hacer scroll)
    ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + h * 0.42f));
    ImGui::InvisibleButton("##sl", ImVec2(w, h * 0.58f));
    const bool active = ImGui::IsItemActive();

    const float padX = fs, knobW = fs * 2.2f, knobH = fs * 1.4f, trackH = fs * 0.5f;
    const float tl = p.x + padX, tr = p.x + w - padX;
    const float cy = p.y + h - fs * 1.35f;
    const float span = (tr - tl) - knobW;

    bool changed = false;
    if (active && span > 1.f) {
        g_uiSliderDrag = true;
        float t = UI_Clamp01((io.MousePos.x - (tl + knobW * 0.5f)) / span);
        float nv = mn + t * (mx - mn);
        if (nv != *v) { *v = nv; changed = true; }
    }
    const float t = UI_Clamp01((*v - mn) / (mx - mn));
    const float kx = tl + knobW * 0.5f + t * span;

    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), g_pal.panel, UI_CardRad());
    dl->AddText(ImVec2(p.x + padX, p.y + fs * 0.9f), g_pal.text, label);

    char buf[32];
    snprintf(buf, sizeof(buf), fmt, *v);
    ImVec2 ts = ImGui::CalcTextSize(buf);
    dl->AddText(ImVec2(p.x + w - padX - ts.x, p.y + fs * 0.9f), g_pal.accent, buf);

    const float tRad = g_uiRounded ? trackH * 0.5f : 2.f;
    dl->AddRectFilled(ImVec2(tl, cy - trackH * 0.5f), ImVec2(tr, cy + trackH * 0.5f), g_pal.track, tRad);
    dl->AddRectFilled(ImVec2(tl, cy - trackH * 0.5f), ImVec2(kx, cy + trackH * 0.5f), g_pal.accent, tRad);

    const float kRad = g_uiRounded ? knobH * 0.5f : 4.f;
    dl->AddRectFilled(ImVec2(kx - knobW * 0.5f, cy - knobH * 0.5f), ImVec2(kx + knobW * 0.5f, cy + knobH * 0.5f),
                      IM_COL32(255, 255, 255, 255), kRad);
    dl->AddRect(ImVec2(kx - knobW * 0.5f, cy - knobH * 0.5f), ImVec2(kx + knobW * 0.5f, cy + knobH * 0.5f),
                IM_COL32(0, 0, 0, 45), kRad, 0, 1.f);
    ImGui::PopID();
    return changed;
}

// Control segmentado (Red | Blue | Green | Pink, Rounded | Square...)
static bool UI_Segmented(const char *id, const char *const *items, int n, int *sel) {
    ImGui::PushID(id);
    const float fs = ImGui::GetFontSize();
    const float w = ImGui::GetContentRegionAvail().x;
    const float h = fs * 2.7f;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    bool pressed = ImGui::InvisibleButton("##seg", ImVec2(w, h)) && UI_ClickOK();

    bool changed = false;
    const float pad = 4.f;
    const float sw = (w - pad * 2.f) / (float)n;
    if (pressed) {
        int i = (int)((ImGui::GetIO().MousePos.x - p.x - pad) / sw);
        if (i < 0) i = 0;
        if (i >= n) i = n - 1;
        if (i != *sel) { *sel = i; changed = true; }
    }

    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), g_pal.panel, UI_PillRad(h));
    for (int i = 0; i < n; i++) {
        ImVec2 a(p.x + pad + sw * i, p.y + pad);
        ImVec2 b(a.x + sw, p.y + h - pad);
        bool on = (i == *sel);
        if (on) dl->AddRectFilled(a, b, g_pal.accent, UI_PillRad(h - pad * 2.f));
        ImVec2 ts = ImGui::CalcTextSize(items[i]);
        dl->AddText(ImVec2(a.x + (sw - ts.x) * 0.5f, p.y + (h - ts.y) * 0.5f),
                    on ? g_pal.onAccent : g_pal.text, items[i]);
    }
    ImGui::PopID();
    return changed;
}

// Fila informativa: etiqueta tenue a la izquierda, valor a la derecha
static void UI_InfoRow(const char *k, const char *val, ImU32 valCol = 0) {
    const float fs = ImGui::GetFontSize();
    const float w = ImGui::GetContentRegionAvail().x;
    const float h = fs * 2.3f;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(w, h));
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), g_pal.panel, UI_CardRad());
    dl->AddText(ImVec2(p.x + fs, p.y + (h - fs) * 0.5f), g_pal.dim, k);
    ImVec2 ts = ImGui::CalcTextSize(val);
    dl->AddText(ImVec2(p.x + w - fs - ts.x, p.y + (h - fs) * 0.5f), valCol ? valCol : g_pal.text, val);
}

// Botón de ancho completo
static bool UI_Button(const char *label, const char *icon, bool filled) {
    ImGui::PushID(label);
    const float fs = ImGui::GetFontSize();
    const float w = ImGui::GetContentRegionAvail().x;
    const float h = fs * 2.9f;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    bool clicked = ImGui::InvisibleButton("##b", ImVec2(w, h)) && UI_ClickOK();
    bool hov = ImGui::IsItemHovered(), act = ImGui::IsItemActive();

    ImU32 bg = filled ? g_pal.accent : (hov ? g_pal.panelHi : g_pal.panel);
    if (filled && act) bg = (g_pal.accent & 0x00FFFFFFu) | (200u << 24);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), bg, UI_PillRad(h));
    if (!filled) dl->AddRect(p, ImVec2(p.x + w, p.y + h), g_pal.border, UI_PillRad(h), 0, 1.2f);

    char buf[160];
    snprintf(buf, sizeof(buf), "%s  %s", icon, label);
    ImVec2 ts = ImGui::CalcTextSize(buf);
    dl->AddText(ImVec2(p.x + (w - ts.x) * 0.5f, p.y + (h - ts.y) * 0.5f),
                filled ? g_pal.onAccent : g_pal.text, buf);
    ImGui::PopID();
    return clicked;
}

// Cabecera: pastilla con título + botón tema (luna/sol) + botón cerrar
static void UI_Header(const char *icon, const char *title, bool *pOpen) {
    const float fs = ImGui::GetFontSize();
    const float H = fs * 2.9f;
    const float gap = ImGui::GetStyle().ItemSpacing.x;
    const float pillW = ImGui::GetContentRegionAvail().x - 2.f * (H + gap);
    const ImVec2 p = ImGui::GetCursorScreenPos();

    ImGui::Dummy(ImVec2(pillW, H));
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + pillW, p.y + H), g_pal.panel, UI_PillRad(H));
    char buf[96];
    snprintf(buf, sizeof(buf), "%s  %s", icon, title);
    dl->AddText(ImVec2(p.x + fs * 1.2f, p.y + (H - fs) * 0.5f), g_pal.accent, buf);

    ImGui::SameLine();
    if (UI_IconButton("##theme", g_uiDark ? ICON_FA_MOON : ICON_FA_SUN, H, false, g_pal.accent))
        g_uiDark = !g_uiDark;
    ImGui::SameLine();
    if (UI_IconButton("##close", ICON_FA_TIMES, H, false, IM_COL32(236, 64, 76, 255)) && pOpen)
        *pOpen = false;
}

// ============================================================================
// Pestañas
// ============================================================================
static void UI_TabESP(void) {
    UI_Label("GENERAL");
    UI_Toggle("Enable ESP", &g_espEnabled);

    UI_Label("DISPLAY");
    UI_Toggle("Prediction Lines", &g_drawPredictionLine);
    UI_Toggle("Draw Pockets", &g_drawPockets);
    UI_Toggle("Pocket Shot State", &g_drawPocketsShotState);

    UI_Label("LINE");
    UI_Slider("Line Width", &g_lineWidth, 0.5f, 5.0f, "%.1f");

    UI_Label("TABLE CALIBRATION (OFFSETS)");
    UI_Slider("Offset X", &g_tableOffsetX, -50.0f, 50.0f, "%.1f");
    UI_Slider("Offset Y", &g_tableOffsetY, -50.0f, 50.0f, "%.1f");
    UI_Slider("Scale Factor", &g_tableScaleFactor, 0.900f, 1.100f, "%.3f");
}

static void UI_TabAuto(void) {
    UI_Label("ASSIST");
    UI_Toggle("Auto Aim (only rotate cue)", &AutoPlay::bAutoAim);
    if (UI_Toggle("AutoPlay", &AutoPlay::bAutoPlaying) && AutoPlay::bAutoPlaying)
        AutoPlay::ClearState();

    UI_Label("STATUS");
    const char *st =
        AutoPlay::state == AutoPlay::IDLE ? "Idle" :
        AutoPlay::state == AutoPlay::SCANNING ? "Scanning" :
        AutoPlay::state == AutoPlay::SELECTING ? "Selecting" :
        AutoPlay::state == AutoPlay::NOMINATING ? "Nominating" : "Shooting";
    UI_InfoRow("State", st);
    UI_InfoRow("Detected Rule", isNineBallRuntime() ? "9-Ball" : "8-Ball");
    UI_InfoRow("Call pocket every shot", isCallPocketEveryShotMode() ? "YES" : "NO");
    UI_InfoRow("Power", "MAX | Scan 7s + Select 1s");
    if (g_CurrentCandidate.idx != -1) {
        char buf[64];
        snprintf(buf, sizeof(buf), "Ball %d -> Pocket %d", g_CurrentCandidate.idx, g_CurrentCandidate.pocketIndex);
        UI_InfoRow("Target", buf, IM_COL32(60, 220, 110, 255));
    }

    UI_Label("QUEUE");
    UI_Toggle("AutoQueue (Rematch)", &AutoQueue::bAutoQueue);
    if (AutoQueue::bAutoQueue) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", AutoQueue::getMenuStateId());
        UI_InfoRow("MenuState", buf);
    }
}

static void UI_TabSettings(void) {
    UI_Label("PRIMARY COLOR");
    static const char *cols[] = {"Red", "Blue", "Green", "Pink"};
    UI_Segmented("##accent", cols, 4, &g_uiAccent);

    UI_Label("MENU STYLE");
    static const char *sty[] = {"Rounded", "Square"};
    int stSel = g_uiRounded ? 0 : 1;
    if (UI_Segmented("##style", sty, 2, &stSel)) g_uiRounded = (stSel == 0);

    UI_Label("UI CUSTOMIZATION");
    UI_Toggle("Dark Mode", &g_uiDark);

    UI_Label("PRIVACY");
    UI_Toggle("I'm Not Hacker", &g_noScreenShot);
}

static void UI_TabAccount(void) {
    const float fs = ImGui::GetFontSize();
    const float w = ImGui::GetContentRegionAvail().x;
    const float h = fs * 6.6f;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(w, h));
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), g_pal.panel, UI_CardRad());

    // Avatar
    const float ar = fs * 1.9f;
    const ImVec2 c(p.x + fs * 1.2f + ar, p.y + h * 0.5f);
    if (g_uiRounded) dl->AddCircleFilled(c, ar, g_pal.accent, 40);
    else             dl->AddRectFilled(ImVec2(c.x - ar, c.y - ar), ImVec2(c.x + ar, c.y + ar), g_pal.accent, 8.f);
    {
        const float isz = fs * 2.2f;
        ImVec2 its = ImGui::GetFont()->CalcTextSizeA(isz, FLT_MAX, 0.f, "D");
        dl->AddText(ImGui::GetFont(), isz, ImVec2(c.x - its.x * 0.5f, c.y - its.y * 0.5f), g_pal.onAccent, "D");
    }

    // Nombre + producto + insignia
    const float tx = c.x + ar + fs;
    const float ny = p.y + h * 0.5f - fs * 2.2f;
    dl->AddText(ImGui::GetFont(), fs * 1.35f, ImVec2(tx, ny), g_pal.text, "Dariel Modz");
    dl->AddText(ImVec2(tx, ny + fs * 1.75f), g_pal.dim, "8 Ball Pool iOS");
    {
        const float bs = fs * 0.8f;
        const char *bt = "CREATOR";
        ImVec2 bts = ImGui::GetFont()->CalcTextSizeA(bs, FLT_MAX, 0.f, bt);
        float bw = bts.x + fs * 1.2f, bh = fs * 1.5f;
        ImVec2 bp(tx, ny + fs * 3.5f);
        dl->AddRectFilled(bp, ImVec2(bp.x + bw, bp.y + bh), g_pal.accentSoft, g_uiRounded ? bh * 0.5f : 4.f);
        dl->AddText(ImGui::GetFont(), bs, ImVec2(bp.x + (bw - bts.x) * 0.5f, bp.y + (bh - bts.y) * 0.5f), g_pal.accent, bt);
    }

    UI_Label("PANEL INFO");
    UI_InfoRow("Product", "8 Ball Pool iOS");
    UI_InfoRow("Version", "v1.0 (iOS)");
    UI_InfoRow("Creator", "@didierxit7", g_pal.accent);
    UI_InfoRow("Group", "t.me/primexios");
    UI_InfoRow("Status", isInGame() ? "In match" : "Lobby",
               isInGame() ? IM_COL32(60, 220, 110, 255) : 0);

    UI_Label("COMMUNITY");
    if (UI_Button("Telegram Group", ICON_FA_PAPER_PLANE, true))
        ESP_OpenURL("https://t.me/primexios");
    if (UI_Button("Creator  @didierxit7", ICON_FA_USER, false))
        ESP_OpenURL("https://t.me/didierxit7");
}

// Contenido de debug (el mismo que tenías en la pestaña Misc)
static void UI_TabMisc(void) {
    UI_Label("DEBUG");
    ImGui::Text("GameManager: 0x%llX %s", (unsigned long long)g_gameManager, isValidPtr(g_gameManager) ? "OK" : "N/A");
    ImGui::Text("MainManager: 0x%llX", (unsigned long long)g_mainManager);
    ImGui::Text("In Game: %s", isInGame() ? "Yes" : "No");
    ImGui::Text("Player Turn: %s", isPlayerTurn() ? "Yes" : "No");
    ImGui::Text("GM StateID: %d", g_cache.gmStateId);
    ImGui::Text("MM StateID: %d", g_cache.mmStateId);
    ImGui::Text("mGameMode: %d", getRuntimeGameMode());
    ImGui::Text("Mode name: %s", gameModeName(getRuntimeGameMode()));
    ImGui::Text("is9BallGame(): %s", g_cache.is9BallByGameApi ? "true" : "false");
    ImGui::Text("Rule detect: %s", isNineBallRuntime() ? "9-Ball" : "8-Ball");
    ImGui::Text("Call pocket mode: %s", isCallPocketEveryShotMode() ? "All shots" : "Normal");
    ImGui::Text("Table: 0x%llX", (unsigned long long)GM_Table());
    ImGui::Text("Balls: %d", getTableBallCount(GM_Table()));
    ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
    ImGui::Separator();

    if (ImGui::TreeNode("Cue Power/Spin (FULL DUMP)")) {
        const char *srcName[] = {"Fallback", "Interp", "Table"};
        auto &d = g_diag;
        ImGui::TextColored(ImVec4(0.3f,1.0f,0.3f,1.0f), "EFFECTIVE MaxPower: %.4f [%s]", g_effectiveMaxPower, srcName[g_powerSource]);
        ImGui::TextColored(ImVec4(0.3f,1.0f,0.3f,1.0f), "EFFECTIVE SpinMul:  %.11f [%s]", CUE_PROPERTIES_SPIN_VALUE, srcName[g_spinSource]);
        ImGui::TextColored(ImVec4(0.3f,1.0f,0.3f,1.0f), "ShotPower (computed): %.4f", d.computedShotPower);
        ImGui::TextColored(ImVec4(0.3f,1.0f,0.3f,1.0f), "ShotSpin  (computed): (%.6f, %.6f)", d.computedSpinX, d.computedSpinY);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f,0.8f,0.3f,1.0f), "UserInfo Ratios (server-sent):");
        ImGui::Text("  Power: low=%d high=%d", d.lowPowerRatio, d.highPowerRatio);
        ImGui::Text("  Spin:  low=%d high=%d", d.lowSpinRatio, d.highSpinRatio);
        ImGui::Text("  Aim:   low=%d high=%d", d.lowAimRatio, d.highAimRatio);
        ImGui::Text("  Interp Power: %.4f", d.interpPower);
        ImGui::Text("  Interp Spin:  %.4f (raw int)", d.interpSpin);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.5f,0.8f,1.0f,1.0f), "CueId: %d", g_cachedCueId);
        ImGui::Text("Base  (getCueStats):       P=%d S=%d A=%d T=%d", d.basePower, d.baseSpin, d.baseAim, d.baseTime);
        ImGui::Text("Bonus (getCueStatsBonus):  P=%d S=%d A=%d T=%d", d.bonusPower, d.bonusSpin, d.bonusAim, d.bonusTime);
        ImGui::Text("Table Power[%d]: %.4f", d.bonusPower, d.tablePowerLookup);
        ImGui::Text("Table Spin[%d]:  %.11f", d.bonusSpin, d.tableSpinLookup);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f,0.6f,0.6f,1.0f), "VisualCue ivars:");
        ImGui::Text("  mPower: %.6f (0-1)", d.vc_mPower);
        ImGui::Text("  mCurrentPowerOffset: %.4f", d.vc_mCurrentPowerOffset);
        ImGui::Text("  mMaxCuePowerOffset:  %.4f", d.vc_mMaxCuePowerOffset);
        ImGui::Text("  mPowerMode: %s", d.vc_mPowerMode ? "Y" : "N");
        ImGui::Text("  powerControlActive: %s", d.vc_powerControlActive ? "Y" : "N");
        ImGui::Text("  mPowerSliderVisible: %s", d.vc_mPowerSliderVisible ? "Y" : "N");
        ImGui::Text("  mControlMode: %d", d.vc_mControlMode);
        ImGui::Text("  mMaxAngleForPowerMode: %.4f", d.vc_mMaxAngleForPowerMode);
        ImGui::Text("  mDistanceFromTip: %.4f", d.vc_mDistanceFromTip);
        ImGui::Text("  mAnimatePowerTarget: %.6f", d.vc_mAnimatePowerTarget);
        ImGui::Text("  mUserSensitivity: %.4f", d.vc_mUserSensitivity);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f,0.6f,0.6f,1.0f), "GamePowerBarView:");
        ImGui::Text("  getMinimumPower: %.6f", d.pbv_minimumPower);
        ImGui::Text("  getPowerSliderDistance: %.4f", d.pbv_sliderDistance);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.6f,0.6f,1.0f,1.0f), "VisualEnglishControl ivars:");
        ImGui::Text("  mEnglish: (%.6f, %.6f)", d.vec_englishX, d.vec_englishY);
        ImGui::Text("  mSpinLevel: %u", d.vec_spinLevel);
        ImGui::Text("  mInputRadius: %.4f", d.vec_mInputRadius);
        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNode("Resolved Offsets")) {
        ImGui::Text("GM.mTable: 0x%lX", (long)g_off.gm_mTable);
        ImGui::Text("GM.mVisualCue: 0x%lX", (long)g_off.gm_mVisualCue);
        ImGui::Text("GM.mStateManager: 0x%lX", (long)g_off.gm_mStateManager);
        ImGui::Text("Tbl.mBalls: 0x%lX", (long)g_off.tbl_mBalls);
        ImGui::Text("Tbl.mTableProperties: 0x%lX", (long)g_off.tbl_mTableProperties);
        ImGui::Text("Ball._physicsProperties: 0x%lX", (long)g_off.ball_physicsProperties);
        ImGui::Text("Ball.classification: 0x%lX", (long)g_off.ball_classification);
        ImGui::Text("Ball.state: 0x%lX", (long)g_off.ball_state);
        ImGui::Text("VC.mVisualGuide: 0x%lX", (long)g_off.vc_mVisualGuide);
        ImGui::Text("VC.mPower: 0x%lX", (long)g_off.vc_mPower);
        ImGui::Text("SM.mStateStack: 0x%lX", (long)g_off.sm_mStateStack);
        ImGui::Text("MM.mStateManager: 0x%lX", (long)g_off.mm_mStateManager);
        ImGui::Text("VC.mMaxCuePowerOffset: 0x%lX", (long)g_off.vc_mMaxCuePowerOffset);
        ImGui::Text("VEC.mSpinLevel: 0x%lX", (long)g_off.vec_spinLevel);
        ImGui::TreePop();
    }
    ImGui::Separator(); ImGui::Spacing();

    static bool logSaved = false;
    static char logStatus[128] = "";
    if (UI_Button("Save Log to Documents", ICON_FA_COG, false)) {
        @try {
            NSMutableString *log = [NSMutableString string];
            [log appendFormat:@"=== 8BP Debug Log ===\n"];
            [log appendFormat:@"Date: %@\n", [NSDate date]];
            [log appendFormat:@"poolBase: 0x%llX  slide: 0x%lX\n", (unsigned long long)g_poolBase, g_poolSlide];
            [log appendFormat:@"GameManager: 0x%llX\n", (unsigned long long)g_gameManager];
            [log appendFormat:@"MainManager: 0x%llX\n", (unsigned long long)g_mainManager];
            [log appendFormat:@"MenuManager: 0x%llX\n", (unsigned long long)g_menuManager];
            [log appendFormat:@"UserInfo: 0x%llX\n", (unsigned long long)g_userInfo];
            [log appendFormat:@"InGame: %d  PlayerTurn: %d\n", isInGame(), isPlayerTurn()];
            [log appendFormat:@"GM_StateID: %d  MM_StateID: %d\n", g_cache.gmStateId, g_cache.mmStateId];
            [log appendFormat:@"mGameMode: %d (%s)\n", getRuntimeGameMode(), gameModeName(getRuntimeGameMode())];
            [log appendFormat:@"is9BallGame(): %d  RuleDetected: %s\n", g_cache.is9BallByGameApi ? 1 : 0, isNineBallRuntime() ? "9-Ball" : "8-Ball"];
            [log appendFormat:@"CallPocketEveryShot: %d\n", isCallPocketEveryShotMode() ? 1 : 0];
            [log appendFormat:@"Table: 0x%llX  Balls: %d\n", (unsigned long long)GM_Table(), getTableBallCount(GM_Table())];
            [log appendFormat:@"\n=== Resolved Offsets ===\n"];
            [log appendFormat:@"GM.mTable: 0x%lX\n", (long)g_off.gm_mTable];
            [log appendFormat:@"GM.mVisualCue: 0x%lX\n", (long)g_off.gm_mVisualCue];
            [log appendFormat:@"GM.mVisualEnglishControl: 0x%lX\n", (long)g_off.gm_mVisualEnglishControl];
            [log appendFormat:@"GM.mStateManager: 0x%lX\n", (long)g_off.gm_mStateManager];
            [log appendFormat:@"GM._rules: 0x%lX\n", (long)g_off.gm_rules];
            [log appendFormat:@"Tbl.mTableProperties: 0x%lX\n", (long)g_off.tbl_mTableProperties];
            [log appendFormat:@"Tbl.mBalls: 0x%lX\n", (long)g_off.tbl_mBalls];
            [log appendFormat:@"Tbl._frictionProperties: 0x%lX\n", (long)g_off.tbl_frictionProperties];
            [log appendFormat:@"Tbl.mTableCollisionBounds: 0x%lX\n", (long)g_off.tbl_mTableCollisionBounds];
            [log appendFormat:@"VC.mVisualGuide: 0x%lX\n", (long)g_off.vc_mVisualGuide];
            [log appendFormat:@"VC.mPower: 0x%lX\n", (long)g_off.vc_mPower];
            [log appendFormat:@"VEC.mEnglish: 0x%lX\n", (long)g_off.vec_mEnglish];
            [log appendFormat:@"SM.mStateStack: 0x%lX\n", (long)g_off.sm_mStateStack];
            [log appendFormat:@"MM.mStateManager: 0x%lX\n", (long)g_off.mm_mStateManager];
            [log appendFormat:@"TP.mPockets: 0x%lX\n", (long)g_off.tp_mPockets];
            [log appendFormat:@"Ball._physicsProperties: 0x%lX\n", (long)g_off.ball_physicsProperties];
            [log appendFormat:@"Ball.classification: 0x%lX\n", (long)g_off.ball_classification];
            [log appendFormat:@"Ball.state: 0x%lX\n", (long)g_off.ball_state];
            {
                const char *src[] = {"Fallback", "Interp", "Table"};
                auto &d = g_diag;
                [log appendFormat:@"\n=== Cue Power/Spin (FULL DUMP) ===\n"];
                [log appendFormat:@"EFFECTIVE MaxPower: %.4f [%s]\n", g_effectiveMaxPower, src[g_powerSource]];
                [log appendFormat:@"EFFECTIVE SpinMul: %.11f [%s]\n", CUE_PROPERTIES_SPIN_VALUE, src[g_spinSource]];
                [log appendFormat:@"ShotPower (computed): %.4f\n", d.computedShotPower];
                [log appendFormat:@"ShotSpin  (computed): (%.6f, %.6f)\n", d.computedSpinX, d.computedSpinY];
                [log appendFormat:@"CueId: %d\n", g_cachedCueId];
                [log appendFormat:@"--- UserInfo Ratios ---\n"];
                [log appendFormat:@"Power: low=%d high=%d\n", d.lowPowerRatio, d.highPowerRatio];
                [log appendFormat:@"Spin:  low=%d high=%d\n", d.lowSpinRatio, d.highSpinRatio];
                [log appendFormat:@"Aim:   low=%d high=%d\n", d.lowAimRatio, d.highAimRatio];
                [log appendFormat:@"Interp Power: %.4f  Spin: %.4f\n", d.interpPower, d.interpSpin];
                [log appendFormat:@"--- CueStats ---\n"];
                [log appendFormat:@"Base:  P=%d S=%d A=%d T=%d\n", d.basePower, d.baseSpin, d.baseAim, d.baseTime];
                [log appendFormat:@"Bonus: P=%d S=%d A=%d T=%d\n", d.bonusPower, d.bonusSpin, d.bonusAim, d.bonusTime];
                [log appendFormat:@"Table Power[%d]: %.4f\n", d.bonusPower, d.tablePowerLookup];
                [log appendFormat:@"Table Spin[%d]: %.11f\n", d.bonusSpin, d.tableSpinLookup];
                [log appendFormat:@"--- VisualCue ---\n"];
                [log appendFormat:@"mPower: %.6f  mCurrentPowerOffset: %.4f  mMaxCuePowerOffset: %.4f\n", d.vc_mPower, d.vc_mCurrentPowerOffset, d.vc_mMaxCuePowerOffset];
                [log appendFormat:@"mPowerMode: %d  powerControlActive: %d  mPowerSliderVisible: %d\n", d.vc_mPowerMode, d.vc_powerControlActive, d.vc_mPowerSliderVisible];
                [log appendFormat:@"mControlMode: %d  mMaxAngleForPowerMode: %.4f\n", d.vc_mControlMode, d.vc_mMaxAngleForPowerMode];
                [log appendFormat:@"mDistanceFromTip: %.4f  mAnimatePowerTarget: %.6f  mUserSensitivity: %.4f\n", d.vc_mDistanceFromTip, d.vc_mAnimatePowerTarget, d.vc_mUserSensitivity];
                [log appendFormat:@"--- GamePowerBarView ---\n"];
                [log appendFormat:@"getMinimumPower: %.6f  getPowerSliderDistance: %.4f\n", d.pbv_minimumPower, d.pbv_sliderDistance];
                [log appendFormat:@"--- VisualEnglishControl ---\n"];
                [log appendFormat:@"mEnglish: (%.6f, %.6f)  mSpinLevel: %u  mInputRadius: %.4f\n", d.vec_englishX, d.vec_englishY, d.vec_spinLevel, d.vec_mInputRadius];
            }
            if (isValidPtr(g_gameManager)) {
                ptr obj3b0 = F(ptr, g_gameManager + 0x3b0);
                [log appendFormat:@"\n=== Shot Debug ===\n"];
                [log appendFormat:@"GM+0x3B0: 0x%llX\n", (unsigned long long)obj3b0];
                [log appendFormat:@"AimAngle: %.6f\n", getAimAngle()];
                [log appendFormat:@"Classification: %d\n", (int)getPlayerClassification()];
            }
            ptr table = GM_Table();
            int bc = getTableBallCount(table);
            if (bc > 0) {
                [log appendFormat:@"\n=== Ball Data (%d balls) ===\n", bc];
                for (int i = 0; i < bc && i < 16; i++) {
                    ptr b = getTableBall(table, i);
                    if (!b) continue;
                    Vec2d pos = ballPosition(b);
                    auto cls = ballClassification(b);
                    auto bst = ballState(b);
                    [log appendFormat:@"Ball[%d]: pos=(%.2f,%.2f) class=%d state=%d\n", i, pos.x, pos.y, cls, bst];
                }
            }
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            NSString *docDir = [paths firstObject];
            NSString *filePath = [docDir stringByAppendingPathComponent:@"8bp_debug.log"];
            NSError *err = nil;
            [log writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:&err];
            if (err) {
                snprintf(logStatus, sizeof(logStatus), "Error: %s", [[err localizedDescription] UTF8String]);
            } else {
                snprintf(logStatus, sizeof(logStatus), "Saved: %s", [filePath UTF8String]);
                logSaved = true;
            }
        } @catch (NSException *e) {
            snprintf(logStatus, sizeof(logStatus), "Exception: %s", [[e reason] UTF8String]);
        }
    }
    if (logStatus[0]) {
        ImGui::TextColored(logSaved ? ImVec4(0.3f,1.f,0.3f,1.f) : ImVec4(1.f,0.3f,0.3f,1.f), "%s", logStatus);
    }
    ImGui::Spacing();
    ImGui::TextWrapped("3-Finger Double Tap to toggle menu");
}

// ============================================================================
// Menú principal: barra lateral + cabecera + cuerpo con scroll
// Llamar así dentro del ImGui::Begin(...):  ESP_DrawMenu(&g_menuVisible);
// ============================================================================
static void ESP_DrawMenu(bool *pOpen = nullptr) {
    g_uiSliderDragPrev = g_uiSliderDrag;
    g_uiSliderDrag = false;
    ESP_ApplyTheme();

    struct TabDef { const char *icon; const char *title; };
    static const TabDef tabs[] = {
        { ICON_FA_EYE,     "ESP"      },
        { ICON_FA_GAMEPAD, "AUTO"     },
        { ICON_FA_CUBE,    "MISC"     },
        { ICON_FA_COG,     "SETTINGS" },
        { ICON_FA_USER,    "ACCOUNT"  },
    };
    const int nTabs = (int)(sizeof(tabs) / sizeof(tabs[0]));
    if (g_uiTab < 0 || g_uiTab >= nTabs) g_uiTab = 0;

    const float fs = ImGui::GetFontSize();
    const float btn = fs * 2.9f;

    // ── Barra lateral ──
    ImGui::BeginChild("##side", ImVec2(btn, 0), false, ImGuiWindowFlags_NoScrollbar);
    for (int i = 0; i < nTabs; i++) {
        ImGui::PushID(i);
        bool on = (g_uiTab == i);
        if (UI_IconButton("##tab", tabs[i].icon, btn, on, on ? g_pal.accent : g_pal.dim))
            g_uiTab = i;
        ImGui::PopID();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // ── Zona principal ──
    ImGui::BeginChild("##main", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
    UI_Header(tabs[g_uiTab].icon, tabs[g_uiTab].title, pOpen);
    ImGui::Spacing();

    ImGui::BeginChild("##body", ImVec2(0, 0), false);
    // Scroll táctil: arrastrar en vacío / tarjetas (excepto mientras se mueve un slider)
    if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(0, 6.f) && !g_uiSliderDragPrev)
        ImGui::SetScrollY(ImGui::GetScrollY() - ImGui::GetIO().MouseDelta.y);

    switch (g_uiTab) {
        case 0: UI_TabESP();      break;
        case 1: UI_TabAuto();     break;
        case 2: UI_TabMisc();     break;
        case 3: UI_TabSettings(); break;
        case 4: UI_TabAccount();  break;
    }
    ImGui::Dummy(ImVec2(0, fs));
    ImGui::EndChild();

    ImGui::EndChild();
}
