//
// Created by Rutvik Choudhary on 7/25/15.
//

#pragma once

#include <unordered_map>
#include <vector>
#include <exception>
#include <string>

#include "element.h"

// fwd declaration
enum class Element;


// exception to be thrown if a given element is not on the periodic table or is currently unsupported
class NotAnElementException : public std::runtime_error
{
public:
    NotAnElementException(const std::string& element) :
            std::runtime_error("attempted to use nonexistent element")
    {
        message = "ERROR: " + std::string(std::runtime_error::what()) + ": " + element;
    }

    virtual const char* what() const noexcept
    {
        return message.c_str();
    }

private:
    std::string message;
};


// exception to be thrown if a given compound has an invalid charge
class InvalidChargeException : public std::runtime_error
{
public:
    InvalidChargeException(int charge) :
        std::runtime_error("attempted to create an element with an invalid charge")
    {
        message = "ERROR: " + std::string(std::runtime_error::what()) + ": " + std::to_string(charge);
    }

    virtual const char* what() const noexcept
    {
        return message.c_str();
    }

private:
    std::string message;
};


class Compound
{
public:
    // constructor (1)
    // converts a formula passed as a string to a compound object.
    Compound(std::string formula);

    // constructor (2)
    // constructs a compound object given a vector of pairs where the first piece
    // is the element and the second piece is the count.
    Compound(std::vector<std::pair<Element, int>> elements, int charge = 1);

    // calculates and returns the molar mass of the compound
    double getMolarMass() const;

    int numElements() const
    { return elements.size(); }

    // returns all the elements and their counts in the compound without duplicates
    std::unordered_map<Element, int, ElementHash> getElements() const
    { return elements; }

    // returns true if the compound contains the element, false otherwise
    bool contains(Element e) const;

    // returns the count of the element in the compound or 0 if it is not present
    int elementCount(Element e) const;

    std::string toString() const
    { return formula; }

private:
    // key = element
    // value = count
    std::unordered_map<Element, int, ElementHash> elements;

    std::string formula;

    int charge;

    // if the element is already contained in `elements` then the count is incremeneted
    // otherwise the element is added to `elements`
    void addElement(const Element& e, int count);

    // takes a given string that contains and it's count (optinoal) and turns it into an
    // element object and its count (1 if unspecified)
    void processChunk(const std::string& chunk, Element& e, int& count) throw(NotAnElementException);
};
