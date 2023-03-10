/*  Game.cpp
    3/7/23 8:10 PM

    TODO optional:
    - make it so player screen clears after every play so it isn't cluttered, or maybe just print out a line to
          divide the screen, or any other way to make it easier to read
    - make server/host screen look nicer (anything cout outside of gameMenu is host output) (not really needed)
    - move client code to a separate Client.cpp (not really needed but would make code cleaner)
    - add the 8 card mechanic
    - when a player has more than 10 cards in their hand the list only shows up to 9 cards, ascii is still printed though

*/

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <sstream>

#include "Card.cpp"
#include "Server.cpp"

using namespace std;

//#define HOST "10.158.82.40"
//CSS lab 10: 10.158.82.40
//CSS lab 4: 10.158.82.34

int numOfPlayers;

Card mainDeck; // source deck of cards that is drawn from
Card *currentCard;
Card discardDeck;
Card *playerDecks;

Server *s;
int client_fd, valread;

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

// checks whether if a player has won / has no more cards left
bool checkWinner(Card *playerDeck) {
    if(playerDeck->deck.empty()) {
        return true;
    }

    return false;
}

// plays turn for given player deck
void playTurn(Card *playerDeck, int player) {
    stringstream ss;

    ss << "Player " << player+1 << " turn" << endl;

    ss << "Current Card: " << endl;
    ss << currentCard->cardToString() << endl;
    ss << currentCard->cardToAscii() << endl;
    ss << endl;

    ss << "Your hand: " << endl;

    // print hands & message for waiting players
    for(int i = 0; i < numOfPlayers; i++) {
        if(i != player) {
            string msg = playerDecks[i].printDeckHorizontal() + "Waiting for Player " + to_string(player+1) + "...\n";
            s->sendMsg(ss.str() + msg, i);
        }
    }

    ss << playerDeck->printDeckHorizontal();

    int chosenCard;
    cout << ss.str();
    s->sendMsg(ss.str(), player);
    s->sendMsg("YOUR TURN", player);

    cout << "Waiting for player move..." << endl;

    chosenCard = stoi(s->receiveMsg(player));

    chosenCard--;

    if(chosenCard == -1) {
        dealCard(playerDeck);
    } else if(checkCard(&playerDeck->deck[chosenCard])) { // if valid card, place
        discardDeck.addToDeck(currentCard->suit, currentCard->instanceRank);
        currentCard->suit = playerDeck->deck[chosenCard].suit;
        currentCard->instanceRank = playerDeck->deck[chosenCard].instanceRank;

        playerDeck->deck.erase(playerDeck->deck.begin() + chosenCard);
    } else {
        s->sendMsg("Invalid play\n", player);
        playTurn(playerDeck, player);
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

            playTurn(&playerDecks[i], i);

            if (checkWinner(&playerDecks[i])) {
                // send game over results to clients
                stringstream ss;
                string gameover = "\n"
                                  " ██████   █████  ███    ███ ███████      ██████  ██    ██ ███████ ██████  \n"
                                  "██       ██   ██ ████  ████ ██          ██    ██ ██    ██ ██      ██   ██ \n"
                                  "██   ███ ███████ ██ ████ ██ █████       ██    ██ ██    ██ █████   ██████  \n"
                                  "██    ██ ██   ██ ██  ██  ██ ██          ██    ██  ██  ██  ██      ██   ██ \n"
                                  " ██████  ██   ██ ██      ██ ███████      ██████    ████   ███████ ██   ██ \n"
                                  "                                                                          \n"
                                  "                                                                          ";
                ss << gameover << endl;
                ss << "Player " << i+1 << " wins!!!\n" << endl;

                for(int k = 0; k < numOfPlayers; k++) {
                    s->sendMsg(ss.str(), k);
                    s->sendMsg("GAMEOVER", k);
                }

                cout << "-----GAME OVER-----" << endl;
                cout << "Player " << i+1 << " wins!!!" << endl;

                return;
            }
        }
    }
}

