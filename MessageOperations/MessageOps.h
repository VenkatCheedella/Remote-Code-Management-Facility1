#ifndef MESSAGEOPS_H
#define MESSAGEOPS_H

////////////////////////////////////////////////////////////////////////////////////
// MessageOps.h - Message creation and message parsing		                      //
// v 1.1                                                                          //
// Application: CSE687 - Object Oriented Design- Remote Code Management Facility  //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013                        //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity                   //
//              vcheedel@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////

/*

Package Operation
====================
Client send the request similar to http messages. Using this package, the recevied message is parsed
and destination, sender details, size of the message, file operation type are retrived. Based on the parsed information
the appropriate action is performed. When a file need to be uploaded, each packet is encapsulated with header information 
and transmitted across, the package creation is performed in this package.

Required Files
==============
BlockingQueue.h, BlockingQueue.cpp, AppHelpers.h, AppHelpers.cpp, FileSystem.h, FileSystem.cpp,
MessageOps.h, MessageOps.cpp,Socket.h, Socket.cpp

Build Process
=============
From the Visual Studio Developer's Command Prompt:
devenv Sockets-Server.sln /rebuild debug

Public Interfaces
=================
string parseMsg(string& body);									--> used to parse the packet received
bool sentMsg(string& fbuf, Socket& _socket);					--> Helps to respond to the client after the operation is performed
void setSenderIPaddr(string _ipaddr) { senderIpaddr = _ipaddr; }--> set the Sender IP address
string getSenderIPaddr(){ return senderIpaddr; }				--> Get the sender IP address
void setSenderPortNum(int portnum){ senderPortNum = portnum; }	--> Set the sender port number
int getSenderPortNum(){ return senderPortNum; }					--> Get the sender port number
void setListenPortNum(int portNum){ listenPortNum = portNum; }	--> Set the listner port number
int getListenPortNum(){ return listenPortNum; }					--> Get the listner port numner
void setListenIPaddr(string IPaddr){ listenIPaddr = IPaddr; }	--> Set the listner IP address
string getIPaddr(){ return listenIPaddr; }						--> Get the listner IP address
void setFileName(string _filename){ filename = _filename; }		--> Set the filename received
string GetFileName(){ return filename; }						--> Get the filename received
void setOperation(string _operation){ operation = _operation; }	--> Set the operation requested
string getOperation(){ return operation; }						--> Get the operation received
void setPatterns(string _patterns);								--> Set the patterns to search
void setTextToSrch(string _text);								--> Set the text to search
string getTextToSrch(){ return textToSrch; }					--> Get the text to search

Maintenance History
=========================

ver 1.0  : 13 Apr 2015
- First Release
ver 1.1  : 28 Apr 2015
- Now supports to extract and encapsulate information for patterns and text search
*/


#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <winsock2.h>    
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
#include "../FileSystemDemo/FileSystem.h"

using namespace std;

class MessageOps
{
public:
	MessageOps();
	~MessageOps();
	string parseMsg(string& body);
	bool sentMsg(string& fbuf, Socket& _socket);
	bool pushBufferMsgs(BlockingQueue<pair<char*, size_t>>& sbq, string& output);
	bool pushBinaryFiles(BlockingQueue<pair<char*, size_t>>& sbq, string& output);
	bool pushTextFiles(BlockingQueue<pair<char*, size_t>>& sbq, string& output);
	void setSenderIPaddr(string _ipaddr) { senderIpaddr = _ipaddr; }
	string getSenderIPaddr(){ return senderIpaddr; }
	void setSenderPortNum(int portnum){ senderPortNum = portnum; }
	int getSenderPortNum(){ return senderPortNum; }
	void setListenPortNum(int portNum){ serverPortNum = portNum; }
	int getListenPortNum(){ return serverPortNum; }
	void setListenIPaddr(string IPaddr){ serverIPaddr = IPaddr; }
	string getIPaddr(){ return serverIPaddr; }
	void setFileName(string _filename){ filename = _filename; }
	string GetFileName(){ return filename; }
	void setOperation(string _operation){ operation = _operation; }
	string getOperation(){ return operation; }
	vector<string> getPatterns();
	void setPatterns(string _patterns);
	void setTextToSrch(string _text);
	string getTextToSrch(){ return textToSrch; }
	string getPath(){ return path; }
	long long getCurrentTime();
	void enQueuProcTime(BlockingQueue<pair<char*, size_t>>& sbq);	
	string getReqStartTime(){ return reqStrtTime; }

private:	
	string messagebody = "";
	string senderIpaddr;
	int senderPortNum;
	int serverPortNum;
	string serverIPaddr;
	string filename;
	string operation;
	vector<string> patterns;
	string textToSrch;
	string path;
	void createHeader(char* msg, string& header);	
	long long startProcTime;
	long long endProcTime;
	string reqStrtTime;
};

#endif