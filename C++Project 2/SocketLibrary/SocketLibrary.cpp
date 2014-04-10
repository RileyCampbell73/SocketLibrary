#include "SocketLib.hpp"

class MySocket{
protected:
	WSAData wsaData;
	SOCKET hSocket;
	bool isClient;
	sockaddr_in service;
public: 
	MySocket(){//have this take in something to indicate whether its UDP or TCP. 
				//ALSO maybe something to indicate whether its a client or server(maybe)
	}

	~MySocket(){

	}

};

class TCPSocket : MySocket
{
public:
	TCPSocket( bool client = false )
	{
		isClient = client;
	}
};
	
class UDPSocket : MySocket{
public:

	UDPSocket( bool client): MySocket(){
		
		isClient = client;
	}

	~UDPSocket(){
		// terminate
		closesocket(hSocket);
		WSACleanup();
	}

	boolean OpenConnection(const char * , int );

	void Sendmessage(std::string);

	void Sendmessage(std::string, const char *, int);

};


boolean UDPSocket::OpenConnection(const char * address, int port){

	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
	if(iResult!=0) {
		//cerr<<"WSAStartup failed: " << iResult << endl;
		//return EXIT_FAILURE;
		return false;
	}
		hSocket = socket( AF_INET, SOCK_DGRAM, 0 );
		
	

	// Create the server address
	//service = { 0 };
	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	service.sin_addr.s_addr	= inet_addr(address);

	if (isClient == false){

			//bind service to socket
			int res = bind(hSocket, (sockaddr*)&service, sizeof(service));
			if(res==SOCKET_ERROR) {
			//	cout << "Error on bind\n";
				res = WSAGetLastError();
			//	cout << "Result: "<< res<< endl;
				closesocket(hSocket);
			//	return EXIT_FAILURE;
				return false;
			}
	}	
	return true;
}

void UDPSocket::Sendmessage(std::string message){

	
	sendto(hSocket, message.c_str(), message.size(), 0, 
			(sockaddr*)&service, 
			sizeof(service)
			);

}

void UDPSocket::Sendmessage(std::string message, const char * address, int port){


	//sockaddr_in clientAddress = { 0 };
	//clientAddress.sin_family = AF_INET;
	//clientAddress.sin_port = htons(port);

	//clientAddress.sin_addr.s_addr= htonl(address);

	sendto(hSocket, message.c_str(), message.size(), 0, 
		(sockaddr*)&service, 
		sizeof(service)
		);
}
	
