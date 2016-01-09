////////////////////////////////////////////////////////////////////////////////////
// MessageOps.cpp - Message creation and message parsing		                      //
// v 1.1                                                                          //
// Application: CSE687 - Object Oriented Design- Remote Code Management Facility  //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013                        //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity                   //
//              vcheedel@syr.edu                                                  //
////////////////////////////////////////////////////////////////////////////////////

#include "MessageOps.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <chrono>
#include <ctime>

//using sbyte = char;
using namespace std;
using namespace std::chrono;

//-------------<Default constructor>-------------------

MessageOps::MessageOps()
{
}

//-------------<Default Destructor>---------------------

MessageOps::~MessageOps()
{
}

//---------------<Set patterns>-------------------------

void MessageOps::setPatterns(string _patterns)
{
	size_t  pos;
	while (true)
	{
		pos = _patterns.find("!^%");
		if (pos == std::string::npos)
		{
			patterns.push_back(_patterns);
			break;
		}			
		patterns.push_back(_patterns.substr(0, pos));
		_patterns = _patterns.substr(pos + 3);
	}
}

//-------------------------<return list of patterns>---------------------

vector<string> MessageOps::getPatterns()
{
	return patterns;
}

//-----------------------<Create messages to respond to the client>-----------------

bool MessageOps::pushBufferMsgs(BlockingQueue<pair<char*, size_t>>& sbq, string& output)
{
	string delim = "&%^!";
	string header;
	string patterns = "";
	string tempoutput;
	if (operation == "download")
		filename = output.substr(output.find_last_of("\\") + 1);			
	header = operation + delim + std::to_string(serverPortNum) + delim + senderIpaddr + delim + std::to_string(senderPortNum) + delim + senderIpaddr;
	header = header + delim + filename + delim + reqStrtTime + delim;
	startProcTime = getCurrentTime();
	auto iter = header.begin();
	bool pushstatus = false;
	char* pBuf = new char[4096];
	for (int i = 0; i < 4096; i++)
		pBuf[i] = '\0';
	pair<char*, size_t> msgpair;
	while (true)
	{
		for (unsigned int i = 0; i < header.size(); i++)
		{
			pBuf[i] = *iter;
			iter++;
		}
		if (iter == header.end())
			break;	
	}
	for (unsigned int i = header.size(); i < 4096; i++)
		pBuf[i] = 'x';
	msgpair.first = pBuf;
	msgpair.second = 4096;
	sbq.enQ(msgpair);
	if (operation == "download")
	{
		size_t pos = filename.find(".txt");		
		if (pos == std::string::npos)
			pushstatus = pushBinaryFiles(sbq, output);
		else
			pushstatus = pushTextFiles(sbq, output);
	}
	else
		pushstatus = pushTextFiles(sbq, output);
	return pushstatus;
}

//--------------------------<Create messages if the response file is a binary>--------------

bool MessageOps::pushBinaryFiles(BlockingQueue<pair<char*, size_t>>& sbq, string& output)
{
	
	bool pushstatus = false;
	try
	{
		pair<char*, size_t> msgpair;
		size_t bufsize;
		FileSystem::File sendfile(output);
		sendfile.open(FileSystem::File::in, FileSystem::File::binary);
		while (true)
		{
			char* pBuf = new char[4096];
			bufsize = sendfile.getBuffer(4096, pBuf);
			msgpair.first = pBuf;
			msgpair.second = bufsize;
			sbq.enQ(msgpair);
			if (bufsize != 4096)
			{
				pushstatus = true;
				break;
			}
		}
	}
	catch (exception e)
	{
		ApplicationHelpers::putLine(e.what());
	}
	return pushstatus;
	return true;
}

//-----------------------<Enque chrono execution time>--------------------

long long MessageOps::getCurrentTime()
{	
	std::chrono::time_point<std::chrono::system_clock> start;
	start = std::chrono::system_clock::now();
	std::chrono::system_clock::duration tp = start.time_since_epoch();
	std::chrono::milliseconds starttime = duration_cast<milliseconds>(tp);
	//std::cout << "\n" << "time in milli seconds : " << starttime.count() << "\n";
	long long time_int = starttime.count();
	return time_int;
}


void MessageOps::enQueuProcTime(BlockingQueue<pair<char*, size_t>>& sbq)
{
	pair<char*, size_t> msgpair;
	char* pBuf = new char[4096];
	for (int i = 0; i < 4096; i++)
		pBuf[i] = '\0';
	endProcTime = getCurrentTime();
	long long procTime = endProcTime - startProcTime;
	string _procTime = std::to_string(procTime);
	auto iter = _procTime.begin();
	for (unsigned int i = 0; i < _procTime.size(); i++)
	{
		pBuf[i] = *iter;
		iter++;
	}
	msgpair.first = pBuf;
	msgpair.second = _procTime.size();
	sbq.enQ(msgpair);
}

