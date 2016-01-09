///////////////////////////////////////////////////////////////////////////////////
// receiver.cpp - Receive the message and perform the requested service		     //
// v 1.1                                                                         //
// Application: CSE687 - Object Oriented Design- Remote Code Management Facility //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013                       //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity                  //
//              vcheedel@syr.edu                                                 //
///////////////////////////////////////////////////////////////////////////////////

#include "receiver.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include <iostream>
#include <fstream>



//-------------------------------------<Promotion constructor>-------------------------------------------

receiver::receiver(int portNumber, string IPaddress, BlockingQueue<string>& bq) : listportNumber(portNumber), listIPaddress(IPaddress), _bq(bq)
{
}

//-------------------------------<Default destrutor>-----------------------------

receiver::~receiver()
{
}


//-----------------------<Initiliase listner to listen for a request>------------------------

void receiver::upAndRunning(SocketListener& sl)
{
	ApplicationHelpers::Verbose vl(true);	
	clientHandler ch(_bq, sl);
	sl.start(ch);	
	std::cout.flush();
	std::cin.get();
}



//---------------<File upload at server>-----------------------

void clientHandler::enQueBinaryMsgs(MessageOps& msgops, Socket& socket_)
{
	size_t bytesRecvd = 0, bytesLeft = 4096;	
	string absolutefilename =  msgops.GetFileName();
	FileSystem::File writefile(absolutefilename);
	writefile.open(FileSystem::File::out, FileSystem::File::binary);
	while (true)
	{
		char* pBuf = new char[4096];
		for (int i = 0; i < 4096; i++)
			pBuf[i] = '\0';
		bytesRecvd = ::recv(socket_, pBuf, bytesLeft, 0);
		writefile.putBuffer(bytesRecvd, pBuf);
		if (bytesRecvd < 4096)
		{
			ApplicationHelpers::putLine("File uploaded at server!");
			break;
		}
	}
	writefile.close();
}

//----------------<Upload text file from client>--------------------------

void clientHandler::enQueTextMsgs(MessageOps& msgops, Socket& socket_)
{
	size_t bytesRecvd = 0, bytesLeft = 4096;
	ofstream ofs(msgops.GetFileName(), std::ofstream::out | std::ofstream::app);	// Process if it is a text file
	while (true)
	{
		char* pBuf = new char[4096];
		for (int i = 0; i < 4096; i++)
			pBuf[i] = '\0';
		bytesRecvd = ::recv(socket_, pBuf, bytesLeft, 0);
		ofs << pBuf;
		if (bytesRecvd < 4096)
		{
			ApplicationHelpers::putLine("File uploaded at server!");
			break;
		}
	}
	ofs.close();
}

//-----------------------<Send response to the client>------------------------------

void clientHandler::sendResponse(MessageOps& msg, string& output)
{
	string dispmsg = "response from server";
	Sender send(dispmsg);
	send.createConnection(msg, output);
}

//---------------------< Handles filesearch requests from client>---------------------

void clientHandler::fileSearchOps(MessageOps& msg)
{
	vector<string> cmdargs;
	//cout << "The path takn is " << msg.getPath()<<endl;
	cmdargs.push_back(msg.getPath());
	string patt;
	for (unsigned int i = 0; i < msg.getPatterns().size(); i++)
		patt = patt+ msg.getPatterns()[i]+ " ";
	cmdargs.push_back(patt);
	cmdargs.push_back("/s");
	cmdargs.push_back("");
	cmdargs.push_back("");
	TestCatalogue filecat(cmdargs);
	filecat.controller();
	Store store = filecat.getStore();
	/*for (auto miter = store.getFileBegin(); miter != store.getFileEnd(); miter++)
	{

		ApplicationHelpers::putLine("\n" +miter->first + ":");
		for (auto liter = miter->second.begin(); liter != miter->second.end(); liter++)
			ApplicationHelpers::putLine(*(*liter));
	}*/
	XmlProcessing::XmlDocument xmlFilenames;
	sPtr headElem = XmlProcessing::makeTaggedElement("FileSearch");
	xmlFilenames.getDocElement()->addChild(headElem);
	for (auto miter = store.getFileBegin(); miter != store.getFileEnd(); miter++)
	{
		sPtr filechildPtr = XmlProcessing::makeTaggedElement(miter->first);
		headElem->addChild(filechildPtr);
		for (auto liter = miter->second.begin(); liter != miter->second.end(); liter++)
		{
			sPtr pathchildPtr = XmlProcessing::makeTextElement(*(*liter));
			filechildPtr->addChild(pathchildPtr);
		}
	}
	string outputXml = xmlFilenames.getDocElement()->toString();	
	sendResponse(msg, outputXml);
}

//----------------------<Handle text Search request from client>----------------------

