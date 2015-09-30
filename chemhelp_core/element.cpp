//
// Created by Rutvik Choudhary on 7/25/15.
//

#include "element.h"

using namespace std;

const std::unordered_map<std::string, Element> elementObjectLookupTable =
    {
        {"H",  Element::H},
        {"He", Element::He},
        {"Li", Element::Li},
        {"Be", Element::Be},
        {"B",  Element::B},
        {"C",  Element::C},
        {"N",  Element::N},
        {"O",  Element::O},
        {"F",  Element::F},
        {"Ne", Element::Ne},
        {"Na", Element::Na},
        {"Mg", Element::Mg},
        {"Al", Element::Al},
        {"Si", Element::Si},
        {"P",  Element::P},
        {"S",  Element::S},
        {"Cl", Element::Cl},
        {"Ar", Element::Ar},
        {"K",  Element::K},
        {"Ca", Element::Ca},
        {"Sc", Element::Sc},
        {"Ti", Element::Ti},
        {"V",  Element::V},
        {"Cr", Element::Cr},
        {"Mn", Element::Mn},
        {"Fe", Element::Fe},
        {"Co", Element::Co},
        {"Ni", Element::Ni},
        {"Cu", Element::Cu},
        {"Zn", Element::Zn},
        {"Ga", Element::Ga},
        {"Ge", Element::Ge},
        {"As", Element::As},
        {"Se", Element::Se},
        {"Br", Element::Br},
        {"Kr", Element::Kr},
        {"Rb", Element::Rb},
        {"Sr", Element::Sr},
        {"Y",  Element::Y},
        {"Zr", Element::Zr},
        {"Nb", Element::Nb},
        {"Mo", Element::Mo},
        {"Tc", Element::Tc},
        {"Ru", Element::Ru},
        {"Rh", Element::Rh},
        {"Pd", Element::Pd},
        {"Ag", Element::Ag},
        {"Cd", Element::Cd},
        {"In", Element::In},
        {"Sn", Element::Sn},
        {"Sb", Element::Sb},
        {"Te", Element::Te},
        {"I",  Element::I},
        {"Xe", Element::Xe}
    };


const std::unordered_map<Element, ElementData, ElementHash> elementDataLoopkupTable =
    {
        {Element::H,  {"hydrogen",   "H",  1,  1.008}},
        {Element::He, {"helium",     "He", 2,  4.003}},
        {Element::Li, {"lithium",    "Li", 3,  6.941}},
        {Element::Be, {"beryllium",  "Be", 4,  9.012}},
        {Element::B,  {"boron",      "B",  5,  10.811}},
        {Element::C,  {"carbon",     "C",  6,  12.011}},
        {Element::N,  {"nitrogen",   "N",  7,  14.007}},
        {Element::O,  {"oxygen",     "O",  8,  15.999}},
        {Element::F,  {"fluorine",   "F",  9,  18.998}},
        {Element::Ne, {"neon",       "Ne", 10, 20.180}},
        {Element::Na, {"sodium",     "Na", 11, 22.990}},
        {Element::Mg, {"magnesium",  "Mg", 12, 24.305}},
        {Element::Al, {"aluminum",   "Al", 13, 26.982}},
        {Element::Si, {"silicon",    "Si", 14, 28.086}},
        {Element::P,  {"phosphorus", "P",  15, 30.974}},
        {Element::S,  {"sulfur",     "S",  16, 32.065}},
        {Element::Cl, {"chlorine",   "Cl", 17, 35.453}},
        {Element::Ar, {"argon",      "Ar", 18, 39.948}},
        {Element::K,  {"potassium",  "K",  19, 39.098}},
        {Element::Ca, {"calcium",    "Ca", 20, 40.078}},
        {Element::Sc, {"scandium",   "Sc", 21, 44.956}},
        {Element::Ti, {"titanium",   "Ti", 22, 47.867}},
        {Element::V,  {"vanadium",   "V",  23, 50.942}},
        {Element::Cr, {"chromium",   "Cr", 24, 51.996}},
        {Element::Mn, {"manganese",  "Mn", 25, 54.938}},
        {Element::Fe, {"iron",       "Fe", 26, 55.845}},
        {Element::Co, {"cobalt",     "Co", 27, 58.933}},
        {Element::Ni, {"nickel",     "Ni", 28, 58.693}},
        {Element::Cu, {"copper",     "Cu", 29, 63.546}},
        {Element::Zn, {"zinc",       "Zn", 30, 65.380}},
        {Element::Ga, {"gallium",    "Ga", 31, 69.723}},
        {Element::Ge, {"germanium",  "Ge", 32, 72.640}},
        {Element::As, {"arsenic",    "As", 33, 74.922}},
        {Element::Se, {"selenium",   "Se", 34, 78.960}},
        {Element::Br, {"bromine",    "Br", 35, 79.904}},
        {Element::Kr, {"krypton",    "Kr", 36, 83.798}},
        {Element::Rb, {"rubidium",   "Rb", 37, 85.468}},
        {Element::Sr, {"strontium",  "Sr", 38, 87.620}},
        {Element::Y,  {"yttrium",    "Y",  39, 88.906}},
        {Element::Zr, {"zirconium",  "Zr", 40, 91.224}},
        {Element::Nb, {"niobium",    "Nb", 41, 92.906}},
        {Element::Mo, {"molybdenum", "Mo", 42, 95.960}},
        {Element::Tc, {"technetium", "Tc", 43, 98.000}},
        {Element::Ru, {"ruthenium",  "Ru", 44, 101.070}},
        {Element::Rh, {"rhodium",    "Rh", 45, 102.906}},
        {Element::Pd, {"palladium",  "Pd", 46, 106.420}},
        {Element::Ag, {"silver",     "Ag", 47, 107.868}},
        {Element::Cd, {"cadmium",    "Cd", 48, 112.411}},
        {Element::In, {"indium",     "In", 49, 114.818}},
        {Element::Sn, {"tin",        "Sn", 50, 118.710}},
        {Element::Sb, {"antimony",   "Sb", 51, 121.760}},
        {Element::Te, {"tellurium",  "Te", 52, 127.600}},
        {Element::I,  {"iodine",     "I",  53, 126.904}},
        {Element::Xe, {"xenon",      "Xe", 54, 131.293}},
    };
