//
// Created by Rutvik Choudhary on 7/25/15.
//

#include "compound.h"
#include <iostream>
#include "chem_util.h"


using namespace std;


void Compound::addElement(const Element& e, int count)
{
    if (elements.find(e) != elements.end())
    {
        elements[e] += count;
    }
    else
    {
        elements[e] = count;
    }
}


void Compound::processChunk(const string& chunk, Element& e, int& count) throw(NotAnElementException)
{
    // find where the numeric part of the chunk starts

    unsigned long countStart = 0;
    while (!isdigit(chunk[countStart]) && countStart < chunk.length())
    {
        countStart++;
    }

    // get the element name portion and if it is a real element, set `e` to the object it represents
    // otherwise throw NotAnElementException

    string elementName = chunk.substr(0, countStart);
    if (elementObjectLookupTable.find(elementName) == elementObjectLookupTable.end())
        throw NotAnElementException(elementName);
    e = elementObjectLookupTable.at(elementName);

    // if a numeric part of the chunk was detected then return its int value

    if (countStart < chunk.length())
    {
        count = stoi(chunk.substr(countStart));
    }
        // otherwise return the default count of 1
    else
    {
        count = 1;
    }
}


// a "chunk" represents a single element and its count (if it has one)
// e.g. "Ca", "H3"

Compound::Compound(string formula) :
    formula(move(formula))
{
    unsigned long chargeStart = this->formula.find('^');
    if (chargeStart != string::npos)
    {
        string chargeStr = this->formula.substr(chargeStart + 1);
        if (chargeStr == "")
        {
            charge = 1;
        }
        else if (chargeStr == "-")
        {
            charge = -1;
        }
        else if (chargeStr == "+")
        {
            charge = 1;
        }
        else
        {
            charge = stoi(chargeStr);
        }
        this->formula.erase(chargeStart);
    }
    else
    {
        charge = 1;
    }

    unsigned long chunkStart = 0;
    for (unsigned long i = 0; i <= this->formula.length(); i++)
    {
        // if i points to a capital letter or a opening parenthesis then its the end of a chunk

        if (isupper(this->formula[i]) || this->formula[i] == '(')
        {
            // make sure an empty chunk won't be created

            if (i > chunkStart)
            {
                string chunk = this->formula.substr(chunkStart, i - chunkStart);
                Element e;
                int count;
                processChunk(chunk, e, count);
                addElement(e, count);
                chunkStart = i;
            }
        }
            // make sure that the very last chunk is read too
        else if (i == this->formula.length())
        {
            string chunk = this->formula.substr(chunkStart);
            Element e;
            int count;
            processChunk(chunk, e, count);
            addElement(e, count);
        }

        // if the start of the chunk is an opening parenthesis, then it is the start of a subgroup
        // and a subprocess must be started

        if (this->formula[chunkStart] == '(')
        {
            unsigned long subChunkStart = chunkStart + 1;
            unsigned long subgroupEnd = (int) this->formula.find(')', subChunkStart);

            // determine the overall count of the subgroup so the count of the indiv. elements
            // in it can be scaled appropriately

            unsigned long subgroupCountStart = subgroupEnd + 1, subgroupCountEnd = subgroupCountStart;
            int subgroupCount = 1;
            if (isdigit(this->formula[subgroupCountStart]))
            {
                while (isdigit(this->formula[subgroupCountEnd])) subgroupCountEnd++;
                subgroupCount = stoi(this->formula.substr(subgroupCountStart, subgroupCountEnd - subgroupCountStart));
            }

            // this is similar to the way the outer this->formula was looped through

            for (unsigned long j = subChunkStart; j <= subgroupEnd; j++)
            {
                if (isupper(this->formula[j]) || this->formula[j] == ')')
                {
                    if (j > subChunkStart)
                    {
                        string subChunk = this->formula.substr(subChunkStart, j - subChunkStart);
                        Element e;
                        int count;
                        processChunk(subChunk, e, count);
                        addElement(e, count * subgroupCount);
                        subChunkStart = j;
                    }
                }
            }
            i = subgroupCountEnd;
            chunkStart = subgroupCountEnd;
        }
    }
}


Compound::Compound(std::vector<std::pair<Element, int>> elements, int charge)
{
    this->charge = charge;
    for (std::pair<Element, int> pair : elements)
    {
        addElement(pair.first, pair.second);
        formula += ::toString(pair.first) + (pair.second > 1 ? std::to_string(pair.second) : "");
    }
}


double Compound::getMolarMass() const
{
    double totalMolarMass = 0;
    for (auto it = elements.cbegin(); it != elements.cend(); it++)
    {
        Element e = it->first;
        int count = it->second;
        totalMolarMass += elementDataLoopkupTable.at(e).atomicMass * count;
    }
    return totalMolarMass;
}


bool Compound::contains(Element e) const
{
    return (elements.find(e) != elements.end());
}


int Compound::elementCount(Element e) const
{
    if (elements.find(e) != elements.end())
    {
        return elements.at(e);
    }
    else
    {
        return 0;
    }
}