//--------------------------<Create messages if the response file is a text file>------------

bool MessageOps::pushTextFiles(BlockingQueue<pair<char*, size_t>>& sbq, string& output)
{
	bool pushstatus = false;
	pair<char*, size_t> msgpair;
	filebuf ifb;                       // create filebuf	
	if (true)
	{
		string input;
		if (ifb.open(output, ios::in))
		{
			ostringstream readbuf;
			readbuf << &ifb;
			input = readbuf.str();
		}
		else
			input = output;
		auto inputiter = input.begin();
		long long int  inputpos = input.size();
		while (inputiter != input.end())
		{
			char* pBuf = new char[4096];
			for (int i = 0; i < 4096; i++)
				pBuf[i] = '\0';
			int lenofmsg = 0;
			for (int i = 0; i < 4096; i++)
			{
				if (inputiter == input.end())
					break;
				pBuf[i] = *inputiter;
				inputiter++;
				lenofmsg++;
			}
			msgpair.first = pBuf;
			msgpair.second = lenofmsg;
			sbq.enQ(msgpair);
			if (lenofmsg < 4096)
			{			
				enQueuProcTime(sbq);
				pushstatus = true;
				break;
			}
		}
	}
	else
		ApplicationHelpers::putLine("File : " + filename + "is not present in the given location");
	return pushstatus;
	return true;
}

//------------------------<used to parse the packet received>----------------------------

string MessageOps::parseMsg(string& body)
{	
	int pos = body.find_first_of("&%^!");
	int len = pos;
	operation = body.substr(0, pos);
	body = body.substr(pos + 4);
	pos = body.find_first_of("&%^!");
	serverPortNum = std::stoi(body.substr(0, pos));
	body = body.substr(pos + 4);
	pos = body.find_first_of("&%^!");
	serverIPaddr = body.substr(0, pos);
	body = body.substr(pos + 4);
	pos = body.find_first_of("&%^!");
	senderPortNum = std::stoi(body.substr(0, pos));
	setSenderPortNum(senderPortNum);
	body = body.substr(pos + 4);
	pos = body.find_first_of("&%^!");
	senderIpaddr = body.substr(0, pos);
	setSenderIPaddr(senderIpaddr);
	body = body.substr(pos + 4);
	pos = body.find_first_of("&%^!");
	filename = body.substr(0, pos);
	body = body.substr(pos + 4);
	//std::cout << "\nbody: " << body << endl;
	pos = body.find("&%^!");	
	string patt = body.substr(0, pos);
	setPatterns(patt);
	body = body.substr(pos + 4);
	pos = body.find_first_of("&%^!");
	textToSrch = body.substr(0, pos);
	body = body.substr(pos + 4);
	pos = body.find_first_of("&%^!");
	path = body.substr(0, pos);
	body = body.substr(pos + 4);
	pos = body.find_first_of("&%^!");
	reqStrtTime = body.substr(0, pos);
	return body;
}

//--------------------< Header creation >---------------------------------

void MessageOps::createHeader(char* msg, string& header)
{
	auto iter = header.begin();
	while (true)
	{
		for (unsigned int i = 0; i < header.size(); i++)		// header information
		{
			msg[i] = *iter;
			iter++;
		}
		if (iter == header.end())
			break;
	}
}

//--------------------<Helps to respond to the client after the operation is performed>---------------------------

bool MessageOps::sentMsg(string& input, Socket& _lsocket)
{	
	string delim = "&%^!";
	auto inputiter = input.begin();
	long long int  inputpos = input.size();
	//int lenofbody = header.size();
	char msg[4096] = { 0 };	
	while (inputiter != input.end())
	{
		for (int i = 0; i < 4096; i++)
		{
			if (inputiter == input.end())
				break;
			msg[i] = *inputiter;
			inputiter++;
		}
		const char* pmsg = msg;
		if (!_lsocket.sendblock(msg, 4096))
			return false;
		for (int i = 0; i < 4096; i++)
			msg[i] = '\0';
	}
	return true;
}

//---------------<Test Stub>---------------------

#ifdef TEST_MESSAGEOPS
int main()
{	
	string delim = "&%^!";
	string pattdelim = "!^%";
	string patterns = "*.cpp" + pattdelim + "*.h";
	string header = "filenames" + delim + "1040" + delim + "localhost" + delim + "1100" + delim + "localhost" + delim+ "" + delim + patterns;
	header += delim + "main";
	std::cout << "\n Header: " << header << endl;
	MessageOps msgOps;
	msgOps.parseMsg(header);

}

#endif