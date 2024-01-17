#include <iostream>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iterator>

using namespace std;

// define enumeration to store card suit names
enum CardSuit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

// define enumeration to store card suit ranks
enum CardRank {
    TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};

// define Card class to represent a playing card
class Card {
    private:
        CardSuit suit;
        CardRank rank;
    
    public:
        // define constructor that initializes a card (consisting of a suit and rank)
        Card(CardSuit s, CardRank r) {
            suit = s;
            rank = r;
        }

        // get method to retrieve suit of Card
        CardSuit getSuit() const {
            return suit;
        }

        // get method to retrieve rank of Card
        CardRank getRank() const {
            return rank;
        }

        // define member function that can display Card values
        void display() const {
            const char* suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
            const char* ranks[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

            cout << ranks[rank] << " of " << suits[suit] << endl;
        }
};

// define Deck class to represent the deck of cards
class Deck {
    public:
        // define constructor with member functions that initialize and shuffle the deck
        Deck() {
            initializeDeck();
            shuffleDeck();
        }

        // define method to deal cards from deck
        const Card& dealCard() {
            const Card& topCard = deck.front(); // get reference to top card of deck (1st card in 'deck' list)
            deck.pop_front(); // remove top card
            return topCard;
        }

        // define method to check if deck is empty
        bool isEmpty() const {
            return deck.empty();
        }

    private:
        // initialize linked list of Card objects 
        list<Card> deck;

        // define method to initialize all 52 cards in the deck
        void initializeDeck() {
            for (int s = HEARTS; s <= SPADES; ++s) {
                for (int r = TWO; r <= ACE; ++r) {
                    // initialize new Card object with specified suit s and rank r
                    Card newCard(static_cast<CardSuit>(s), static_cast<CardRank>(r)); // use static_cast to convert int to enum
                    // add new card to linked list
                    deck.push_back(newCard);
                }
            }
        }

        // define method to randomly shuffle the deck
        void shuffleDeck() {
            srand(static_cast<unsigned int>(time(nullptr)));
            deck.sort([](const Card& a, const Card& b) { return rand() % 2 == 0; });
        }

};

// define class to represent the Crazy 8s Game
class CrazyEightsGame {
    public:
        // define constructor to initialize game with inputted number of players
        CrazyEightsGame(int numPlayers) : numPlayers(numPlayers), currentPlayer(0) {
            initializePlayers(); // initialize given number of players
            dealInitialHands(); // deal card to each player
            discardPile.push_back(deck.dealCard()); // add a single card to the discard pile
        }

        // define method to play game
        void play() {
            while (!isGameOver()) {
                displayGameState();
                playTurn();
                rotatePlayers();
            }
            // display winner once the game is over
            displayWinner();
        }

    private:
        int numPlayers; // number of players
        int currentPlayer; // index of current player
        int roundNumber = 1; // set initial round number to 1
        vector<list<Card>> playerCards; // initialize vector to store the cards of each player
        list<Card> discardPile; // initialize list to represent the discard pile
        Deck deck; // intitlaize Deck object for the deck of cards

        // define method to initialize playerCards according to the number of players
        void initializePlayers() {
            playerCards.resize(numPlayers);
        }

        // define method to deal the initial set of 8 cards to each player
        void dealInitialHands() {
        // add 8 cards to each player's hand
            for (int i = 0; i < numPlayers; ++i) {
                for (int j = 0; j < 7; ++j) {
                    playerCards[i].push_back(deck.dealCard());
                }
            }
        }

        // define method to display the current status of the game
        void displayGameState() {
            cout << "Round " << roundNumber << " : "<< endl << "Current card on the discard pile: ";
            discardPile.back().display(); // display latest card of the discard pile
            cout << "---------------------------------------" << endl;
            roundNumber++;
        }

        // define method to enable current player to draw a card
        void drawCard() {
            if (!deck.isEmpty()) {
                // deal card to player
                playerCards[currentPlayer].push_back(deck.dealCard());
            } else {
                // when the deck is empty, reshuffle by temporarily storing cards in a temp list
                list<Card> temp;
                while (discardPile.size() > 1) {
                    temp.push_back(discardPile.back());
                    discardPile.pop_back();
                }

                // re-initialize the playing deck and discard deck
                deck = Deck();
                discardPile = temp;
            }
        }

        // define method to handle a player's turn
        void playTurn() {
            cout << "PLAYER " << currentPlayer + 1 << "'S TURN" << endl;
            // display all cards currently in the player's hand
            displayPlayerHand();

            bool validPlay = false;
            while (!validPlay) {
                int choice;
                cout << "Enter the number of the card you want to play (Input '0' to draw a card): ";
                cin >> choice;

                // draw card
                if (choice == 0) {
                    drawCard();
                    validPlay = true;
                } // check if current player's hand is not empty
                else if (!playerCards[currentPlayer].empty()) {
                    // craete iterator that will poiny to the card chosen by the player
                    auto it = playerCards[currentPlayer].begin();
                    advance(it, choice - 1); // move iterator to the position of the chosen card
                    if (isValidPlay(*it)) { // check if chosen vard is valid
                        // determine the index of the chosen card
                        int index = distance(playerCards[currentPlayer].begin(), it);
                        // play the chosen card
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

        // define method to check if the specified card is a value mod
        bool isValidPlay(const Card& card) {
            // get reference to the top card of the discard pile
            const Card& topCard = discardPile.back();
            // determine if the specified card can be played on the top card
            return (card.getSuit() == topCard.getSuit() || card.getRank() == topCard.getRank() || card.getRank() == EIGHT);
        }

        // method to play card
        void playCard(int index) {
            auto it = playerCards[currentPlayer].begin();
            advance(it, index);
            Card playedCard = *it;
            playerCards[currentPlayer].erase(it);
            discardPile.push_back(playedCard);

            if (playedCard.getRank() == EIGHT) {
                handleEight();
            }
        }

        // method to handle the 8 rank
        void handleEight() {
            bool validInput = false;
            int suitChoice;

            while (!validInput) {
                cout << "Input '0' for Hearts, '1' for Diamonds, '2' for Clubs, or '3' for Spades";
                cin >> suitChoice;
                if (suitChoice==0 || suitChoice==1 || suitChoice==2 || suitChoice==3) {
                    CardSuit newSuit = static_cast<CardSuit>(suitChoice);
                    discardPile.back() = Card(newSuit, EIGHT);
                } else {
                    cout << "Invalid input. Please enter either '0', '1', '2', or '3'";
                }
            }
        }

        // method to display all cards in the player's hand
        void displayPlayerHand() const {
            cout << "Player " << currentPlayer + 1 << "'s hand: " << endl;
            int cardNumber = 1;
            for (const auto& card : playerCards[currentPlayer]) {
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
            for (const auto& hand : playerCards) {
                if (hand.empty()) {
                    return true;
                }
            }
            return false;
        }

        void displayWinner() const {
            for (int i = 0; i < numPlayers; ++i) {
                if (playerCards[i].empty()) {
                    cout << "Player " << i + 1 << " wins!\n";
                    return;
                }
            }
        }
};

int main() {
    int numPlayers;
    cout << "WELCOME TO THE CRAZY 8s CONSOLE GAME!" << endl;
    system("CLS"); 
    cout << "Please enter the number of players (2-6): ";
    cin >> numPlayers;

    if (numPlayers < 2 || numPlayers > 6) {
        cout << "Invalid number of players. Please enter a number between 2 and 6.\n";
        return 1;
    }

    CrazyEightsGame game(numPlayers);
    game.play();

    return 0;
}
