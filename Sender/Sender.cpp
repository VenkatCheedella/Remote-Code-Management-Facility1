#include "Sender.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

//---------------------<Promotion constructor>------------------------

Sender::Sender(string& input) :_input(input)
{
	ApplicationHelpers::Verbose(true);
}


//--------------------<Make a connection to respond to the cient>-------------

void Sender::createConnection(MessageOps& msg, string& output)
{
	SocketSystem ss;
	SocketConnecter si;
	bool manualinterup = false;
	while (!si.connect(msg.getSenderIPaddr(), msg.getSenderPortNum()))
	{		
		ApplicationHelpers::Verbose::show("Server waiting to connect");
		::Sleep(100);
	}
	BlockingQueue<pair<char*, size_t>> sbq;
	std::thread pushMsgsThread(
		[&]()
	{
		msg.pushBufferMsgs(sbq, output);
	}
	);
	bool sendstatus;
	std::thread sendMsgsThread(
		[&]()
	{
		int count = 0;
		while (true)
		{
			pair<char*, size_t> temppair;
			temppair = sbq.deQ();
			if (!si.sendblock(temppair.first, temppair.second))
			{
				sendstatus = false;
				ApplicationHelpers::putLine("Send failed");
				break;
			}
			if (temppair.second < 4096)
			{
				count++;
				if (count > 1)
				{
					ApplicationHelpers::putLine("\n\nResponse succesfully sent!\n");
					break;
				}

			}

		}
	}
	);
	pushMsgsThread.join();
	sendMsgsThread.join();
	si.shutDownSend();
}

//-------------------<Make a request for connection>---------------------

void Sender::createConnection(string IPaddress, int portNumber)
{
	SocketSystem ss;
	SocketConnecter si;
	while (!si.connect(IPaddress, portNumber))
	{
		ApplicationHelpers::Verbose::show("client waiting to connect");
		::Sleep(100);
	}
	MessageOps msgopr;
	if (msgopr.sentMsg(_input, si))
		ApplicationHelpers::Verbose::show("Ack sent successfully!");
	Verbose::show("\n  client calling send shutdown\n");
	si.shutDownSend();
}

//---------------------<default destructor>---------------------------

Sender::~Sender()
{
}

//---------------Test Stub>---------------------------

#ifdef TEST_SENDER

int main()
{
	filebuf ifb;                       // create filebuf
	if (ifb.open("sample2.txt", ios::in))
	{
		ostringstream readbuf;
		readbuf << &ifb;
		string inputfile = readbuf.str();
		Sender snd(inputfile);
		snd.createConnection("localhost", 1080);
	}
	else
		std::cout << "Please provide the input file" << endl;
}

#endif