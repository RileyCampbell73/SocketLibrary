#include "SocketLib.hpp"

boolean UDPSocket::OpenConnection(const char * address, int port){
	return Sockt_ ->OpenConnection (address, port);
}

void UDPSocket::Sendmessage(std::string message){
	Sockt_->Sendmessage(message);
}

//void UDPSocket::Sendmessage(std::string message, const char * address, int port){


	////sockaddr_in clientAddress = { 0 };
	////clientAddress.sin_family = AF_INET;
	////clientAddress.sin_port = htons(port);

	////clientAddress.sin_addr.s_addr= htonl(address);

	//sendto(hSocket, message.c_str(), message.size(), 0, 
	//	(sockaddr*)&service, 
	//	sizeof(service)
	//	);
//}

boolean UDPSocket::MySocket2::OpenConnection(const char * address, int port){
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

void UDPSocket::MySocket2::Sendmessage(std::string message){
	sendto(hSocket, message.c_str(), message.size(), 0, 
			(sockaddr*)&service, 
			sizeof(service)
			);
}
