<<<<<<< HEAD
=======
// File:  SocketLib.hpp
// Purpose: Hold all of the forward declaration of out library 
// Authors:  James Haig, Riley Campbell
// Date:  April 21, 2014
>>>>>>> Riley-Branch
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
<<<<<<< HEAD
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




=======
#include <memory>
#include <map>
#pragma comment (lib,"ws2_32.lib")


// this is the abstraction that the client will actually use
class TCPSocket 
{
 class MySocket; //forward declaration of inner class
 std::shared_ptr<MySocket> Sockt_;
public:
 TCPSocket(bool c);
 virtual ~TCPSocket();
 int openConnection( const char * address, USHORT port );
 void listenForConnections( int maxConnections );
 int acceptConnection();
 int sendMessage( std::string message );
 int sendMessage( std::string message, int id );
 std::string receiveMessage();
};

// this inner class actually holds the data and function implementation, hides the handles from the user
class TCPSocket::MySocket{
 WSAData wsaData;
 SOCKET hSocket;
 bool isClient;
 sockaddr_in service;
 std::map<int, SOCKET> clientData; // holds the clients' handles

public:
 MySocket(bool client){isClient = client;}

 virtual ~MySocket()
 {
  closesocket(hSocket);
  if( !isClient )
  {
   for( auto it = clientData.begin(); it != clientData.end(); ++it )
    closesocket(it->second);
  }
  WSACleanup();
 }
 int openConnection( const char * , USHORT );
 void listenForConnections( int maxConnections );
 int acceptConnection();

 int sendMessage( std::string );
 int sendMessage( std::string message, int id );
 std::string receiveMessage();

};

class UDPSocket{ 
	//IMPLEMENTING PIMPL
	class MySocket; //forward declaration
	std::shared_ptr<MySocket> Sockt_;
	
public:	
	UDPSocket(bool);
	virtual ~UDPSocket();

	bool OpenConnection(const char * , USHORT );
	void Sendmessage(std::string);
	void Sendmessage(std::string, sockaddr);
	std::string RecieveMessage();
	std::string RecieveMessage(int);
	//this will give the users a way to get the client address to the server
	sockaddr GetLastClientSockaddr();
	std::string GetLastClientIPAddr();
};

class UDPSocket::MySocket{
	WSAData wsaData;
	SOCKET hSocket;
	bool isClient;
	sockaddr_in service;
	sockaddr client;
public:
	MySocket(bool client){isClient = client;}
	~MySocket(){
	closesocket(hSocket);
	WSACleanup();
	}
	bool OpenConnection(const char * , USHORT );
	void Sendmessage(std::string);
	void Sendmessage(std::string,sockaddr);
	std::string RecieveMessage();
	std::string RecieveMessage(int);
	//this will give the users a way to get the client address to the server
	sockaddr GetLastClientSockaddr();
	std::string GetLastClientIPAddr();

};

>>>>>>> Riley-Branch
#endif