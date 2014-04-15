#include <SocketLib.hpp>
#include <iostream>
#include <thread>
using namespace std;

void Listen(UDPSocket &sock){

	for(;;){
	string message = sock.RecieveMessage();
	//wait for unregistration message then end loop
		//else
	cout<< message<<endl;
	}
}

int main(){


	//open socket
	UDPSocket socket = UDPSocket(true);// set UDPSocket to false to indicate this is a server
	string name;
	//IN THE FUTURE ADD THE ABILITY FOR USER TO ENTER AN IP ADDRESS AND PORT
		//WILL HAVE TO VERIFY ADDRESS AND PORT #
	string address = "127.0.0.1";
	if (!socket.OpenConnection(address.c_str(), 49153)){
		//error handling stuff here!!!!
	}
	else{
		//after opening, send message to server (something different from other messages) to register this client
			//maybe ask user for a username to use for messages
		cout <<"Welcome to the chat room. What would you like your username to be? " <<endl;
		
		getline(cin,name);
		socket.Sendmessage("NEWCLIENTNAMEIS" + name);
		cout << " you are now connected as " << name<<"."<<endl;
	}
	//meanwhile, have a seperate thread to listen for messages then display them.
	thread t( Listen,socket );
	//t.join();
	string line;
	//then use getlineto send messages
	while(getline(cin,line)) {
		socket.Sendmessage(name + " : " +line);	
	//until the user wants to quit. 
	// on cleanup, send another message to server to unregister this client
	}
	t.join();
	



	return 0;

}