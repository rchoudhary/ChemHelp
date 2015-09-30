//
// Created by Rutvik Choudhary on 7/25/15.
//

#pragma once

#include <string>
#include <unordered_map>


// element objects
enum class Element
{
    H, He, Li, Be, B, C, N, O, F, Ne, Na, Mg, Al, Si, P, S, Cl, Ar,
    K, Ca, Sc, Ti, V, Cr, Mn, Fe, Co, Ni, Cu, Zn, Ga, Ge, As, Se, Br, Kr,
    Rb, Sr, Y, Zr, Nb, Mo, Tc, Ru, Rh, Pd, Ag, Cd, In, Sn, Sb, Te, I, Xe
};

class ElementHash
{
public:
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};


// struct that contains all necessary data associated with an element
struct ElementData
{
    const std::string name;
    const std::string symbol;
    const int atomicNumber;
    const double atomicMass;
};


// get element object given element symbol
extern const std::unordered_map<std::string, Element> elementObjectLookupTable;

// get element data given element object
extern const std::unordered_map<Element, ElementData, ElementHash> elementDataLoopkupTable;
