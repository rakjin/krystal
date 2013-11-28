#include "SyntaxTree.h"

#include <list>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <assert.h>

#include <boost/format.hpp>

#include "parser.tab.hh"

#include "template.common.h"
#include "template.cs.h"

using namespace std;
using namespace Rakjin::Krystal;

// class Node
// {
// public:
    Node::Node(Rakjin::Context* _context)
    {
        context = _context;
    }

    Node::~Node()
    {
    }

    int Node::getType() { return 0; } //TODO: remove getType() if unnecessary
    size_t Node::getHash(vector<Node*>* referencingStack) { return 0; }
    string* Node::getParsed(int as) { return 0; }

// };

// class NodeKst : public Node
// {
    // list<Node*>* commands;
    // string* fileName;
    // public:
    NodeKst::NodeKst(Rakjin::Context* _context, list<Node*>* _commands, string* _fileName) : Node(_context)
    {
        commands = _commands;
        fileName = _fileName;
    }

    int NodeKst::getType()
    {
       return CsNodeType::kst;
    }

    size_t NodeKst::getHash(vector<Node*>* referencingStack)
    {
        return 0;
    }

    string* NodeKst::getParsed(int as)
    {
        stringstream parsed;

        parsed << boost::format(TCS_HEADER) % *fileName;
        parsed << TCS_USINGS;

        string namespaceByFileName = fileName->substr(0, fileName->find("."));

        parsed << boost::format(TCS_NAMESPACE_BEGIN) % namespaceByFileName;

        list<Node*>::iterator i = commands->begin();
        list<Node*>::iterator end = commands->end();

        string* temp;

        for (; i != end; ++i)
        {
            temp = (*i)->getParsed(CsParseAs::Default);
            temp = indent(temp);
            parsed << *temp;
            parsed << "\n";
        }

        parsed << TCS_NAMESPACE_END;

        parsed << "\n\n";

        return new string(parsed.str());
    }
// };

// class NodeInclude : public Node
// {
//     string* value;
//     public:
    NodeInclude::NodeInclude(Rakjin::Context* _context, string* _value) : Node(_context)
    {
        value = _value;
    }

    int NodeInclude::getType()
    {
       return CsNodeType::include;
    }

    size_t NodeInclude::getHash(vector<Node*>* referencingStack)
    {
        return 0;
    }

    string* NodeInclude::getParsed(int as)
    {
        stringstream parsed;

        parsed << "#include \"";
        parsed << *(value);
        parsed << "\"";

        return new string(parsed.str());
    }
// };

// class NodePacket : public Node
// {
//     string* packetName;
//     list<Node*>* packetMembers;
//     public:
    NodePacket::NodePacket(Rakjin::Context* _context, string* _packetName, list<Node*>* _packetMembers) : Node(_context)
    {
        packetName = _packetName;
        packetMembers = _packetMembers;
    }

    int NodePacket::getType()
    {
       return CsNodeType::packet;
    }

    size_t NodePacket::getHash(vector<Node*>* referencingStack)
    {
        if (referencingStack == NULL)
        {
            referencingStack = new vector<Node*>();
        }

        referencingStack->push_back(this);

        size_t packetHash = getHashCode(packetName);

        list<Node*>::iterator i = packetMembers->begin();
        list<Node*>::iterator end = packetMembers->end();
        for (; i != end; ++i)
        {
            combineHashCode(packetHash, (*i)->getHash(referencingStack));
        }

        return packetHash;
    }

    string* NodePacket::getParsed(int as)
    {
        stringstream parsed;

        switch (as)
        {
            case CsParseAs::Default:
            {
                parsed << boost::format(TCS_PACKET_BEGIN) % *packetName;
                parsed << "\t<temp> packet hash: " << getHash(NULL) << "\n";
                list<Node*>::iterator i = packetMembers->begin();
                list<Node*>::iterator end = packetMembers->end();
                for (; i != end; ++i)
                {
                    parsed << "\t" << *((*i)->getParsed(CsParseAs::Default));
                }
                parsed << TCS_PACKET_END;
            }
            break;
        }

        return new string(parsed.str());
    }
// };

