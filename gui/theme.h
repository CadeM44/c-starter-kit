#ifndef THEME_H
#define THEME_H

#include "clay.h"

/* Muted 1977 Apple rainbow on the cream field from the reference mark.
   Brace-initialized so MSVC treats them as constant initializers. */

static const Clay_Color UI_Color_None = { 0.0f, 0.0f, 0.0f, 0.0f };
static const Clay_Color UI_Color_Bg = { 233.0f, 223.0f, 205.0f, 255.0f };
static const Clay_Color UI_Color_Panel = { 244.0f, 237.0f, 224.0f, 255.0f };
static const Clay_Color UI_Color_Card = { 250.0f, 245.0f, 236.0f, 255.0f };
static const Clay_Color UI_Color_CardHot = { 238.0f, 228.0f, 210.0f, 255.0f };
static const Clay_Color UI_Color_CardOn = { 236.0f, 230.0f, 214.0f, 255.0f };
static const Clay_Color UI_Color_Field = { 252.0f, 249.0f, 243.0f, 255.0f };
static const Clay_Color UI_Color_Line = { 206.0f, 196.0f, 178.0f, 255.0f };
static const Clay_Color UI_Color_Text = { 52.0f, 46.0f, 38.0f, 255.0f };
static const Clay_Color UI_Color_Muted = { 122.0f, 112.0f, 98.0f, 255.0f };
static const Clay_Color UI_Color_BtnText = { 250.0f, 245.0f, 236.0f, 255.0f };
static const Clay_Color UI_Color_Ok = { 96.0f, 140.0f, 84.0f, 255.0f };
static const Clay_Color UI_Color_Err = { 176.0f, 86.0f, 82.0f, 255.0f };

static const Clay_Color UI_Color_Green = { 118.0f, 158.0f, 100.0f, 255.0f };
static const Clay_Color UI_Color_Yellow = { 196.0f, 160.0f, 72.0f, 255.0f };
static const Clay_Color UI_Color_Orange = { 196.0f, 132.0f, 86.0f, 255.0f };
static const Clay_Color UI_Color_Red = { 176.0f, 96.0f, 92.0f, 255.0f };
static const Clay_Color UI_Color_Purple = { 148.0f, 112.0f, 150.0f, 255.0f };
static const Clay_Color UI_Color_Blue = { 72.0f, 132.0f, 176.0f, 255.0f };
static const Clay_Color UI_Color_BlueHot = { 58.0f, 114.0f, 156.0f, 255.0f };

static const Clay_Color UI_Color_Stripe[] = {
    { 118.0f, 158.0f, 100.0f, 255.0f },
    { 196.0f, 160.0f, 72.0f, 255.0f },
    { 196.0f, 132.0f, 86.0f, 255.0f },
    { 176.0f, 96.0f, 92.0f, 255.0f },
    { 148.0f, 112.0f, 150.0f, 255.0f },
    { 72.0f, 132.0f, 176.0f, 255.0f },
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
