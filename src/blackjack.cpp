#include "state.hpp"

#include "raylib.h"
#include "blackjack.hpp"
#include "globals.h"
#include "ui/button.hpp"
#include "ui/ui.hpp"
#include "profile.hpp"

#include <algorithm>
#include <array>
#include <random>
#include <string>
#include <vector>

namespace
{
	constexpr float CARD_WIDTH  = 70.0f;
	constexpr float CARD_HEIGHT = 100.0f;
	constexpr float CARD_PADDING = 12.0f;
	constexpr int   MIN_BET = 10;
	constexpr int   BET_STEP = 10;

	Color MakeCardColor(int card)
	{
		int suit = card / 13;
		bool red = (suit == 1) || (suit == 2); // hearts or diamonds
		return red ? Color{ 180, 20, 40, 255 } : Color{ 230, 230, 230, 255 };
	}

	Color CardBorderColor(int card)
	{
		int suit = card / 13;
		bool red = (suit == 1) || (suit == 2);
		return red ? MAROON : DARKGRAY;
	}

	std::string CardLabel(int card)
	{
		static const char *ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
		static const char *suits[] = { "S", "H", "D", "C" };
		int rank = card % 13;
		int suit = card / 13;
		return std::string(ranks[rank]) + suits[suit];
	}
}

struct BlackjackNode::Impl
{
	enum class Phase
	{
		Betting,
		PlayerTurn,
		DealerTurn,
		RoundOver
	};

	enum class Outcome
	{
		PlayerWin,
		DealerWin,
		Push,
		PlayerBlackjack
	};

	Phase phase = Phase::Betting;
	std::vector<int> deck;
	std::vector<int> playerHand;
	std::vector<int> dealerHand;
	int betAmount = 50;
	int stake = 0;
	bool dealerReveal = false;
	bool playerCanDouble = true;
	std::string status = "Adjust your bet and press Deal.";
	std::string lastOutcome = "";

	ui::Button backButton;
	ui::Button dealButton;
	ui::Button hitButton;
	ui::Button standButton;
	ui::Button doubleButton;
	ui::Button playAgainButton;
	ui::Button betUpButton;
	ui::Button betDownButton;

	std::mt19937 rng;
	profile::PlayerProfile *playerProfile = nullptr;
	float lastWidth = 0.0f;
	float lastHeight = 0.0f;

	Impl()
	{
		rng.seed(std::random_device{}());
		resetDeck();

		backButton = ui::Button(Rectangle{ 0,0,0,0 }, "Back");
		dealButton = ui::Button(Rectangle{ 0,0,0,0 }, "Deal");
		hitButton = ui::Button(Rectangle{ 0,0,0,0 }, "Hit");
		standButton = ui::Button(Rectangle{ 0,0,0,0 }, "Stand");
		doubleButton = ui::Button(Rectangle{ 0,0,0,0 }, "Double");
		playAgainButton = ui::Button(Rectangle{ 0,0,0,0 }, "Play Again");
		betDownButton = ui::Button(Rectangle{ 0,0,0,0 }, "-10");
		betUpButton = ui::Button(Rectangle{ 0,0,0,0 }, "+10");
		playerProfile = &g_.profile;
		updateLayout();
	}

	void updateLayout()
	{
		lastWidth = (float)g_.screenWidth;
		lastHeight = (float)g_.screenHeight;
		float bottom = (float)g_.screenHeight - 70.0f;
		backButton.bounds = Rectangle{ 30, 20, 140, 42 };
		dealButton.bounds = Rectangle{ 30, bottom, 140, 40 };
		hitButton.bounds = Rectangle{ 190, bottom, 120, 40 };
		standButton.bounds = Rectangle{ 320, bottom, 120, 40 };
		doubleButton.bounds = Rectangle{ 450, bottom, 140, 40 };
		playAgainButton.bounds = Rectangle{ (float)g_.screenWidth - 190.0f, bottom, 160, 40 };
		betDownButton.bounds = Rectangle{ (float)g_.screenWidth - 220.0f, 160, 60, 40 };
		betUpButton.bounds = Rectangle{ (float)g_.screenWidth - 150.0f, 160, 60, 40 };
	}

