//
// Created by Rutvik Choudhary on 7/25/15.
//

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "compound.h"


class Reaction
{
public:
    // constructor (1)
    Reaction(std::string reactantsStr, std::string productsStr, char delim = '+');

    // constructor (2)
    Reaction(std::vector<std::pair<Compound, int>> reactants, std::vector<std::pair<Compound, int>> products) :
        reactants(std::move(reactants)),
        products(std::move(products))
    { }

    // returns all the elements in the reaction while eliminating duplicates.
    // elements aren't in any particular order
    std::vector<Element> getUniqueElements() const;

    std::vector<std::pair<Compound, int>> getReactants() const
    { return reactants; }

    unsigned long numReactants() const
    { return reactants.size(); }

    std::vector<std::pair<Compound, int>> getProducts() const
    { return products; }

    unsigned long numProducts() const
    { return products.size(); }

    std::string toString() const;

    std::vector<int> getCoeffs() const;

private:
    // key = compound
    // value = count
    std::vector<std::pair<Compound, int>> reactants, products;

    // helper function to parse the list of reactants and products provided to
    // constructor (1)
    friend void parseList(
        const std::string& listStr,
        char delim,
        std::vector<std::pair<Compound, int>>& list);
};