void clientHandler::textSearchOps(MessageOps& msg)
{
	vector<string> cmdargs;
	cmdargs.push_back("..");
	string patt;
	for (unsigned int i = 0; i < msg.getPatterns().size(); i++)
		patt = patt + msg.getPatterns()[i] + " ";
	cmdargs.push_back(patt);
	cmdargs.push_back("/s");
	cmdargs.push_back("");
	string text = "/f" + msg.getTextToSrch();
	cmdargs.push_back(text);
	TestCatalogue textcat(cmdargs);
	textcat.controller();
	std::vector<std::map<key, value>::iterator> fileiters = textcat.getTextSearchFiles();
	XmlProcessing::XmlDocument xmlFilenames;	
	sPtr headElem = XmlProcessing::makeTaggedElement("TextSearch");
	xmlFilenames.getDocElement()->addChild(headElem);
	for (unsigned int i = 0; i < fileiters.size(); i++)
	{
		for (auto liter = fileiters[i]->second.begin(); liter != fileiters[i]->second.end(); liter++)
		{
			sPtr fileTElem = XmlProcessing::makeTextElement(FileSystem::Path::fileSpec(**liter, fileiters[i]->first));
			headElem->addChild(fileTElem);
		}
	}
	string filenames = xmlFilenames.getDocElement()->toString();	
	sendResponse(msg, filenames);
}

//----------------------<Handles download request>------------------------

void clientHandler::downloadOps(MessageOps& msg)
{
	vector<string> cmdargs;
	cmdargs.push_back("../server/");
	string patt;
	for (unsigned int i = 0; i < msg.getPatterns().size(); i++)
		patt = patt + msg.getPatterns()[i] + " ";
	cmdargs.push_back(patt);
	cmdargs.push_back("/s");
	cmdargs.push_back("");
	cmdargs.push_back("");
	TestCatalogue filecat(cmdargs);
	filecat.controller();
	Store store = filecat.getStore();	
	string output;
	if (store.getFileBegin() != store.getFileEnd())
	{
		auto liter = store.getFileBegin()->second.begin();
		auto miter = store.getFileBegin()->first;
		output = FileSystem::Path::fileSpec(**liter, miter);
	}
	else
		output = "File doesn't exist";
	sendResponse(msg, output);
}

//------------------------------<Handle a client request>-----------------------------------------

void clientHandler::operator()(Socket& socket_)
{
	size_t bytesRecvd = 0, bytesLeft = 4096;
	char* pBuf = new char[4096];
	for (int i = 0; i < 4096; i++)
		pBuf[i] = '\0';
	bytesRecvd = ::recv(socket_, pBuf, bytesLeft, 0);
	MessageOps msgops;
	string header = pBuf;
	msgops.parseMsg(header);							// Parse the header
	ApplicationHelpers::putLine("\nConnection Accepted from client :" + msgops.getSenderIPaddr() + "," + std::to_string(msgops.getSenderPortNum()) + "\n");
	string check = msgops.getOperation();
	if (check == "upload")
	{	
		string delim = "&%^!";
		string tempheader = msgops.getOperation() + delim + std::to_string(msgops.getSenderPortNum()) + delim + msgops.getSenderIPaddr() + delim + std::to_string(msgops.getListenPortNum()) + delim + msgops.getIPaddr();
		tempheader = tempheader + delim + msgops.GetFileName() + delim + msgops.getReqStartTime() + delim + "";		
		_bq.enQ(tempheader);
		size_t locpos;
		locpos = msgops.GetFileName().find(".txt");
		int count = 0;
		if (locpos == std::string::npos)							// Process if it is a binary file		
			enQueBinaryMsgs(msgops, socket_);
		else
			enQueTextMsgs(msgops, socket_);							//process if it is text file		
		string currentDir = FileSystem::Directory::getCurrentDirectory();
		currentDir = FileSystem::Path::fileSpec(currentDir, msgops.GetFileName());
		ApplicationHelpers::putLine("\n File can be found at : " + FileSystem::Path::getFullFileSpec(currentDir) + "\n");	
	}	
	else
	{
		if (check == "filesearch")
			fileSearchOps(msgops);
		else if (check == "textsearch")
			textSearchOps(msgops);
		else
			downloadOps(msgops);
	}
	Verbose::show("ClientHandler socket connection closing");
	socket_.shutDown();
	socket_.close();
	Verbose::show("ClientHandler thread terminating");
}


//----------------------<Test stub>----------------------------

#ifdef TEST_RECEIVER

int main()
{
	BlockingQueue<string> bq1;
	receiver rcv(1080, "localhost", bq1);
	SocketSystem ssl;
	SocketListener sl(1090, Socket::IP6);
	rcv.upAndRunning(sl);
	std::cout << "Receive is completed" << endl;
}

#endif