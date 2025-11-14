#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

#include "profile.hpp"

struct AppGlobals
{
	int screenWidth;
	int screenHeight;
	std::string profileFilePath;
	profile::PlayerProfile profile;
};

extern AppGlobals g_;

static inline void g_get_screen_center(float *x, float *y)
{
	*x = (float)g_.screenWidth * 0.5f;
	*y = (float)g_.screenHeight * 0.5f;
}

#endif // GLOBALS_H
