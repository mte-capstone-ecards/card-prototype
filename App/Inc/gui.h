#pragma once

#include "ugui.h"
#include "button.h"

void GUI_init(void);

void GUI_highlightButton(UG_WINDOW *wnd, UG_U8 id);
void GUI_unhighlightButton(UG_WINDOW *wnd, UG_U8 id);

void GUI_buttonCallback(ButtonHandle button, PressType type);
void GUI_updateCurrentMenu(void);
void GUI_setMenu(void);
