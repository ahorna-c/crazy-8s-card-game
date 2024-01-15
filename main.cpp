#include <iostream>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iterator>

using namespace std;

enum Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

enum Rank {
    TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};

class Card {
    private:
        Suit suit;
        Rank rank;
    
    public:
        Card(Suit s, Rank r) {
            suit = s;
            rank = r;
        }

        Suit getSuit() const {
            return suit;
        }

        Rank getRank() const {
            return rank;
        }

        void display() const {
            const char* suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
            const char* ranks[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

            cout << ranks[rank] << " of " << suits[suit] << endl;
        }
};

class Deck {
public:
    Deck() {
        initializeDeck();
        shuffleDeck();
    }

    const Card& dealCard() {
        const Card& topCard = cards.front();
        cards.pop_front();
        return topCard;
    }

    bool isEmpty() const {
        return cards.empty();
    }

private:
    list<Card> cards;

    void initializeDeck() {
    // initialize card deck with all 52 cards
        for (int s = HEARTS; s <= SPADES; ++s) {
            for (int r = TWO; r <= ACE; ++r) {
                Card newCard(static_cast<Suit>(s), static_cast<Rank>(r));
                cards.push_back(newCard);
            }
        }
    }

    void shuffleDeck() {
    // randomly shuffle deck
        srand(static_cast<unsigned int>(time(nullptr)));
        cards.sort([](const Card& a, const Card& b) { return rand() % 2 == 0; });
    }

};

class CrazyEightsGame {
    public:
        CrazyEightsGame(int numPlayers) : numPlayers(numPlayers), currentPlayer(0) {
            initializePlayers();
            dealInitialHands();
            discardPile.push_back(deck.dealCard());
        }

        void play() {
            while (!isGameOver()) {
                displayGameState();
                playTurn();
                rotatePlayers();
            }
            displayWinner();
        }

    private:
        int numPlayers;
        int currentPlayer;
        int roundNumber = 1;
        vector<list<Card>> playerHands;
        list<Card> discardPile;
        Deck deck;

        void initializePlayers() {
            playerHands.resize(numPlayers);
        }

        void dealInitialHands() {
        // add 8 cards to each player's hand
            for (int i = 0; i < numPlayers; ++i) {
                for (int j = 0; j < 7; ++j) {
                    playerHands[i].push_back(deck.dealCard());
                }
            }
        }

        void displayGameState() {
            cout << "Round " << roundNumber << " : "<< endl << "Current card on the discard pile: ";
            discardPile.back().display();
            cout << "---------------------------------------" << endl;
            roundNumber ++;
        }

        void drawCard() {
            if (!deck.isEmpty()) {
                playerHands[currentPlayer].push_back(deck.dealCard());
            } else {
                // when the deck is empty, reshuffle by temporarily storing cards in a temp list
                list<Card> temp;
                while (discardPile.size() > 1) {
                    temp.push_back(discardPile.back());
                    discardPile.pop_back();
                }

                deck = Deck();
                discardPile = temp;
            }
        }

        void playTurn() {
            cout << "PLAYER " << currentPlayer + 1 << "'S TURN" << endl;
            displayPlayerHand();

            bool validPlay = false;
            while (!validPlay) {
                int choice;
                cout << "Enter the number of the card you want to play (Input '0' to draw a card): ";
                cin >> choice;

                if (choice == 0) {
                    drawCard();
                    validPlay = true;
                }
                else if (!playerHands[currentPlayer].empty()) {
                    auto it = playerHands[currentPlayer].begin();
                    advance(it, choice - 1);
                    if (isValidPlay(*it)) {
                        int index = distance(playerHands[currentPlayer].begin(), it);
                        playCard(index);
                        validPlay = true;
                    } else {
                        cout << "Invalid move. Please input a valid card number or draw a card.\n";
                    }
                } else {
                    cout << "Invalid move. Please choose a valid card number or draw a card.\n";
                }
            }
        }

        bool isValidPlay(const Card& card) {
            const Card& topCard = discardPile.back();
            return (card.getSuit() == topCard.getSuit() || card.getRank() == topCard.getRank() || card.getRank() == EIGHT);
        }

        void playCard(int index) {
            auto it = playerHands[currentPlayer].begin();
            advance(it, index);
            Card playedCard = *it;
            playerHands[currentPlayer].erase(it);
            discardPile.push_back(playedCard);

            if (playedCard.getRank() == EIGHT) {
                handleEight();
            }
        }

        void handleEight() {
            bool validInput = false;
            int suitChoice;

            while (!validInput) {
                cout << "Input '0' for Hearts, '1' for Diamonds, '2' for Clubs, or '3' for Spades";
                cin >> suitChoice;
                if (suitChoice!=0 || suitChoice!=1 || suitChoice!=2|| suitChoice!=3) {
                    Suit newSuit = static_cast<Suit>(suitChoice);
                    discardPile.back() = Card(newSuit, EIGHT);
                } else {
                    cout << "Invalid input. Please enter either '0', '1', '2', or '3'";
                }
            }
        }

        void displayPlayerHand() const {
            cout << "Player " << currentPlayer + 1 << "'s hand: " << endl;
            int cardNumber = 1;
            for (const auto& card : playerHands[currentPlayer]) {
                cout << cardNumber << " - ";
                card.display();
                cardNumber ++;
            }
            cout << "\n";
        }

        void rotatePlayers() {
            currentPlayer = (currentPlayer + 1) % numPlayers;
        }

        bool isGameOver() const {
            for (const auto& hand : playerHands) {
                if (hand.empty()) {
                    return true;
                }
            }
            return false;
        }

        void displayWinner() const {
            for (int i = 0; i < numPlayers; ++i) {
                if (playerHands[i].empty()) {
                    cout << "Player " << i + 1 << " wins!\n";
                    return;
                }
            }
        }
};

int main() {
    int numPlayers;
    cout << "WELCOME TO THE CRAZY 8s CONSOLE GAME!" << endl;
    cout << "Please enter the number of players (2-4): ";
    cin >> numPlayers;

    if (numPlayers < 2 || numPlayers > 4) {
        cout << "Invalid number of players. Please enter a number between 2 and 4.\n";
        return 1;
    }

    CrazyEightsGame game(numPlayers);
    game.play();

    return 0;
}
