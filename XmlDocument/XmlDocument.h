#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
///////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes               //
// Ver 1.3                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Venkata Subba rao Cheedella, 315-728-8879        //
//				vcheedel@syr.edu                                 //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is intended to help students in CSE687 - Object Oriented Design
* get started with Project #2 - XML Document Model.  It uses C++11 constructs,
* most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with 
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp, 
*     XmlElement.h, XmlElement.cpp, XmlReader.h, XmlReader.cpp, NodeInformation.h
*	  NodeInformation.cpp
*
* Public Interfaces
* --------------------
* 	void elementParsing();								--> Get the token, encapsulate the infomration for an element creation
*	void treeBuild(NodeInformation& nodeinfo);			--> With the node information, an element is created and inserted at the right place
*	void controller();									--> Initiate the tree creation
*	sPtr getDocElement(){ return pDocElement_; }		-->Get the root element of the tree
*	sPtr getRootTaggedElement(){return _pRootTagElem;}	-->Get the root tagged element of a tree
*
*
Build Process
===============
From the Visual Studio Developer's Command Prompt:
devenv Server.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.3 : 23 Mar 15
* - A tree is constructed for a given XML file
* ver 1.2 : 21 Feb 15
* - modified these comments
* ver 1.1 : 14 Feb 15
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 15
* - first release
*
* 
* 
*
*/

#include <memory>
#include <string>
#include <string>
#include <iostream>
#include "../XmlElement/XmlElement.h"
#include "../XmlReader/XmlReader.h"
#include "NodeInformation.h"
#include <stack>
#include <vector>

using namespace std;

namespace XmlProcessing
{
  class XmlDocument
  {
  public:
    using sPtr = std::shared_ptr < AbstractXmlElement > ;

    enum sourceType { string, filename };	
    XmlDocument(std::string& src, sourceType srcType=string);					// How to use sourceType ?     
	XmlDocument();
	void elementParsing();
	void treeBuild(NodeInformation& nodeinfo);
	void controller();
	sPtr getDocElement(){ return pDocElement_; }
	sPtr getRootTaggedElement(){return _pRootTagElem;}
	

  private:
    sPtr pDocElement_ = nullptr;         // AST that holds procInstr, comments, XML root, and more comments 
	int typeofElem;	
	stack<pair<sPtr, std::string>> checkStack;
	sPtr tempPtr1 = nullptr;
	bool docTaggedElemChild = false;
	bool isChildPresent(std::string tag, std::string body);
	bool addChild(stack <pair<sPtr, std::string>>& checkStack, NodeInformation& nodeinfo);
	sPtr _pRootTagElem = nullptr;
	std::string sourceXML;
	bool addTaggedElem(NodeInformation& nodeinfo);
	void insertXmlDecl(NodeInformation& nodeinfo);
  };  
}
#endif
