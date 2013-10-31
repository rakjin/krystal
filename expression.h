#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <list>
#include <ostream>
#include <sstream>
#include <iostream>
#include <stdexcept>


class Node;

class NodeKst;
    class NodeInclude;
    class NodePacketMember;
        class NodePacketMemberType;
        class NodePacketMemberName;


class Node
{
public:
    virtual ~Node()
    {
    }

    virtual std::string     getType() { return 0; }; //TODO: remove getType() if unnecessary
    virtual std::string*	getParsed() { return 0; };

};

class NodeKst : public Node
{
    std::list<Node> children;
    
    public:
    explicit NodeKst() : Node()
    {
    }

    virtual std::string getType()
    {
       return "KST_FILE";
    }

    virtual std::string* getParsed()
    {
       return new std::string("");
    }
};

class NodeInclude : public Node
{
    public:
    explicit NodeInclude() : Node()
    {
    }

    virtual std::string getType()
    {
       return "INCLUDE";
    }

    virtual std::string* getParsed()
    {
        return new std::string("#include \"somefile\"\n");
    }
};

class NodePacketMember : public Node
{
    Node* nodeType;
    Node* nodeName;

    public:
    explicit NodePacketMember(Node* _nodeType, Node* _nodeName) : Node()
    {
        nodeType = _nodeType;
        nodeName = _nodeName;
    }

    virtual std::string getType()
    {
       return "PacketMember";
    }

    virtual std::string* getParsed()
    {
        std::stringstream parsed;

        parsed << *(nodeType->getParsed());
        parsed << " ";
        parsed << *(nodeName->getParsed());
        parsed << ";\n";

        return new std::string(parsed.str());
    }
};

class NodePacketMemberType : public Node
{
    std::string* value;

    public:
    explicit NodePacketMemberType(std::string* _value) : Node()
    {
        value = _value;
    }

    virtual std::string getType()
    {
       return "PacketMemberType";
    }

    virtual std::string* getParsed()
    {
       return value;
    }
};

class NodePacketMemberName : public Node
{
    std::string* value;

    public:
    explicit NodePacketMemberName(std::string* _value) : Node()
    {
        value = _value;
    }

    virtual std::string getType()
    {
       return "PacketMemberName";
    }

    virtual std::string* getParsed()
    {
       return value;
    }
};


#endif // EXPRESSION_H
