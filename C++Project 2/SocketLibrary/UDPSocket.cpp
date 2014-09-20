// File:  UDPSocket.cpp
// Purpose:  This is the implementation of the UDPSocket class.  It contains both the abstraction and implementation code.  The socket handles and guts of our implementation will be hidden from the user by the use of this bridge
// Authors:  James Haig, Riley Campbell
// Date:  April 21, 2014
#include "SocketLib.hpp"

//constructor
UDPSocket::UDPSocket(bool c) : Sockt_( new UDPSocket::MySocket(c) ) { }
//deconstructor
UDPSocket::~UDPSocket(){}

//returns the last clients IP address
std::string UDPSocket::GetLastClientIPAddr(){
	return Sockt_->GetLastClientIPAddr();
}

//returns the last clients IP address
std::string UDPSocket::MySocket::GetLastClientIPAddr(){
	char ipAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, (client.sa_data), ipAddress, INET_ADDRSTRLEN);
	return (std::string)ipAddress;
}

//this will give the users a way to get the client address to the server
sockaddr UDPSocket::GetLastClientSockaddr(){
	return Sockt_-> GetLastClientSockaddr();
}

//this will give the users a way to get the client address to the server
sockaddr UDPSocket::MySocket::GetLastClientSockaddr(){
	return client;
}

//recieves messages from the socket
std::string UDPSocket::RecieveMessage(){
	return Sockt_->RecieveMessage();
}
//recieves messages from the socket
std::string UDPSocket::MySocket::RecieveMessage(){

	if (isClient == true){
		int const MAX_LINE = 256;
		char msg[MAX_LINE];
		recvfrom(hSocket,msg,MAX_LINE,0,NULL,NULL);
		return msg;
	}
	else{
		int const MAX_LINE = 256;
		char msg[MAX_LINE];
		socklen_t cbClientAddress = sizeof(client);
		recvfrom(hSocket,msg,MAX_LINE,0,(sockaddr*)&client,&cbClientAddress);
		return msg;
	}

}
//recieves messages from the socket. Will timeout after the alloted seconds
std::string UDPSocket::RecieveMessage(int timeOut){
	return Sockt_->RecieveMessage(timeOut);
}
//recieves messages from the socket. Will timeout after the alloted seconds
std::string UDPSocket::MySocket::RecieveMessage(int timeOut){
	
	fd_set fds ;
	int n ;
	struct timeval tv ;

	// Set up the file descriptor set.
	FD_ZERO(&fds) ;
	FD_SET(hSocket, &fds) ;

	// Set up the struct timeval for the timeout.
	tv.tv_sec = timeOut ;
	tv.tv_usec = 0 ;

	// Wait until timeout or data received.
	n = select ( (int)hSocket, &fds, NULL, NULL, &tv ) ;
	if ( n == 0)
	{ 
	  return"!TIMEOUT!";
	  //return 0 ;
	}
	else if( n == -1 )
	{
	  return "Error..\n";
	 // return 1;   

	}

	if (isClient == true){
		int const MAX_LINE = 256;
		char msg[MAX_LINE];
		recvfrom(hSocket,msg,MAX_LINE,0,NULL,NULL);
		return msg;
	}
	else{
		int const MAX_LINE = 256;
		char msg[MAX_LINE];
		socklen_t cbClientAddress = sizeof(client);
		recvfrom(hSocket,msg,MAX_LINE,0,(sockaddr*)&client,&cbClientAddress);
		return msg;
	}
}

//sends a message to a specific socket address
void UDPSocket::Sendmessage(std::string message, sockaddr reciever){
	Sockt_->Sendmessage(message, reciever);
}

//sends a message to a specific socket address
void UDPSocket::MySocket::Sendmessage(std::string message, sockaddr reciever){

	sendto(hSocket, message.c_str(), (int)message.size() + 1, 0, 
		(sockaddr*)&reciever, 
		sizeof(reciever)
		);
}

//opens the connection to the socket
bool UDPSocket::OpenConnection(const char * address, USHORT port){
	return Sockt_ ->OpenConnection (address, port);
}

//opens the connection to the socket
bool UDPSocket::MySocket::OpenConnection(const char * address, USHORT port){
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

	if (isClient == false){//bind service to socket
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

//send a message
void UDPSocket::Sendmessage(std::string message){
	Sockt_->Sendmessage(message);
}

//send a message
void UDPSocket::MySocket::Sendmessage(std::string message){

	if (isClient==true){

	sendto(hSocket, message.c_str(), (int)message.size() + 1, 0, 
			(sockaddr*)&service, 
			sizeof(service)
			);

	}
	else{
		sendto(hSocket, message.c_str(), (int)message.size() + 1, 0, 
			(sockaddr*)&client, 
			sizeof(client)
			);
	}
}
