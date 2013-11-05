#include <list>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "expression.h"


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
    // std::list<Node*>* NodeKst::commands;
    // public:
    NodeKst::NodeKst(std::list<Node*>* _commands) : Node()
    {
        commands = _commands;
    }

    std::string NodeKst::getType()
    {
       return "KST";
    }

    std::string* NodeKst::getParsed()
    {
        std::stringstream parsed;

        parsed << "/* file info here */\n\n";

        std::list<Node*>::iterator i = commands->begin();
        std::list<Node*>::iterator end = commands->end();
        for (; i != end; ++i)
        {
            parsed << *((*i)->getParsed());
        }

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
        parsed << "\"\n";

        return new std::string(parsed.str());
    }
// };

// class NodePacket : public Node
// {
//     Node* packetName;
//     std::list<Node*>* packetMembers;
//     public:
    NodePacket::NodePacket(Node* _packetName, std::list<Node*>* _packetMembers) : Node()
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

        parsed << "packet ";
        parsed << *(packetName->getParsed());
        parsed << " {\n";

        //parsed << "\t[packets_members]\n";
        std::list<Node*>::iterator i = packetMembers->begin();
        std::list<Node*>::iterator end = packetMembers->end();
        for (; i != end; ++i)
        {
            parsed << "\t" << *((*i)->getParsed());
        }

        parsed << "}\n";

        return new std::string(parsed.str());
    }
// };

// class NodePacketMember : public Node
// {
//     Node* memberType;
//     Node* memberName;
//     public:
    NodePacketMember::NodePacketMember(Node* _packetName, Node* _packetMembers) : Node()
    {
        memberType = _packetName;
        memberName = _packetMembers;
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
//     std::string* value1; // "int", "bool", ..., "MyPacket", "Skill", LIST<value1>
//     std::string* value2; // MAP<value1, value2>
//     std::string* value3; // reserved
//     public:
    NodePacketMemberType::NodePacketMemberType(int _type, std::string* _value) : Node()
    {
        typeType = _type;
        value1 = _value;
        value2 = NULL;
        value3 = NULL;
    }

    NodePacketMemberType::NodePacketMemberType(int _type, std::string* _value1, std::string* _value2) : Node()
    {
        typeType = _type;
        value1 = _value1;
        value2 = _value2;
        value3 = NULL;
    }

    NodePacketMemberType::NodePacketMemberType(int _type, std::string* _value1, std::string* _value2, std::string* _value3) : Node()
    {
        typeType = _type;
        value1 = _value1;
        value2 = _value2;
        value3 = _value3;
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
            parsed << *value1;
            break;

            case Rakjin::Krystal::Parser::token::REFERENCE_DATA_TYPE:
            parsed << *value1;
            break;

            case Rakjin::Krystal::Parser::token::MAP:
            parsed << "Dictionary";
            parsed << "<" << *value1 << ", " << *value2 << ">";
            break;

            case Rakjin::Krystal::Parser::token::LIST:
            parsed << "List";
            parsed << "<" << *value1 << ">";
            break;

            default:
            throw(std::runtime_error("Unknown variable."));
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

