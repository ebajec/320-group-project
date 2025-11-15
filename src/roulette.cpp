#include "routlette.hpp"
#include "roulette_game.hpp"

#include "state.hpp"
#include "globals.h"
#include "ui/button.hpp"
#include "ui/ui.hpp"
#include "profile.hpp"

#include "raylib.h"

#include <vector>
#include <array>
#include <string>
#include <complex>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdlib>

#define TWOPI 6.28318530718

namespace
{
	struct BetCell
	{
		Rectangle bounds{};
		int betId = 0;
		Color baseColor{ 30, 30, 30, 255 };
		std::string label;
		int wheelValue = -1; // 0-36 or 37 for 00 when applicable
	};

	bool IsRedNumber(int number)
	{
		static const int reds[] = { 1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36 };
		for (int value : reds)
		{
			if (value == number)
				return true;
		}
		return false;
	}
}

static inline Mesh gen_wheel_mesh()
{
	// number of wheel sections
	size_t N = 37;
	float r1 = 1.5f;
	float r2 = 2.0f;
	float h = 0.2f;

	double dtht = TWOPI/(double)N;

	std::complex<double> d1 = std::polar(1.0,0.5*dtht);
	std::complex<double> c = (1.0i);
	c /= d1;

	// number of vertices per section
	size_t M = 1;
	std::complex<double> d2 = std::polar(1.0,dtht/(double)M);

	size_t K = M*N;

	size_t vidx = 0;
	size_t nidx = 0;
	size_t iidx = 0;
	size_t tidx = 0;

	size_t vcount = 4*K + 4;

	Mesh mesh {};

	mesh.vertexCount = vcount;
	mesh.triangleCount = 8*K;

	mesh.vertices = (float*)malloc(sizeof(Vector3) * vcount);
	mesh.normals = (float*)malloc(sizeof(Vector3) * vcount);
	mesh.texcoords = (float*)malloc(sizeof(Vector2) * vcount);
	mesh.indices = (unsigned short*)malloc(24*sizeof(unsigned short) * K);

	auto add_quad = [&](
		int ll, 
		int lr, 
		int ur, 
		int ul) {
			mesh.indices[iidx++] = (unsigned short)ll;
			mesh.indices[iidx++] = (unsigned short)lr;
			mesh.indices[iidx++] = (unsigned short)ur;
			mesh.indices[iidx++] = (unsigned short)ll;
			mesh.indices[iidx++] = (unsigned short)ur;
			mesh.indices[iidx++] = (unsigned short)ul;
		};

	Vector3 *verts = (Vector3*)mesh.vertices;
	Vector3 *normals = (Vector3*)mesh.normals;
	Vector2 *uvs = (Vector2*)mesh.texcoords;

	for (size_t i = 0; i < K + 1; ++i) {
		float u = (double)i/(double)(K - 1);

		float x1 = r1*c.real();
		float y1 = r1*c.imag();

		float x2 = r2*c.real();
		float y2 = r2*c.imag();

		verts[vidx++] = {x1, y1, h};
		verts[vidx++] = {x2, y2, h};
		verts[vidx++] = {x1, y1, -h};
		verts[vidx++] = {x2, y2, -h};

		uvs[tidx++] = {u,0};
		uvs[tidx++] = {u,1};
		uvs[tidx++] = {u,0};
		uvs[tidx++] = {u,1};

		normals[nidx++] = {0, 0, 1};
		normals[nidx++] = {0, 0, 1};
		normals[nidx++] = {0, 0, -1};
		normals[nidx++] = {0, 0, -1};

		if (i == K)
			break;

		size_t base = 4*i;
		size_t next = 4*i + 4;

		// top
		add_quad(base, base + 1, next + 1, next);

		//bottom
		add_quad(base + 2, base + 3, next + 3, next + 2);

		// front
		add_quad(base, next, next + 2, base + 2);

		// back
		add_quad(next + 3, next + 1, base + 1, base + 3);

		c *= d2;
	}

	return mesh;
}

