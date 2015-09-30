//
// Created by Rutvik Choudhary on 7/25/15.
//

#include "reaction.h"
#include <sstream>
#include "frac.h"
#include <iomanip>


using namespace std;


// takes a string (`piece`) and extracts the compound string and its count
void parsePiece(const string& piece, string& compound, int& count)
{
    auto compStartIt = find_if(piece.cbegin(), piece.cend(), ::isalpha);
    string coeffStr = string(piece.begin(), compStartIt);
    count = (coeffStr != "" ? stoi(coeffStr) : 1);
    compound = string(compStartIt, piece.end());
}


// takes a list of reactants/products seperated by a delimeter and populates the list object
// with the compound and its count
void parseList(const string& listStr, char delim, vector<pair<Compound, int>>& list)
{
    unsigned long pos = 0, prevPos = 0;
    int count;
    string piece, compound;
    while ((pos = listStr.find(delim, prevPos)) != string::npos)
    {
        piece = listStr.substr(prevPos, pos - prevPos);
        parsePiece(piece, compound, count);
        list.push_back(make_pair(compound, count));
        prevPos = pos + 1;
    }
    piece = listStr.substr(prevPos, pos - prevPos);
    parsePiece(piece, compound, count);
    list.push_back(make_pair(compound, count));
}


Reaction::Reaction(string reactantsStr, string productsStr, char delim)
{
    string cleanedReactantsStr(reactantsStr.begin(), remove(reactantsStr.begin(), reactantsStr.end(), ' '));
    string cleanedProductsStr(productsStr.begin(), remove(productsStr.begin(), productsStr.end(), ' '));
    parseList(cleanedReactantsStr, delim, reactants);
    parseList(cleanedProductsStr, delim, products);
}


// loops through both the reactants and the products and compiles a list of all
// the elements involved in the reaction (doesn't double count)
vector<Element> Reaction::getUniqueElements() const
{
    vector<Element> uniqueElements;
    for (auto reactant : reactants)
    {
        vector<Element> compoundElements;
        for (auto elementCountPair : reactant.first.getElements())
            compoundElements.push_back(elementCountPair.first);
        for (auto element : compoundElements)
        {
            if (find(uniqueElements.begin(), uniqueElements.end(), element) == uniqueElements.end())
                uniqueElements.push_back(element);
        }
    }
    for (auto product : products)
    {
        vector<Element> compoundElements;
        for (auto elementCountPair : product.first.getElements())
            compoundElements.push_back(elementCountPair.first);
        for (auto element : compoundElements)
        {
            if (find(uniqueElements.begin(), uniqueElements.end(), element) == uniqueElements.end())
                uniqueElements.push_back(element);
        }
    }
    return uniqueElements;
}


#define DISPLAY_IF_MULTI(x) (x > 1 ? std::to_string(x) : "")

std::string Reaction::toString() const
{
    stringstream ss;
    auto it = reactants.begin();
    while (it != reactants.begin() + reactants.size() - 1)
    {
        ss << DISPLAY_IF_MULTI(it->second) << it->first.toString() << " + ";
        it++;
    }
    ss << DISPLAY_IF_MULTI(it->second) << it->first.toString() << " -> ";
    it = products.begin();
    while (it != products.begin() + products.size() - 1)
    {
        ss << DISPLAY_IF_MULTI(it->second) << it->first.toString() << " + ";
        it++;
    }
    ss << DISPLAY_IF_MULTI(it->second) << it->first.toString();
    return ss.str();
}


vector<int> Reaction::getCoeffs() const
{
    vector<int> coeffs;
    for (pair<Compound, int> pair : reactants)
        coeffs.push_back(pair.second);
    for (pair <Compound, int> pair : products)
        coeffs.push_back(pair.second);
    return coeffs;
}
