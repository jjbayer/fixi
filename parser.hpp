#pragma once
#include "token.hpp"

#include <memory>


using TokenIterator = std::vector<std::shared_ptr<Token> >::iterator;


class Ast
{
public:

    virtual const char * typeName() const = 0;

    const std::vector<std::unique_ptr<Ast>> & children() const { return children_; }

protected:
    std::vector<std::unique_ptr<Ast>> children_;

};


void print(const Ast & tree, const int indent = 0)
{
    for(int i = 0; i < indent; i++) {
        std::cout << "  ";
    }
    std::cout << tree.typeName() << std::endl;
    for( const auto & child : tree.children() ) {
        print(*child, indent + 1);
    }
}

class ListAssignment: public Ast
{
public:
    ListAssignment(TokenIterator & it, const TokenIterator & end)
    {
        do {
            it++;
        } while(it != end && (*it)->type() != Token::Type::LIST_SEPARATOR);
    }

    const char * typeName() const override { return "ASSIGNMENT"; }
};


class ListElement: public Ast
{
public:
    ListElement(TokenIterator & it, const TokenIterator & end)
    {
        children_.emplace_back(new ListAssignment(it, end));
        if(it!= end && (*it)->type() == Token::Type::LIST_SEPARATOR) {
            it++;
            children_.emplace_back(new ListElement(it, end));
        }
    }

    const char * typeName() const override { return "LIST_ELEMENT"; }
};


class ListBody: public Ast
{
public:
    ListBody(TokenIterator & it, const TokenIterator & end)
    {
        while(it != end) {
            it++;
            children_.emplace_back(new ListElement(it, end));
        }
    }

    const char * typeName() const override { return "LIST_BODY"; }
};


