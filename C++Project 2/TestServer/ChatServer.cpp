// File:  ChatServer.cpp
// Purpose: To demonstrates the librates capabilities. This is the server half of the chat client/server application that was made.
//			The server relays messages recieved from clients to the other clients.
// Authors:  James Haig, Riley Campbell
// Date:  April 21, 2014
#include <SocketLib.hpp>
#include <iostream>
#include <vector>
#include <thread>
using namespace std;

class Client{

public:
	string name;
	string address;
	sockaddr sockAddr;
	
	Client(string n, string a, sockaddr s){
		name = n;
		address = a;
		sockAddr = s;
	}
	~Client(){}
};

bool done = false;
vector<Client> clients;

void Listen(UDPSocket &sock){

	while (!done){
	string message = sock.RecieveMessage(30);//THE '30' is the amount of seconds it will wait until a timeout. Timeout put in place so server can shut down even if no clients present
		//depending on message, send it to other clients
		if(message.substr(0, 11) == "!NEWCLIENT!"){
			bool bClientExists = false;
			//check if the ipaddress is already in the vector
			for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
				//if client exists, break out of loop
				if (it->address == 	sock.GetLastClientIPAddr()){
					bClientExists = true;
					break;
				}
			}
			//if they dont exist, add them to the vector and send confirmation message
			if(bClientExists == false){
				Client c = Client(message.substr(11, message.length()),sock.GetLastClientIPAddr(), sock.GetLastClientSockaddr());
				clients.push_back(c);
				sock.Sendmessage("",c.sockAddr);
				for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
					sock.Sendmessage(c.name + " Has joined the server.", it->sockAddr);
				}
			}
		}
		//this code is for when a client is leaving
		else if(message == "!LEAVINGCLIENT!"){
			for(unsigned i = 0; i <= clients.size();++i) {
				if (clients[i].address == sock.GetLastClientIPAddr()){
					for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
					sock.Sendmessage(clients[i].name + " Has left the server.", it->sockAddr);
					}
					clients.erase(clients.begin() + i);
					break;
				}
			}
			
		}
		else if(message == "!TIMEOUT!"){
			cout << message << endl;
		}
		//any other messages get sent to all other clients (not sending the message back to the client who sent it)
		else{
			cout << message << endl;
			string sendytemp = sock.GetLastClientIPAddr();
			for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
				if (it->address != sendytemp){
					sock.Sendmessage(message,it->sockAddr);
				}
			}
		}
	}
}

int main(){

	UDPSocket socket = UDPSocket(false);// set UDPSocket to false to indicate this is a server
	
	string address = "127.0.0.1";
	if (!socket.OpenConnection(address.c_str(), 49153)){
		cout << "An error has occured"<<endl;
		return 0;
	}
	cout << "Server has started" << endl;
	cout << "Type !quit to shut down the server"<<endl;
	//start a listening thread here that will listen to the socket for clients
	thread t( Listen,socket );
	string line;
	//get the server commands
		//meant to add more but only got the quit command in.
	while(getline(cin,line)) {
		if (line.substr(0,5) == "!quit"){
			done = true;
			for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
				socket.Sendmessage("!SERVERSHUTDOWN!", it->sockAddr);	
			}
			t.join();
			break;
		}
	}


	return 0;

	
	
	

}