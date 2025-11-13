#include "raylib.h"

#include "fsm.hpp"
#include "test_color_fsm.hpp"

int main(int argc, char *argv[])
{
	const char * name = "spin with samir";
    const int screenWidth  = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, name);

    SetTargetFPS(60);

	std::unique_ptr<FSM> fsm (color_fsm::create());

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
