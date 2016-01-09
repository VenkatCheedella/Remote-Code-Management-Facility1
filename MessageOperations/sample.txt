///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.3                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Toshiba Portege R705, Windows 7, Home Premium, SP1   //
// Application: Summer 2011 Projects                                 //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              www.lcs.syr.edu/faculty/fawcett, (315) 443-3948      //
///////////////////////////////////////////////////////////////////////

#include "XmlReader.h"
#include <ctype.h>
#include <stack>
#include <utility>
#include <iostream>
#include <algorithm>
#include <string>

XmlReader::XmlReader(const std::string& xml) 
  : _xml(xml), _position(0), localposition(0) {}

//----< helper identifies markup chars >-----------------------------

bool specialChar(char ch)
{
  bool test = isspace(ch) || ch == '/' || ch == '>' || ch == '<' || ch == '=';
  test = test || ch == '\'' || ch == '"';
  return test;
}
//----< helper extracts string >-------------------------------------

std::string XmlReader::extractString(size_t& pos)
{
  size_t start = pos+1;
  std::string str;
  char ch = _xml[pos];
  while(_xml[++pos] != ch)
    str += _xml[pos];
  ++pos;
  return str;
}
//----< helper finds identifiers >-----------------------------------

std::string XmlReader::extractIdentifier(size_t& pos)
{
  std::string ident;
  while(true)
  {
    if(pos == _xml.size())
      return ident;
    char ch = _xml[pos];
    if(specialChar(ch))
    {
      if(ch == '\'' || ch == '"')
      {
        return extractString(pos);
      }
      ++pos;
    }
    else
      break;
  }
  while(true)
  {
    char ch = _xml[pos];
    if(specialChar(ch))
      break;
    ident += ch;
    ++pos;
  }
  return ident;
}
//----< return current element string >------------------------------

std::string XmlReader::element()
{
  try {
  // find tag - assumes _position points to character after
  // opening "<" on entry
  localposition = _position;
  _tag = extractIdentifier(localposition);

  // is declaration?
  std::string decTag = "?xml";
  if(_tag.compare("?xml") == 0)
  {
    size_t locpos = _xml.find("?>");
    return _xml.substr(_position-1,locpos-_position+3); 
  }

  // xml processing instruction
  signed int charpos = -1;
  charpos = _tag.find("?");
  if (_tag.compare("?xml") != 0 && charpos > -1)
  {
	  size_t locpos = _xml.find("?>", _position+1);
	  size_t lenofstr = locpos - _position;
	  return _xml.substr(_position - 1, lenofstr+3);
  }
  // is comment?
  if(_tag.compare("!--") == 0)
  {
    size_t locpos = _xml.find("-->",_position);
    return _xml.substr(_position-1,locpos-_position+4); 
  }

  // find end of element </tag>
  size_t locpos1 = localposition;

  // note: tracks element scope with tagStack, so will correctly
  // return nested elements with the same tag name
  std::stack<std::string> tagStack;
  tagStack.push(_tag);
  while(true)
  {
    locpos1 = _xml.find(_tag,locpos1);
    if(locpos1 >= _xml.size())
      break;
    if(_xml[locpos1-1]=='/' && _xml[locpos1 + _tag.size()] == '>')  // modified 2/15/2012
      tagStack.pop();
    else if(_xml[locpos1-1]=='<')
      tagStack.push(_tag);
    if(tagStack.size() == 0)
      break;
    ++locpos1;
  }

  // find end element of self-closing tag, e.g., <tag />
  size_t locpos2 = _xml.find(">",localposition);
  if(_xml[locpos2-1] != '/')
    locpos2 = _xml.size();

  // find end element </tag>
  localposition = std::min(locpos1,locpos2);
  if(localposition >= _xml.size())
    throw std::exception("malformed XML");
  if(localposition == locpos1)
  {
    localposition = _xml.find('>',localposition);
    return _xml.substr(_position-1, localposition - _position +2);
  }
  return _xml.substr(_position-1, localposition - _position + 2);
  }
  catch(...)
  {
    std::cout << "\n  ill-formed XML";
	return "";
  }
}

//---------------<loop to search for tag>---------------------

bool XmlReader::looptoSearchForText(string& tag, int& findsymbol, vector<string>& textElems, string& body)
{
	endtagpos = body.find(">", starttagpos);
	int lenOfTag = endtagpos - starttagpos;
	tag = body.substr(starttagpos + 1, lenOfTag - 1);
	if (tag.find(" ") != -1)
		tag = tag.substr(0, tag.find_first_of(" "));
	if (tag == "!--")
		textExist = checkTagPresence(body, textElems);
	findsymbol = body.find("/", endtagpos);
	return textExist;
}

//---------------<Recursive search for the text elements in the body of an element>---------------

