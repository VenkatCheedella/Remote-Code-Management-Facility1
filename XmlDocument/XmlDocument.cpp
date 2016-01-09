///////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes             //
// Ver 1.3                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Venkata Subba rao Ch, 315-728-8879                //
//              vcheedel@syr.edu                                 //
///////////////////////////////////////////////////////////////////

#include <iostream>
#include "XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include "../XmlReader/XmlReader.h"
#include "NodeInformation.h"
#include <memory>

using namespace XmlProcessing;
using attribs = std::vector<std::pair<std::string, std::string>>;
using sPtr = std::shared_ptr < AbstractXmlElement >;


//---------------<Default construtor>---------------------

XmlProcessing::XmlDocument::XmlDocument() :sourceXML("")
{
	pDocElement_ = XmlProcessing::makeDocElement();
}

//------------------<Promotion constructor>-------------------------

XmlProcessing::XmlDocument::XmlDocument(std::string& src, sourceType srcType) :sourceXML(src)
{	
	
}


// To check whether the present element is a chid of the existing elements

bool XmlProcessing::XmlDocument::isChildPresent(std::string elem, std::string presentBody)
{	
	if (presentBody.find(elem) != std::string::npos)
		return true;
	else
		return false;	
}

// Add the present node to the right parent of the existing node as child

bool XmlProcessing::XmlDocument::addChild(stack <pair<sPtr, std::string>>& checkStack, NodeInformation& nodeinfo)
{
	pair<sPtr, std::string> tempPair;
	bool isChildadded = false;
	while (true)
	{
		if (checkStack.empty())
			break;
		pair<sPtr, std::string> presentElem = checkStack.top();
		if (isChildPresent(nodeinfo.getElement(), presentElem.second))
		{
			presentElem.first->addChild(tempPtr1);
			isChildadded = true;
			if (nodeinfo.getBody().find("<", 0) != std::string::npos)
			{
				tempPair.first = tempPtr1;
				tempPair.second = nodeinfo.getBody();
				checkStack.push(tempPair);
			}
			break;
		}
		else
		{
			if (!checkStack.empty())
				checkStack.pop();
			else
				break;
		}
	}
	return isChildadded;
}

//-----------------------<Insert Xml Declaration Element to tree>-------------------------------

void XmlProcessing::XmlDocument::insertXmlDecl(NodeInformation& nodeinfo)
{
	DocElement* docElem = dynamic_cast<XmlProcessing::DocElement*>(pDocElement_.get());
	bool decElemExist = false;
	tempPtr1 = XmlProcessing::makeXmlDeclarElement();
	if (docElem != nullptr)
	{
		for (auto child : docElem->getChildern())
		{
			if (child->value() == "?xml")
			{
				pDocElement_->removeChild(child);
				break;
			}
		}
	}
	for (auto s : nodeinfo.getAttributes())
		tempPtr1->addAttrib(s.first, s.second);
	docElem->getChildern().insert(docElem->getChildern().begin(), tempPtr1);		// XmlDecl insert to DocElem
}

//------------------------------<With the node information, an element is created and inserted at the right place>-------------------
void XmlProcessing::XmlDocument::treeBuild(NodeInformation& nodeinfo)
{
	bool addChildstatus = false;
	if (pDocElement_ == nullptr)	
		pDocElement_ = XmlProcessing::makeDocElement(pDocElement_);				
		if (nodeinfo.getTag() == "!--")
		{
			tempPtr1 = XmlProcessing::makeCommentElement(nodeinfo.getComment());
			if (checkStack.empty())
			{
				pDocElement_->addChild(tempPtr1);
				addChildstatus = true;
			}
			else				
				addChildstatus = addChild(checkStack, nodeinfo);
		}			
		else if (nodeinfo.getTag() == "?xml")
		{
			insertXmlDecl(nodeinfo);			
		}			
		else if (nodeinfo.getTag().find("?") != std::string::npos && nodeinfo.getTag() != "?xml")
		{
			tempPtr1 = XmlProcessing::makeProcInstrElement(nodeinfo.getTag());
			for (auto s : nodeinfo.getAttributes())
				tempPtr1->addAttrib(s.first, s.second);
			pDocElement_->addChild(tempPtr1);
			addChildstatus = true;
		}	
		else		
			addChildstatus = addTaggedElem(nodeinfo);			
		if (!addChildstatus && nodeinfo.getTag() != "?xml" && dynamic_cast<TaggedElement*>(tempPtr1.get()) == nullptr)
			pDocElement_->addChild(tempPtr1);		
}


