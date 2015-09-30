//
// Created by Rutvik Choudhary on 8/12/15.
//

#pragma once

#include <string>
#include <iostream>

enum class Element;
class Reaction;

// exception to be thrown if an error occurs when trying to balance a reaction
class CouldNotBalanceException : std::runtime_error
{
public:
    CouldNotBalanceException(const Reaction& r, std::string desc);

    const char* what() const noexcept;

private:
    std::string message;
};

template <typename T>
std::string toString(const T& t)
{
    return t.toString();
}

template <>
std::string toString(const Element& e);


// returns a balanced reaction
// if verbose is true, then the steps performed are outputted to `out`
Reaction balance(Reaction& reaction, bool verbose = false,
                 std::ostream& out = std::cout) throw(CouldNotBalanceException);