	void ensureLayout()
	{
		if (lastWidth != (float)g_.screenWidth || lastHeight != (float)g_.screenHeight)
		{
			updateLayout();
		}
	}

	int &bankroll()
	{
		return playerProfile->bankroll;
	}

	void persist()
	{
		profile::SaveProfile(g_.profileFilePath, *playerProfile);
	}

	void resetDeck()
	{
		deck.clear();
		deck.reserve(52);
		for (int i = 0; i < 52; ++i)
		{
			deck.push_back(i);
		}
		std::shuffle(deck.begin(), deck.end(), rng);
	}

	int drawCard()
	{
		if (deck.size() < 10)
		{
			resetDeck();
		}
		int card = deck.back();
		deck.pop_back();
		return card;
	}

	int cardValue(int card) const
	{
		int rank = card % 13;
		if (rank == 0)
			return 11; // Ace
		if (rank >= 10)
			return 10;
		return rank + 1;
	}

	int handValue(const std::vector<int> &hand) const
	{
		int total = 0;
		int aceCount = 0;
		for (int card : hand)
		{
			int value = cardValue(card);
			total += value;
			if (value == 11)
				++aceCount;
		}
		while (total > 21 && aceCount > 0)
		{
			total -= 10;
			--aceCount;
		}
		return total;
	}

	bool isBlackjack(const std::vector<int> &hand) const
	{
		return hand.size() == 2 && handValue(hand) == 21;
	}

	void startRound()
	{
		if (bankroll() <= 0)
		{
			status = "You are out of chips!";
			return;
		}
		if (betAmount > bankroll())
		{
			betAmount = bankroll();
		}
		if (betAmount < MIN_BET)
		{
			betAmount = std::min(MIN_BET, bankroll());
		}
		if (betAmount <= 0)
		{
			status = "Increase your bet to start.";
			return;
		}

		playerHand.clear();
		dealerHand.clear();
		stake = betAmount;
		bankroll() -= stake;
		dealerReveal = false;
		playerCanDouble = true;
		phase = Phase::PlayerTurn;
		status = "Hit or stand.";

		playerHand.push_back(drawCard());
		dealerHand.push_back(drawCard());
		playerHand.push_back(drawCard());
		dealerHand.push_back(drawCard());

		if (isBlackjack(playerHand))
		{
			dealerReveal = true;
			if (isBlackjack(dealerHand))
			{
				settle(Outcome::Push, "Both have blackjack. Push.");
			}
			else
			{
				settle(Outcome::PlayerBlackjack, "Blackjack! You win 3:2.");
			}
		}
		else if (isBlackjack(dealerHand))
		{
			dealerReveal = true;
			settle(Outcome::DealerWin, "Dealer blackjack.");
		}
	}

	void handleBetDelta(int delta)
	{
		if (phase != Phase::Betting && phase != Phase::RoundOver)
			return;
		betAmount += delta;
		if (betAmount < MIN_BET)
			betAmount = MIN_BET;
		if (betAmount > bankroll())
			betAmount = bankroll();
		if (bankroll() == 0)
			betAmount = 0;
	}

	void playerHit()
	{
		if (phase != Phase::PlayerTurn)
			return;
		playerHand.push_back(drawCard());
		playerCanDouble = false;
		int total = handValue(playerHand);
		if (total > 21)
		{
			dealerReveal = true;
			settle(Outcome::DealerWin, "You busted.");
		}
	}

	void playerStand()
	{
		if (phase != Phase::PlayerTurn)
			return;
		playerCanDouble = false;
		phase = Phase::DealerTurn;
		dealerReveal = true;
		dealerPlay();
	}

