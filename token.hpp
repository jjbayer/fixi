#pragma once

#include <sstream>
#include <string>
#include <vector>


class Token
{
public:
    enum class Type {
        NAME,
        INTEGER,
        TIMES,
        PLUS,
        LIST_OPEN,
        LIST_CLOSE,
        DIVIDE
    };
    static const std::vector<std::string> TypeNames;

    Token(Type type, const std::string & string)
        : type_(type)
    {
        switch(type) {

        case Type::NAME:
            string_ = string;
            break;

        case Type::INTEGER: {
            std::stringstream ss;
            ss << string;
            ss >> integer_;
            break;
        }
        default:
            // Nothing to do
            break;

        }
    }

    Type type() const {return type_; }
    const std::string & typeName() const { return TypeNames[static_cast<int>(type_)]; }
    std::string toString() {
        switch(type_) {

        case Type::NAME:
            return string_;

        case Type::INTEGER: {
            return string_;
        }
        default:
            return typeName();

        }
    }

private:

    Type type_;
    std::string string_;
    int integer_;
};
