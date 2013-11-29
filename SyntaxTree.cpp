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
using namespace Krystal;

// class Node
// {
// public:
    Krystal::Node::Node(Context* _context)
    {
        context = _context;
    }

    Krystal::Node::~Node()
    {
    }

    int Krystal::Node::getType() { return 0; } //TODO: remove getType() if unnecessary
    size_t Krystal::Node::getHash(vector<Node*>* referencingStack) { return 0; }
    string* Krystal::Node::getParsed(int as) { return 0; }

// };

// class NodeKst : public Node
// {
    // list<Node*>* commands;
    // string* fileName;
    // public:
    Krystal::NodeKst::NodeKst(Context* _context, list<Node*>* _commands, string* _fileName) : Node(_context)
    {
        commands = _commands;
        fileName = _fileName;
    }

    int Krystal::NodeKst::getType()
    {
       return CsNodeType::kst;
    }

    size_t Krystal::NodeKst::getHash(vector<Node*>* referencingStack)
    {
        return 0;
    }

    string* Krystal::NodeKst::getParsed(int as)
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
    Krystal::NodeInclude::NodeInclude(Context* _context, string* _value) : Node(_context)
    {
        value = _value;
    }

    int Krystal::NodeInclude::getType()
    {
       return CsNodeType::include;
    }

    size_t Krystal::NodeInclude::getHash(vector<Node*>* referencingStack)
    {
        return 0;
    }

    string* Krystal::NodeInclude::getParsed(int as)
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
    Krystal::NodePacket::NodePacket(Context* _context, string* _packetName, list<Node*>* _packetMembers) : Node(_context)
    {
        packetName = _packetName;
        packetMembers = _packetMembers;
    }

    int Krystal::NodePacket::getType()
    {
       return CsNodeType::packet;
    }

    size_t Krystal::NodePacket::getHash(vector<Node*>* referencingStack)
    {
        if (referencingStack == NULL)
        {
            referencingStack = new vector<Node*>();
        }
        else // check circular reference
        {
            vector<Node*>::iterator end = referencingStack->end();
            for (vector<Node*>::iterator i = referencingStack->begin(); i != end; i++)
            {
                if (*i == this)
                {
                    throw(runtime_error("Circular reference between packets"));
                }
            }
        }

        referencingStack->push_back(this);

        size_t packetHash = getHashCode(packetName);

        list<Node*>::iterator i = packetMembers->begin();
        list<Node*>::iterator end = packetMembers->end();
        for (; i != end; ++i)
        {
            combineHashCode(packetHash, (*i)->getHash(referencingStack));
        }

        referencingStack->pop_back();

        return packetHash;
    }

    string* Krystal::NodePacket::getParsed(int as)
    {
        stringstream parsed;

        switch (as)
        {
            case CsParseAs::Default:
            {
                parsed << boost::format(TCS_PACKET_BEGIN) % *packetName;

                {
                    stringstream body;

                    body << boost::format(TCS_PACKET_ID_FIELD) % ((TCS_PACKET_ID_TYPE)getHash(NULL));

                    list<Node*>::iterator i = packetMembers->begin();
                    list<Node*>::iterator end = packetMembers->end();
                    for (; i != end; ++i)
                    {
                        body << *((*i)->getParsed(CsParseAs::Default));
                    }

                    parsed << *(indent(new string(body.str())));
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
    Krystal::NodePacketMember::NodePacketMember(Context* _context, Node* _memberType, Node* _memberName) : Node(_context)
    {
        memberType = _memberType;
        memberName = _memberName;
    }

    int Krystal::NodePacketMember::getType()
    {
       return CsNodeType::packetMember;
    }

    size_t Krystal::NodePacketMember::getHash(vector<Node*>* referencingStack)
    {
        assert(referencingStack != NULL);

        size_t packetMemberHash = memberType->getHash(referencingStack);
        combineHashCode(packetMemberHash, memberName->getHash(referencingStack));

        return packetMemberHash;
    }

    string* Krystal::NodePacketMember::getParsed(int as)
    {
        stringstream parsed;

        switch (as)
        {
            case CsParseAs::Default:
            {
                parsed << boost::format(TCS_PACKET_MEMBER_AS_DEFAULT)
                    % *(memberType->getParsed(CsParseAs::Default))
                    % *(memberName->getParsed(CsParseAs::Default))
                    % *(memberType->getParsed(CsParseAs::Initialization));
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
    Krystal::NodePacketMemberType::NodePacketMemberType(Context* _context, int _type, string* _value) : Node(_context)
    {
        typeType = _type;
        value = _value;
        generic1 = NULL;
        generic2 = NULL;
        generic3 = NULL;
    }

    Krystal::NodePacketMemberType::NodePacketMemberType(Context* _context, int _type, Node* _generic1) : Node(_context)
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = NULL;
        generic3 = NULL;
    }

    Krystal::NodePacketMemberType::NodePacketMemberType(Context* _context, int _type, Node* _generic1, Node* _generic2) : Node(_context)
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = _generic2;
        generic3 = NULL;
    }

    Krystal::NodePacketMemberType::NodePacketMemberType(Context* _context, int _type, Node* _generic1, Node* _generic2, Node* _generic3) : Node(_context)
    {
        typeType = _type;
        value = NULL;
        generic1 = _generic1;
        generic2 = _generic2;
        generic3 = _generic3;
    }

    int Krystal::NodePacketMemberType::getType()
    {
       return CsNodeType::packetMemberType;
    }

    size_t Krystal::NodePacketMemberType::getHash(vector<Node*>* referencingStack)
    {
        assert(referencingStack != NULL);

        size_t packetMemberTypeHash = 0;
        switch(typeType)
        {
            case Parser::token::PRIMITIVE_DATA_TYPE:
            {
                packetMemberTypeHash = getHashCode(value);
            }
            break;

            case Parser::token::REFERENCE_DATA_TYPE:
            {
                // lookup Context::declarations table
                Node* typePacketNode = context->getDeclarationNode(value);
                if (typePacketNode == NULL)
                {
                    throw(runtime_error("No such packet type."));
                }
                packetMemberTypeHash = typePacketNode->getHash(referencingStack);
            }
            break;

            case Parser::token::MAP:
            {
                // must be specified this is map type
                // in case of other generic<t1, t2> added.
                packetMemberTypeHash = getHashCode((int)Parser::token::MAP);
                combineHashCode(packetMemberTypeHash, generic1->getHash(referencingStack));
                combineHashCode(packetMemberTypeHash, generic2->getHash(referencingStack));
            }
            break;

            case Parser::token::LIST:
            {
                // must be specified this is list type
                // in case of other generic<t> added.
                packetMemberTypeHash = getHashCode((int)Parser::token::LIST);
                combineHashCode(packetMemberTypeHash, generic1->getHash(referencingStack));
            }
            break;
        }

        return packetMemberTypeHash;
    }

    string* Krystal::NodePacketMemberType::getParsed(int as)
    {
        stringstream parsed;

        switch (as)
        {
            case CsParseAs::Default:
            {
                switch (typeType) {
                    case Parser::token::PRIMITIVE_DATA_TYPE:
                    case Parser::token::REFERENCE_DATA_TYPE:
                    {
                        parsed << *value;
                    }
                    break;

                    case Parser::token::MAP:
                    {
                        parsed << "Dictionary";
                        parsed << "<" << *(generic1->getParsed(CsParseAs::Default)) << ", " << *(generic2->getParsed(CsParseAs::Default)) << ">";
                    }
                    break;

                    case Parser::token::LIST:
                    {
                        parsed << "List";
                        parsed << "<" << *(generic1->getParsed(CsParseAs::Default)) << ">";
                    }
                    break;

                    default:
                    {
                        throw(runtime_error("Unknown NodePacketMemberType type."));
                    }
                    break;
                }
            }
            break;

            case CsParseAs::Initialization:
            {
                switch (typeType) {
                    case Parser::token::PRIMITIVE_DATA_TYPE:
                    {
                        parsed << "";
                    }
                    break;

                    case Parser::token::REFERENCE_DATA_TYPE:
                    case Parser::token::MAP:
                    case Parser::token::LIST:
                    {
                        parsed << boost::format(TCS_PACKET_MEMBER_TYPE_NEW_AS_INITIALIZATION)
                            % *(getParsed(CsParseAs::Default));
                    }
                    break;

                    default:
                    {
                        throw(runtime_error("Unknown NodePacketMemberType type."));
                    }
                    break;
                }
            }
            break;
        }

        return new string(parsed.str());
    }
// };

// class NodePacketMemberName : public Node
// {
//     string* value;
//     public:
    Krystal::NodePacketMemberName::NodePacketMemberName(Context* _context, string* _value) : Node(_context)
    {
        value = _value;
    }

    int Krystal::NodePacketMemberName::getType()
    {
       return CsNodeType::packetMemberName;
    }

    size_t Krystal::NodePacketMemberName::getHash(vector<Node*>* referencingStack)
    {
        size_t packetMemberNameHash = getHashCode(value);
        return packetMemberNameHash;
    }

    string* Krystal::NodePacketMemberName::getParsed(int as)
    {
       return value;
    }
// };