bool XmlReader::recursiveTextElemSrch(vector<string>& textElems, string& body)
{	
	while (true)
	{
		int findsymbol;
		string tag;
		//textExist = looptoSearchForText(tag, findsymbol, textElems, body);
		endtagpos = body.find(">", starttagpos);	
		int lenOfTag = endtagpos - starttagpos;
		tag = body.substr(starttagpos + 1, lenOfTag - 1);
		if (tag.find(" ") != -1)
			tag = tag.substr(0, tag.find_first_of(" "));
		if (tag == "!--")
			textExist = checkTagPresence(body, textElems);				
		findsymbol = body.find("/", endtagpos);
		if (findsymbol == -1)
			break;
		while (true)
		{
			string temp = body.substr(findsymbol + 1, tag.size());
			if (temp == tag)
				break;
			findsymbol = body.find("/", findsymbol + 1);
		}

		endtagpos = body.find(tag, findsymbol);
		endtagpos = body.find(">", endtagpos);
		if (endtagpos == body.size() - 1)
			break;
		else
		{
			if (body.find("<", endtagpos) != -1)
				starttagpos = body.find("<", endtagpos);
			else
			{
				lenofText = body.size() - 1 - endtagpos;
				textElems.push_back(body.substr(endtagpos + 1, lenofText - 1));
				textExist = true;
				break;
			}
			if (endtagpos + 1 != starttagpos)
			{
				lenofText = starttagpos - endtagpos;
				textElems.push_back(body.substr(endtagpos + 1, lenofText - 1));
				textExist = true;
			}
		}
	}
	return textExist;
}

//-------------------<Check if tag is present after a comment in the body>---------------

bool XmlReader::checkTagPresence(string& body, vector<string>& textElems)
{
	bool textExist = false;
	int lenofText;
	int lenOfTag;
	string tag;
	starttagpos = body.find("<", endtagpos);
	if (starttagpos + 1 > endtagpos)
	{
		lenofText = starttagpos - endtagpos;
		textElems.push_back(body.substr(endtagpos + 1, lenofText - 1));
		textExist = true;
	}
	endtagpos = body.find(">", starttagpos);
	lenOfTag = endtagpos - starttagpos;
	tag = body.substr(starttagpos + 1, lenOfTag - 1);
	if (tag.find(" ") != -1)
		tag = tag.substr(0, tag.find_first_of(" "));
	return textExist;
}

//----<Capturing text elements in the body>-------------

bool XmlReader::getTextElems(string& body, vector<string>& textElems)
{
	size_t pos = 0;
	//int lenofText;
	//starttagpos = -1;	
	starttagpos = body.find("<", pos);
	if (starttagpos == std::string::npos)
	{
		textElems.push_back(body);
		return true;
	}
	bool textExist = false;
	int lenofText;
	if (pos < starttagpos)
	{
		lenofText = starttagpos - pos;
		textElems.push_back(body.substr(pos, lenofText));
		textExist = true;
	}
	textExist = recursiveTextElemSrch(textElems, body);	
	return textExist;
}

//----< return body string >-----------------------------------------

std::string XmlReader::body()
{
  if(_tag.compare("?xml")==0 || _tag.compare("!--")==0)
  {
    return "";
  }
  signed int charpos = -1;
  charpos = _tag.find("?");
  if (_tag.compare("?xml") != 0 && charpos > -1)
  {
	  return "";
  }
  std::string elem = element();
  size_t locpos1 = elem.find('>');
  if(locpos1 >= elem.size())
    throw std::exception("malformed XML");
  if(elem[locpos1-1] == '/')
    return "";
  std::string localtag = tag();
  size_t locpos2 = elem.find_last_of("</");
  if(locpos2 < elem.size())
    return elem.substr(locpos1+1,locpos2-locpos1-2);
  return "";
}
//----< move to next XML tag >---------------------------------------

bool XmlReader::next()
{
  while(true)
  {
    _position = _xml.find('<',_position);
    if(_position >= _xml.size())
      return false;
    ++_position;
    if(_xml[_position] != '/')
      break;
  }
  return true;
}
//----< return tag string >------------------------------------------

std::string XmlReader::tag()
{
  localposition = _position;
  return extractIdentifier(localposition);
}
//----< return vector of attributes >--------------------------------

XmlReader::attribElems XmlReader::attributes()
{
  _attributes.clear();
  localposition = _position;  
  extractIdentifier(localposition);
  if(_tag.compare("!--")==0)				// xml processing instruction can have attributes.So, why we dont we capture attributes here?
    return _attributes;
  // find attributes
  size_t locpos = _xml.find('>',localposition);
  if (_tag.compare("?xml") == 0)
  {
	  while (true)
	  {
		  std::string name = extractIdentifier(localposition);
		  if (locpos <= localposition)									// to make the attributes work for xml-processing instruction, I made change to comparision(changd from < to <=)
			  return _attributes;
		  std::string value = extractIdentifier(localposition);
		  if (locpos < localposition)
			  throw std::exception("malformed XML");					// When can we expect this exception to arise?
		  std::pair<std::string, std::string> elem;
		  elem.first = name;
		  elem.second = value;
		  _attributes.push_back(elem);
	  }

  }
  if (_attributes.size() != 0)
	  return _attributes;
  while(true)
  {
    std::string name = extractIdentifier(localposition);
    if(locpos <= localposition)									// to make the attributes work for xml-processing instruction, I made change to comparision(changd from < to <=)
      return _attributes;
    std::string value = extractIdentifier(localposition);
    if(locpos < localposition)
      throw std::exception("malformed XML");					// When can we expect this exception to arise?
    std::pair<std::string,std::string> elem;
    elem.first = name;
    elem.second = value;
    _attributes.push_back(elem);
  }
  return _attributes;
}
//----< reposition string index >------------------------------------

