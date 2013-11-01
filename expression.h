#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <list>
#include <sstream>
#include <iostream>


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
    virtual ~Node()
    {
    }

    virtual std::string     getType() { return 0; }; //TODO: remove getType() if unnecessary
    virtual std::string*	getParsed() { return 0; };

};

class NodeKst : public Node
{
    std::list<Node*>* commands;
    
    public:
    explicit NodeKst(std::list<Node*>* _commands) : Node()
    {
        commands = _commands;
    }

    virtual std::string getType()
    {
       return "KST";
    }

    virtual std::string* getParsed()
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
};

class NodeInclude : public Node
{
    std::string* value;

    public:
    explicit NodeInclude(std::string* _value) : Node()
    {
        value = _value;
    }

    virtual std::string getType()
    {
       return "INCLUDE";
    }

    virtual std::string* getParsed()
    {
        std::stringstream parsed;

        parsed << "#include \"";
        parsed << *(value);
        parsed << "\"\n";

        return new std::string(parsed.str());
    }
};

class NodePacket : public Node
{
    Node* packetName;
    std::list<Node*>* packetMembers;

    public:
    explicit NodePacket(Node* _packetName, std::list<Node*>* _packetMembers) : Node()
    {
        packetName = _packetName;
        packetMembers = _packetMembers;
    }

    virtual std::string getType()
    {
       return "Packet";
    }

    virtual std::string* getParsed()
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
};

class NodePacketMember : public Node
{
    Node* memberType;
    Node* memberName;

    public:
    explicit NodePacketMember(Node* _packetName, Node* _packetMembers) : Node()
    {
        memberType = _packetName;
        memberName = _packetMembers;
    }

    virtual std::string getType()
    {
       return "PacketMember";
    }

    virtual std::string* getParsed()
    {
        std::stringstream parsed;

        parsed << *(memberType->getParsed());
        parsed << " ";
        parsed << *(memberName->getParsed());
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
