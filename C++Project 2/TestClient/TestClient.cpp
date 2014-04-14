#include <SocketLib.hpp>
#include <iostream>
using namespace std;

int main(){
	UDPSocket SOCK = UDPSocket(true);
	
	string address = "127.0.0.1";
	if (!SOCK.OpenConnection(address.c_str(), 49153)){
	//error handling stuff here!!!!
	}

	cout << "Connection established"<<endl;

	//SOCK.Sendmessage("Hey!");
	//cout << SOCK.RecieveMessage() << endl;

	string line;
	while(getline(cin,line)) {
		SOCK.Sendmessage(line);
	cout << SOCK.RecieveMessage() << endl;

	}
	int sexy = 0;
}