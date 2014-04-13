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


class TCPSocket 
{
	class MySocket; //forward declaration
	std::unique_ptr<MySocket> Sockt_;
public:
	TCPSocket(bool c);
	virtual ~TCPSocket();
	bool OpenConnection( const char * address, int port );
};

class TCPSocket::MySocket{
	WSAData wsaData;
	SOCKET hSocket;
	bool isClient;
	sockaddr_in service;
public:
	MySocket(bool client){isClient = client;}

	virtual ~MySocket(){}
	bool OpenConnection(const char * , int );

};



class UDPSocket{ 
	//IMPLEMENTING PIMPL
	class MySocket; //forward declaration
	std::unique_ptr<MySocket> Sockt_;
public:	
	UDPSocket(bool);
	virtual ~UDPSocket();// terminate
		//closesocket(hSocket);
		//WSACleanup();

	bool OpenConnection(const char * , int );
	void Sendmessage(std::string);
	void Sendmessage(std::string, const char *, int);
	std::string RecieveMessage();
	std::string RecieveMessage(sockaddr);
};

class UDPSocket::MySocket{
	WSAData wsaData;
	SOCKET hSocket;
	bool isClient;
	sockaddr_in service;
	sockaddr client;
public:
	MySocket(bool client){isClient = client;}
	~MySocket(){}
	bool OpenConnection(const char * , int );
	void Sendmessage(std::string);
	void Sendmessage(std::string, const char *, int);
	std::string RecieveMessage();
	std::string RecieveMessage(sockaddr);

};





#endif