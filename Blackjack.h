//
// Created by neptu on 2025-11-13.
//

#ifndef BLACKJACK_BLACKJACK_H
#define BLACKJACK_BLACKJACK_H

#include <vector>
#include <string> // Only need <string> and <vector> for the declaration

// --- Class Declaration ---
class Blackjack {

// Private members: Can only be accessed by the class itself
private:
    std::vector<int> shoe;
    std::vector<int> playerHand;
    std::vector<int> dealerHand;

    // --- Private Methods ---
    void generateShoe();
    int dealCard();
    int value(const std::vector<int>& cards);
    void printHand(const std::vector<int>& hand, const std::string& name);

// Public members: Can be accessed from outside the class (e.g., in main)
public:
    /**
     * Constructor: Initializes and shuffles the shoe.
     */
    Blackjack();

    /**
     * The main game loop for a single hand.
     */
    int playHand(int bet);
};

#endif //BLACKJACK_BLACKJACK_H
