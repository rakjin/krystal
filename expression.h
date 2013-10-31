#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <list>
#include <ostream>
#include <stdexcept>

class Node
{
public:
    virtual ~Node()
    {
    }

    std::string     getType() { return 0; };
    std::string*	getParsed() { return 0; };

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



#endif // EXPRESSION_H
