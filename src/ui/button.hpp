#pragma once

#include "raylib.h"

#include <string>

namespace ui
{

struct Button
{
    Rectangle bounds { 0, 0, 140, 48 };
    std::string text;
    Color idleColor    { 30, 30, 40, 255 };
    Color hoverColor   { 60, 60, 90, 255 };
    Color pressedColor { 20, 20, 30, 255 };
    Color textColor    { 255, 255, 255, 255 };
    float fontSize     = 22.0f;
    float textSpacing  = 0.0f;

    Button() = default;
    Button(Rectangle rect, std::string label)
        : bounds(rect)
        , text(std::move(label))
    {}
};

inline bool IsMouseOverButton(const Button &button, Vector2 mousePos = GetMousePosition())
{
    return CheckCollisionPointRec(mousePos, button.bounds);
}

inline bool IsButtonPressed(const Button &button, MouseButton mouseButton = MOUSE_BUTTON_LEFT)
{
    return IsMouseOverButton(button) && IsMouseButtonDown(mouseButton);
}

inline bool IsButtonClicked(const Button &button, MouseButton mouseButton = MOUSE_BUTTON_LEFT)
{
    return IsMouseOverButton(button) && IsMouseButtonPressed(mouseButton);
}

inline void DrawButton(const Button &button)
{
    Color color = button.idleColor;
    if (IsMouseOverButton(button))
    {
        color = IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? button.pressedColor : button.hoverColor;
    }

    DrawRectangleRec(button.bounds, color);

    const char *label = button.text.c_str();
    const Vector2 size = MeasureTextEx(GetFontDefault(), label, button.fontSize, button.textSpacing);
    const Vector2 pos { button.bounds.x + (button.bounds.width - size.x) * 0.5f,
                        button.bounds.y + (button.bounds.height - size.y) * 0.5f };

    DrawTextEx(GetFontDefault(), label, pos, button.fontSize, button.textSpacing, button.textColor);
}

} // namespace ui