struct RenderData
{
	Camera3D camera{};
	Model wheel_model{};
	Mesh wheel_mesh{};
	Shader wheel_shader{};
	Texture bg_tex{};
	Image bg_img{};
};

int init_render_data(RenderData * rd)
{
	std::string vspath = g_get_path("shader/wheel.vert");
	std::string fspath = g_get_path("shader/wheel.frag");
	std::string bgpath = g_get_path("background.png");

	rd->wheel_shader = LoadShader(
		vspath.c_str(),
		fspath.c_str()
	);
	
	// I don't really care about cleaning up here

	if (!rd->wheel_shader.id)
		return -1;

	rd->wheel_mesh = gen_wheel_mesh();
	UploadMesh(&rd->wheel_mesh, true);
	rd->wheel_model = LoadModelFromMesh(rd->wheel_mesh);
	rd->wheel_model.materials[0].shader = rd->wheel_shader;

	rd->camera = Camera3D{
		.position = Vector3{2,2,2.5},
		.target = Vector3{0,0,0},
		.up = Vector3{0,0,1},
		.fovy = 90,
		.projection = CAMERA_PERSPECTIVE,
	};

	Image bg_tmp = LoadImage(bgpath.c_str());

	if (!bg_tmp.data)
		return -1;

	Texture tex = rd->bg_tex = LoadTextureFromImage(bg_tmp);
	UnloadImage(bg_tmp);

	SetTextureWrap(tex, TEXTURE_WRAP_REPEAT);
    SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);

	return 0;
}

struct RouletteNode::Impl
{
	static constexpr int CHIP_COUNT = 5;
	static constexpr double SPIN_TIME = 5;

	std::vector<BetCell> straightCells;
	std::vector<BetCell> outsideCells;
	ui::Button backButton;
	ui::Button spinButton;
	ui::Button clearButton;
	std::array<ui::Button, CHIP_COUNT> chipButtons;
	std::array<int, CHIP_COUNT> chipValues{ 1, 5, 25, 100, 500 };
	int selectedChip = 1;
	std::array<int, ROULETTE_BET_COUNT> bets{};
	int currentStake = 0;
	int lastRoll = -1;
	int lastWin = 0;
	int lastStake = 0;
	std::string status = "Select a chip, click the board to add bets, right-click to remove.";
	std::string lastOutcome;
	profile::PlayerProfile *playerProfile = nullptr;
	float lastWidth = 0.0f;
	float lastHeight = 0.0f;

	double spin_countdown = 0;
	double spinner_phase = 0;

	RenderData rd;

	Impl()
	{
		std::srand(static_cast<unsigned>(std::time(nullptr)));
		playerProfile = &g_.profile;
		buildBoard();
		backButton = ui::Button(Rectangle{ 0,0,0,0 }, "Back");
		spinButton = ui::Button(Rectangle{ 0,0,0,0 }, "Spin");
		clearButton = ui::Button(Rectangle{ 0,0,0,0 }, "Clear Bets");
		float chipY = 240.0f;
		for (int i = 0; i < CHIP_COUNT; ++i)
		{
			chipButtons[i] = ui::Button(Rectangle{ 0,0,0,0 }, "$" + std::to_string(chipValues[i]));
			chipY += 52.0f;
		}
		bets.fill(0);
		updateLayout();

		init_render_data(&rd);
	}

	int &bankroll()
	{
		return playerProfile->bankroll;
	}

	void persist()
	{
		profile::SaveProfile(g_.profileFilePath, *playerProfile);
	}

	bool spinning() {
		return GetTime() < spin_countdown;
	}

