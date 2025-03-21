#pragma once

#include "ugui.h"
#include "button.h"
#include "ugui_fonts.h"

#define FONT_12 FONT_7X12
#define FONT_16 FONT_10X16
#define FONT_20 FONT_12X20

void GUI_task(void*);

void GUI_highlightButton(UG_WINDOW *wnd, UG_U8 id);
void GUI_unhighlightButton(UG_WINDOW *wnd, UG_U8 id);

void GUI_buttonCallback(ButtonHandle button, PressType type);
void GUI_updateCurrentMenu(void);

bool GUI_cardTap(uint32_t UUID);
