/*  Server.cpp
    3/7/23 8:10 PM

    TODO: Start game option for server once all players have joined
        - right now game just auto starts after 2 players join
        - might be a Server.cpp thing

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

using namespace std;

#define PORT 21094
#define BUFFSIZE 3000
#define MAXPLAYERS 2

class Server{
public:
    int clientCounter = 0;
    int server_fd;
    vector<int> clients;
    int valread;

    // constructor
    Server(){
        cout << "Setting up server..." << endl;

        struct sockaddr_in server_addr, client_addr;

        //create
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            cerr << "bad sock" << endl;
            //return -1;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);
        //bind
        if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            cerr << "bad bind" << endl;
            //return -1;
        }

        //listen
        if (listen(server_fd, 3) < 0)
        {
            cerr << "bad listen" << endl;
            //return -1;
        }
        cout << "Server running, waiting for clients..." << endl;

        socklen_t client_size = sizeof(client_addr);

        //accepting connections
        // until MAXPLAYERS number connections are reached, keep accepting connections
        while(clientCounter < MAXPLAYERS) {
            int newClient;
            if ((newClient = accept(server_fd, (struct sockaddr *)&client_addr, &client_size)) < 0)
            {
                cerr << "bad accept" << endl;
                //return -1;
            }

            clients.push_back(newClient);
            clientCounter++;
            cout << "Client #" << clientCounter << " connected" << endl;
            //sendMsg("You are Player " + to_string(clientCounter+1), clientCounter); // not working
        }
    }

    // destructor
    ~Server() {
        close(clientCounter);
        close(server_fd);
        cout << "offline" << endl;
    }

    // sends message to given client
    void sendMsg(string str, int player){
        if (send(clients[player], str.c_str(), strlen(str.c_str())+1, 0) < 0) {
            cout << "Message send failed" << endl;
        } else {
            cout << "Message sent" << endl;
        }
    }

    // receives message from a given client
    string receiveMsg(int player){
        //reading messages from client
        char buffer[BUFFSIZE];

        while (true)
        {
            valread = read(clients[player], buffer, BUFFSIZE);
            if (valread == 0)
            {
                cout << "Client disconnected" << endl;
                break;
            }
            cout << "Message received: " << buffer << endl;
            return string(buffer);
        }
        return string(buffer);
    }
};