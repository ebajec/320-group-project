#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

#include "profile.hpp"

struct AppGlobals
{
	int screenWidth;
	int screenHeight;
	std::string resourcePath = ".";
	std::string profileFilePath;
	profile::PlayerProfile profile;
};

extern AppGlobals g_;

// in case we want to resolve these paths differently
//
// It's a terrible solution, sorry
static inline std::string g_get_path(const std::string & s)
{
	return g_.resourcePath + "/" + s;
}

static inline void g_get_screen_center(float *x, float *y)
{
	*x = (float)g_.screenWidth * 0.5f;
	*y = (float)g_.screenHeight * 0.5f;
}

#endif // GLOBALS_H