bool XmlReader::position(size_t p)
{
  if(p < 0 || _xml.size() <= p)
    return false;
  _position = p;
  return true;
}
//----< test stub >--------------------------------------------------

#ifdef TEST_XMLREADER
#include <iostream>

int main()
{
  try {
  std::cout << "\n  Testing XmlReader";
  std::cout << "\n ===================\n";
  std::string Prologue = "<? xml version=\"1.0\" ?><!-- top level comment -->";
  std::string test1 = Prologue; 
  test1 += "<parent att1='val1 with spaces' att2='val2'><child1 /><child2>child2 body</child2><child1 /></parent>";
  std::string test2 = Prologue;
  test2 += "<Dir><path>/TestFile/Dir1</path><File><name>text1.txt</name><date>02/11/2012</date></File></Dir>";
  std::string test3 = "<parent att1='val1 with spaces' att2='val2'>This is textelem1<tag1><!--child1 comment-->This is inside text elem1</tag1>This is text elem2<tag2>This is insisetext elem2<tag8><!--this is comment tag-->this is tag8</tag8>This is inside tag2 and outside tag8</tag2>This is third text elem3<tag5></tag5></parent>";
  string body1 = "<parent>This is parent text1<child1><!--child1 comment-->This is child1 text<child2><!--this is comment tag-->child2 text</child2></child1>this is parent text2";
  string body2 = "<child3>child3 text</child3>parent text3</parent>";
  body1 += body2;
  XmlReader rdr(test3);

  std::cout << "\n  source = " << test3 << std::endl;

  // testing helper function

  std::cout << "\n  Testing extractIdentifier";
  std::cout << "\n ---------------------------";
  std::string ident;
  size_t position = 1;
  do
  {
    ident = rdr.extractIdentifier(position);
    std::cout << "\n  " << ident.c_str();
  } while(ident.size() > 0);

  // testing typical use, step through each element extracting tags, attributes,
  // bodies, and or entire element strings

  std::cout << "\n  testing next(), tag(), element(), body(), and attributes()";
  std::cout << "\n -----------------------------------------------------------";
  rdr.reset();
  while(rdr.next())
  {
    std::cout << "\n  tag:     " << rdr.tag().c_str();
    std::cout << "\n  element: " << rdr.element().c_str();
    std::cout << "\n  body:    " << rdr.body().c_str();
	vector<string> textElems;	
    XmlReader::attribElems attribs = rdr.attributes();
    for(size_t i=0; i<attribs.size(); ++i)
      std::cout << "\n  attributes: " << attribs[i].first.c_str() << ", " << attribs[i].second.c_str();
	//textElems = XmlReader::printTextElements(rdr.body(), textElems);
	//for (auto s : textElems)	
	//	cout << "   " << s << "\n";	
	vector<string> textElements;
	if (rdr.body() != "")
	{
		if (rdr.getTextElems(rdr.body(), textElements))
		{
			for (string s : textElements)
				cout << "\n" << s;
		}
	}
  }
  std::cout << "\n\n";

  // testing typical use, step through each element extracting tags, attributes,
  // bodies, and or entire element strings

  rdr.xml() = test2;
  std::cout << "\n  source = " << rdr.xml().c_str() << std::endl;

  std::cout << "\n  testing next(), tag(), element(), body(), and attributes()";
  std::cout << "\n -----------------------------------------------------------";

  rdr.reset();
  while(rdr.next())
  {
    std::cout << "\n  tag:     " << rdr.tag().c_str();
    std::cout << "\n  element: " << rdr.element().c_str();
    std::cout << "\n  body:    " << rdr.body().c_str();

    XmlReader::attribElems attribs = rdr.attributes();
    for(size_t i=0; i<attribs.size(); ++i)
      std::cout << "\n  attributes: " << attribs[i].first.c_str() << ", " << attribs[i].second.c_str();
  }
  std::cout << "\n\n";
  std::cout << "Last statement of main" << "\n";
  return 0;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n  threw std::exception due to ill-formed XML: " << ex.what() << "\n\n";
    return 1;
  }
  catch(...)
  {
    std::cout << "\n  threw non-standard exception due to ill-formed XML: ";
    std::cout << "\n\n";
    return 1;
  }
}
#endif

