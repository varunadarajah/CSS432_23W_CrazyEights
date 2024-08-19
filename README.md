# CSS432_23W_CrazyEights
![image](https://github.com/user-attachments/assets/37c5c578-029d-4cce-866e-6da59f44bb44)

## Compiling and Running Instructions
In linux with all files copied, run the following command to compile: 
```g++ Game.cpp Card.cpp Server.cpp -o crazyEights```

Then run the crazyEights file with ```./crazyEights```

## How to Play
From the host computer, run the compiled game and enter “1” to start the host server. It will then ask for the number of players you want for your game. Enter “2” for the minimum. The host server will then start listening for incoming client connections.
From two separate computers, run the game and enter “2” from the menu to join a game. Enter the host address or IP to join the game. Once all players have joined, the game will automatically start.
On your turn, you will be shown the top of the discard pile as well as the cards in your hand. If you have a card in your hand that matches the rank or suit of the card on top of the discard pile, you may select to discard it (Enter its corresponding number) Otherwise, you must draw a card from the main deck (Enter 0). Either of these actions ends your turn. 
The first player to discard all their cards wins the game. 
Note: To simplify and accelerate game pace, two changes have been made from classic crazy eights. Eights function as normal cards, and drawing a card from the main deck ends your turn.

## Implementation
The game is made up of 3 classes, Card.cpp, Server.cpp, and Game.cpp.

### Card.cpp:
This class defines the way the cards and decks work and interact with each other. It allows cards to store a rank and suit variable. It also allows for decks of card objects to be created can be added to, removed from, or combined with the appropriate error checking. Decks can have cards manually added/removed from it, or can be automatically populated with all the cards found in a standard deck (52). It allows for cards to be printed in ASCII art or as a simple string.

### Server.cpp:
The Server class manages and holds the TCP connections for the game. When created, it asks the user to enter the max number of players for the game, and starts listening on the defined port. Once a connection is received, the socket is stored in a vector array of clients, corresponding to the player number (i.e. Player 1 is stored as clients[0]). It will keep listening until the max number of players has been reached. These connections are maintained until the game is over.
The class has two methods, sendMsg and recieveMsg, that is utilized by the main Game.cpp. Each of these methods takes an input of the player number that message should be sent/received.
When the game is over and the Server object is deleted, the destructor for the class closes the client connections and the server socket.

### Game.cpp:
This class contains the core management for the game as well as the code for client connection to the server. When a user hosts a game, a new Server object is created. Once creation is finished/all players are connected, the startGame method is ran. This creates the mainDeck of cards, shuffles the cards, distributes them to each player in order, and draws one card as the currentCard on the table. The players’ hands are stored as an array of decks corresponding to the player number.
From there, it loops through each players turn in order until there is a winner/a player’s hand is empty after a play. When the playTurn method is called, a message is sent to each client displaying the current card and their personal hands. For the player whose turn it is, the message “YOURTURN” is sent to indicate to the client that it is there turn and to ask the client for what card they want to play.
Once there is a winner, the “GAMEOVER” message is sent to the clients to indicate the game is over and sends them back to the main menu. On the host’s end, the server object is deleted and is brought back to the main menu.

![image](https://github.com/user-attachments/assets/7701dee2-040b-4c3d-8d59-46c91ae4cb8f)
Team Member Contribution
Varun - 50% (Game.cpp, Server.cpp)
Dominic - 50% (Card.cpp, Client/server network logic)