// class NodePacketMember : public Node
// {
//     Node* memberType;
//     Node* memberName;
//     public:
    NodePacketMember::NodePacketMember(Rakjin::Context* _context, Node* _memberType, Node* _memberName) : Node(_context)
    {
        memberType = _memberType;
        memberName = _memberName;
    }

    int NodePacketMember::getType()
    {
       return CsNodeType::packetMember;
    }

    size_t NodePacketMember::getHash(vector<Node*>* referencingStack)
    {
        assert(referencingStack != NULL);

        size_t packetMemberHash = memberType->getHash(referencingStack);
        combineHashCode(packetMemberHash, memberName->getHash(referencingStack));

        return packetMemberHash;
    }

    string* NodePacketMember::getParsed(int as)
    {
        stringstream parsed;

        switch (as)
        {
            case CsParseAs::Default:
            {
                parsed << boost::format(TCS_PACKET_MEMBER_AS_DEFAULT)
                    % *(memberType->getParsed(CsParseAs::Default))
                    % *(memberName->getParsed(CsParseAs::Default));
            }
            break;
        }

        return new string(parsed.str());
    }
// };

// class NodePacketMemberType : public Node
// {
//     int typeType; // one of PRIMITIVE_DATA_TYPE, REFERENCE_DATA_TYPE, MAP, LIST
//     string* value; // "int", "bool", ..., "MyPacket", "Skill" or NULL when type is MAP or LIST
//     Node* generic1; // LIST<generic1>
//     Node* generic2; // MAP <generic1, generic2>
//     Node* generic3; // reserved
//     public:
    NodePacketMemberType::NodePacketMemberType(Rakjin::Context* _context, int _type, string* _value) : Node(_context)
    {
        typeType = _type;
        value = _value;
        generic1 = NULL;
        generic2 = NULL;
        generic3 = NULL;
    }

    NodePacketMemberType::NodePacketMemberType(Rakjin::Context* _context, int _type, Node* _generic1) : Node(_context)
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = NULL;
        generic3 = NULL;
    }

    NodePacketMemberType::NodePacketMemberType(Rakjin::Context* _context, int _type, Node* _generic1, Node* _generic2) : Node(_context)
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = _generic2;
        generic3 = NULL;
    }

    NodePacketMemberType::NodePacketMemberType(Rakjin::Context* _context, int _type, Node* _generic1, Node* _generic2, Node* _generic3) : Node(_context)
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = _generic2;
        generic3 = _generic3;
    }

    int NodePacketMemberType::getType()
    {
       return CsNodeType::packetMemberType;
    }

    size_t NodePacketMemberType::getHash(vector<Node*>* referencingStack)
    {
        assert(referencingStack != NULL);

        size_t packetMemberTypeHash = 0;
        switch(typeType)
        {
            case Parser::token::PRIMITIVE_DATA_TYPE:
            packetMemberTypeHash = getHashCode(value);
            break;
        }

        return packetMemberTypeHash;
    }

    string* NodePacketMemberType::getParsed(int as)
    {
        stringstream parsed;

        switch (typeType) {

            case Parser::token::PRIMITIVE_DATA_TYPE:
            parsed << *value;
            break;

            case Parser::token::REFERENCE_DATA_TYPE:
            parsed << *value;
            break;

            case Parser::token::MAP:
            parsed << "Dictionary";
            parsed << "<" << *(generic1->getParsed(CsParseAs::Default)) << ", " << *(generic2->getParsed(CsParseAs::Default)) << ">";
            break;

            case Parser::token::LIST:
            parsed << "List";
            parsed << "<" << *(generic1->getParsed(CsParseAs::Default)) << ">";
            break;

            default:
            throw(runtime_error("Unknown NodePacketMemberType type."));
            break;
        }

        return new string(parsed.str());
    }
// };

// class NodePacketMemberName : public Node
// {
//     string* value;
//     public:
    NodePacketMemberName::NodePacketMemberName(Rakjin::Context* _context, string* _value) : Node(_context)
    {
        value = _value;
    }

    int NodePacketMemberName::getType()
    {
       return CsNodeType::packetMemberName;
    }

    size_t NodePacketMemberName::getHash(vector<Node*>* referencingStack)
    {
        size_t packetMemberNameHash = getHashCode(value);
        return packetMemberNameHash;
    }

    string* NodePacketMemberName::getParsed(int as)
    {
       return value;
    }
// };

