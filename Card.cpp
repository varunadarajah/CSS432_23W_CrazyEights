/*  Card.cpp 
    2/10/23 8:01PM
    
    TODO: probably will need a different class to manage all these decks

*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>       
#include <chrono>       
#include <array>

using namespace std;

class Card{
public:
string suit = "";
string instanceRank = "";
vector<Card> deck;


void combineDecks(Card inputDeck){ //adds cards from both decks into the this. object. deletes all cards in inputDeck
    while(inputDeck.deck.size() > 0){
        Card currentCard = inputDeck.removeFromTopOfDeck();
        addToDeck(currentCard.suit, currentCard.instanceRank);
        }
    
}

bool addToDeck(string suit, string rank){ //returns false if invalid input, true otherwise
    if(!checkValidCard(suit, rank)){
        return false;
    }

    for(int i = 0; i < deck.size() ; i++){
        if(deck[i].suit == suit && deck[i].instanceRank == rank){
            cout << "Duplicate card detected, not adding this one to the deck" << endl;
            return false;
        }
    }

    Card newCard = Card(suit, rank);
    deck.push_back(newCard);


    return true;
}

Card removeFromTopOfDeck(){//removes and returns card at the end(top) of the vector(deck)

    if(deck.size() == 0){
        cout << "Trying to remove from empty deck error" << endl;
        return Card();
    }
    Card removedCard = deck[deck.size()-1];
    deck.pop_back();
    return removedCard;
}

bool checkValidCard( string suit, string rank){ //returns true if valid, false if not
    if(suit == "Spades" || "Hearts" || "Clubs" || "Diamonds"){
        if(rank  == "2" || rank  == "3" ||rank  == "4" ||rank  == "5" ||rank  == "6" ||rank  == "7" ||rank  == "8" ||rank  == "9" ||rank  == "10" ||
         rank == "Jack" || rank == "Queen" || rank == "King" || rank == "Ace");

         return true;
    }
    return false;
}

string cardToString(){
    return instanceRank + " of " + suit;
}

string cardToAscii() {
    const string suitSymbols[4] = {"♠", "♥", "♣", "♦"};

    string suitSymbol = "";
    if(suit == "Spades") {
        suitSymbol = suitSymbols[0];
    } else if(suit == "Hearts") {
        suitSymbol = suitSymbols[1];
    } else if(suit == "Clubs") {
        suitSymbol = suitSymbols[2];
    } else if(suit == "Diamonds") {
        suitSymbol = suitSymbols[3];
    }

    string rankSymbolTop = "";
    string rankSymbolBottom = "";
    if(instanceRank == "10") {
        rankSymbolTop = "10";
        rankSymbolBottom = "10";
    } else if (instanceRank == "Jack") {
        rankSymbolTop = "J ";
        rankSymbolBottom = " J";
    } else if (instanceRank == "Queen") {
        rankSymbolTop = "Q ";
        rankSymbolBottom = " Q";
    } else if (instanceRank == "King") {
        rankSymbolTop = "K ";
        rankSymbolBottom = " K";
    } else if (instanceRank == "Ace") {
        rankSymbolTop = "A ";
        rankSymbolBottom = " A";
    } else {
        rankSymbolTop = instanceRank + " ";
        rankSymbolBottom = " " + instanceRank;
    }

    string cardAscii = "┌─────────┐\n"
                  "│" + rankSymbolTop + "       │\n"
                  "│         │\n"
                  "│         │\n"
                  "│    " + suitSymbol + "    │\n"
                  "│         │\n"
                  "│         │\n"
                  "│       "+ rankSymbolBottom +"│\n"
                  "└─────────┘";

    return cardAscii;
}

void printDeck(){
    for(int i = 0; i < deck.size() ; i++){
        cout << deck[i].cardToString() << endl;
        cout << deck[i].cardToAscii() << endl;
    }
}

void makeDeck(){
    string suits[4] = {"Spades", "Hearts", "Clubs", "Diamonds"};
    string ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    for(int suitNum = 0; suitNum < 4 ; suitNum++){
        string littleSuit = suits[suitNum];
        for(int rankNum = 0 ; rankNum < 13 ; rankNum++){
            string littleRank = ranks[rankNum];
            Card tempCard(littleSuit, littleRank);
            deck.push_back(tempCard);
        }
    }
}

 void shuffleDeck() {
  random_device rd;
  mt19937 g(rd());
  shuffle(deck.begin(), deck.end(), g);
}

Card(){

}

Card(string suit1, string rank1){
    suit = suit1;
    instanceRank = rank1;
}

Card(Card const &c){
    suit = c.suit;
    instanceRank = c.instanceRank;
}

};

/*
int main(){
    Card deck1;
    Card deck2;

    deck1.addToDeck("Hearts", "Queen");
    deck1.addToDeck("Spades", "8");
    deck1.removeFromTopOfDeck(deck1);


    deck2.addToDeck("Clubs", "2");
    deck2.addToDeck("Clubs", "3");
    deck2.addToDeck("Clubs", "4");

    deck1.combineDecks(deck2);
    cout << "DECK 2===============================================================" << endl;
    deck2.printDeck();
    cout << "DECK 1===============================================================" << endl;
    deck1.printDeck();

   // card1.makeDeck();
   // card1.shuffleDeck();
   // card1.printDeck();

    return 0;
}*/
