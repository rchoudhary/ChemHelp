//
// Created by Rutvik Choudhary on 8/13/15.
//

#include "chem_util.h"
#include "reaction.h"
#include "frac.h"
#include <iomanip>

using namespace std;

#define IF_MULTI(x) (x > 1 ? to_string(x) : "")
#define VAR_NAME_START 'A'

Reaction balance(Reaction& reaction, bool verbose, ostream& out) throw(CouldNotBalanceException)
{
    vector<pair<Compound, int>> reactants = reaction.getReactants();
    vector<pair<Compound, int>> products = reaction.getProducts();

    const int kNumReactants = (int) reactants.size();
    const int kNumProducts = (int) products.size();
    const int kNumVars = kNumReactants + kNumProducts;
    const int kNumEquations = (int) reaction.getUniqueElements().size();

    // this number is divisible by 1, 2, 3, 4, 6, 8, 12, and 24
    // so its a great starting number to make sure that we don't end up
    // with decimals
    const int kMagicNumber = 24;

    // holds all the elements that are being balanced
    const vector<Element> kElementsUsed = reaction.getUniqueElements();

    if (verbose)
    {
        out << "balancing equation: ";
        char varName = 'A';
        for (int i = 0; i < kNumReactants; i++, varName++)
        {
            out << varName << "[" << toString(reactants[i].first) << "]";
            if (i < kNumReactants - 1) out << " + ";
            else out << " -> ";
        }
        for (int i = 0; i < kNumProducts; i++, varName++)
        {
            out << varName << "[" << toString(products[i].first) << "]";
            if (i < kNumProducts - 1) out << " + ";
            else out << endl;
        }
        out << "elements being examined: ";
        for (Element e : kElementsUsed) cout << toString(e) << " ";
        out << endl;
        out << "num variables: " << kNumVars << endl;
        out << "num equations: " << kNumEquations << endl;
        out << endl;
    }

    // merged reactants and products into one vector so only one variable
    // has to be manipulated
    vector<pair<Compound, int>> kReactionEquation = reactants;
    kReactionEquation.insert(kReactionEquation.end(),
                             products.begin(), products.end());
    const int kProductsStart = kNumReactants;

    // holds the final values of all the variables
    vector<frac_t> varValues((unsigned long) kNumVars, {1, 1});

    // if varIsSolved[X] == true then X has been determined to
    // be a particular value, otherwise it remains unsolved
    vector<bool> varIsSolved((unsigned long) kNumVars, false);

    // holds all of the equations. each equation corresponds to how a single element
    // can be balanced. each number corresponds to the amount of said element present
    // in a single compound and is the coefficient for the variable that represents
    // said compound
    vector<vector<int>> equations;

    int startingEquationI = -1;
    bool startingEquationFound = false;

    if (verbose)
    {
        out << setw(6) << " ";
        for (int i = 0; i < kNumVars; i++)
            out << setw(3) << char(VAR_NAME_START + i) << " ";
        out << endl;
    }

    // create equations and fill with coefficients. also, make note of which equation
    // should be the starting equation to work with, i.e. the one where only two vars
    // are initially defined

    for (int i = 0; i < kNumEquations; i++)
    {
        // each equation corresponds to an element. this is the current element
        // being investigated
        Element currElement = kElementsUsed[i];

        if (verbose) out << setw(4) << toString(currElement) << "[ ";

        vector<int> currEquation((unsigned long) kNumVars, 0);
        int numVarsDefined = 0;
        for (int j = 0; j < kNumVars; j++)
        {
            Compound compound = kReactionEquation[j].first;
            int compoundCount = kReactionEquation[j].second;
            if (compound.contains(currElement))
            {
                // if the current element is in the compound, then add the total
                // amount of it to the current equation
                currEquation[j] = compoundCount * compound.elementCount(currElement);
                numVarsDefined++;
            }
            if (verbose) out << setw(3) << currEquation[j] << " ";
        }
        equations.push_back(currEquation);

        if (verbose) out << "] ";

        if (!startingEquationFound && numVarsDefined == 2)
        {
            startingEquationI = i;
            startingEquationFound = true;
            if (verbose) out << "<- starting equation";
        }

        if (verbose) out << endl;
    }

    if (verbose) out << endl;


    if (!startingEquationFound)
    {
        // no starting equation was found.
        // maybe the reaction still could be balanced, but I'm not gonna figure out how

        throw CouldNotBalanceException(reaction, "no suitable starting point for reaction-balancing algorithm");
    }

    // if this point is reached then the starting equation has been found

    // NOTE: it's perfectly safe to assume that in this starting equation which only has
    // two vars, one is on the reactants side and one is on the products side because
    // that's how chemistry works

    // set the very first var to the "magic number" (exciting stuff)

    vector<int> startingEquation = equations[startingEquationI];
    int firstVarI = (int) distance(
        startingEquation.begin(),
        find_if(
            startingEquation.begin(),
            startingEquation.end(),
            [](const int& i) { return i != 0; }
        )
    );
    varValues[firstVarI] = {kMagicNumber, 1};
    varIsSolved[firstVarI] = true;

    // now set the second number to the appropriate value

    int secondVarI = (int) distance(
        startingEquation.begin(),
        find_if(
            startingEquation.begin() + firstVarI + 1,
            startingEquation.end(),
            [](const int& i) { return i != 0; }
        )
    );
    varValues[secondVarI] = simplify(
        varValues[firstVarI] * startingEquation[firstVarI] / startingEquation[secondVarI]
    );
    varIsSolved[secondVarI] = true;

    if (verbose)
    {
        out << "equation " << startingEquationI << ": ";
        out << IF_MULTI(startingEquation[firstVarI]) << char(VAR_NAME_START + firstVarI)
        << " = " << IF_MULTI(startingEquation[secondVarI])
        << char(VAR_NAME_START + secondVarI) << endl;
        out << setw(5) << char(VAR_NAME_START + firstVarI) << " = "
        << varValues[firstVarI] << "  <- arbitrarily set" << endl;
        out << setw(5) << char(VAR_NAME_START + secondVarI) << " = "
        << varValues[secondVarI] << "  <- newly determined" << endl;
        out << endl;
    }

    // now solve for the rest of the variables

    int currEquationI = 0;

    // keep going while not all of the vars have been solved for

    while (count(varIsSolved.begin(), varIsSolved.end(), false) != 0)
    {
        vector<int> currEquation;
        bool nextEquationFound = false;

        while (currEquationI < kNumEquations && !nextEquationFound)
        {
            int numExtraUndefVars = 0;
            currEquation = equations[currEquationI];

            // count the number of variables in the current eq. that need to be solved for

            for (int i = 0; i < kNumVars; i++)
            {
                if (currEquation[i] != 0 && !varIsSolved[i])
                {
                    numExtraUndefVars++;
                }
            }

            // if there's only one variable that hasn't been solved for yet in an equation,
            // that equation will be the next focus

            if (numExtraUndefVars == 1)
            {
                nextEquationFound = true;
            }
            else
            {
                currEquationI++;
            }
        }

        if (verbose)
        {
            out << "equation " << currEquationI << ": ";
            for (int i = 0; i < kNumReactants; i++)
            {
                if (currEquation[i] > 0)
                {
                    if (i > 0 && currEquation[i - 1] > 0) out << " + ";
                    out << IF_MULTI(currEquation[i]) << char(VAR_NAME_START + i);
                }
            }
            out << " = ";
            for (int i = kProductsStart; i < kNumProducts + kNumReactants; i++)
            {
                if (currEquation[i] > 0)
                {
                    if (i > kProductsStart && currEquation[i - 1] > 0) out << " + ";
                    out << IF_MULTI(currEquation[i]) << char(VAR_NAME_START + i);
                }
            }
            out << endl;
        }

        // if the next equation wasn't found, then things get too complicated
        // assume the equation can't be balanced

        if (!nextEquationFound)
        {
            throw CouldNotBalanceException(reaction, "algorithm was unable to find a continuation point");
        }


        // multiply all the coefficients on both sides of the equation

        frac_t reactantsTotalCoeff = zero_frac, productsTotalCoeff = zero_frac;
        bool undefVarIsReactant = false;
        int undefVarI = 0;
        for (int i = 0; i < kNumVars; i++)
        {
            if (currEquation[i] != 0)
            {
                if (i < kProductsStart)
                {
                    if (varIsSolved[i])
                    {
                        reactantsTotalCoeff = reactantsTotalCoeff + (varValues[i] * currEquation[i]);
                        if (verbose)
                        {
                            out << setw(5) << char(VAR_NAME_START + i) << " = " << varValues[i] << endl;
                        }
                    }
                    else if (!varIsSolved[i])
                    {
                        undefVarIsReactant = true;
                        undefVarI = i;
                    }
                }
                else if (i >= kProductsStart)
                {
                    if (varIsSolved[i])
                    {
                        productsTotalCoeff = productsTotalCoeff + (varValues[i] * currEquation[i]);
                        if (verbose)
                        {
                            out << setw(5) << char(VAR_NAME_START + i) << " = " << varValues[i] << endl;
                        }
                    }
                    else if (!varIsSolved[i])
                    {
                        undefVarIsReactant = false;
                        undefVarI = i;
                    }
                }
            }
        }

        // if the variable is a reactant the divide all the product values by the reactant values,
        // and if the variable is a product, do the opposite

        if (undefVarIsReactant)
        {
            varValues[undefVarI] = simplify(
                (productsTotalCoeff - reactantsTotalCoeff) / currEquation[undefVarI]
            );
            varIsSolved[undefVarI] = true;
        }
        else
        {
            varValues[undefVarI] = simplify(
                (reactantsTotalCoeff - productsTotalCoeff) / currEquation[undefVarI]
            );
            varIsSolved[undefVarI] = true;
        }

        if (verbose)
        {
            out << setw(5) << char(VAR_NAME_START + undefVarI) << " = " << varValues[undefVarI] <<
            "  <- newly determined" << endl;
        }

        currEquationI++;
        if (currEquationI == kNumEquations)
            currEquationI = 0;

        if (verbose) out << endl;
    }

    // now that all the coefficients have been determined, simplify them (i.e. turn them into
    // the smallest whole numbers possible)

    // first eliminate the denominators by multiplying all the coefficients by the
    // denominators' lcm

    int denomLCM = 1;
    for (int i = 0; i < kNumVars - 1; i++)
    {
        denomLCM = lcm(varValues[i].den, denomLCM);
    }

    for (int i = 0; denomLCM > 1 && i < kNumVars; i++)
    {
        varValues[i] = varValues[i] * denomLCM;
    }

    // now divide all the numerators of the coefficients by their collective gcd

    int numGCD = varValues[0].num;
    for (int i = 0; i < kNumVars - 1; i++)
    {
        numGCD = gcd(varValues[i].num, numGCD);
    }

    for (int i = 0; numGCD > 1 && i < kNumVars; i++)
    {
        varValues[i] = varValues[i] / numGCD;
    }

    if (verbose)
    {
        if (denomLCM > 1) out << "multiply all values by " << denomLCM;
        if (denomLCM > 1 && numGCD > 1) out << " and ";
        if (numGCD > 1)out << "divide all values by " << numGCD;
        if (denomLCM > 1 || numGCD > 1) out << endl;

        out << "simplified coeffs: ";
        for (int i = 0; i < kNumVars; i++)
        {
            out << char(VAR_NAME_START + i) << " = " << varValues[i];
            if (i < kNumVars - 1) out << ", ";
            else out << endl;
        }
        out << endl;
    }

    // perform one final check to make sure that the coefficients are truly correct
    // by checking to make sure that the amount of each element on each side of the
    // reaction is the same

    if (verbose)
    {
        out << "final element tallies" << endl;
        out << setw(25) << setfill('=') << "=" << endl << setfill(' ');
        out << "|" << setw(7) << " " << "|" << setw(7) << "react" << "|" << setw(7) << "prod" << "|" << endl;
        out << "|" << setw(7) << "elem" << "|" << setw(7) << "side" << "|" << setw(7) << "side" << "|" << endl;
        out << "|" << setw(24) << setfill('=') << "=" << endl << setfill(' ');
    }

    for (Element element : kElementsUsed)
    {
        int reactantTotal = 0, productTotal = 0;

        for (int i = 0; i < kNumReactants; i++)
        {
            Compound compound = kReactionEquation[i].first;
            if (compound.contains(element))
            {
                reactantTotal += compound.elementCount(element) * varValues[i].num;
            }
        }

        for (int i = kProductsStart; i < kNumReactants + kNumProducts; i++)
        {
            Compound compound = kReactionEquation[i].first;
            if (compound.contains(element))
            {
                productTotal += compound.elementCount(element) * varValues[i].num;
            }
        }

        if (verbose)
        {
            out << "|" << setw(7) << toString(element) << "|" << setw(7) << reactantTotal << "|" << setw(7) <<
            productTotal << "|" << endl;
        }

        if (reactantTotal != productTotal)
        {
            throw CouldNotBalanceException(reaction, "algorithm yielded incorrect final results");
        }
    }

    if (verbose)
    {
        out << setw(25) << setfill('=') << "=" << endl << setfill(' ');
        out << endl;
    }

    // now create a new reaction with the correct coefficients and return it

    for (int i = 0; i < kNumVars; i++)
    {
        vector<pair<Compound, int>>::iterator it;

        if (i < kProductsStart)
        {
            it = reactants.begin() + i;
        }
        else if (i >= kProductsStart)
        {
            it = products.begin() + (i - kProductsStart);
        }

        it->second = varValues[i].num;
    }

    return Reaction(reactants, products);
}