#include "parse.h"
#include <iostream>

using namespace tinyxml2;

void printNode(XMLNode* child)
{
    std::cout << "\t<" << child->Value() << " ";
    const XMLAttribute* attr = child->ToElement()->FirstAttribute();
    while( attr != nullptr )
    {
        std::cout <<
            attr->Name() << "=\"" <<
            attr->Value() << "\" ";
        attr = attr->Next();
    }
    std::cout << "/>" << std::endl;
}

void parse()
{
    XMLDocument doc;
    doc.LoadFile("FlowProjectV1/flow.xml");
//    std::cout << doc.FirstChildElement("sometag")->FirstChildElement("somesubtab")->FirstAttribute()->Name() << std::endl;
//    std::cout << doc.FirstChildElement("sometag")->FirstChild()->Value() << std::endl;
//    std::cout << doc.FirstChild()->Value() << std::endl;
    int x;
    std::cin >> x;
    XMLNode* parents = doc.FirstChild();
    while( parents != nullptr )
    {
        std::cout <<    "<" << parents->Value() << " " <<
                        parents->ToElement()->FirstAttribute()->Name() << "=\"" <<
                        parents->ToElement()->FirstAttribute()->Value() << "\">" <<
                        std::endl;
        XMLNode* child = parents->FirstChild();
        while( child != nullptr )
        {
            printNode(child);
            child = child->NextSibling();
        }
        std::cout << "</" << parents->Value() << ">" << std::endl;
        parents = parents->NextSibling();
    }
}
