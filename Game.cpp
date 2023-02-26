/*  Game.cpp

    TODO: Network connectivity
    TODO: Display a player's hand side by side
*/

#include <iostream>
#include <vector>
#include "Card.cpp"

using namespace std;

int numOfPlayers = 2;

Card mainDeck; // source deck of cards that is drawn from
Card currentCard;
Card discardDeck;
Card *playerDecks;

// Deals card from main Deck to player hand
bool dealCard(Card *deck) {
    Card newCard = mainDeck.removeFromTopOfDeck();
    return deck->addToDeck(newCard.suit, newCard.instanceRank);
}

// checks if a card being placed is valid
bool checkCard(Card newCard) {
    if(currentCard.suit == newCard.suit) {
        return true;
    }
    if (currentCard.instanceRank == newCard.instanceRank) {
        return true;
    }

    return false;
}

bool checkWinner(Card playerDeck) {
    if(playerDeck.deck.empty()) {
        return true;
    }

    return false;
}

// reshuffle discarded cards into main deck
void restockDeck() {
    discardDeck.shuffleDeck();
    mainDeck.combineDecks(discardDeck);
}

// plays turn for given player deck
void playTurn(Card playerDeck) {
    cout << "Current Card: " << endl;
    currentCard.cardToAscii();

    cout << "Your hand" << endl;
    playerDeck.printDeck();

    int chosenCard;
    cout << "Choose a card to play: ";
    cin >> chosenCard;

    // if valid card, place
    if(checkCard(playerDeck.deck[chosenCard])) {
        discardDeck.addToDeck(currentCard.suit, currentCard.instanceRank);
        currentCard = playerDeck.deck[chosenCard];

        playerDeck.deck.erase(playerDeck.deck.begin() + chosenCard - 1);
    } else {
        playTurn(playerDeck);
    }
}

void startGame() {
    mainDeck.makeDeck();
    mainDeck.shuffleDeck();

    playerDecks = new Card[numOfPlayers];

    // deal initial hands of 5 cards each
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < numOfPlayers; j++) {
            dealCard(&playerDecks[j]);
        }
    }

    currentCard = mainDeck.removeFromTopOfDeck();

    while(mainDeck.deck.size() > 0) {
        for(int i = 0; i < numOfPlayers; i++) {
            cout << "Player " << i << " turn" << endl;
            playTurn(playerDecks[i]);

            if (checkWinner(playerDecks[i])) {
                return;
            }
        }
    }
}

int main() {
    // TODO - network configuration

    startGame();

    return 0;
}
