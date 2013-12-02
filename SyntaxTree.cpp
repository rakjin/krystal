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

#define YES "Y"
#define NO  "N"
#define EQUAL 0

using namespace std;
using namespace boost;
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

        parsed << format(TCS_HEADER) % *fileName;
        parsed << TCS_USINGS;

        string namespaceByFileName = fileName->substr(0, fileName->find("."));

        parsed << format(TCS_NAMESPACE_BEGIN) % namespaceByFileName;

        list<Node*>::iterator i = commands->begin();
        list<Node*>::iterator end = commands->end();

        string* temp;

        for (; i != end; i++)
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
                    throw string("Circular reference between packets");
                }
            }
        }

        referencingStack->push_back(this);

        size_t packetHash = getHashCode(packetName);

        list<Node*>::iterator i = packetMembers->begin();
        list<Node*>::iterator end = packetMembers->end();
        for (; i != end; i++)
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
                parsed << format(TCS_PACKET_BEGIN) % *packetName;

                {
                    stringstream body;

                    body << format(TCS_PACKET_ID_FIELD) % ((TCS_PACKET_ID_TYPE)getHash(NULL));

                    body << TCS_PACKET_COOKIE_FIELD;

                    // Member Variables
                    list<Node*>::iterator i = packetMembers->begin();
                    list<Node*>::iterator end = packetMembers->end();
                    for (; i != end; i++)
                    {
                        body << *((*i)->getParsed(CsParseAs::Default));
                    }

                    body << format(TCS_PACKET_CONSTRUCTOR) % *packetName;

                    body << TCS_PACKET_GET_ID;

                    body << TCS_PACKET_SET_COOKIE;
                    body << TCS_PACKET_GET_COOKIE;

                    body << TCS_PACKET_GET_LENGTH_BEGIN;
                    {
                        stringstream bodyGetLengthBlock;
                        for (i = packetMembers->begin(); i != end; i++)
                        {
                            bodyGetLengthBlock << *((*i)->getParsed(CsParseAs::GetLength));
                        }

                        body << *(indent(new string(bodyGetLengthBlock.str())));
                    }
                    body << TCS_PACKET_GET_LENGTH_END;

                    body << format(TCS_PACKET_GET_NAME) % *packetName;

                    body << TCS_PACKET_WRITE_BEGIN;
                    {
                        stringstream bodyWriteBlock;
                        for (i = packetMembers->begin(); i != end; i++)
                        {
                            bodyWriteBlock << *((*i)->getParsed(CsParseAs::Write));
                        }

                        body << *(indent(new string(bodyWriteBlock.str())));
                    }
                    body << TCS_PACKET_WRITE_END;

                    body << TCS_PACKET_READ_BEGIN;
                    {
                        stringstream bodyReadBlock;
                        for (i = packetMembers->begin(); i != end; i++)
                        {
                            bodyReadBlock << *((*i)->getParsed(CsParseAs::Read));
                        }

                        body << *(indent(new string(bodyReadBlock.str())));
                    }
                    body << TCS_PACKET_READ_END;

                    body << TCS_PACKET_FIXED_METHODS_AFTER_READ;

                    body << format(TCS_PACKET_PARSE_JSON_OBJECT_DATA_BEGIN) % packetMembers->size();
                    {
                        stringstream bodyParseJsonObjectDataBlock;
                        for (i = packetMembers->begin(); i != end; i++)
                        {
                            bodyParseJsonObjectDataBlock << *((*i)->getParsed(CsParseAs::ParseJsonObjectData));
                        }

                        body << *(indent(new string(bodyParseJsonObjectDataBlock.str())));
                    }
                    body << TCS_PACKET_PARSE_JSON_OBJECT_DATA_END;

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
                parsed << format(TCS_PACKET_MEMBER_AS_DEFAULT)
                    % *(memberType->getParsed(CsParseAs::Default))
                    % *(memberName->getParsed(CsParseAs::Default))
                    % *(memberType->getParsed(CsParseAs::Initialization));
            }
            break;

            case CsParseAs::GetLength:
            {
                int typeType = memberType->getType();
                switch (typeType)
                {
                    case CsNodeType::packetMemberTypePrimitive:
                    {
                        string* serializerName = lookupSerializerName(memberType->getParsed(CsParseAs::Default));
                        parsed << format(TCS_PACKET_MEMBER_AS_GET_LENGTH_PRIMITIVE)
                            % *serializerName
                            % *(memberName->getParsed(CsParseAs::Default));
                    }
                    break;

                    case CsNodeType::packetMemberTypeReference:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_GET_LENGTH_REFERENCE)
                            % *(memberName->getParsed(CsParseAs::Default));
                    }
                    break;

                    case CsNodeType::packetMemberTypeMap:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_GET_LENGTH_MAP)
                            % *(memberType->getParsed(CsParseAs::GenericType1))
                            % *(memberType->getParsed(CsParseAs::GenericType2))
                            % *(memberName->getParsed(CsParseAs::Default))
                            % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName1))
                            % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName2));
                    }
                    break;

                    case CsNodeType::packetMemberTypeList:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_GET_LENGTH_LIST)
                            % *(memberType->getParsed(CsParseAs::GenericType1))
                            % *(memberName->getParsed(CsParseAs::Default))
                            % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName1));
                    }
                    break;
                }
            }
            break;

            case CsParseAs::Write:
            {
                int typeType = memberType->getType();
                switch (typeType)
                {
                    case CsNodeType::packetMemberTypePrimitive:
                    {
                        string* serializerName = lookupSerializerName(memberType->getParsed(CsParseAs::Default));
                        parsed << format(TCS_PACKET_MEMBER_AS_WRITE_PRIMITIVE)
                            % *serializerName
                            % *(memberName->getParsed(CsParseAs::Default));
                    }
                    break;

                    case CsNodeType::packetMemberTypeReference:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_WRITE_REFERENCE)
                            % *(memberName->getParsed(CsParseAs::Default));
                    }
                    break;

                    case CsNodeType::packetMemberTypeMap:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_WRITE_MAP_BEGIN)
                            % *(memberName->getParsed(CsParseAs::Default))
                            % *(memberType->getParsed(CsParseAs::GenericType1))
                            % *(memberType->getParsed(CsParseAs::GenericType2));

                        stringstream parsedWriteBlock;

                        // if map<primitive, CUSTOM>
                        string* isPrimitiveTypeValue = memberType->getParsed(CsParseAs::IsPrimitiveTypeValue);
                        if (isPrimitiveTypeValue->compare(YES) == EQUAL)
                        {
                            parsedWriteBlock << format(TCS_PACKET_MEMBER_AS_WRITE_MAP_PRIMITIVE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName1))
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName2));
                        }
                        else // map<primitive, primitive>
                        {
                            parsedWriteBlock << TCS_PACKET_MEMBER_AS_WRITE_MAP_REFERENCE_VALUE;
                        }

                        parsed << *(indent(new string(parsedWriteBlock.str())));

                        parsed << TCS_PACKET_MEMBER_AS_WRITE_MAP_END;
                    }
                    break;

                    case CsNodeType::packetMemberTypeList:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_WRITE_LIST_BEGIN)
                            % *(memberName->getParsed(CsParseAs::Default))
                            % *(memberType->getParsed(CsParseAs::GenericType1));

                        stringstream parsedWriteBlock;

                        // if list<CUSTOM>
                        string* isPrimitiveTypeValue = memberType->getParsed(CsParseAs::IsPrimitiveTypeValue);
                        if (isPrimitiveTypeValue->compare(YES) == EQUAL)
                        {
                            parsedWriteBlock << format(TCS_PACKET_MEMBER_AS_WRITE_LIST_PRIMITIVE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName1));
                        }
                        else // list<primitive>
                        {
                            parsedWriteBlock << format(TCS_PACKET_MEMBER_AS_WRITE_LIST_REFERENCE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName1));
                        }

                        parsed << *(indent(new string(parsedWriteBlock.str())));

                        parsed << TCS_PACKET_MEMBER_AS_WRITE_LIST_END;
                    }
                    break;
                }
            }
            break;

            case CsParseAs::Read:
            {
                int typeType = memberType->getType();
                switch (typeType)
                {
                    case CsNodeType::packetMemberTypePrimitive:
                    {
                        string* serializerName = lookupSerializerName(memberType->getParsed(CsParseAs::Default));
                        parsed << format(TCS_PACKET_MEMBER_AS_READ_PRIMITIVE)
                            % *(memberName->getParsed(CsParseAs::Default))
                            % *serializerName;
                    }
                    break;

                    case CsNodeType::packetMemberTypeReference:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_READ_REFERENCE)
                            % *(memberName->getParsed(CsParseAs::Default));
                    }
                    break;

                    case CsNodeType::packetMemberTypeMap:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_READ_MAP_BEGIN)
                            % *(memberName->getParsed(CsParseAs::Default));

                        stringstream parsedReadBlock;

                        // if map<primitive, CUSTOM>
                        string* isPrimitiveTypeValue = memberType->getParsed(CsParseAs::IsPrimitiveTypeValue);
                        if (isPrimitiveTypeValue->compare(YES) == EQUAL)
                        {
                            parsedReadBlock << format(TCS_PACKET_MEMBER_AS_READ_MAP_PRIMITIVE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericType1))
                                % *(memberName->getParsed(CsParseAs::Default))
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName1))
                                % *(memberType->getParsed(CsParseAs::GenericType2))
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName2));
                        }
                        else // map<primitive, primitive>
                        {
                            parsedReadBlock << format(TCS_PACKET_MEMBER_AS_READ_MAP_REFERENCE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericType1))
                                % *(memberName->getParsed(CsParseAs::Default))
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName1))
                                % *(memberType->getParsed(CsParseAs::GenericType2));
                        }

                        parsed << *(indent(new string(parsedReadBlock.str())));

                        parsed << TCS_PACKET_MEMBER_AS_READ_MAP_END;
                    }
                    break;

                    case CsNodeType::packetMemberTypeList:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_READ_LIST_BEGIN)
                            % *(memberName->getParsed(CsParseAs::Default));

                        stringstream parsedReadBlock;

                        // if list<CUSTOM>
                        string* isPrimitiveTypeValue = memberType->getParsed(CsParseAs::IsPrimitiveTypeValue);
                        if (isPrimitiveTypeValue->compare(YES) == EQUAL)
                        {
                            parsedReadBlock << format(TCS_PACKET_MEMBER_AS_READ_LIST_PRIMITIVE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericType1))
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName1))
                                % *(memberName->getParsed(CsParseAs::Default));
                        }
                        else // list<primitive>
                        {
                            parsedReadBlock << format(TCS_PACKET_MEMBER_AS_READ_LIST_REFERENCE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericType1))
                                % *(memberName->getParsed(CsParseAs::Default));
                        }

                        parsed << *(indent(new string(parsedReadBlock.str())));

                        parsed << TCS_PACKET_MEMBER_AS_READ_LIST_END;
                    }
                    break;
                }
            }
            break;

            case CsParseAs::ParseJsonObjectData:
            {
                int typeType = memberType->getType();
                switch (typeType)
                {
                    case CsNodeType::packetMemberTypePrimitive:
                    {
                        string* serializerName = lookupSerializerName(memberType->getParsed(CsParseAs::Default));
                        parsed << format(TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_PRIMITIVE)
                            % *(memberName->getParsed(CsParseAs::Default))
                            % *serializerName;
                    }
                    break;

                    case CsNodeType::packetMemberTypeReference:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_REFERENCE)
                            % *(memberName->getParsed(CsParseAs::Default));
                    }
                    break;

                    case CsNodeType::packetMemberTypeMap:
                    {
                        parsed << format(TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_MAP_BEGIN)
                            % *(memberName->getParsed(CsParseAs::Default));

                        stringstream parsedParseJsonObjectDataBlock;

                        // if map<primitive, CUSTOM>
                        string* isPrimitiveTypeValue = memberType->getParsed(CsParseAs::IsPrimitiveTypeValue);
                        if (isPrimitiveTypeValue->compare(YES) == EQUAL)
                        {
                            // generic1, convert1, name, generic2, serializer2
                            parsedParseJsonObjectDataBlock << format(TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_MAP_PRIMITIVE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericType1))
                                % *(memberType->getParsed(CsParseAs::GenericTypeConvertPhrase1))
                                % *(memberName->getParsed(CsParseAs::Default))
                                % *(memberType->getParsed(CsParseAs::GenericType2))
                                % *(memberType->getParsed(CsParseAs::GenericTypeSerializerName2));
                        }
                        else // map<primitive, primitive>
                        {
                            // generic1, convert1, name, generic2
                            parsedParseJsonObjectDataBlock << format(TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_MAP_REFERENCE_VALUE)
                                % *(memberType->getParsed(CsParseAs::GenericType1))
                                % *(memberType->getParsed(CsParseAs::GenericTypeConvertPhrase1))
                                % *(memberName->getParsed(CsParseAs::Default))
                                % *(memberType->getParsed(CsParseAs::GenericType2));
                        }

                        parsed << *(indent(new string(parsedParseJsonObjectDataBlock.str()), 2));

                        parsed << TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_MAP_END;
                    }
                    break;
                }
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

        if (typeType == Parser::token::MAP
            &&
            generic1->getType() != CsNodeType::packetMemberTypePrimitive)
        {
            throw string("Key of map must be primitive type.");
        }
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
        switch (typeType)
        {
            case Parser::token::PRIMITIVE_DATA_TYPE:
            {
                return CsNodeType::packetMemberTypePrimitive;
            }
            break;
            
            case Parser::token::REFERENCE_DATA_TYPE:
            {
                return CsNodeType::packetMemberTypeReference;
            }
            break;
            
            case Parser::token::MAP:
            {
                return CsNodeType::packetMemberTypeMap;
            }
            break;
            
            case Parser::token::LIST:
            {
                return CsNodeType::packetMemberTypeList;
            }
            break;
        }
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
                    throw string("No such packet type.");
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
                switch (typeType)
                {
                    case Parser::token::PRIMITIVE_DATA_TYPE:
                    case Parser::token::REFERENCE_DATA_TYPE:
                    {
                        parsed << *value;
                    }
                    break;

                    case Parser::token::MAP:
                    {
                        parsed << format(TCS_PACKET_MEMBER_TYPE_MAP_AS_DEFAULT)
                            % *(generic1->getParsed(CsParseAs::Default))
                            % *(generic2->getParsed(CsParseAs::Default));
                    }
                    break;

                    case Parser::token::LIST:
                    {
                        parsed << format(TCS_PACKET_MEMBER_TYPE_LIST_AS_DEFAULT)
                            % *(generic1->getParsed(CsParseAs::Default));
                    }
                    break;

                    default:
                    {
                        throw string("Unknown NodePacketMemberType type.");
                    }
                    break;
                }
            }
            break;

            case CsParseAs::GenericType1:
            {
                parsed << *(generic1->getParsed(CsParseAs::Default));
            }
            break;

            case CsParseAs::GenericType2:
            {
                parsed << *(generic2->getParsed(CsParseAs::Default));
            }
            break;

            case CsParseAs::GenericType3:
            {
                parsed << *(generic3->getParsed(CsParseAs::Default));
            }
            break;

            case CsParseAs::GenericTypeSerializerName1:
            {
                parsed << *(generic1->getParsed(CsParseAs::SerializerName));
            }
            break;

            case CsParseAs::GenericTypeSerializerName2:
            {
                parsed << *(generic2->getParsed(CsParseAs::SerializerName));
            }
            break;

            case CsParseAs::GenericTypeSerializerName3:
            {
                parsed << *(generic3->getParsed(CsParseAs::SerializerName));
            }
            break;

            case CsParseAs::GenericTypeConvertPhrase1:
            {
                parsed << *(generic1->getParsed(CsParseAs::ConvertPhrase));
            }
            break;

            case CsParseAs::GenericTypeConvertPhrase2:
            {
                parsed << *(generic2->getParsed(CsParseAs::ConvertPhrase));
            }
            break;

            case CsParseAs::GenericTypeConvertPhrase3:
            {
                parsed << *(generic3->getParsed(CsParseAs::ConvertPhrase));
            }
            break;

            case CsParseAs::IsPrimitiveTypeValue:
            {
                if (typeType == Parser::token::MAP)
                {
                    if (generic2->getType() == CsNodeType::packetMemberTypePrimitive)
                    {
                        parsed << YES;
                    }
                    else
                    {
                        parsed << NO;
                    }
                }
                else if (typeType == Parser::token::LIST)
                {
                    if (generic1->getType() == CsNodeType::packetMemberTypePrimitive)
                    {
                        parsed << YES;
                    }
                    else
                    {
                        parsed << NO;
                    }
                }
                else
                {
                    throw string("CsParseAs::IsPrimitiveTypeValue passed to non-generic type.");
                }
            }
            break;

            case CsParseAs::SerializerName:
            {
                switch (typeType)
                {
                    case Parser::token::PRIMITIVE_DATA_TYPE:
                    {
                        string* serializerName = lookupSerializerName(value);
                        parsed << *serializerName;
                    }
                    break;

                    case Parser::token::REFERENCE_DATA_TYPE:
                    {
                        parsed << TCS_PACKET_CUSTOM_SERIALIZER_NAME;
                    }
                    break;

                    default:
                    {
                        throw string("Serializer not supported for nested generic type");
                    }
                    break;
                }
            }
            break;

            case CsParseAs::ConvertPhrase:
            {
                switch (typeType)
                {
                    case Parser::token::PRIMITIVE_DATA_TYPE:
                    {
                        string* convertPhrase = lookupConvertPhrase(value);
                        parsed << *convertPhrase;
                    }
                    break;

                    default:
                    {
                        throw string("Krystal assertion failed: ConvertPhrase called for non-primitive type");
                    }
                    break;
                }
            }
            break;

            case CsParseAs::Initialization:
            {
                switch (typeType)
                {
                    case Parser::token::PRIMITIVE_DATA_TYPE:
                    {
                        parsed << "";
                    }
                    break;

                    case Parser::token::REFERENCE_DATA_TYPE:
                    case Parser::token::MAP:
                    case Parser::token::LIST:
                    {
                        parsed << format(TCS_PACKET_MEMBER_TYPE_NEW_AS_INITIALIZATION)
                            % *(getParsed(CsParseAs::Default));
                    }
                    break;

                    default:
                    {
                        throw string("Unknown NodePacketMemberType type.");
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

