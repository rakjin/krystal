#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <list>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "Context.h"
    namespace Rakjin { class Context; }

using namespace std;

class Node;

class NodeKst;
    class NodeInclude;
    class NodePacket;
        class NodePacketMember;
            class NodePacketMemberType;
            class NodePacketMemberName;


class Node
{
    public:
    virtual ~Node();
    virtual int getType(); //TODO: remove getType() if unnecessary
    virtual int getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class NodeKst : public Node
{
    list<Node*>* commands;
    string* fileName;
    public:
    explicit NodeKst(list<Node*>* _commands, string* fileName);
    virtual int getType();
    virtual int getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class NodeInclude : public Node
{
    string* value;
    public:
    explicit NodeInclude(string* _value);
    virtual int getType();
    virtual int getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class NodePacket : public Node
{
    string* packetName;
    list<Node*>* packetMembers;
    public:
    explicit NodePacket(string* _packetName, list<Node*>* _packetMembers);
    virtual int getType();
    virtual int getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class NodePacketMember : public Node
{
    Node* memberType;
    Node* memberName;
    public:
    explicit NodePacketMember(Node* _memberType, Node* _memberName);
    virtual int getType();
    virtual int getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class NodePacketMemberType : public Node
{
    int typeType; // one of PRIMITIVE_DATA_TYPE, REFERENCE_DATA_TYPE, MAP, LIST
    string* value; // "int", "bool", ..., "MyPacket", "Skill" or NULL when type is MAP or LIST
    Node* generic1; // LIST<generic1>
    Node* generic2; // MAP <generic1, generic2>
    Node* generic3; // reserved
    public:
    explicit NodePacketMemberType(int _type, string* _value);
    explicit NodePacketMemberType(int _type, Node* _generic1);
    explicit NodePacketMemberType(int _type, Node* _generic1, Node* _generic2);
    explicit NodePacketMemberType(int _type, Node* _generic1, Node* _generic2, Node* _generic3);
    virtual int getType();
    virtual int getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};

class NodePacketMemberName : public Node
{
    string* value;
    public:
    explicit NodePacketMemberName(string* _value);
    virtual int getType();
    virtual int getHash(vector<Node*>* referencingStack);
    virtual string* getParsed(int as);
};


#endif // SYNTAXTREE_H