	void updateLayout()
	{
		lastWidth = (float)g_.screenWidth;
		lastHeight = (float)g_.screenHeight;

		float rightPanelX = (float)g_.screenWidth - 190.0f;
		float rightPanelWidth = 150.0f;
		backButton.bounds = Rectangle{ 30, 20, 140, 42 };
		spinButton.bounds = Rectangle{ rightPanelX, (float)g_.screenHeight - 90.0f, rightPanelWidth, 42 };
		clearButton.bounds = Rectangle{ rightPanelX, (float)g_.screenHeight - 140.0f, rightPanelWidth, 42 };
		float chipY = 240.0f;
		for (int i = 0; i < CHIP_COUNT; ++i)
		{
			chipButtons[i].bounds = Rectangle{ rightPanelX, chipY, rightPanelWidth, 42 };
			chipY += 52.0f;
		}
		buildBoard();
	}

	void ensureLayout()
	{
		if (lastWidth != (float)g_.screenWidth || lastHeight != (float)g_.screenHeight)
		{
			updateLayout();
		}
	}

	void buildBoard()
	{
		straightCells.clear();
		outsideCells.clear();
		const float cellWidth = 60.0f;
		const float cellHeight = 48.0f;
		const float tableWidth = cellWidth * 3.0f;
		const float gutter = cellWidth;
		float startX = (float)g_.screenWidth * 0.5f - (tableWidth * 0.5f) + 0.0f;
		const float startY = 120.0f;

		for (int row = 0; row < 12; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				int number = row * 3 + col + 1;
				float x = startX + col * cellWidth;
				float y = startY + (11 - row) * cellHeight;
				Color base = ::IsRedNumber(number) ? Color{ 150, 20, 20, 255 } : Color{ 25, 25, 25, 255 };
				straightCells.push_back({ Rectangle{ x, y, cellWidth, cellHeight }, STRAIGHT0 + number, base, std::to_string(number), number });
			}
		}

		// Single zero
		straightCells.push_back({ Rectangle{ startX - gutter, startY, gutter, cellHeight * 6 - 4 }, STRAIGHT0, Color{ 10, 90, 10, 255 }, "0", 0 });
		// Double zero
		straightCells.push_back({ Rectangle{ startX - gutter, startY + cellHeight * 6 + 4, gutter, cellHeight * 6 - 4 }, STRAIGHT00, Color{ 10, 90, 10, 255 }, "00", 37 });

		const float boardWidth = cellWidth * 3;
		float columnY = startY - cellHeight - 6.0f;
		for (int col = 0; col < 3; ++col)
		{
			float x = startX + col * cellWidth;
			outsideCells.push_back({ Rectangle{ x, columnY, cellWidth, cellHeight - 8.0f }, COLUMN1 + col, Color{ 45, 45, 55, 255 }, "2 to 1", -1 });
		}

		float dozenY = startY + cellHeight * 12 + 8.0f;
		static const char *dozenLabels[] = { "1st 12", "2nd 12", "3rd 12" };
		for (int i = 0; i < 3; ++i)
		{
			outsideCells.push_back({ Rectangle{ startX + i * (boardWidth / 3.0f), dozenY, (boardWidth / 3.0f) - 4.0f, cellHeight }, DOZEN1 + i, Color{ 42, 42, 42, 255 }, dozenLabels[i], -1 });
		}

