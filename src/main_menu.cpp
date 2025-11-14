#include "main_menu.hpp"

#include "globals.h"

#include "fsm.hpp"
#include "state.hpp"
#include "raylib.h"

#include "ui/button.hpp"

#include <cstring>
#include <array>

static Image LoadSamirImage()
{
	static const std::array<const char *, 6> candidates = {
		"samir.jpg",
		"../samir.jpg",
		"../../samir.jpg",
		"src/samir.jpg",
		"../src/samir.jpg",
		"../../src/samir.jpg"
	};

	for (const char *path : candidates)
	{
		if (FileExists(path))
		{
			Image img = LoadImage(path);
			if (img.data)
			{
				return img;
			}
		}
	}

	Image empty{};
	return empty;
}

static inline void draw_title_text()
{
		Vector2 c;
		g_get_screen_center(&c.x, &c.y);

		const char * text = "Are you ready to Spin With Samir?";

		float fontsize = 40;
		float x = c.x - 0.25*fontsize*(float)strlen(text); 
		float y = 2*fontsize;

		DrawText(text, x, y, fontsize, Color{255,255,255,255});
}

void MainMenuNode::refreshLayout()
{
	lastWidth = g_.screenWidth;
	lastHeight = g_.screenHeight;

	Vector2 c;
	g_get_screen_center(&c.x, &c.y);

	float w = 220;
	float h = 100;

	float sh = (float)g_.screenHeight;

	blackjack_button.bounds = Rectangle{ c.x - 1.5f * w, sh - 2.2f * h, w, h };
	roulette_button.bounds = Rectangle{ c.x + 0.5f * w, sh - 2.2f * h, w, h };
}

MainMenuNode::MainMenuNode() : IFSMNode()
{
	Image samir_img = LoadSamirImage();
	if (samir_img.data)
	{
		ImageResize(&samir_img, 200, 200);
		samir_tex = LoadTextureFromImage(samir_img);
		samirTextureLoaded = samir_tex.id != 0;
		UnloadImage(samir_img);
	}
	else
	{
		samir_tex = Texture2D{};
	}

	if (!samirTextureLoaded)
	{
		printf("Failed to load samir texture!\n");
	}

	blackjack_button = ui::Button(Rectangle{ 0,0,0,0 }, "Play Blackjack!");
	roulette_button = ui::Button(Rectangle{ 0,0,0,0 }, "Play Roulette!");
	refreshLayout();
}

MainMenuNode::~MainMenuNode()
{
	if (samirTextureLoaded)
	{
		UnloadTexture(samir_tex);
	}
}

void MainMenuNode::render()
{
	if (g_.screenWidth != (int)lastWidth || g_.screenHeight != (int)lastHeight)
	{
		refreshLayout();
	}
	Vector2 c;
	g_get_screen_center(&c.x, &c.y);

    if (samirTextureLoaded)
    {
		float tw = samir_tex.width; 
		float th = samir_tex.height;
		float tx = c.x - 0.5f*tw; 
		float ty = c.y - 0.60*th;;

		float bd = 0.05f;

		DrawRectangleRec(
			Rectangle{
				.x = tx - bd*tw,
				.y = ty - bd*th,
				.width = (1.0f + 2.0f*bd)*tw,
				.height = (1.0f + 2.0f*bd)*th
			},
			Color{0,0,0,255}
		);
	
		DrawTexture(samir_tex, 
			tx, 
			ty, 
			WHITE
			);
    }
    else
    {
		Rectangle fallback { c.x - 100, c.y - 100, 200, 200 };
		DrawRectangleRounded(fallback, 0.1f, 6, DARKGRAY);
		DrawText("Spin", fallback.x + 60, fallback.y + 80, 30, RAYWHITE);
    }

	const auto &profile = g_.profile;
	DrawText(TextFormat("Player: %s", profile.name.c_str()), 30, 260, 26, GOLD);
	DrawText(TextFormat("Bankroll: $%d", profile.bankroll), 30, 295, 24, RAYWHITE);
	DrawText(TextFormat("Blackjack W/L/P: %d/%d/%d", profile.blackjackWins, profile.blackjackLosses, profile.blackjackPushes), 30, 330, 22, LIGHTGRAY);
	DrawText(TextFormat("Roulette Spins: %d (Net %d)", profile.rouletteSpins, profile.rouletteNet), 30, 360, 22, LIGHTGRAY);

	draw_title_text();
	DrawButton(blackjack_button);
	DrawButton(roulette_button);
}
FSMResult MainMenuNode::update() 
{
	if (ui::IsButtonPressed(blackjack_button)) {
		return MAIN_STATE_BLACKJACK;
	}
	else if (ui::IsButtonPressed(roulette_button)) {
		return MAIN_STATE_ROULETTE;
	}
	return MAIN_STATE_MAIN_MENU;
}
