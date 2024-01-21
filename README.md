# Crazy 8s Console Game

## Overview 

Welcome to the Crazy 8s Console Game! This C++ program implements a console-based version of the Crazy 8s card game. 
If you are unfamiliar with the rules of this card game, click [here](https://www.wikihow.com/Play-Crazy-Eights)!
This game supports 2 to 5 players and features a deck of cards, player hands, and a discard pile.

## Code Structure

### Card Class

- The `Card` class represents a playing card with a suit and rank.
- This class includes methods to retrieve the suit and rank, as well as a `display` method to print the card details.

### Deck Class

- The `Deck` class manages the deck of cards.
- This class initializes and shuffles the deck, deals cards, and checks whether the deck is empty.

### CrazyEightsGame Class

- The `CrazyEightsGame` class orchestrates the gameplay.
- This class initializes the game with the number of players, deals cards, and manages player turns.
- Game status, the discard pile, and the deck are stored within this class.

## How to Play

1. Clone this repo.
2. Compile and run the program.
3. Enter the number of players (2-5).
4. The game will initialize, and each player will be dealt 8 cards.
4. Players take turns playing a card or drawing a new one from the deck.
5. The game continues until a player runs out of cards.
6. The winner is displayed at the end of the game.

## Enjoy the Game!

Enjoy playing Crazy 8s with your friends in this console-based game!
