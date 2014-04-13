#include "SocketLib.hpp"

TCPSocket::TCPSocket(bool d) : Sockt_( new TCPSocket::MySocket(d) ) { }//WARNING HERE: 'boolean' : forcing value to bool 'true' or 'false' (performance warning)
TCPSocket::~TCPSocket(){}

	
bool TCPSocket::OpenConnection( const char * address, USHORT port )
{

	return Sockt_->OpenConnection(address,port);
}

bool TCPSocket::MySocket::OpenConnection( const char * address, USHORT port )
{
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != NO_ERROR)
	{
		//cerr << "WSAStartup failed!" << endl;
		return EXIT_FAILURE;
	}

	 hSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( hSocket == INVALID_SOCKET )
	{
		//cerr << "Error at socket(): " << WSAGetLastError << endl;
		//exitCode = EXIT_FAILURE;
		//goto cleanupWSA; // just jumps right to the cleanup: label
		return EXIT_FAILURE;
	}

	service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(address);
	service.sin_port = htons(port);

	if( isClient )
	{
		if(connect(hSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR)
		{
			//cerr << "Failed to connect" << endl;
			//exitCode = EXIT_FAILURE;
			//goto cleanupSocket;
			return false;
		}
	}

	// we're a client
	else 
	{	
		if(bind(hSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) // bind prepares a machine for transmission (sits on the port and "owns" it for a while, in preparation for sending data).  Makes sure the port is open and that we can use it.
		{
			//cerr << "Bind() failed" << endl;
			//goto cleanupSocket;
			return false;
		}

		if(listen(hSocket,1)==SOCKET_ERROR) // we'll only accept 1 connection for now
		{
			//cerr << "Error listening on socket" << endl;
			//goto cleanupSocket;
			return false;
		}



		//cout << "Waiting for connection" << endl;
		hAccepted = SOCKET_ERROR;
		while( hAccepted == SOCKET_ERROR )
		{
			hAccepted = accept(hSocket, NULL, NULL); // accepts a connection, will BLOCK everything else
		}

		//cout << "Client connected" << endl;
	}
	return true;
}

int TCPSocket::MySocket::SendMessage( std::string message )
{
	char sendbuf[256] = "Test";
	int byteSent = 0;
	if( isClient )
	{
		byteSent = send( hSocket, sendbuf, strlen(sendbuf)+1, 0 );
	}
	else
	{
		byteSent = send( hAccepted, sendbuf, strlen(sendbuf)+1, 0 );
	}
	return byteSent;
}

// have the user pass in the length of the recvbuf?  or restrict it to a certain amount in SendMessage and throw an error if it exceeds the length?
std::string TCPSocket::MySocket::ReceiveMessage()
{
	char recvbuf[256] = "";
	int bytesRecv = 0;

	if( isClient )
	{
		bytesRecv = recv(hSocket, recvbuf, 256, 0);
	}
	else
	{
		bytesRecv = recv(hAccepted, recvbuf, 256, 0);
	}

	return recvbuf;
}