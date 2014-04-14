#include "SocketLib.hpp"

//constructor
UDPSocket::UDPSocket(bool c) : Sockt_( new UDPSocket::MySocket(c) ) { }
//deconstructor
UDPSocket::~UDPSocket(){}

//this will give the users a way to get the client address to the server
sockaddr UDPSocket::GetLastClientAddr(){
	return Sockt_-> GetLastClientAddr();
}
//this will give the users a way to get the client address to the server
sockaddr UDPSocket::MySocket::GetLastClientAddr(){
	return client;
}

std::string UDPSocket::RecieveMessage(){
	
	return Sockt_->RecieveMessage();

}

std::string UDPSocket::MySocket::RecieveMessage(){

	if (isClient == true){
		int const MAX_LINE = 256;
		char msg[MAX_LINE];
		socklen_t cbServiceAddress = sizeof(service);
		int n = recvfrom(hSocket,msg,MAX_LINE,0,NULL,NULL);
		return msg;
	}
	else{
		int const MAX_LINE = 256;
		char msg[MAX_LINE];
		socklen_t cbClientAddress = sizeof(client);
		int n = recvfrom(hSocket,msg,MAX_LINE,0,(sockaddr*)&client,&cbClientAddress);
		return msg;
	}

}



bool UDPSocket::OpenConnection(const char * address, int port){
	return Sockt_ ->OpenConnection (address, port);
}

void UDPSocket::Sendmessage(std::string message){
	Sockt_->Sendmessage(message);
}
void UDPSocket::Sendmessage(std::string message, sockaddr reciever){
	Sockt_->Sendmessage(message, reciever);
}
void UDPSocket::MySocket::Sendmessage(std::string message, sockaddr reciever){


	/*sockaddr_in clientAddress = { 0 };
	clientAddress.sin_family = AF_INET;
	clientAddress.sin_port = htons(port);
	clientAddress.sin_addr.s_addr= inet_addr(address);*/

	sendto(hSocket, message.c_str(), message.size() + 1, 0, 
		(sockaddr*)&reciever, 
		sizeof(reciever)
		);
}

bool UDPSocket::MySocket::OpenConnection(const char * address, int port){
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
			
			//recieve connection establishment from client
	//		socklen_t cbClientAddress = sizeof(client);
	//		int const MAX_LINE = 256;
	//		char msg[MAX_LINE];

	//		int n = recvfrom(hSocket,msg,MAX_LINE,0,&client,&cbClientAddress);
	//		msg[0] = toupper(msg[0]);
	//		msg[min(n,255)]=0;
	//		std::string butts(msg);
	//		sendto(hSocket,butts.c_str(),butts.size(),0,&client,sizeof(client));

	//}
	//else{//establishes connection with the server
	//	std::string msg = "Connection Established";
	//	sendto(hSocket,msg.c_str(),msg.size(),0,(sockaddr*)&service,sizeof(service));
	//	int const MAX_LINE = 256;
	//	char msg2[MAX_LINE];
	//	socklen_t cbServiceAddress = sizeof(service);
	//	int n = recvfrom(hSocket,msg2,MAX_LINE,0,(sockaddr*)&service,&cbServiceAddress);

	//	std::string butts2 (msg2);
	//	int sexy = 0;
	}
	return true;
}

void UDPSocket::MySocket::Sendmessage(std::string message){

	if (isClient==true){

	sendto(hSocket, message.c_str(), message.size() + 1, 0, 
			(sockaddr*)&service, 
			sizeof(service)
			);

	}
	else{
		sendto(hSocket, message.c_str(), message.size() + 1, 0, 
			(sockaddr*)&client, 
			sizeof(client)
			);
	}
}
