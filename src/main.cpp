#include "raylib.h"

#include "fsm.hpp"
#include "test_color_fsm.hpp"

#include "state.hpp"
#include "main_menu.hpp"
#include "routlette.hpp"
#include "blackjack.hpp"

int main(int argc, char *argv[])
{
	const char * name = "spin with samir";
    const int screenWidth  = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, name);

    SetTargetFPS(60);
	SetExitKey(KEY_NULL);

	std::unique_ptr<FSM> fsm (new FSM());

	fsm->add(MAIN_STATE_MAIN_MENU, new MainMenuNode());
	fsm->add(MAIN_STATE_ROULETTE, new RouletteNode());
	fsm->add(MAIN_STATE_BLACKJACK, new BlackjackNode());

    while (!WindowShouldClose())    // main loop
    {
		fsm->update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

		fsm->render();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
