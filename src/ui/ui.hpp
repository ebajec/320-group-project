#ifndef UI_HPP
#define UI_HPP

#include "button.hpp"
#include "input.hpp"
#include "raylib.h"

#include "globals.h"

namespace ui
{

inline void DrawLabel(const char *text, Vector2 position, float size = 20.0f, Color color = LIGHTGRAY)
{
	DrawTextEx(GetFontDefault(), text, position, size, 0.0f, color);
}

inline void DrawBackgroundTexture(Texture tex)
{
	Rectangle rect =  
		Rectangle{0,0,(float)g_.screenWidth,(float)g_.screenHeight};

	DrawTexturePro(
		tex, 
		rect,
		rect,
		Vector2{0,0},0,WHITE
	);

}


} // namespace ui

#endif
