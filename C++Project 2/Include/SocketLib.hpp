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
#include <memory>
#pragma comment (lib,"ws2_32.lib")

class MySocket{
	friend class TCPSocket;
	friend class UDPSocket;
public: 

	MySocket(){//have this take in something to indicate whether its UDP or TCP. 
				//ALSO maybe something to indicate whether its a client or server(maybe)
	}

	~MySocket(){
		// terminate
		closesocket(hSocket);
		WSACleanup();}

private:
	WSAData wsaData;
	SOCKET hSocket;
	bool isClient;
	sockaddr_in service;
};

class TCPSocket 
{
	class MySocket2; //forward declaration
	std::unique_ptr<MySocket2> Sockt_;
public:
	TCPSocket(boolean);
	~TCPSocket();
	boolean OpenConnection( const char * address, int port );
};

class TCPSocket::MySocket2{
	WSAData wsaData;
	SOCKET hSocket;
	bool isClient;
	sockaddr_in service;
public:
	MySocket2(boolean client){isClient = client;}//WARNING HERE: 'boolean' : forcing value to bool 'true' or 'false' (performance warning)

	virtual ~MySocket2(){}
	boolean OpenConnection(const char * , int );

};


//NOT SURE WHAT THESE LINES DO....
TCPSocket::TCPSocket(boolean c) : Sockt_( new TCPSocket::MySocket2(c) ) { }//WARNING HERE: 'boolean' : forcing value to bool 'true' or 'false' (performance warning)
TCPSocket::~TCPSocket(){}

class UDPSocket{ 
	//IMPLEMENTING PIMPL
	class MySocket2; //forward declaration
	std::unique_ptr<MySocket2> Sockt_;
public:
	UDPSocket(boolean);
	virtual ~UDPSocket();// terminate
		//closesocket(hSocket);
		//WSACleanup();
	
	boolean OpenConnection(const char * , int );
	void Sendmessage(std::string);
	void Sendmessage(std::string, const char *, int);
};

class UDPSocket::MySocket2{
	WSAData wsaData;
	SOCKET hSocket;
	bool isClient;
	sockaddr_in service;
public:
	MySocket2(boolean client){isClient = client;}//WARNING HERE: 'boolean' : forcing value to bool 'true' or 'false' (performance warning)
	~MySocket2(){}
	boolean OpenConnection(const char * , int );
	void Sendmessage(std::string);
	void Sendmessage(std::string, const char *, int);

};

//NOT SURE WHAT THESE LINES DO....
UDPSocket::UDPSocket(boolean c) : Sockt_( new UDPSocket::MySocket2(c) ) { }
UDPSocket::~UDPSocket(){}


#endif