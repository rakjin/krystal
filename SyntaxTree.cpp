#include "SyntaxTree.h"

#include <list>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "template.common.h"
#include "template.cs.h"


// class Node
// {
// public:
    Node::~Node()
    {
    }

    std::string     Node::getType() { return 0; }; //TODO: remove getType() if unnecessary
    std::string*	Node::getParsed() { return 0; };

// };

// class NodeKst : public Node
// {
    // std::list<Node*>* commands;
    // std::string* fileName;
    // public:
    NodeKst::NodeKst(std::list<Node*>* _commands, std::string* _fileName) : Node()
    {
        commands = _commands;
        fileName = _fileName;
    }

    std::string NodeKst::getType()
    {
       return "KST";
    }

    std::string* NodeKst::getParsed()
    {
        std::stringstream parsed;

        parsed << boost::format(TCS_HEADER) % *fileName;
        parsed << TCS_USINGS;

        std::string namespaceByFileName = fileName->substr(0, fileName->find("."));

        parsed << boost::format(TCS_NAMESPACE_BEGIN) % namespaceByFileName;

        std::list<Node*>::iterator i = commands->begin();
        std::list<Node*>::iterator end = commands->end();

        std::string* temp;

        for (; i != end; ++i)
        {
            temp = (*i)->getParsed();
            temp = indent(temp);
            parsed << *temp;
            parsed << "\n";
        }

        parsed << TCS_NAMESPACE_END;

        parsed << "\n\n";

        return new std::string(parsed.str());
    }
// };

// class NodeInclude : public Node
// {
//     std::string* value;
//     public:
    NodeInclude::NodeInclude(std::string* _value) : Node()
    {
        value = _value;
    }

    std::string NodeInclude::getType()
    {
       return "INCLUDE";
    }

    std::string* NodeInclude::getParsed()
    {
        std::stringstream parsed;

        parsed << "#include \"";
        parsed << *(value);
        parsed << "\"";

        return new std::string(parsed.str());
    }
// };

// class NodePacket : public Node
// {
//     std::string* packetName;
//     std::list<Node*>* packetMembers;
//     public:
    NodePacket::NodePacket(std::string* _packetName, std::list<Node*>* _packetMembers) : Node()
    {
        packetName = _packetName;
        packetMembers = _packetMembers;
    }

    std::string NodePacket::getType()
    {
       return "Packet";
    }

    std::string* NodePacket::getParsed()
    {
        std::stringstream parsed;

        parsed << boost::format(TCS_PACKET_BEGIN) % *packetName;

        std::list<Node*>::iterator i = packetMembers->begin();
        std::list<Node*>::iterator end = packetMembers->end();
        for (; i != end; ++i)
        {
            parsed << "\t" << *((*i)->getParsed());
        }

        parsed << "}";

        return new std::string(parsed.str());
    }
// };

// class NodePacketMember : public Node
// {
//     Node* memberType;
//     Node* memberName;
//     public:
    NodePacketMember::NodePacketMember(Node* _memberType, Node* _memberName) : Node()
    {
        memberType = _memberType;
        memberName = _memberName;
    }

    std::string NodePacketMember::getType()
    {
       return "PacketMember";
    }

    std::string* NodePacketMember::getParsed()
    {
        std::stringstream parsed;

        parsed << *(memberType->getParsed());
        parsed << " ";
        parsed << *(memberName->getParsed());
        parsed << ";\n";

        return new std::string(parsed.str());
    }
// };

// class NodePacketMemberType : public Node
// {
//     int typeType; // one of PRIMITIVE_DATA_TYPE, REFERENCE_DATA_TYPE, MAP, LIST
//     std::string* value; // "int", "bool", ..., "MyPacket", "Skill" or NULL when type is MAP or LIST
//     Node* generic1; // LIST<generic1>
//     Node* generic2; // MAP <generic1, generic2>
//     Node* generic3; // reserved
//     public:
    NodePacketMemberType::NodePacketMemberType(int _type, std::string* _value) : Node()
    {
        typeType = _type;
        value = _value;
        generic1 = NULL;
        generic2 = NULL;
        generic3 = NULL;
    }

    NodePacketMemberType::NodePacketMemberType(int _type, Node* _generic1) : Node()
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = NULL;
        generic3 = NULL;
    }

    NodePacketMemberType::NodePacketMemberType(int _type, Node* _generic1, Node* _generic2) : Node()
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = _generic2;
        generic3 = NULL;
    }

    NodePacketMemberType::NodePacketMemberType(int _type, Node* _generic1, Node* _generic2, Node* _generic3) : Node()
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = _generic2;
        generic3 = _generic3;
    }

    std::string NodePacketMemberType::getType()
    {
       return "PacketMemberType";
    }

    std::string* NodePacketMemberType::getParsed()
    {
        std::stringstream parsed;

        switch (typeType) {

            case Rakjin::Krystal::Parser::token::PRIMITIVE_DATA_TYPE:
            parsed << *value;
            break;

            case Rakjin::Krystal::Parser::token::REFERENCE_DATA_TYPE:
            parsed << *value;
            break;

            case Rakjin::Krystal::Parser::token::MAP:
            parsed << "Dictionary";
            parsed << "<" << *(generic1->getParsed()) << ", " << *(generic2->getParsed()) << ">";
            break;

            case Rakjin::Krystal::Parser::token::LIST:
            parsed << "List";
            parsed << "<" << *(generic1->getParsed()) << ">";
            break;

            default:
            throw(std::runtime_error("Unknown NodePacketMemberType type."));
            break;
        }

        return new std::string(parsed.str());
    }
// };

// class NodePacketMemberName : public Node
// {
//     std::string* value;
//     public:
    NodePacketMemberName::NodePacketMemberName(std::string* _value) : Node()
    {
        value = _value;
    }

    std::string NodePacketMemberName::getType()
    {
       return "PacketMemberName";
    }

    std::string* NodePacketMemberName::getParsed()
    {
       return value;
    }
// };

