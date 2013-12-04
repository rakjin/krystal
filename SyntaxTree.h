#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <list>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "Context.h"
    namespace Krystal { class Context; }

using namespace std;

namespace Krystal
{

    class Node;

    class NodeKst;
        class NodeInclude;
        class NodePacket;
            class NodePacketMember;
                class NodePacketMemberType;
                class NodePacketMemberName;
}

class Krystal::Node
{
    protected:
    Context* context;
    
    public:
    explicit Node(Context* _context);
    virtual ~Node();
    virtual int getType(); //TODO: remove getType() if unnecessary
    virtual size_t getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class Krystal::NodeKst : public Krystal::Node
{
    list<Node*>* commands;
    string* fileName;
    public:
    explicit NodeKst(Context* _context, list<Node*>* _commands, string* fileName);
    virtual int getType();
    virtual size_t getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class Krystal::NodeInclude : public Krystal::Node
{
    string* value;
    public:
    explicit NodeInclude(Context* _context, string* _value);
    virtual int getType();
    virtual size_t getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class Krystal::NodePacket : public Krystal::Node
{
    string* packetName;
    list<NodePacketMember*>* packetMembers;
    string* uri;
    public:
    explicit NodePacket(Context* _context, string* _packetName, list<NodePacketMember*>* _packetMembers);
    explicit NodePacket(Context* _context, string* _packetName, list<NodePacketMember*>* _packetMembers, string* _uri);
    virtual int getType();
    virtual size_t getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class Krystal::NodePacketMember : public Krystal::Node
{
    Node* memberType;
    Node* memberName;
    public:
    explicit NodePacketMember(Context* _context, Node* _memberType, Node* _memberName);
    virtual int getType();
    virtual size_t getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
    virtual string* getName();
};

class Krystal::NodePacketMemberType : public Krystal::Node
{
    int typeType; // one of PRIMITIVE_DATA_TYPE, REFERENCE_DATA_TYPE, MAP, LIST
    string* value; // "int", "bool", ..., "MyPacket", "Skill" or NULL when type is MAP or LIST
    Node* generic1; // LIST<generic1>
    Node* generic2; // MAP <generic1, generic2>
    Node* generic3; // reserved
    public:
    explicit NodePacketMemberType(Context* _context, int _type, string* _value);
    explicit NodePacketMemberType(Context* _context, int _type, Node* _generic1);
    explicit NodePacketMemberType(Context* _context, int _type, Node* _generic1, Node* _generic2);
    explicit NodePacketMemberType(Context* _context, int _type, Node* _generic1, Node* _generic2, Node* _generic3);
    virtual int getType();
    virtual size_t getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class Krystal::NodePacketMemberName : public Krystal::Node
{
    string* value;
    public:
    explicit NodePacketMemberName(Context* _context, string* _value);
    virtual int getType();
    virtual size_t getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};


#endif // SYNTAXTREE_H