void clientSendMsg(){
    string chosenCard;
    cout << "Choose a card to play (enter 0 to draw): ";
    cin >> chosenCard;
    if(send(client_fd, chosenCard.c_str(), strlen(chosenCard.c_str()), 0) < 0){
        cout<<"failed"<< endl;
    }
}

// returns false when game over
bool clientReceiveMsg(){
    // receive message
    char buffer[BUFFSIZE];
    int n = 0;
    while ((n = recv(client_fd, buffer, sizeof(buffer), 0)) > 0){
        string str = string(buffer);
        if(str == "YOUR TURN") {
            return true;
        } else if(str == "GAMEOVER") {
            return false;
        }
        cout << buffer;
    }
    return true;
}

void clientPlay(){
    while(true){
        if(!clientReceiveMsg()) {
            return;
        }
        clientSendMsg();
    }
}

void hostGame(){
    s = new Server();
    numOfPlayers = s->clientCounter;
    startGame();
    delete(s);
}

int setUpClient(){
    struct hostent *h;

    string HOST;
    cout << "Enter host address (e.g. csslab10.uwb.edu): ";
    cin >> HOST;
    cout << endl;

    cout << "Setting up client..." << endl;
    struct sockaddr_in server_addr;

    //create
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cerr << "bad socket" << endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    h = gethostbyname(HOST.c_str());

    if (inet_pton(AF_INET, inet_ntoa(*(struct in_addr*)h->h_addr), &server_addr.sin_addr) <= 0)
    {
        cerr << "invalid address or address not supported" << endl;
        return -1;
    }

    //connecting to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "connection failed" << endl;
        return -1;
    }

    cout << "Connected to server" << endl;
    cout << endl;

    clientPlay();
    return 1;
}

void howToPlay() {
    system("clear");
    string str = "\n"
                 "██   ██  ██████  ██     ██     ████████  ██████      ██████  ██       █████  ██    ██ \n"
                 "██   ██ ██    ██ ██     ██        ██    ██    ██     ██   ██ ██      ██   ██  ██  ██  \n"
                 "███████ ██    ██ ██  █  ██        ██    ██    ██     ██████  ██      ███████   ████   \n"
                 "██   ██ ██    ██ ██ ███ ██        ██    ██    ██     ██      ██      ██   ██    ██    \n"
                 "██   ██  ██████   ███ ███         ██     ██████      ██      ███████ ██   ██    ██    \n"
                 "                                                                                      \n"
                 "                                                                                      \n"
                 "The main concept of crazy eights is to get rid of all your cards. \n"
                 "On each turn you will have the opportunity to discard one card, if you have a card that matches the rank or suit of the card on the top of the discard pile\n"
                 "On your turn, you will be shown the top of the discard pile as well as the cards in your hand.\n"
                 "\n"
                 "If you have a card in your hand that matches the rank or suit of the card on top of the discard pile, you may select to discard it. \n"
                 "Otherwise, you must draw a card from the main deck. Either of these actions ends your turn.\n"
                 "\n"
                 "The first player to discard all their cards wins the game.\n"
                 "\n"
                 "Note: To simplify and accelerate game pace, two changes have been made from classic crazy eights. Eights function as normal cards, and drawing a card from the main deck ends your turn.\n";

    cout << str << endl;

    cout << "Press enter to return to menu..." << endl;
    system("read");
    system("clear");
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
        cout << "1. Host game" << endl;
        cout << "2. Join game" << endl;
        cout << "3. How to Play" << endl;
        cout << "4. Quit" << endl;
        cout << "Choose an option: ";
        cin >> option;

        if(option == 1) {
            hostGame();
        } else if(option == 2) {
            setUpClient();
        } else if(option == 3) {
            howToPlay();
        } else if(option == 4) {
            return;
        } else {
            cout << "Choose a valid option" << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    system("clear");
    gameMenu();

    return 0;
}