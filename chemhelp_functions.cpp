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
            // creates a compound from the commond-line argument

            Compound c(ARG_MMASS_COMPOUND);

            if (verbose)
            {
                auto elements = c.getElements();

                // outputs the head of the table. columns are element, count, molar mass, and total

                cout << setfill('=') << setw(41) << "" << setfill(' ') << endl;
                cout << "|" << setw(7) << "elem" << "|" << setw(7) \
                    << "count" << "|" << setw(11) << "mol. mass" << "|" \
                    << setw(11) << "total" << "|" << endl;
                cout << setfill('=') << setw(41) << "" << setfill(' ') << endl;

                // loops through every element pair (contains the element and its count) in the list of elements

                for (pair<Element, int> elementPair : elements)
                {
                    Element element = elementPair.first;
                    int count = elementPair.second;
                    double elementMMass = elementDataLoopkupTable.at(element).atomicMass;

                    // outputs each row of the table

                    cout << "|" << setw(7) << toString(element) << "|" << setw(7) << count \
                        << "|" << setw(11) << elementMMass << "|" << setw(11) \
                        << count * elementMMass << "|" << endl;
                }

                // prints out the bottom of the table

                cout << setfill('=') << setw(41) << "" << setfill(' ') << endl;
                cout << endl;
            }

            // prints out the total molar mass of the element/compound

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
            // gets the first compound and its amount (which is in either grams or mol)

            Compound c1(ARG_STOICH_FROM_COMPOUND);
            double amt1 = stod(ARG_STOICH_FROM_AMT);

            Compound c2(ARG_STOICH_TO_COMPOUND);

            // will hold the number of mols of compund 1

            double mol1;

            if (strcmp(ARG_STOICH_FROM_UNIT, UNIT_GRAMS) == 0)
            {
                // if the unit provided by the user is grams then divide the amount provided by the compound's
                // molar mass to get the amount in mols

                mol1 = amt1 / c1.getMolarMass();
            }
            else if (strcmp(ARG_STOICH_FROM_UNIT, UNIT_MOL) == 0)
            {
                // if the unit provided by the user is in mols no conversion is needed

                mol1 = amt1;
            }

            // will hold the number of mols of compound 2
            double mol2;

            bool properRatioFormat = true;

            // if argc is equal to this value then a ratio was provided

            if (argc == CMD_STOICH_SPEC_NUMARGS)
            {
                // holds the string

                string ratioStr = ARG_STOICH_SPEC_RATIO;

                // will hold the location of the delim (':')

                unsigned long needle;

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
                        int numer = stoi(ratioStr.substr(0, needle));
                        int denom = stoi(ratioStr.substr(needle + 1));
                        mol2 = (mol1 * denom) / (double)numer;
                    }
                }
            }
            else if (argc == CMD_STOICH_DEF_NUMARGS)
            {
                // if no ratio was provided assume 1:1

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
    else
    {
        // invalid number of arguments given to `chemhelp -s`

        cout << "error: invalid number of arguments to -s" << endl;
        cout << "usage: chemhelp -s [compund 1] [amount 1] [unit 1 (g, mol)] [compound 2]" << endl;
        cout << "    [unit 2 (g, mol)] [OPTIONAL: ratio (x:y)]" << endl;
    }
}


// helper function to split string based on a provided delimeter.
// returns a vector with the pieces

vector<string> split(string str, string delim)
{
    unsigned long pos = 0, prevPos = 0;
    vector<string> pieces;
    while ((pos = str.find(delim, prevPos)) != string::npos)
    {
        pieces.push_back(str.substr(prevPos, pos - prevPos));
        prevPos = pos + 1;
    }
    pieces.push_back(str.substr(prevPos));
    return pieces;
}


