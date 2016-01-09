#include "NodeInformation.h"

//----------------<Constructor>-----------------

NodeInformation::NodeInformation()
{
	 _ntag = "";
	_body = "";	
	 comment = "";
	 element = "";
}

//---------------< Destructor>-------------------

NodeInformation::~NodeInformation()
{
	
}

//-------------<Get attributes of element>----------------------
attribElems& NodeInformation::getAttributes()
{
	return attributes;
}

//----------------<set attributes of elements>-----------------

void NodeInformation::addAttribute(attribElems attribs)
{
	this->attributes = attribs;
}

//-----------------<Get tag information>-----------------

string& NodeInformation::getTag()
{
	return _ntag;
}

//-------------------<Set tag element>-------------------

void NodeInformation::setTag(string str)
{
	this->_ntag = str;
}

//-------------------<Get body of an element>------------------------
string& NodeInformation::getBody()
{
	return _body;
}

//-----------------------<Set body of an element>---------------

void NodeInformation::setBody(string str)
{
	this->_body = str;
}

//------------------<Get text elements for an element>--------------

vector<string>& NodeInformation::getTextElements()
{
	return textElements;
}

//---------------<Set text elements for an element>-----------------

void NodeInformation::addTextElem(vector<string> texteles)
{
	this->textElements = texteles;
}

//----------------<Set comment text if it is a comment element>------------------
void NodeInformation::setComment(string cmt)
{
	comment = cmt;
}

//-------------------<get Comment element>------------------

string NodeInformation::getComment()
{
	return comment;
}


