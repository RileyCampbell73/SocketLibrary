#include <SocketLib.hpp>
#include <iostream>
#include <vector>
#include <thread>
using namespace std;

class Client{

public:
	string name;
	sockaddr address;

	Client(string n, sockaddr a){
		name = n;
		address = a;
	}
	~Client(){}
};

vector<Client> clients;
void Listen(UDPSocket &sock){

	for (;;){
	string message = sock.RecieveMessage();
		//depending on message, send it to other clients
		//Listen for specific message ( differentiate from normal messages somehow)
		if(message.substr(0, 15) == "NEWCLIENTNAMEIS"){//CHANGE THIS. FIND A BETTER WAY THAT ISNT DUPLICATABLE
			Client c = Client(message.substr(15, message.length()),sock.GetLastClientAddr());
			clients.push_back(c);
		}
		//any other messages get sent to all other clients (not sending the message back to the client who sent it)
		else{
			cout << message << endl;
			sockaddr sendytemp = sock.GetLastClientAddr();
			char str2[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, (sendytemp.sa_data), str2, INET_ADDRSTRLEN);

			for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
				
				char str[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &(it->address.sa_data), str, INET_ADDRSTRLEN);
	
				if ((string)str != (string)str2){//Have to cast to a string. comparing chars was problematic
					sock.Sendmessage(message,it->address);
				}
			}
			
		}
	}
}

int main(){


	UDPSocket socket = UDPSocket(false);// set UDPSocket to false to indicate this is a server
	
	string address = "127.0.0.1";
	if (!socket.OpenConnection(address.c_str(), 49153)){
		//error handling stuff here!!!!
	}
	cout << "Server has started" << endl;
	
	
	//start a listening thread here that will listen to the socket for clients
		//Dont use a thread to listen. Spawn a thread when a message is recieved.
	thread t( Listen,socket );
	t.join();

	//start a listening thread here that will listen to the socket for clients
	

	


	return 0;

	
	
	

}