#pragma once

#include "raylib.h"

namespace ui
{
namespace input
{

inline Vector2 MousePosition()
{
    return GetMousePosition();
}

inline bool MouseDown(MouseButton button = MOUSE_BUTTON_LEFT)
{
    return IsMouseButtonDown(button);
}

inline bool MousePressed(MouseButton button = MOUSE_BUTTON_LEFT)
{
    return IsMouseButtonPressed(button);
}

inline bool MouseReleased(MouseButton button = MOUSE_BUTTON_LEFT)
{
    return IsMouseButtonReleased(button);
}

inline bool MouseOver(const Rectangle &bounds)
{
    return CheckCollisionPointRec(GetMousePosition(), bounds);
}

inline bool MouseClickedInside(const Rectangle &bounds, MouseButton button = MOUSE_BUTTON_LEFT)
{
    return MouseOver(bounds) && MousePressed(button);
}

} // namespace input
} // namespace ui
