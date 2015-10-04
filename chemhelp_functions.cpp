//
// Created by Rutvik Choudhary on 7/25/15.
//


#include "chemhelp_defines.h"
#include <fstream>
#include <iostream>
#include "compound.h"
#include <iomanip>
#include "reaction.h"
#include "chem_util.h"


using namespace std;


void outputHelp()
{
    ifstream fin("help-page.txt");

    if (fin.is_open())
    {
        string buffer((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
        cout << buffer << endl;
        fin.close();
    }
    else
    {
        cout << "help page could not be found" << endl;
    }
}


void outputVersion()
{
    cout << PROGRAM_NAME << " " << VERSION << endl;
    cout << "Written by " << AUTHOR << endl;
}


void performMolarMass(int argc, char** argv, bool verbose)
{
    if (argc == CMD_MMASS_NUMARGS)
    {
        try
        {
            Compound c(ARG_MMASS_COMPOUND);
            if (verbose)
            {
                auto elements = c.getElements();
                cout << setfill('=') << setw(41) << "" << setfill(' ') << endl;
                cout << "|" << setw(7) << "elem" << "|" << setw(7) \
                    << "count" << "|" << setw(11) << "mol. mass" << "|" \
                    << setw(11) << "total" << "|" << endl;
                cout << setfill('=') << setw(41) << "" << setfill(' ') << endl;
                for (pair<Element, int> elementPair : elements)
                {
                    Element element = elementPair.first;
                    int count = elementPair.second;
                    double elementMMass = elementDataLoopkupTable.at(element).atomicMass;
                    cout << "|" << setw(7) << toString(element) << "|" << setw(7) << count \
                        << "|" << setw(11) << elementMMass << "|" << setw(11) \
                        << count * elementMMass << "|" << endl;
                }
                cout << setfill('=') << setw(41) << "" << setfill(' ') << endl;
                cout << endl;
            }
            cout << c.getMolarMass() << " g/mol" << endl;
        }
        catch (NotAnElementException& e)
        {
            cout << e.what() << endl;
        }
    }
    else
    {
        cout << "error: invalid number of arguments to -m" << endl;
        cout << "usage: chemhelp -m [compound]" << endl;
    }
}


void performStoich(int argc, char** argv, bool verbose)
{
    if (argc == CMD_STOICH_DEF_NUMARGS || argc == CMD_STOICH_SPEC_NUMARGS)
    {
        try
        {
            Compound c1(ARG_STOICH_FROM_COMPOUND);
            double amt1 = stod(ARG_STOICH_FROM_AMT);
            Compound c2(ARG_STOICH_TO_COMPOUND);

            // holds the number of mols of compund 1
            double mol1;

            if (strcmp(ARG_STOICH_FROM_UNIT, UNIT_GRAMS) == 0)
            {
                mol1 = amt1 / c1.getMolarMass();
            }
            else if (strcmp(ARG_STOICH_FROM_UNIT, UNIT_MOL) == 0)
            {
                mol1 = amt1;
            }

            // will hold the number of mols of compound 2
            double mol2;

            bool properRatioFormat = true;

            // if argc is equal to this value then a ratio was provided

            if (argc == CMD_STOICH_SPEC_NUMARGS)
            {
                string ratioStr = ARG_STOICH_SPEC_RATIO;
                int needle;

                // if no delim (':') is found then an improper ratio was provided   

                if ((needle = ratioStr.find(STOICH_RATIO_DELIM)) == string::npos)
                {
                    properRatioFormat = false;
                }

                if (properRatioFormat)
                {
                    // count the number of times the delim appears (it should only be there once)
                    // also check to make sure every character is a numerical digit (save for the delim)

                    int numDelims = 0;
                    for (int i = 0; i < ratioStr.length(); i++)
                    {
                        if (ratioStr[i] == STOICH_RATIO_DELIM)
                        {
                            numDelims++;
                            if (numDelims > 1)
                            {
                                properRatioFormat = false;
                                break;
                            }
                        }
                        else if (!isdigit(ratioStr[i]))
                        {
                            properRatioFormat = false;
                            break;
                        }
                    }

                    if (properRatioFormat)
                    {
                        int numer = stoi(ratioStr.substr(0, (unsigned long) needle));
                        int denom = stoi(ratioStr.substr((unsigned long)(needle + 1)));
                        mol2 = (mol1 * denom) / (double)numer;
                    }
                }
            }
            // if no ratio was provided assume 1:1
            else if (argc == CMD_STOICH_DEF_NUMARGS)
            {
                mol2 = mol1;
            }

            if (properRatioFormat)
            {
                // if the unit for the "to amount" is grams then use the molar mass of compound 2
                // to determine it

                if (strcmp(ARG_STOICH_TO_UNIT, UNIT_GRAMS) == 0)
                {
                    double amt2 = mol2 * c2.getMolarMass();
                    cout << amt2 << " g" << endl;
                }
                else if (strcmp(ARG_STOICH_TO_UNIT, UNIT_MOL) == 0)
                {
                    cout << mol2 << " mol" << endl;
                }
            }
            else
            {
                cout << "error: improper format for the ratio was provided" << endl;
                cout << "ratio should be in the format x:y where x and y are both integers" << endl;
            }
        }
        catch (NotAnElementException& e)
        {
            cout << e.what() << endl;
        }
    }
    // invalid number of arguments given to `chemhelp -s`
    else
    {
        cout << "error: invalid number of arguments to -s" << endl;
        cout << "usage: chemhelp -s [compund 1] [amount 1] [unit 1 (g, mol)] [compound 2]" << endl;
        cout << "    [unit 2 (g, mol)] [OPTIONAL: ratio (x:y)]" << endl;
    }
}


vector<string> split(string str, string delim)
{
    int pos = 0, prevPos = 0;
    vector<string> pieces;
    while ((pos = str.find(delim, prevPos)) != string::npos)
    {
        pieces.push_back(str.substr(prevPos, pos - prevPos));
        prevPos = pos + 1;
    }
    pieces.push_back(str.substr(prevPos));
    return pieces;
}


template <typename T>
ostream& operator<<(ostream& out, const vector<T>& vec)
{
    int i;
    for (i = 0; i < vec.size() - 1; i++)
        out << vec[i] << ", ";
    out << vec[i];
    return out;
}


void performBalance(int argc, char** argv, bool verbose)
{
    if (argc > CMD_BALANCE_NUMARGS)
    {
        cout << "error: too many arguments were provided" << endl;
        cout << "hint: if the chem equation contains spaces, surround it with quotes" << endl;
        cout << "use `chemhelp --help` for more info" << endl;
        return;
    }
    else if (argc < CMD_BALANCE_NUMARGS)
    {
        cout << "error: not enough arguments were provided" << endl;
        cout << "use `chemhelp --help` for more info" << endl;
        return;
    }

    string equation = ARG_BALANCE_REACTION;
    int splitPos = equation.find(REACT_PROD_SPLIT);
    if (splitPos == string::npos)
    {
        cout << "error: every equation must contain a \'=\'" << endl;
        cout << "use `chemhelp --help` for further info" << endl;
        return;
    }

    string reactantsStr = equation.substr(0, splitPos);
    string productsStr = equation.substr(splitPos + 1);

    try
    {
        Reaction r(reactantsStr, productsStr, EQUATION_DELIM[0]);
        r = balance(r, verbose);
        cout << r.getCoeffs() << endl;
    }
    catch (NotAnElementException& e)
    {
        cout << e.what() << endl;
    }
    catch (CouldNotBalanceException& e)
    {
        cout << e.what() << endl;
    }
}


void performLimitingReactant(int argc, char** argv, bool verbose)
{
    if (argc != CMD_LIMITING_NUMARGS)
    {
        cout << "error: not enough arguments were provided" << endl;
        cout << "use `chemhelp --help` for more info" << endl;
        return;
    }

    double smallestProductAmount = numeric_limits<double>::max();
    string limitingReactant;

    string reactionStr = ARG_LIMITING_REACTION;
    vector<string> reactantStrs = split(reactionStr.substr(0, reactionStr.find('=')), "+");

    unsigned long firstProductStrStart = reactionStr.find('=') + 1;
    while (isspace(reactionStr[firstProductStrStart])) firstProductStrStart++;
    unsigned long firstProductStrEnd = firstProductStrStart + 1;
    while (isalnum(reactionStr[firstProductStrEnd])) firstProductStrEnd++;
    string firstProductStr = reactionStr.substr(firstProductStrStart, firstProductStrEnd - firstProductStrStart);

    unsigned long nameStart = 0;
    int productCoeff = 1;
    string productName = "";
    if (isalpha(firstProductStr[nameStart]))
    {
        productCoeff = 1;
        productName = firstProductStr;
    }
    else
    {
        while (isdigit(firstProductStr[nameStart]))
        {
            nameStart++;
        }
        productCoeff = stoi(firstProductStr.substr(0, nameStart));
        productName = firstProductStr.substr(nameStart);
    }

    cout << "enter amounts of reactants (-1 if not given) in the format \"X mol\" or \"X g\"" << endl;
    cout << "NOTE: this won't give the right answer unless the equation is balanced!" << endl;

    for (string reactantStr : reactantStrs)
    {
        int reactantCoeff;
        string reactantName;

        if (!isalpha(reactantStr[0]))
        {
            unsigned long reactantNameStart = 0;
            while (isdigit(reactantStr[reactantNameStart])) reactantNameStart++;
            reactantCoeff = stoi(reactantStr.substr(0, reactantNameStart));
            reactantName = reactantStr.substr(reactantNameStart);
        }
        else
        {
            reactantCoeff = 1;
            reactantName = reactantStr;
        }

        string rawInput;
        cout << reactantStr << ": ";
        getline(cin, rawInput);
        unsigned long spacePos = rawInput.find(' ');

        string amountStr, unitStr;
        if (spacePos != string::npos)
        {
            amountStr = rawInput.substr(0, spacePos);
            unsigned long unitStart = spacePos;
            while (isspace(rawInput[unitStart])) unitStart++;
            unitStr = rawInput.substr(unitStart);
        }
        else
        {
            unsigned long splitPos = 0;
            while (splitPos < rawInput.length() && !isalpha(rawInput[splitPos])) splitPos++;
            if (splitPos == rawInput.length())
            {
                cout << "error: no unit provided." << endl;
                return;
            }
            amountStr = rawInput.substr(0, splitPos);
            unitStr = rawInput.substr(splitPos);
        }

        double amount;

        try
        {
            amount = stod(amountStr);
        }
        catch (invalid_argument e)
        {
            cout << "invalid input: \"" << rawInput << "\" is not a number" << endl;
            return;
        }

        double productAmountMols = 0.0;

        if (unitStr == UNIT_MOL)
        {
            productAmountMols = amount;
        }
        else if (unitStr == UNIT_GRAMS)
        {
            try
            {
                Compound c(reactantName);
                productAmountMols = amount / c.getMolarMass();
            }
            catch (runtime_error& e)
            {
                cout << e.what() << endl;
            }
        }

        productAmountMols = productAmountMols / reactantCoeff * productCoeff;
        if (productAmountMols < smallestProductAmount)
        {
            smallestProductAmount = productAmountMols;
            limitingReactant = reactantName;
        }

        double productAmountGrams;

        try
        {
            productAmountGrams = productAmountMols * Compound(productName).getMolarMass();
        }
        catch (NotAnElementException e)
        {
            cout << e.what() << endl;
        }
        cout << productAmountMols << " mols (" << productAmountGrams << " g) of " << productName << endl;
    }
    cout << "the limiting reactant is " << limitingReactant << endl;
}