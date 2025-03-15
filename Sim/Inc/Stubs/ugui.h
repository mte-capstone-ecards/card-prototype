#pragma once

#define UG_WINDOW uint32_t
#define UG_TEXTBOX uint32_t
#define UG_BUTTON uint32_t
#define UG_OBJECT uint32_t
#define UG_MESSAGE uint32_t

#define UGUI_POS(a,b,c,d) a
#define UG_DrawLine(a, b)
#define UG_DrawFrame(a, b)

#define UG_WindowCreate(w, a, b, c)
#define UG_WindowSetStyle(w, a)
#define UG_TextboxCreate(w, a, b, c)
#define UG_TextboxSetFont(w, a, b)
#define UG_TextboxSetText(w, a, b)
#define UG_TextboxSetBackColor(w, a, b)
#define UG_ButtonCreate(w, a, b, c)
#define UG_ButtonSetFont(w, a, b)
#define UG_ButtonSetText(w, a, b)
#define UG_ButtonSetAlignment(w, a, b)

#define UG_WindowShow(a)
#define UG_Update()
#define UG_Init(a, b)
