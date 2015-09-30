//
// Created by Rutvik Choudhary on 8/12/15.
//

#include "chem_util.h"
#include "element.h"
#include "reaction.h"

using namespace std;

template <>
string toString(const Element& e)
{
    for (auto it = elementObjectLookupTable.cbegin(); it != elementObjectLookupTable.cend(); it++)
    {
        if (it->second == e)
        {
            return it->first;
        }
    }
    return "n/a";
}


CouldNotBalanceException::CouldNotBalanceException(const Reaction& r, std::string desc) :
    runtime_error(desc)
{
    message = "ERROR: " + string(runtime_error::what()) + "\n   reaction: " + r.toString();
}


const char* CouldNotBalanceException::what() const noexcept
{
    return message.c_str();
}