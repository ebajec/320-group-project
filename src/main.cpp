#include "raylib.h"

#include "globals.h"

#include "fsm.hpp"
#include "state.hpp"

#include "main_menu.hpp"
#include "routlette.hpp"
#include "blackjack.hpp"
#include "profile.hpp"

#include <filesystem>

int main(int argc, char *argv[])
{
    const char * name = "spin with samir";
    g_.screenWidth  = 800;
    g_.screenHeight = 800;
    std::filesystem::path profilePath = "profile.env";
    g_.profileFilePath = profilePath.string();
    profile::LoadProfile(g_.profileFilePath, g_.profile);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(g_.screenWidth, g_.screenHeight, name);
    SetWindowMinSize(640, 480);

    SetTargetFPS(60);
	SetExitKey(KEY_NULL);

	std::unique_ptr<FSM> fsm (new FSM());

	fsm->add(MAIN_STATE_MAIN_MENU, new MainMenuNode());
	fsm->add(MAIN_STATE_ROULETTE, new RouletteNode());
	fsm->add(MAIN_STATE_BLACKJACK, new BlackjackNode());

    while (!WindowShouldClose())    // main loop
    {
        g_.screenWidth = GetScreenWidth();
        g_.screenHeight = GetScreenHeight();
		fsm->update();

        BeginDrawing();
        ClearBackground(MAROON);

		fsm->render();

        EndDrawing();
    }

    profile::SaveProfile(g_.profileFilePath, g_.profile);
    CloseWindow();
    return 0;
}
