#if !defined( GUARD_MY_LIB )
#define GUARD_MY_LIB

#if defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib, "SocketLibrary-mt-s-gd.lib")
#elif defined(_DEBUG) && defined(_DLL)
#pragma comment (lib, "SocketLibrary-mt-gd.lib")
#elif !defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib, "SocketLibrary-mt-s.lib")
#elif !defined(_DEBUG) && defined(_DLL)
#pragma comment (lib, "SocketLibrary-mt.lib")
#endif

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment (lib,"ws2_32.lib")

class MySocket{
	//friend class TCPSocket;
public: 
	

	MySocket(){//have this take in something to indicate whether its UDP or TCP. 
				//ALSO maybe something to indicate whether its a client or server(maybe)
	}

	~MySocket(){
		// terminate
		closesocket(hSocket);
		WSACleanup();
	}

protected:
	WSAData wsaData;
	SOCKET hSocket;
	bool isClient;
	sockaddr_in service;
};

class TCPSocket : public MySocket
{
	friend class MySocket;
public:
	TCPSocket( bool client = false ) : MySocket()
	{
		isClient = client;
	}

	~TCPSocket()
	{

	}

	bool OpenConnection( const char * address, int port );
};






#endif