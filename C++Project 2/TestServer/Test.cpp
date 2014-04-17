#include <SocketLib.hpp>
#include <iostream>
#include <list>
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
	string message = sock.RecieveMessage(60);//THE '60' is the amount of seconds it will wait until a timeout. Timeout put in place so server can shut down even if no clients present
		//depending on message, send it to other clients
		//Listen for specific message ( differentiate from normal messages somehow)
		if(message.substr(0, 11) == "!NEWCLIENT!"){//CHANGE THIS. FIND A BETTER WAY THAT ISNT DUPLICATABLE
			bool bClientExists = false;
			for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
				if (it->address == 	sock.GetLastClientIPAddr()){//ALSO CHECK IF THE IPADDDRESS IS IN THE VECTOR
					bClientExists = true;
					break;
				}
			}
			if(bClientExists == false){
				Client c = Client(message.substr(11, message.length()),sock.GetLastClientIPAddr(), sock.GetLastClientSockaddr());
				clients.push_back(c);
			}
		}//any other messages get sent to all other clients (not sending the message back to the client who sent it)
		else if(message == "!LEAVINGCLIENT!"){//CHANGE THIS. FIND A BETTER WAY THAT ISNT DUPLICATABLE 
			for(unsigned i = 0; i <= clients.size();++i) {
				if (clients[i].address == sock.GetLastClientIPAddr()){
					sock.Sendmessage("", clients[i].sockAddr);
					clients.erase(clients.begin() + i);
					break;
				}
			}
		}
		else if(message == "!TIMEOUT!"){
			cout << message << endl;
		}
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
		//error handling stuff here!!!!
	}
	cout << "Server has started" << endl;
	
	
	//start a listening thread here that will listen to the socket for clients
		//Dont use a thread to listen. Spawn a thread when a message is recieved.
	thread t( Listen,socket );
	string line;
	while(getline(cin,line)) {
		if (line.substr(0,5) == "!quit"){
			done = true;
			t.join();//Problem with this is it still is waiting to recieve a message
			break;
		}
	}
	

	//start a listening thread here that will listen to the socket for clients
	

	


	return 0;

	
	
	

}