	void playerDouble()
	{
		if (phase != Phase::PlayerTurn || !playerCanDouble)
		{
			status = "Can only double after the initial deal.";
			return;
		}
		if (bankroll() < stake)
		{
			status = "Not enough chips to double.";
			return;
		}
		bankroll() -= stake;
		stake += stake;
		playerCanDouble = false;
		playerHand.push_back(drawCard());
		int total = handValue(playerHand);
		if (total > 21)
		{
			dealerReveal = true;
			settle(Outcome::DealerWin, "Busted after doubling.");
			return;
		}
		playerStand();
	}

	void dealerPlay()
	{
		if (phase != Phase::DealerTurn)
			return;
		while (handValue(dealerHand) < 17)
		{
			dealerHand.push_back(drawCard());
		}
		int dealerTotal = handValue(dealerHand);
		int playerTotal = handValue(playerHand);
		if (dealerTotal > 21)
		{
			settle(Outcome::PlayerWin, "Dealer busts. You win!");
		}
		else if (playerTotal > dealerTotal)
		{
			settle(Outcome::PlayerWin, "You beat the dealer!");
		}
		else if (playerTotal < dealerTotal)
		{
			settle(Outcome::DealerWin, "Dealer wins.");
		}
		else
		{
			settle(Outcome::Push, "Push.");
		}
	}

	void settle(Outcome outcome, const std::string &message)
	{
		switch (outcome)
		{
		case Outcome::PlayerWin:
			bankroll() += stake * 2;
			playerProfile->blackjackWins++;
			break;
		case Outcome::DealerWin:
			playerProfile->blackjackLosses++;
			// stake already removed
			break;
		case Outcome::Push:
			bankroll() += stake;
			playerProfile->blackjackPushes++;
			break;
		case Outcome::PlayerBlackjack:
			bankroll() += (stake * 5) / 2; // 3:2 payout
			playerProfile->blackjackWins++;
			break;
		}
		stake = 0;
		phase = Phase::RoundOver;
		lastOutcome = message;
		status = message + " Press Play Again.";
		persist();
	}

	void resetForNextRound()
	{
		playerHand.clear();
		dealerHand.clear();
		stake = 0;
		dealerReveal = false;
		playerCanDouble = true;
		phase = Phase::Betting;
		status = "Adjust your bet and press Deal.";
	}

	void refundStake()
	{
		if (stake > 0)
		{
			bankroll() += stake;
			stake = 0;
		}
		playerHand.clear();
		dealerHand.clear();
		phase = Phase::Betting;
	}

	void renderHand(const std::vector<int> &hand, Vector2 origin, bool hideHole) const
	{
		for (size_t i = 0; i < hand.size(); ++i)
		{
			Rectangle rect { origin.x + (CARD_WIDTH + CARD_PADDING) * (float)i, origin.y, CARD_WIDTH, CARD_HEIGHT };
			bool hidden = hideHole && i == 1;
			Color faceColor = hidden ? DARKGRAY : MakeCardColor(hand[i]);
			DrawRectangleRounded(rect, 0.1f, 6, faceColor);
			DrawRectangleRoundedLines(rect, 0.1f, 6, 3.0f, hidden ? DARKGRAY : CardBorderColor(hand[i]));
			if (!hidden)
			{
				std::string label = CardLabel(hand[i]);
				DrawText(label.c_str(), rect.x + 10, rect.y + 10, 22, hidden ? LIGHTGRAY : BLACK);
			}
			else
			{
				DrawText("??", rect.x + 16, rect.y + 38, 30, LIGHTGRAY);
			}
		}
	}