//----------------------<To add a tagged element to a tree>-------------------

bool XmlProcessing::XmlDocument::addTaggedElem(NodeInformation& nodeinfo)
{
	bool addChildstatus = false;
	tempPtr1 = XmlProcessing::makeTaggedElement(nodeinfo.getTag());
	if (nodeinfo.getAttributes().size() != 0)
	{
		for (auto s : nodeinfo.getAttributes())
			tempPtr1->addAttrib(s.first, s.second);
	}
	if (docTaggedElemChild == false)
	{
		pDocElement_->addChild(tempPtr1);
		pair<sPtr, std::string> temp;
		temp.first = tempPtr1;
		temp.second = nodeinfo.getBody();
		checkStack.push(temp);
		docTaggedElemChild = true;
		addChildstatus = true;
		_pRootTagElem = tempPtr1;
	}
	if (nodeinfo.getTextElements().size() != 0)
	{
		sPtr textTemp = nullptr;
		for (auto s : nodeinfo.getTextElements())
		{
			textTemp = XmlProcessing::makeTextElement(s);
			tempPtr1->addChild(textTemp);
		}
	}
	if (!addChildstatus)
		addChildstatus = addChild(checkStack, nodeinfo);
	return addChildstatus;
}

//----<Get the token, encapsulate the infomration for an element creation>------------
void XmlProcessing::XmlDocument::elementParsing()
{
	XmlReader* sReader = new XmlReader(sourceXML);	
		while (sReader->next())
		{
			NodeInformation* nodeinfo = new NodeInformation();
			 sReader->tag();
			 sReader->element();
			 sReader->body();			
			std::string cmt;
			if (sReader->tag() == ("!--"))
			{
				int startpoint = sReader->element().find_first_of(" ");
				int endpoint = sReader->element().find_last_of("--");
				int lenofcomment = endpoint - startpoint;
				cmt = sReader->element().substr(startpoint, lenofcomment - 1);
				nodeinfo->setComment(cmt);
			}
			XmlReader::attribElems attribs = sReader->attributes();		
			vector<std::string> textElements;
			if (sReader->body() != "")
			{
				if (sReader->getTextElems(sReader->body(), textElements))
				{					
					nodeinfo->addTextElem(textElements);
				}
			}
			nodeinfo->setTag(sReader->tag());
			nodeinfo->setBody(sReader->body());
			nodeinfo->addAttribute(sReader->attributes());
			nodeinfo->setElement(sReader->element());			
			treeBuild(*nodeinfo);			
		}			
	if (pDocElement_ != nullptr)
	std::cout << "\n" << pDocElement_->toString();	
}

//----------------<Initiate the tree creation>-----------------------

void XmlProcessing::XmlDocument::controller()
{
	std::cout << "Tree construction is here: " << "\n";
	elementParsing();
	
}

//---------------Test stub>---------------------

#ifdef TEST_XMLPROCESSING
int main()
{
	title("Testing XmlDocument class");
	std::string inputSample = "< ? xml version = \"1.1\" encoding=\"UTF - 8\" ?><?xml-proc color=\"black\" background=\"white\" ?><!--  XML test case  -->";
	XmlDocument doc(inputSample);
	doc.controller();

  std::cout << "\n\n";
}

#endif