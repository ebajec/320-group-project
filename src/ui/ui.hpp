#ifndef UI_HPP
#define UI_HPP

#include "button.hpp"
#include "input.hpp"
#include "raylib.h"

namespace ui
{

inline void DrawLabel(const char *text, Vector2 position, float size = 20.0f, Color color = LIGHTGRAY)
{
	DrawTextEx(GetFontDefault(), text, position, size, 0.0f, color);
}

} // namespace ui

#endif
