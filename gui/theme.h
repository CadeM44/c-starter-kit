#ifndef THEME_H
#define THEME_H

#include "clay.h"

/* Muted 1977 Apple rainbow on the cream field from the reference mark. */

#define UI_COL(r, g, b) ((Clay_Color) { (float)(r), (float)(g), (float)(b), 255.0f })
#define UI_COL_A(r, g, b, a) ((Clay_Color) { (float)(r), (float)(g), (float)(b), (float)(a) })

static const Clay_Color UI_Color_Bg = UI_COL(233, 223, 205);
static const Clay_Color UI_Color_Panel = UI_COL(244, 237, 224);
static const Clay_Color UI_Color_Card = UI_COL(250, 245, 236);
static const Clay_Color UI_Color_CardHot = UI_COL(238, 228, 210);
static const Clay_Color UI_Color_CardOn = UI_COL(236, 230, 214);
static const Clay_Color UI_Color_Field = UI_COL(252, 249, 243);
static const Clay_Color UI_Color_Line = UI_COL(206, 196, 178);
static const Clay_Color UI_Color_Text = UI_COL(52, 46, 38);
static const Clay_Color UI_Color_Muted = UI_COL(122, 112, 98);
static const Clay_Color UI_Color_BtnText = UI_COL(250, 245, 236);
static const Clay_Color UI_Color_Ok = UI_COL(96, 140, 84);
static const Clay_Color UI_Color_Err = UI_COL(176, 86, 82);

static const Clay_Color UI_Color_Green = UI_COL(118, 158, 100);
static const Clay_Color UI_Color_Yellow = UI_COL(196, 160, 72);
static const Clay_Color UI_Color_Orange = UI_COL(196, 132, 86);
static const Clay_Color UI_Color_Red = UI_COL(176, 96, 92);
static const Clay_Color UI_Color_Purple = UI_COL(148, 112, 150);
static const Clay_Color UI_Color_Blue = UI_COL(72, 132, 176);
static const Clay_Color UI_Color_BlueHot = UI_COL(58, 114, 156);

static const Clay_Color UI_Color_Stripe[] = {
    UI_COL(118, 158, 100),
    UI_COL(196, 160, 72),
    UI_COL(196, 132, 86),
    UI_COL(176, 96, 92),
    UI_COL(148, 112, 150),
    UI_COL(72, 132, 176),
};

#define UI_STRIPE_COUNT 6

#define UI_SPACE_XS 6
#define UI_SPACE_SM 10
#define UI_SPACE_MD 16
#define UI_SPACE_LG 22
#define UI_SPACE_XL 28

#define UI_RADIUS_SM 6.0f
#define UI_RADIUS_MD 10.0f
#define UI_RADIUS_LG 14.0f
#define UI_RADIUS_PILL 20.0f

#define UI_SIDEBAR_WIDTH 196.0f
#define UI_FIELD_HEIGHT 40.0f
#define UI_STRIPE_HEIGHT 8.0f
#define UI_TITLEBAR_HEIGHT 56.0f

#define UI_FONT_BODY 0
#define UI_FONT_COUNT 1

#endif
