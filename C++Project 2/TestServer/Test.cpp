#include <SocketLib.hpp>
#include <iostream>
#include <vector>
using namespace std;

int main(){
	vector<sockaddr> socks;
	UDPSocket SICK2 =  UDPSocket(false);
	
	string address = "127.0.0.1";
	if (!SICK2.OpenConnection(address.c_str(), 49153)){
		//error handling stuff here!!!!
	}
	else
		cout << "Connection established"<<endl;

		cout << "Looping until '!quit' is recieved"<<endl;
	for (;;){
		
		//int n = recvfrom(hSocket,msg,MAX_LINE,0,&clientAddress,&cbClientAddress);
		string butts = SICK2.RecieveMessage();
		//const char * msg = butts.c_str();
		//char  msg2[256];
		//strncpy(msg2,msg,256);
		//msg2[0] = toupper(msg[0]);
		//msg2[min(0,255)]=0;
		const char * som = SICK2.GetLastClientAddr().sa_data;
		bool test = false;
		for (unsigned i=0; i < socks.size(); i++) {
			if (som == socks[i].sa_data)//this doesnt work for some reason?//How can I compare these?//overload?
				test = true;
		}
		
		if (test == false){
		socks.push_back(SICK2.GetLastClientAddr());
		}
		cout << "Recv: " << butts <<" From: " << som << endl;
		
		
		if(!strcmp(butts.c_str(),"!quit")) {
			string const terminateMsg = "server exit";
			//sendto(hSocket,terminateMsg.c_str(), terminateMsg.size(), 0, &clientAddress, cbClientAddress);
			SICK2.Sendmessage(terminateMsg,socks[1]);
			SICK2.Sendmessage("bye",socks[0]);//THIS CLIENT ISNT WAITING TO RECIEVE A MESSAGE SO IT WILL NEVER GET IT
			//ONLY WAY TO ALWAYS BE LISTENING IS WITH THREADS


			break;
		}

		//sendto(hSocket,msg,n,0,&clientAddress,cbClientAddress);
		SICK2.Sendmessage(butts);
	}
	
	
	

}