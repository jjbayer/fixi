#include "lookup.hpp"
#include "token.hpp"


LookupError::LookupError(const std::__cxx11::string &what)
    : Error(what)
{

}

const Token &Lookup::get(const std::__cxx11::string &name) const
{
    try {
        return data_.at(name);
    } catch(const std::out_of_range&) {
        throw LookupError(name);
    }
}

void Lookup::set(const std::string &name, const Token &token)
{
//    std::cout << "SET LOOKUP " << name << " " << token.toString() << std::endl;
    data_.insert(std::make_pair(name, token));
}
