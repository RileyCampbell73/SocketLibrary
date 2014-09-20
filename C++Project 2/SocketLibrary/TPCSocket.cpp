// File:  TCPSocket.cpp
// Purpose:  This is the implementation of the TCPSocket class.  It contains both the abstraction and implementation code.  The socket handles and guts of our implementation will be hidden from the user by the use of this bridge
// Authors:  James Haig, Riley Campbell
// Date:  April 21, 2014
#include "SocketLib.hpp"

TCPSocket::TCPSocket(bool d) : Sockt_( new TCPSocket::MySocket(d) ) { }
TCPSocket::~TCPSocket(){ }

// Make the abstraction code only call the inner class' implementation, so the user will see none of the guts
int TCPSocket::openConnection( const char * address, USHORT port )
{

 return Sockt_->openConnection(address,port);
}

void TCPSocket::listenForConnections( int maxConnections )
{
 return Sockt_->listenForConnections( maxConnections );
}

int TCPSocket::acceptConnection()
{
 return Sockt_->acceptConnection();
}

int TCPSocket::sendMessage( std::string message )
{
 return Sockt_->sendMessage(message);
}

int TCPSocket::sendMessage( std::string message, int id )
{
 return Sockt_->sendMessage(message, id);
}

std::string TCPSocket::receiveMessage()
{
 return Sockt_->receiveMessage();
}

// opens a connection, for either the client or the server, provided an address and port
int TCPSocket::MySocket::openConnection( const char * address, USHORT port )
{
 int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
 if(iResult != NO_ERROR)
 {
  return -1;
 }

 hSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
 if( hSocket == INVALID_SOCKET )
 {
  return -1;
 }

 service.sin_family = AF_INET;
 service.sin_addr.s_addr = inet_addr(address);
 service.sin_port = htons(port);

 if( isClient )
 {
  if(connect(hSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR)
  {
   int fail = WSAGetLastError();
   return fail;
  }
 }
 else 
 { 
  if(bind(hSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
  {
   int fail = WSAGetLastError();
   return fail;
  }
 }
 return 0;
}

// For a server only, listens for a provided number of connections
void TCPSocket::MySocket::listenForConnections( int maxConnections )
{
 if( !isClient )
 {
  /////////////////time out handling////////////////////////
  fd_set fds;
  int n;
  struct timeval tv;

  // Set up the file descriptor set.
  FD_ZERO(&fds);
  FD_SET(hSocket, &fds);

  // Set up the struct timeval for the timeout.
  tv.tv_sec = 10;
  tv.tv_usec = 0;

  // Wait until timeout or data received.
  n = select ( (int)hSocket, &fds, NULL, NULL, &tv );
  //////////////////////////////////////////////////////
  if ( n != 0 && n != -1 )
  { 
   listen(hSocket,maxConnections);
  }
 }
}

// accepts a connection and feeds the socket handle into the clientData map, returns the key so the clients can be addressed individually
int TCPSocket::MySocket::acceptConnection()
{
 if( !isClient )
 {
  SOCKET hAccepted = (SOCKET)SOCKET_ERROR;

  /////////////////time out handling////////////////////////
  fd_set fds;
  int n;
  struct timeval tv;

  // Set up the file descriptor set.
  FD_ZERO(&fds);
  FD_SET(hSocket, &fds);

  // Set up the struct timeval for the timeout.
  tv.tv_sec = 10;
  tv.tv_usec = 0;

  // Wait until timeout or data received.
  n = select ( (int)hSocket, &fds, NULL, NULL, &tv );
  if ( n == 0)
  { 
   return -1;
  }
  else if( n == -1 )
  {
   return -1;
  }
  /////////////////////////////////////////////////////////////

  while( hAccepted == SOCKET_ERROR )
  {
   hAccepted = accept(hSocket, NULL, NULL);
   if( hAccepted != SOCKET_ERROR )
   {
	  int clientId = (int)clientData.size();
    clientData[clientId] = hAccepted;
    return clientId; // this will end up being the id of the client
   }
  }
 }
 return -1;
}

// Send a message to the server, or to every client
int TCPSocket::MySocket::sendMessage( std::string message )
{
 int byteSent = 0;
 if( isClient )
 {
  byteSent = send( hSocket, message.c_str(), (int)message.size(), 0 );
 }
 else
 {
  for(auto it = clientData.begin(); it != clientData.end(); ++it)
   byteSent += send( it->second, message.c_str(), (int)message.size(), 0 );
 }
 return byteSent;
}

// Send a message to the server, or to one specific client
int TCPSocket::MySocket::sendMessage( std::string message, int id )
{
 int byteSent = 0;
 if( isClient )
 {
  byteSent = send( hSocket, message.c_str(), (int)message.size(), 0 );
 }
 else
 {
  byteSent = send( clientData[id], message.c_str(), (int)message.size(), 0 );
 }
 return byteSent;
}

// receives a message from either the client or server, will timeout if one isn't received in 10 seconds
std::string TCPSocket::MySocket::receiveMessage()
{
 char recvbuf[256] = "";
 int bytesRecv = 0;

 if( isClient )
 {
  /////////////////time out handling////////////////////////
  fd_set fds;
  int n;
  struct timeval tv;

  // Set up the file descriptor set.
  FD_ZERO(&fds);
  FD_SET(hSocket, &fds);

  // Set up the struct timeval for the timeout.
  tv.tv_sec = 10;
  tv.tv_usec = 0;

  // Wait until timeout or data received.
  n = select ( (int)hSocket, &fds, NULL, NULL, &tv );
  if ( n == 0)
  { 
   return"!TIMEOUT!";
  }
  else if( n == -1 )
  {
   return "Error..\n";
  }
  ///////////////////////////////////////////////////////
  bytesRecv = recv(hSocket, recvbuf, 256, 0);
 }
 else
 {
  // check every client to see if we've received a message, timeout in 1 second before checking the next client
  for( auto it = clientData.begin(); it != clientData.end(); ++it )
  {
   /////////////////time out handling////////////////////////
   fd_set fds;
   int n;
   struct timeval tv;

   // Set up the file descriptor set.
   FD_ZERO(&fds);
   FD_SET(it->second, &fds);

   // Set up the struct timeval for the timeout.
   tv.tv_sec = 1;
   tv.tv_usec = 0;

   // Wait until timeout or data received.
   n = select ( (int)it->second, &fds, NULL, NULL, &tv );
   ///////////////////////////////////////

   if ( n != 0 && n != -1 )
   {
    bytesRecv = recv(it->second, recvbuf, 256, 0);
    break;
   }
  }
 }

 std::string retStr(recvbuf);
 return retStr;
}