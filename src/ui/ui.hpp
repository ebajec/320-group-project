#pragma once

#include "ui/button.hpp"
#include "ui/input.hpp"

namespace ui
{

inline void DrawLabel(const char *text, Vector2 position, float size = 20.0f, Color color = LIGHTGRAY)
{
	DrawTextEx(GetFontDefault(), text, position, size, 0.0f, color);
}

} // namespace ui