// helper function for easily writing a vector of elements to a stream

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

    // the raw equation provided to the program

    string equation = ARG_BALANCE_REACTION;

    // position of the equal sign that indicates the split between reactants and products

    unsigned long splitPos = equation.find(REACT_PROD_SPLIT);

    // if no equal sign was found then the equation is invalid

    if (splitPos == string::npos)
    {
        cout << "error: every equation must contain a \'=\'" << endl;
        cout << "use `chemhelp --help` for further info" << endl;
        return;
    }

    // Get the reactants and products from the reaction. Right now they're delimeted by plus signs.
    // The `Reaction` constructor will parse them

    string reactantsStr = equation.substr(0, splitPos);
    string productsStr = equation.substr(splitPos + 1);

    try
    {
        // Create a reaction from the reactant and product strings using the standard delim ('+')

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

    // This will hold the smallest amount of product produced.

    double smallestProductAmount = numeric_limits<double>::max();

    // This will hold the name of the reactant that produces the smallest amount of product.

    string limitingReactant;

    // Get the reaction string from the arguments and then split the reactants half into the individual
    // reactants.

    string reactionStr = ARG_LIMITING_REACTION;
    vector<string> reactantStrs = split(reactionStr.substr(0, reactionStr.find('=')), "+");

    // Find the start of the first product by starting right past the  '=' in the reaction string and moving forward
    // until something other than a space is encountered.

    unsigned long firstProductStrStart = reactionStr.find('=') + 1;
    while (isspace(reactionStr[firstProductStrStart])) firstProductStrStart++;

    // Find the end of the product by moving forward until a non alphanumeric character is encountered

    unsigned long firstProductStrEnd = firstProductStrStart + 1;
    while (isalnum(reactionStr[firstProductStrEnd])) firstProductStrEnd++;

    // Get the first product as a string

    string firstProductStr = reactionStr.substr(firstProductStrStart, firstProductStrEnd - firstProductStrStart);

    unsigned long nameStart = 0;
    int productCoeff = 1;
    string productName = "";

    if (isalpha(firstProductStr[nameStart]))
    {
        // If the first character of the product string is a letter, then the product name is the string and the
        // coefficient is implied to be 1

        productCoeff = 1;
        productName = firstProductStr;
    }

    else
    {
        // Otherwise, move the start of the name to the first alphabetic character and get the coefficient and the
        // name separately

        while (isdigit(firstProductStr[nameStart]))
        {
            nameStart++;
        }
        productCoeff = stoi(firstProductStr.substr(0, nameStart));
        productName = firstProductStr.substr(nameStart);
    }

    cout << "enter amounts of reactants (-1 if not given) in the format \"X mol\" or \"X g\"" << endl;
    cout << "NOTE: this won't give the right answer unless the equation is balanced!" << endl;

    // for each reactant in the list of reactants

    for (string reactantStr : reactantStrs)
    {
        int reactantCoeff;
        string reactantName;

        // Just like with the product, extract the coefficient and name from the string depending on whether
        // or not the first character is a letter.

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

        // ask the user to input the amount of this reactant

        string rawInput;
        cout << ">>> " << reactantStr << ": ";
        getline(cin, rawInput);

        // The space pos delimits where the unit starts

        unsigned long spacePos = rawInput.find(' ');

        string amountStr, unitStr;

        if (spacePos != string::npos)
        {
            // If a space was provided then the amount is before and the unit is after

            amountStr = rawInput.substr(0, spacePos);
            unsigned long unitStart = spacePos;
            while (isspace(rawInput[unitStart])) unitStart++;
            unitStr = rawInput.substr(unitStart);
        }
        else
        {
            // If no space was provided then the unit needs to be extracted.The unit starts when the numbers from the
            // amount become letters from the unit

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

        // If the unit of the reactant amount is mols, simply assign it, otherwise perform the necessary conversion

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

        // Apply the ratio of coefficients

        productAmountMols = productAmountMols / reactantCoeff * productCoeff;

        // If the amount of mols the reactant produces is less than the smallest amount of mols produced
        // previously, then this reactant is now the current limiting reactant

        if (productAmountMols < smallestProductAmount)
        {
            smallestProductAmount = productAmountMols;
            limitingReactant = reactantName;
        }

        // get the product amount in grams for output purposes

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

    // Output the limiting reactant
    cout << "the limiting reactant is " << limitingReactant << endl;
}