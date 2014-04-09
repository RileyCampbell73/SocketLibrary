#include "SocketLib.hpp"

class Socket{
private:
	WSAData wsaData;
	SOCKET hSocket;
public: 

	Socket(){//have this take in something to indicate whether its UDP or TCP. 
				//ALSO maybe something to indicate whether its a client or server(maybe)
	}

	~Socket(){
		// terminate
		closesocket(hSocket);
		WSACleanup();
	}

	boolean OpenConnection(const char * , int );

	// ARE the methods for the socket library going to be contained within the object or outside? 
		//Is it a library if all it contains is a class?
};

	//FOR UDP SO FAR
	boolean Socket::OpenConnection(const char * address, int port){

		int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
		if(iResult!=0) {
		//cerr<<"WSAStartup failed: " << iResult << endl;
		//return EXIT_FAILURE;
		return false;
		}
		else{
			hSocket = socket( AF_INET, SOCK_DGRAM, 0 );
			// Create the server address
			sockaddr_in serverAddress = { 0 };
			serverAddress.sin_family = AF_INET;
			serverAddress.sin_port = htons(port);
			serverAddress.sin_addr.s_addr	= inet_addr(address);
				return true;
			}

		//THIS CODE IS FOR SERVER ONLY 
		// wait for a message
		//int res = bind(hSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
		//if(res==SOCKET_ERROR) {
		//	cout << "Error on bind\n";
		//	res = WSAGetLastError();
		//	cout << "Result: "<< res<< endl;
		//	closesocket(hSocket);
		//	return EXIT_FAILURE;
		//}
	}
	