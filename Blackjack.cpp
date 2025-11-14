//
// Created by neptu on 2025-11-13.
//
#include "Blackjack.h"

// --- Additional Includes for Implementation ---
#include <iostream>
#include <algorithm> // For std::shuffle
#include <random>    // For std::random_device, std::mt19937

// --- Constructor Implementation ---
// Use Blackjack:: to specify we are defining a member of the Blackjack class
Blackjack::Blackjack() {
    generateShoe(); // Call the private member function
}

// --- Public Method Implementation ---

int Blackjack::playHand(int bet) {
    // 1. Clear previous hands
    playerHand.clear();
    dealerHand.clear();

    // 2. Initial Deal
    playerHand.push_back(dealCard());
    dealerHand.push_back(dealCard());
    playerHand.push_back(dealCard());
    dealerHand.push_back(dealCard());

//    // 3. Show hands
//    // We can't call printHand for the dealer yet, as one card is hidden
//    std::cout << "--- New Hand --- \n";
//    std::cout << "Dealer's up card: ";
//    // Manually print just the first card
//    int rank = dealerHand[0] % 13;
//    if (rank == 0) std::cout << "A\n";
//    else if (rank == 10) std::cout << "J\n";
//    else if (rank == 11) std::cout << "Q\n";
//    else if (rank == 12) std::cout << "K\n";
//    else if (rank == 9) std::cout << "10\n";
//    else std::cout << (rank + 1) << "\n";

    printHand(playerHand, "Player");

    // 4. Check for immediate Blackjack
    if (value(playerHand) == 21) {
        if (value(dealerHand) == 21) {
            return bet;
        } else {
            return (5 * bet) / 2;
        }
    }

    // 5. Player's Turn (Hit or Stand)
    char choice = ' ';
    while (value(playerHand) < 21) {
        std::cout << "Hit (h) or Stand (s)? ";
        std::cin >> choice;

        if (choice == 'h' || choice == 'H') {
            playerHand.push_back(dealCard());
            printHand(playerHand, "Player");
            if (value(playerHand) > 21) {
                //bust
                return 0; // Hand is over
            }
        } else if (choice == 's' || choice == 'S') {
            //stand
            break; // Exit the loop
        }
    }

    // 7. Dealer's Turn
    std::cout << "\n--- Dealer's Turn ---\n";
    printHand(dealerHand, "Dealer"); // Now show the full hand

    while (value(dealerHand) < 17) {
        std::cout << "Dealer hits.\n";
        dealerHand.push_back(dealCard());
        printHand(dealerHand, "Dealer");
    }

    // 8. Determine Winner
    int playerVal = value(playerHand);
    int dealerVal = value(dealerHand);

    std::cout << "\n--- Final Result ---\n";
    printHand(playerHand, "Player");
    printHand(dealerHand, "Dealer");

    if (dealerVal > 21) {
        return 2 * bet;
    } else if (playerVal > dealerVal) {
        return 2 * bet;
    } else if (dealerVal > playerVal) {
        return 0;
    } else {
        return bet;
    }
}

// --- Private Method Implementations ---

void Blackjack::generateShoe() {
    int numDecks = 4;
    shoe.clear(); // Clear the shoe before regenerating

    for (int i = 0; i < numDecks * 52; i++) {
        shoe.push_back(i % 52);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shoe.begin(), shoe.end(), g);
    std::cout << "(Shoe has been shuffled with " << numDecks << " decks.)\n";
}

int Blackjack::dealCard() {
    // Reshuffle if shoe is low
    if (shoe.size() < 52) {
        //std::cout << "\nReshuffling the shoe...\n";
        generateShoe();
    }

    int card = shoe.back(); // Get card from end of vector
    shoe.pop_back();        // Remove it
    return card;
}

int Blackjack::value(const std::vector<int>& cards) {
    int val = 0;
    int numAces = 0;

    // First pass: sum all cards, count Aces as 1
    for (int card : cards) {
        int rank = card % 13; // Rank 0=Ace, 1=2, ..., 9=10, 10=J, 11=Q, 12=K

        if (rank == 0) {
            numAces++;
            val += 1;
        } else if (rank >= 9) { // 10, J, Q, K
            val += 10;
        } else { // 2 through 9
            val += (rank + 1);
        }
    }

    // Second pass: "upgrade" Aces from 1 to 11
    for (int i = 0; i < numAces; i++) {
        if (val + 10 <= 21) {
            val += 10;
        } else {
            break;
        }
    }
    return val;
}

void Blackjack::printHand(const std::vector<int>& hand, const std::string& name) {
    std::cout << name << "'s hand: ";
    for (int card : hand) {
        int rank = card % 13;
        if (rank == 0) std::cout << "A ";
        else if (rank == 10) std::cout << "J ";
        else if (rank == 11) std::cout << "Q ";
        else if (rank == 12) std::cout << "K ";
        else if (rank == 9) std::cout << "10 ";
        else std::cout << (rank + 1) << " "; // rank 1 is '2', etc.
    }
    std::cout << " (Value: " << value(hand) << ")\n";
}
