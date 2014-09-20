// File:  ChatClientr.cpp
// Purpose: To demonstrates the librates capabilities. This is the client half of the chat client/server application that was made.
//			The client sets up a user to connect with the chat server and can send messages to anyone else connected to the chat client.
// Authors: James Haig, Riley Campbell
// Date:  April 21, 2014
#include <SocketLib.hpp>
#include <iostream>
#include <thread>
#include <windows.h> 
using namespace std;

bool done = false;

//this method is threaded to constantly listen for messages
void Listen(UDPSocket &sock){

	try{
		//loop until program is ready to end
		while (!done){
			string message = sock.RecieveMessage();
			//if the server is shutting down
			if (message == "!SERVERSHUTDOWN!"){
				done = true;
				cout << "The server has shut down. Closing client now."<<endl;
				cout << "---------------------------------------------"<<endl;
			}
			else
				cout<< message<<endl;
			}
		}
	catch(exception ex){
		string a = ex.what();
		std::wstring b = std::wstring(a.begin(), a.end());
		LPCWSTR except = b.c_str();
		MessageBox(0,except,L"Exception Caught",MB_OK);
	}
}

int main(){

	string name;
	try{
		//open socket
		UDPSocket socket = UDPSocket(true);// set UDPSocket to true to indicate this is a client
		
		string address = "127.0.0.1";
		//open the connection
		if (!socket.OpenConnection(address.c_str(), 49153)){
			cout << "an error occured when attempting to open a connection"<<endl;
			return -1;
		}
		else{
			//after opening, send message to server (something different from other messages) to register this client
				
			cout <<"Welcome to the chat room. What would you like your username to be? " <<endl;
			cout <<"------------------------------------------------------------------" << endl;
			getline(cin,name);//ask user for a username to use for messages
			socket.Sendmessage("!NEWCLIENT!" + name);
			string message = socket.RecieveMessage(30);//timeout of 30.
			if(message == "!TIMEOUT!"){
				cout << "Connection timed out. Please check the server." << endl;
				return -1;
			}
			else
			cout << "you are now connected as " << name<<". Type '!quit' to quit the chat client."<<endl;
			cout << "----------------------------------------------------------------------------"<<endl;
		}
		//meanwhile, have a seperate thread to listen for messages then display them.
		thread t( Listen,socket );
		string line;
		//then use getline to send messages
		while(getline(cin,line)) {
			//until the user wants to quit.
			if (line.substr(0,5) == "!quit"){
				socket.Sendmessage("!LEAVINGCLIENT!");
				done = true;
				t.join();
				break;
			}
			else if(line != ""){
				socket.Sendmessage(name + " : " +line);	
			}
		}
	
	}
	catch(exception ex){
		string a = ex.what();
		std::wstring b = std::wstring(a.begin(), a.end());
		LPCWSTR except = b.c_str();
		MessageBox(0,except,L"Exception Caught",MB_OK);
	}


	return 0;

}