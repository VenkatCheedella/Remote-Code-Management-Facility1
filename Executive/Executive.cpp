////////////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Start point for the server			                          //
// v 1.1                                                                          //
// Application: CSE687 - Object Oriented Design- Remote Code Management Facility  //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013                        // 
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity                   //
//              vcheedel@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////


/*
Package Operation
==================

Server provides services to client for following services.

a) Upload
b) Downlaod
c) File Search
d) Text Search

Once the server is up and running, it listens for a request  from a client. It process the request and respond to the client.
Server can provode service to multiple requests at a given instance.
Using windows sockets, we can communicate to other systems in the network. Executive is the starting point to
get the port number, IP address to listen to  a request from the server. It makes the call to socket connector to 
listen on a port. Once the request is received, the requests operation is performed, the process is initiated at the 
executive. Once the request is processed, a new thread is spun to respond to the client for the request.
It gives an option to user to shut down the user.

Public Interfaces
=================

void initiatePrcess();											--> make a request to listen, process the request and spun a thread to enque the responses
void initSendResp(BlockingQueue<string>& bq);					--> Spun a thread to send response
void setUserInter(bool _userinter){ userinter = _userinter; }	--> To shut the listen thread.


Required Files
==============

BlockingQueue.h, BlockingQueue.cpp, AppHelpers.h, AppHelpers.cpp, FileSystem.h, FileSystem.cpp, 
MessageOps.h, MessageOps.cpp, receiver.h, receiver.cpp, Sender.cpp, Sender,h, Socket.h, Socket.cpp
TestCatalogue.cpp, TestCatalogue.h, XmlDocument.h, XmlDocument.cpp, XmlElement.h, XmlElement.cpp
TextSearch.cpp, TextSearch.h, XmlReader.h, XmlReader.cpp, ApplicationHelpers.cpp, ApplicationHelpers.h
WindowsHelpers.h, WindowsHelpers.cpp

Build Process
==============

From the Visual Studio Developer's Command Prompt:
devenv Sockets-Server.sln /rebuild debug

Maintenance History
===================

ver 1.0 : 13th Apr 2015
- First Release
ver 1.1 : 28th Apr 2015
- Introduces new functionalties, able to provide services like filedownload, textSearch, file search.


*/

#include <iostream>
#include "../Receiver/receiver.h"
#include "../Sender/Sender.h"
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../FileSystemDemo/FileSystem.h"
#include <string>
#include <thread>

using namespace std;
using namespace ApplicationHelpers;

//----------------<Executive class declaration>--------------------------

class Executive
{
public:
	Executive(int argc, char* argv[]);
	void initiatePrcess();
	void initSendResp(BlockingQueue<string>& bq);
	void setUserInter(bool _userinter){ userinter = _userinter; }	
private:
	int listportNumber;
	string listIPaddress;
	bool userinter = false;
};

//---------------------------<Promotion constructor>-------------------------

inline Executive::Executive(int argc, char* argv[]) : userinter(false)
{
	if (argc >= 3)
	{
		listIPaddress = argv[1];
		ApplicationHelpers::putLine("Server IP address : " + listIPaddress);
		listportNumber = std::stoi(argv[2]);
		ApplicationHelpers::putLine("Server Listen to port : " + std::to_string(listportNumber) + "\n\n");
	}
	else
		ApplicationHelpers::putLine("Please enter listen port number and IP address");
}

//-------------------------<Initiate connection, listner>-----------------------------------

inline void Executive::initiatePrcess()
{
	BlockingQueue<string> bq;
	BlockingQueue<string> sendBq;
	SocketSystem ssl;
	SocketListener sl(listportNumber, Socket::IP6);

	std::thread rcvThread(
	[&]()
	{
		receiver rcv(listportNumber, listIPaddress, bq);
		rcv.upAndRunning(sl);		
	}
	);
	rcvThread.detach();
	std::thread collectAckThread(
		[&]()
	{
		while (true)
		{
			if (bq.size() > 0)
			{
				string ack = bq.deQ();
				sendBq.enQ(ack);
			}			
			if (userinter)
				break;
		}
	}
	);
	collectAckThread.detach();
	std::thread initSendRespThr(
	[&]()
	{
		initSendResp(sendBq);
	}
	);
	initSendRespThr.detach();	
	std::cout.flush();
	char quit = std::cin.get();
	if (quit == '\n')
		setUserInter(true);
	sl.stop();
}

//----------------------<Send a response to client after the request is processed>-----------------------------------

inline void Executive::initSendResp(BlockingQueue<string>& bq)
{
	while (true)
	{
		if (userinter)
			break;
		if (bq.size() <= 0)
			continue;
		string tempack1 = bq.deQ();		
		Sender snd(tempack1);
		string tempack = tempack1;
		int pos = tempack.find_first_of("&%^!");
		int len = pos;
		string operation1 = tempack.substr(0, pos);
		tempack = tempack.substr(pos + 4);
		pos = tempack.find_first_of("&%^!");
		int portNumber = std::stoi(tempack.substr(0, pos));
		tempack = tempack.substr(pos + 4);
		pos = tempack.find_first_of("&%^!");
		string IPaddress = tempack.substr(0, pos);
			snd.createConnection(IPaddress, portNumber);		
	}
}

//---------------<Cosmetic struct>---------------------------

struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press key to exit: ";
		std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

//---------------------<main to start the application>----------------------------

int main(int argc, char* argv[])
{
	Executive exe(argc, argv);
	exe.initiatePrcess();	
}