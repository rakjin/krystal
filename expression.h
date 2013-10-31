#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <list>
#include <ostream>
#include <stdexcept>

class Node
{
public:
    /// required for virtual functions. in the derived classes the operands are
    /// deleted.
    virtual ~Node()
    {
    }

    virtual std::string getType();
    virtual std::string	getParsed();

};

class NodeKst : public Node
{
    std::list<Node> children;
    
    public:
    /// construct a constant calculation node from a value
    explicit NodeKst() : Node()
    {
    }

    virtual std::string getType()
    {
       return "KST_FILE";
    }

    virtual std::string getParsed()
    {
       return "parsed contents";
    }
};

class NodeInclude : public Node
{
    public:
    /// construct a constant calculation node from a value
    explicit NodeInclude() : Node()
    {
    }

    virtual std::string getType()
    {
       return "INCLUDE";
    }

    virtual std::string getParsed()
    {
       return "#include \"somefile\"\n";
    }
};


class NodePacketMemberName : public Node
{
    std::string value;

    public:
    explicit NodePacketMemberName(std::string _value) : Node()
    {
        value = _value;
    }

    virtual std::string getType()
    {
       return "PacketMemberName";
    }

    virtual std::string getParsed()
    {
       return value;
    }
};


class NodePacketMemberType : public Node
{
    std::string value;

    public:
    explicit NodePacketMemberType(std::string _value) : Node()
    {
        value = _value;
    }

    virtual std::string getType()
    {
       return "PacketMemberType";
    }

    virtual std::string getParsed()
    {
       return value;
    }
};



#endif // EXPRESSION_H
