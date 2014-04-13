#include <SocketLib.hpp>
//
using namespace std;
//
int main(){
//	
	//UDPSocket Sock = new UDPSocket(0) ;
	UDPSocket SICK2 =  UDPSocket(false);
//	
	string address = "127.0.0.1";
	if (!SICK2.OpenConnection(address.c_str(), 49153)){
	//error handling stuff here!!!!
	}


	

	
//	
}