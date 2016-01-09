#ifndef SENDER_H
#define SENDER_H

/////////////////////////////////////////////////////////////////////////////////
// Sender.h - Packets are sent to the destination port.                   	   //
// v 1.0                                                                            //
// Application: CSE687 - Object Oriented Design- Message Passing Communication //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013                     //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity                //
//              vcheedel@syr.edu                                               //
/////////////////////////////////////////////////////////////////////////////////

/*


Package Operation
==================

A connection is requested to the destination peer. After the request is accepted, the packets are sent to the
destination port.

Required Files
==============

BlockingQueue.h, BlockingQueue.cpp, AppHelpers.h, AppHelpers.cpp, FileSystem.h, FileSystem.cpp,
MessageOps.h, MessageOps.cpp,Socket.h, Socket.cpp,

Build Process
=============
From the Visual Studio Developer's Command Prompt:
devenv Sockets-Server.sln /rebuild debug


Public Interfaces
=================

void createConnection(string IPaddress, int portNumber);		--> A connection is requested to the destination peer to send a request

Maintenance History
=========================

ver 1.0  : 13 Apr 2015
- First Release


*/

#include "../MessageOperations/MessageOps.h"
#include <string>
#include "../Sockets/Sockets.h"
#include <Windows.h>      // Windnows API
#include <winsock2.h>     // Windows sockets, ver 2
#include <WS2tcpip.h>     // support for IPv6 and other things
#include <IPHlpApi.h>     // ip helpers
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Sockets/Sockets.h"

class Sender
{
public:
	Sender(string& input);	
	~Sender();
	void createConnection(string IPaddress, int portNumber);
	void createConnection(MessageOps& msg, string& output);

private:
	string& _input;
};

#endif