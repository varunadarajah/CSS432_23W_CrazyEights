/*  Game.cpp

    TODO: Create a starting menu with how to play
    TODO: Network connectivity
    TODO: Display a player's hand side by side
    TODO: Numbered label for each card in hand
*/

#include <iostream>
#include <vector>
#include "Card.cpp"

using namespace std;

int numOfPlayers = 2;

Card mainDeck; // source deck of cards that is drawn from
Card *currentCard;
Card discardDeck;
Card *playerDecks;

// Deals card from main Deck to player hand
bool dealCard(Card *deck) {
    Card newCard = mainDeck.removeFromTopOfDeck();
    return deck->addToDeck(newCard.suit, newCard.instanceRank);
}

// checks if a card being placed is valid
bool checkCard(Card *newCard) {
    if(currentCard->suit == newCard->suit) {
        return true;
    }
    if (currentCard->instanceRank == newCard->instanceRank) {
        return true;
    }

    return false;
}

bool checkWinner(Card *playerDeck) {
    if(playerDeck->deck.empty()) {
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
void playTurn(Card *playerDeck) {
    cout << "Current Card: " << endl;
    currentCard->cardToAscii();
    currentCard->cardToString();
    cout << currentCard->instanceRank << " of " << currentCard->suit << endl;

    cout << "Your hand" << endl;
    playerDeck->printDeck();

    int chosenCard;
    cout << "Choose a card to play (enter 0 to draw): ";
    cin >> chosenCard;
    chosenCard--;

    if(chosenCard == -1) {
        dealCard(playerDeck);
    } else if(checkCard(&playerDeck->deck[chosenCard])) { // if valid card, place
        discardDeck.addToDeck(currentCard->suit, currentCard->instanceRank);
        currentCard->suit = playerDeck->deck[chosenCard].suit;
        currentCard->instanceRank = playerDeck->deck[chosenCard].instanceRank;

        playerDeck->deck.erase(playerDeck->deck.begin() + chosenCard);
    } else {
        cout << "Invalid play" << endl;
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

    Card newCard = mainDeck.removeFromTopOfDeck();
    currentCard = new Card(newCard.suit, newCard.instanceRank);

    while(mainDeck.deck.size() > 0) {
        for(int i = 0; i < numOfPlayers; i++) {
            cout << "Player " << i+1 << " turn" << endl;
            playTurn(&playerDecks[i]);

            if (checkWinner(&playerDecks[i])) {
                cout << "-----GAME OVER-----" << endl;
                cout << "Player " << i+1 << " wins!!!" << endl;
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
