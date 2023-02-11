/*  Card.cpp 
    2/10/23 8:01PM
    
    TODO: allow for cards to be added/removed to a deck manually (check for duplicates and spit error?)
    TODO: allow for decks to be combined using the above functonality
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

string cardToString(){
    return instanceRank + " of " + suit;
}

void printDeck(){
    for(int i = 0; i < deck.size() ; i++){
            cout << deck[i].cardToString() << endl;
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
};

int main(){
    Card card1;
    card1.makeDeck();
    card1.shuffleDeck();
    card1.printDeck();

    return 0;
}