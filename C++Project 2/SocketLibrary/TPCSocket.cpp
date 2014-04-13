#include "SocketLib.hpp"

TCPSocket::TCPSocket(bool d) : Sockt_( new TCPSocket::MySocket(d) ) { }//WARNING HERE: 'boolean' : forcing value to bool 'true' or 'false' (performance warning)
TCPSocket::~TCPSocket(){}

bool TCPSocket::OpenConnection( const char * address, int port )
{

	return Sockt_->OpenConnection(address,port);
}

bool TCPSocket::MySocket::OpenConnection( const char * address, int port )
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

	if( !isClient )
	{
		if(bind(hSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) // bind prepares a machine for transmission (sits on the port and "owns" it for a while, in preparation for sending data).  Makes sure the port is open and that we can use it.
		{
			//cerr << "Bind() failed" << endl;
			//goto cleanupSocket;
			return false;
		}

		if(listen(hSocket,1)==SOCKET_ERROR) // sit on the socket and port and listen for an attempt to connect
			// params:  first is socket, second is backlog
				// backlog = how many attempts to connect can be held in the backlog, since we only have 1 we'll be attentive to only one and forget the rest
					// other connections will FAIL when we've got one attempt already
		{
			//cerr << "Error listening on socket" << endl;
			//goto cleanupSocket;
			return false;
		}
	}
	return true;
}