#ifndef NODEINFORMATION_H
#define NODEINFORMATION_H


/////////////////////////////////////////////////////////////////////
// NodeInformation.h - Transport the node information to construct //
//                     tree                                        //
// Application: CSE687 - Object Oriented Design-XML Document Model //
// Platform:    Dell Inspiron, Win 8.1, Visual Studio 2013         //
// Author:      Venkata Subba Rao Cheedella, Syracuse Univesity    //
//              vcheedel@syr.edu                                   //
/////////////////////////////////////////////////////////////////////

/*


Package Operation
=======================

It transport the parse information of an element from one place to the other.


Required Files
===============

NodeInformation.h, NodeInformation.cpp


Public Interfaces
==================

attribElems& getAttributes();							--> Get the attributes
void addAttribute(attribElems attribs);					--> Set attributes
string& getTag();										--> Get tag
void setTag(string str);								--> set Tag
string& getBody();										--> Get body
void setBody(string str);								--> set body
vector<string>& getTextElements();						--> gettext elements
void addTextElem(vector<string> texteles);				--> set text elements
void setComment(string cmt);							--> set comments
string getComment();									--> get comments
void setElement(string elem){ element = elem; }			--> set elements
string getElement(){ return element; }					--> get elements



Maintenance History
=========================

ver 1.0  : 23 Mar 2015
- First Release



*/


#include <iostream>
#include <string>
#include <vector>


using namespace std;
typedef std::vector<std::pair<std::string, std::string>> attribElems;

class NodeInformation
{
public:
	NodeInformation();
	~NodeInformation();
	attribElems& getAttributes();
	void addAttribute(attribElems attribs);
	string& getTag();
	void setTag(string str);
	string& getBody();
	void setBody(string str);
	vector<string>& getTextElements();
	void addTextElem(vector<string> texteles);
	void setComment(string cmt);
	string getComment();
	void setElement(string elem){ element = elem; }
	string getElement(){ return element; }
private:
	string _ntag;
	string _body;
	attribElems attributes;
	vector<string> textElements;
	string comment;
	string element;
};

#endif

