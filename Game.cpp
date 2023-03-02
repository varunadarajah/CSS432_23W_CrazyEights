/*  Game.cpp
    3/1/23 6:31 PM

    TODO: Create a starting menu with how to play
    TODO: Network connectivity
    TODO: Display a player's hand side by side
    TODO: Numbered label for each card in hand
*/

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
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


int setUpServer(){
    cout << "setting up server" << endl;
    int clientCounter = 0;
    int server_fd;
    int client_fd;
    int valread;

    struct sockaddr_in server_addr, client_addr;
    char buffer[1024] = {0};
    
    //create
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        cerr << "bad sock" << endl;
        return -1;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(21094);
    //bind
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "bad bind" << endl;
        return -1;
    }
    
    //listen
    if (listen(server_fd, 3) < 0)
    {
        cerr << "bad listen" << endl;
        return -1;
    }
    
    socklen_t client_size = sizeof(client_addr);
    
    //accepting connections
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_size)) < 0)
    {
        cerr << "bad accept" << endl;
        return -1;
    }
    
    clientCounter++;
    cout << "Client #" << clientCounter << " connected" << endl;
    
    //reading messages from client
    while (true)
    {
        valread = read(client_fd, buffer, 1024);
        if (valread == 0)
        {
            cout << "Client disconnected" << endl;
            break;
        }
        cout << "Message received: " << buffer << endl;
        //message back to client
        send(client_fd, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }
    
    close(client_fd);
    close(server_fd);
    cout << "offline" << endl;
    return 1;
}

int setUpClient(){
    cout << "setting up client" << endl;
    int client_fd, valread;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    
    //create
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cerr << "bad socket" << endl;
        return -1;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(21094);
    
    
    if (inet_pton(AF_INET, "10.158.82.40"/*csslab10*/, &server_addr.sin_addr) <= 0)
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
}
//no args = client
//arg = server
int main(int argc, char *argv[]) {
    if(argc > 1){
        setUpServer();
    }
    else{
        setUpClient();
    }
    

    //startGame();

    return 0;
}