#ifndef XMLREADER_H
#define XMLREADER_H
///////////////////////////////////////////////////////////////////////
// XmlReader.h - Parse XML strings                                   //
// ver 2.4                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Toshiba Portege R705, Windows 7, Home Premium, SP1   //
// Application: Summer 2011 Projects                                 //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              www.lcs.syr.edu/faculty/fawcett, (315) 443-3948      //
///////////////////////////////////////////////////////////////////////
/*
 * Note: XmlReader and XmlWriter supercede both XmlTran and XmlParser.
 *
 * Package Operations:
 * ===================
 * This package contains a single class, XmlReader.  It provides a
 * simple interface to step from XML element to element and extract
 * constituent parts, e.g., tag, attributes, and body.  The package
 * does not support editing XML strings.  For that, you should use
 * the XmlDocument package.
 *
 * Parsing honors element scope, e.g., if there are two elements with
 * the same tag names at different levels of the document scope, the 
 * XmlReader will not get confused and prematurely close an element
 * because it found a matching closing tag for a contained element.
 *
 * Exceptions will be thrown by element(), body(), and attributes()
 * if malformed XML is detected.
 *
 * Required Files:
 * ===============
 * XmlReader.h, XmlReader.cpp
 *
 Build Process
 ===============
 From the Visual Studio Developer's Command Prompt:
 devenv XmlDocument.sln /rebuild debug
 *
 * Maintenance History:
 * ====================
 *ver 2.4 : 23 Mar 15
 *- Text elements are parsed 
 *- Proc Instruction attributes are retrieved
 * ver 2.3 : 26 Feb 13
 * - fixed bug found by Yang Zhou in extractIdentifier when used
 *   to parse attributes having values with spaces.
 * ver 2.2 : 15 Feb 12
 * - fixed bug found by Yateen Nikharge in element(). Search for
 *   element end was not selective enough.
 * - previously added member functions position() and position(size_t p)
 *   to enable measuring and changing internal string index - used in
 *   creating Visual Studio project.
 * ver 2.1 : 25 Jul 11
 * - added comments to manual page, above, and implementations in
 *   XmlReader.cpp
 * ver 2.0 : 27 Jun 11
 * - moved #include<stack> to XmlReader.cpp
 * - moved #include<iostream> to test stub in XmlReader.cpp
 * ver 1.0 : 26 Jun 11
 * - first release
 */
#include <vector>
#include <iostream>
using namespace std;

class XmlReader
{
public:
  typedef std::vector<std::pair<std::string,std::string>> attribElems;
  XmlReader(const std::string& xml);
  bool next();
  std::string tag();
  attribElems attributes();
  std::string body();
  std::string element();
  std::string extractIdentifier(size_t& pos);
  std::string extractString(size_t& pos);  
  bool getTextElems(string& body, vector<string>& textElems);
  void reset();
  size_t position();
  bool position(size_t p);
  std::string& xml();
private:
  // source string
  std::string _xml;
  size_t _position;
  // current element parts
  std::string _tag;
  attribElems _attributes;
  size_t localposition;
  size_t  starttagpos;
  size_t endtagpos;
  int lenofText;
  bool textExist = false;
  bool recursiveTextElemSrch(vector<string>& textElems, string& body);
  bool checkTagPresence(string& body, vector<string>& textElems);
  bool looptoSearchForText(string& tag, int& symbol, vector<string>& textElems, string& body);
};

inline void XmlReader::reset() { _position = 0; }

inline size_t XmlReader::position() { return _position; }

inline std::string& XmlReader::xml() { return _xml; }

#endif