	void render()
	{
		ensureLayout();
		DrawRectangleGradientV(0, 0, g_.screenWidth, g_.screenHeight, Color{ 4, 74, 53, 255 }, Color{ 2, 48, 34, 255 });
		DrawText(TextFormat("Blackjack | %s", playerProfile->name.c_str()), 30, 60, 40, GOLD);
		DrawText(TextFormat("Bankroll: $%d", bankroll()), 30, 110, 28, RAYWHITE);
		DrawText(TextFormat("Current Bet: $%d", betAmount), 30, 145, 24, RAYWHITE);
		DrawText(TextFormat("W/L/P: %d/%d/%d", playerProfile->blackjackWins, playerProfile->blackjackLosses, playerProfile->blackjackPushes), 30, 175, 22, LIGHTGRAY);
		if (!lastOutcome.empty())
		{
			DrawText(lastOutcome.c_str(), 30, 180, 22, LIGHTGRAY);
		}

		DrawText("Dealer", 30, 230, 22, RAYWHITE);
		renderHand(dealerHand, Vector2{ 30, 260 }, !dealerReveal && phase == Phase::PlayerTurn);
		if (!dealerHand.empty())
		{
			bool showValue = dealerReveal || phase == Phase::RoundOver;
			if (showValue)
			{
				DrawText(TextFormat("Total: %d", handValue(dealerHand)), 30, 260 + CARD_HEIGHT + 10, 22, LIGHTGRAY);
			}
		}

		DrawText("Player", 30, 420, 22, RAYWHITE);
		renderHand(playerHand, Vector2{ 30, 450 }, false);
		if (!playerHand.empty())
		{
			DrawText(TextFormat("Total: %d", handValue(playerHand)), 30, 450 + CARD_HEIGHT + 10, 22, LIGHTGRAY);
		}

		DrawText(status.c_str(), 30, g_.screenHeight - 120, 24, RAYWHITE);
		DrawText("Adjust bet:", g_.screenWidth - 240.0f, 120, 22, RAYWHITE);
		DrawButton(betDownButton);
		DrawButton(betUpButton);
		DrawButton(backButton);
		DrawButton(dealButton);
		DrawButton(hitButton);
		DrawButton(standButton);
		DrawButton(doubleButton);
		DrawButton(playAgainButton);

		if (phase != Phase::PlayerTurn)
		{
			DrawRectangleRec(hitButton.bounds, Color{ 0, 0, 0, 120 });
			DrawRectangleRec(standButton.bounds, Color{ 0, 0, 0, 120 });
		}
		if (!playerCanDouble)
		{
			DrawRectangleRec(doubleButton.bounds, Color{ 0, 0, 0, 120 });
		}
		if (phase != Phase::Betting)
		{
			DrawRectangleRec(dealButton.bounds, Color{ 0, 0, 0, 120 });
		}
		if (phase != Phase::RoundOver)
		{
			DrawRectangleRec(playAgainButton.bounds, Color{ 0, 0, 0, 120 });
		}
	}

	FSMResult update()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			refundStake();
			return MAIN_STATE_MAIN_MENU;
		}
		if (ui::IsButtonClicked(backButton))
		{
			refundStake();
			return MAIN_STATE_MAIN_MENU;
		}
		if (ui::IsButtonClicked(betDownButton))
		{
			handleBetDelta(-BET_STEP);
		}
		if (ui::IsButtonClicked(betUpButton))
		{
			handleBetDelta(BET_STEP);
		}
		if (phase == Phase::Betting && ui::IsButtonClicked(dealButton))
		{
			startRound();
		}
		if (phase == Phase::PlayerTurn)
		{
			if (ui::IsButtonClicked(hitButton))
			{
				playerHit();
			}
			if (ui::IsButtonClicked(standButton))
			{
				playerStand();
			}
			if (ui::IsButtonClicked(doubleButton))
			{
				playerDouble();
			}
		}
		if (phase == Phase::RoundOver && ui::IsButtonClicked(playAgainButton))
		{
			resetForNextRound();
		}
		return MAIN_STATE_BLACKJACK;
	}
};

BlackjackNode::BlackjackNode()
	: impl(new Impl())
{
}

BlackjackNode::~BlackjackNode() = default;

void BlackjackNode::render()
{
	impl->render();
}

FSMResult BlackjackNode::update()
{
	return impl->update();
}
