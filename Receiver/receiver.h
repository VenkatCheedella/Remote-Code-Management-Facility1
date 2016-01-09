#ifndef RECEIVER_H
#define RECEIVER_H

///////////////////////////////////////////////////////////////////////////////////
// receiver.h - Receive the message and perform the requested service		     //
// v 1.1                                                                         //
// Application: CSE687 - Object Oriented Design- Remote Code Management Facility //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013                       //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity                  //
//              vcheedel@syr.edu                                                 //
///////////////////////////////////////////////////////////////////////////////////

/*

Package Operation
==================

A socket listner receives request and dedicated port is assigned(data port) to receive the packets.
using the message operations package the header is parsed and body of the message is uploaded as
the requested file is uploaded to server. The ack message to the client is pushed to the queue.

Required Files
==============

BlockingQueue.h, BlockingQueue.cpp, AppHelpers.h, AppHelpers.cpp, FileSystem.h, FileSystem.cpp,
MessageOps.h, MessageOps.cpp,Socket.h, Socket.cpp, receiver.h, receiver.cpp

Build Process
=============
From the Visual Studio Developer's Command Prompt:
devenv Sockets-Server.sln /rebuild debug

Public Interfaces
=================

void upAndRunning(SocketListener& sl);		--> A port is enabled and start listening for the requests from the clients
void operator()(Socket& socket_);			--> Handles the client request
void fileSearchOps(MessageOps& msg);						--> handles fileSearch requests
void textSearchOps(MessageOps& msg);						--> handles text search requests
void downloadOps(MessageOps& msg);							--> handles download requests
void sendResponse(MessageOps& msg, string& filename);		--> It trasfers the response to sender
void enQueBinaryMsgs(MessageOps& msgops, Socket& socket_);	--> It handles the binary file downloads
void enQueTextMsgs(MessageOps& msgops, Socket& socket_);	--> It handles the text file downloads

Maintenance History
=========================

ver 1.0  : 13 Apr 2015
- First Release
ver 2.0  : 28 Apr 2015
- It handles different request such as File download, text search , file search

*/

#include "../MessageOperations/MessageOps.h"
#include <iostream>
#include <string>
#include <thread>
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../FileMgr/FileMgr.h"
#include "../FileSystemDemo/FileSystem.h"
#include "../Store/Store.h"
#include "../TestCatalogue/TestCatalogue.h"
#include "../TextSearch/TextSearch.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include "../XmlReader/XmlReader.h"
#include "../Sender/Sender.h"


using namespace XmlProcessing;
using namespace ApplicationHelpers;
using namespace std;



class receiver
{
public:
	receiver(int portNumber, string IPaddress, BlockingQueue<string>& bq);
	void upAndRunning(SocketListener& sl);
	~receiver();
private:
	int listportNumber;
	string listIPaddress;
	BlockingQueue<string>& _bq;	
};

class clientHandler
{
	using sPtr = std::shared_ptr < AbstractXmlElement >;
public:
	clientHandler(BlockingQueue<string>& bq, SocketListener& sl) :_bq(bq), _sl(sl){}
	void operator()(Socket& socket_);		
private:
	BlockingQueue<string>& _bq;
	void fileSearchOps(MessageOps& msg);
	void textSearchOps(MessageOps& msg);
	void downloadOps(MessageOps& msg);
	void sendResponse(MessageOps& msg, string& filename);
	void enQueBinaryMsgs(MessageOps& msgops, Socket& socket_);
	void enQueTextMsgs(MessageOps& msgops, Socket& socket_);

	SocketListener& _sl;
	string output;
};

#endif