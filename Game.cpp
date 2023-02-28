/*  Game.cpp

    TODO: Create a starting menu with how to play
    TODO: Network connectivity
    TODO: Display a player's hand side by side
    TODO: Numbered label for each card in hand
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "Card.cpp"

using namespace std;

int numOfPlayers = 2;

Card mainDeck; // source deck of cards that is drawn from
Card *currentCard;
Card discardDeck;
Card *playerDecks;

// reshuffle discarded cards into main deck
void restockDeck() {
    cout << "Restocking deck..." << endl;
    discardDeck.shuffleDeck();
    mainDeck.combineDecks(discardDeck);
}

// Deals card from main Deck to player hand
bool dealCard(Card *deck) {
    Card newCard = mainDeck.removeFromTopOfDeck();
    if(mainDeck.deck.empty()) {
        restockDeck();
    }
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

// plays turn for given player deck
void playTurn(Card *playerDeck) {
    cout << "Current Card: " << endl;
    cout << currentCard->cardToString() << endl;
    cout << currentCard->cardToAscii() << endl;
    cout << endl;

    cout << "Your hand" << endl;
    playerDeck->printDeckHorizontal();

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
        //system("clear");
        cout << "Invalid play" << endl;
        //this_thread::sleep_for(1s);
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

    Card newCard = mainDeck.removeFromTopOfDeck();
    currentCard = new Card(newCard.suit, newCard.instanceRank);

    while(true) {
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

void howToPlay() {
    // TODO
}

void gameMenu() {
    string title = "\n"
                    " ██████╗██████╗  █████╗ ███████╗██╗   ██╗    ███████╗██╗ ██████╗ ██╗  ██╗████████╗███████╗\n"
                    "██╔════╝██╔══██╗██╔══██╗╚══███╔╝╚██╗ ██╔╝    ██╔════╝██║██╔════╝ ██║  ██║╚══██╔══╝██╔════╝\n"
                    "██║     ██████╔╝███████║  ███╔╝  ╚████╔╝     █████╗  ██║██║  ███╗███████║   ██║   ███████╗\n"
                    "██║     ██╔══██╗██╔══██║ ███╔╝    ╚██╔╝      ██╔══╝  ██║██║   ██║██╔══██║   ██║   ╚════██║\n"
                    "╚██████╗██║  ██║██║  ██║███████╗   ██║       ███████╗██║╚██████╔╝██║  ██║   ██║   ███████║\n"
                    " ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝   ╚═╝       ╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝\n"
                    "                                                                                          ";

    int option = 0;

    while(option != -1) {
        cout << title << endl;
        cout << "1. Play game" << endl;
        cout << "2. How to Play" << endl;
        cout << "3. Quit" << endl;
        cout << "Choose an option: ";
        cin >> option;

        if(option == 1) {
            system("clear");
            startGame();
        } else if(option == 2) {
            howToPlay();
        } else if(option == 3) {
            return;
        } else {
            cout << "Choose a valid option" << endl;
        }
    }


}

int main() {
    // TODO - network configuration

    gameMenu();

    return 0;
}
