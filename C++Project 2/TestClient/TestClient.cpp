#include <SocketLib.hpp>

using namespace std;

int main(){
	UDPSocket SOCK = UDPSocket(true);
	
	string address = "127.0.0.1";
	if (!SOCK.OpenConnection(address.c_str(), 49153)){
	//error handling stuff here!!!!
	}

}