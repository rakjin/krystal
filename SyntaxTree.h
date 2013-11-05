#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <list>
#include <sstream>
#include <iostream>
#include <stdexcept>

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
    virtual std::string     getType(); //TODO: remove getType() if unnecessary
    virtual std::string*	getParsed();
};

class NodeKst : public Node
{
    std::list<Node*>* commands;
    public:
    explicit NodeKst(std::list<Node*>* _commands);
    virtual std::string getType();
    virtual std::string* getParsed();
};

class NodeInclude : public Node
{
    std::string* value;
    public:
    explicit NodeInclude(std::string* _value);
    virtual std::string getType();
    virtual std::string* getParsed();
};

class NodePacket : public Node
{
    Node* packetName;
    std::list<Node*>* packetMembers;
    public:
    explicit NodePacket(Node* _packetName, std::list<Node*>* _packetMembers);
    virtual std::string getType();
    virtual std::string* getParsed();
};

class NodePacketMember : public Node
{
    Node* memberType;
    Node* memberName;
    public:
    explicit NodePacketMember(Node* _packetName, Node* _packetMembers);
    virtual std::string getType();
    virtual std::string* getParsed();
};

class NodePacketMemberType : public Node
{
    int typeType; // one of PRIMITIVE_DATA_TYPE, REFERENCE_DATA_TYPE, MAP, LIST
    std::string* value; // "int", "bool", ..., "MyPacket", "Skill" or NULL when type is MAP or LIST
    Node* generic1; // LIST<generic1>
    Node* generic2; // MAP <generic1, generic2>
    Node* generic3; // reserved
    public:
    explicit NodePacketMemberType(int _type, std::string* _value);
    explicit NodePacketMemberType(int _type, Node* _generic1);
    explicit NodePacketMemberType(int _type, Node* _generic1, Node* _generic2);
    explicit NodePacketMemberType(int _type, Node* _generic1, Node* _generic2, Node* _generic3);
    virtual std::string getType();
    virtual std::string* getParsed();
};

class NodePacketMemberName : public Node
{
    std::string* value;
    public:
    explicit NodePacketMemberName(std::string* _value);
    virtual std::string getType();
    virtual std::string* getParsed();
};


#endif // SYNTAXTREE_H