		float evenOddY = dozenY + cellHeight + 8.0f;
		float sectionWidth = boardWidth / 6.0f;
		struct OutsideDef { int bet; const char *label; Color color; };
		const OutsideDef outsideDefs[] = {
			{ HALF1, "1-18", Color{ 45, 45, 45, 255 } },
			{ EVEN, "Even", Color{ 35, 35, 35, 255 } },
			{ RD, "Red", Color{ 150, 20, 20, 255 } },
			{ BLK, "Black", Color{ 10, 10, 10, 255 } },
			{ ODD, "Odd", Color{ 35, 35, 35, 255 } },
			{ HALF2, "19-36", Color{ 45, 45, 45, 255 } }
		};
		for (int i = 0; i < 6; ++i)
		{
			outsideCells.push_back({ Rectangle{ startX + i * sectionWidth, evenOddY, sectionWidth - 3.0f, cellHeight }, outsideDefs[i].bet, outsideDefs[i].color, outsideDefs[i].label, -1 });
		}
	}

	void drawCells(const std::vector<BetCell> &cells) const
	{
		for (const ::BetCell &cell : cells)
		{
			DrawRectangleRec(cell.bounds, cell.baseColor);
			DrawRectangleLinesEx(cell.bounds, 2.0f, DARKGRAY);
			DrawText(cell.label.c_str(), cell.bounds.x + 5, cell.bounds.y + 5, 18, RAYWHITE);
			if (bets[cell.betId] > 0)
			{
				DrawRectangleLinesEx(cell.bounds, 4.0f, GOLD);
				DrawText(TextFormat("$%d", bets[cell.betId]), cell.bounds.x + 5, cell.bounds.y + cell.bounds.height - 22, 18, YELLOW);
			}
			if (cell.wheelValue >= 0 && cell.wheelValue == lastRoll)
			{
				DrawRectangleLinesEx(cell.bounds, 4.0f, SKYBLUE);
			}
		}
	}

	std::string rollLabel(int roll) const
	{
		if (roll < 0)
			return "--";
		if (roll == 37)
			return "00";
		return std::to_string(roll);
	}

	float spinner_theta(float t)
	{
		static float c = 8.f;
		static float p = 8.f;
		return c*exp(-p*t);
	}

	void render_spin_screen()
	{
		Vector2 c;
		g_get_screen_center(&c.x, &c.y);

		ui::DrawBackgroundTexture(rd.bg_tex);

		const char * text = "YOU ARE SPINNING!!!";

		float fontsize = 40;
		float offset = 0.25*(float)strlen(text)*fontsize;

		DrawText(text, c.x - offset,100, fontsize, WHITE);

		double t = 1.0 - (spin_countdown - GetTime())/SPIN_TIME;
		double tht = 360*spinner_theta(t); 

		BeginMode3D(rd.camera);
		DrawModelEx(
			rd.wheel_model, 
			Vector3{0,0,0},
			Vector3{0,0,1},
			spinner_phase + tht,
			Vector3{1.f,1.f,1.f},
			WHITE
		);
		EndMode3D();
	}

	void render()
	{
		ensureLayout();

		if (spinning()) {
			render_spin_screen();
			return;
		}

		DrawRectangleGradientV(0, 0, g_.screenWidth, g_.screenHeight, Color{ 3, 61, 43, 255 }, Color{ 7, 33, 18, 255 });
		DrawText(TextFormat("Roulette | %s", playerProfile->name.c_str()), 30, 60, 40, GOLD);
		DrawText(TextFormat("Bankroll: $%d", bankroll()), 30, 110, 28, RAYWHITE);
		DrawText(TextFormat("Current Bet: $%d", currentStake), 30, 145, 24, RAYWHITE);
		DrawText(TextFormat("Selected Chip: $%d", chipValues[selectedChip]), 30, 175, 22, LIGHTGRAY);
		DrawText(TextFormat("Last Roll: %s", rollLabel(lastRoll).c_str()), 30, 210, 28, lastRoll >= 0 ? YELLOW : LIGHTGRAY);
		DrawText(TextFormat("Spins: %d | Net: %d", playerProfile->rouletteSpins, playerProfile->rouletteNet), 30, 245, 22, LIGHTGRAY);
		if (!lastOutcome.empty())
		{
			DrawText(lastOutcome.c_str(), 30, 280, 22, RAYWHITE);
		}
		DrawText(status.c_str(), 30, g_.screenHeight - 130, 22, RAYWHITE);

		drawCells(straightCells);
		drawCells(outsideCells);

		for (int i = 0; i < CHIP_COUNT; ++i)
		{
			DrawButton(chipButtons[i]);
			if (i == selectedChip)
			{
				DrawRectangleLinesEx(chipButtons[i].bounds, 3.0f, GOLD);
			}
		}

		DrawButton(backButton);
		DrawButton(clearButton);
		DrawButton(spinButton);
		if (currentStake == 0)
		{
			DrawRectangleRec(spinButton.bounds, Color{ 0, 0, 0, 120 });
		}
		if (currentStake == 0)
		{
			DrawRectangleRec(clearButton.bounds, Color{ 0, 0, 0, 120 });
		}
	}

	void placeChip(int betId)
	{
		if (betId < 0 || betId >= ROULETTE_BET_COUNT)
			return;
		int chip = chipValues[selectedChip];
		if (bankroll() < chip)
		{
			status = "Not enough chips for that wager.";
			return;
		}
		bankroll() -= chip;
		bets[betId] += chip;
		currentStake += chip;
	}

	void refundBet(int betId)
	{
		if (betId < 0 || betId >= ROULETTE_BET_COUNT)
			return;
		int amount = bets[betId];
		if (amount <= 0)
			return;
		bankroll() += amount;
		currentStake -= amount;
		bets[betId] = 0;
	}

	void resetBets(bool refund)
	{
		if (refund && currentStake > 0)
		{
			bankroll() += currentStake;
		}
		bets.fill(0);
		currentStake = 0;
	}

	void handleBoardInput()
	{
		Vector2 mouse = GetMousePosition();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			for (auto &cell : straightCells)
			{
				if (CheckCollisionPointRec(mouse, cell.bounds))
				{
					placeChip(cell.betId);
					return;
				}
			}
			for (auto &cell : outsideCells)
			{
				if (CheckCollisionPointRec(mouse, cell.bounds))
				{
					placeChip(cell.betId);
					return;
				}
			}
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			for (auto &cell : straightCells)
			{
				if (CheckCollisionPointRec(mouse, cell.bounds))
				{
					refundBet(cell.betId);
					return;
				}
			}
			for (auto &cell : outsideCells)
			{
				if (CheckCollisionPointRec(mouse, cell.bounds))
				{
					refundBet(cell.betId);
					return;
				}
			}
		}
	}

	void performSpin()
	{
		if (currentStake == 0)
		{
			status = "Place at least one bet before spinning.";
			return;
		}
		lastStake = currentStake;
		int rollValue = -1;
		int winnings = spin(bets.data(), &rollValue);
		lastRoll = rollValue;
		lastWin = winnings;
		bankroll() += winnings;
		int net = winnings - lastStake;
		playerProfile->rouletteSpins++;
		playerProfile->rouletteNet += net;
		if (net > 0)
		{
			lastOutcome = std::string("Hit ") + rollLabel(lastRoll) + "! Won $" + std::to_string(net) + ".";
		}
		else if (net == 0)
		{
			lastOutcome = std::string("Hit ") + rollLabel(lastRoll) + "! Broke even.";
		}
		else
		{
			lastOutcome = std::string("Hit ") + rollLabel(lastRoll) + " - Lost $" + std::to_string(-net) + ".";
		}
		status = lastOutcome;
		resetBets(false);
		persist();

		spin_countdown = GetTime() + SPIN_TIME;
		spinner_phase = 360*(float)rand()/(float)RAND_MAX;
	}

	FSMResult update()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			resetBets(true);
			return MAIN_STATE_MAIN_MENU;
		}
		if (ui::IsButtonClicked(backButton))
		{
			resetBets(true);
			return MAIN_STATE_MAIN_MENU;
		}

		if (spinning())
			return MAIN_STATE_ROULETTE;

		for (int i = 0; i < CHIP_COUNT; ++i)
		{
			if (ui::IsButtonClicked(chipButtons[i]))
			{
				selectedChip = i;
			}
		}
		handleBoardInput();
		if (ui::IsButtonClicked(clearButton) && currentStake > 0)
		{
			resetBets(true);
			status = "Bets refunded.";
		}
		if (ui::IsButtonClicked(spinButton))
		{
			performSpin();
		}
		return MAIN_STATE_ROULETTE;
	}
};

RouletteNode::RouletteNode()
	: impl(new Impl())
{
}

RouletteNode::~RouletteNode() = default;


void RouletteNode::render()
{
	impl->render();
}

FSMResult RouletteNode::update()
{
	return impl->update();
}
