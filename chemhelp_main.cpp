//
// Created by Rutvik Choudhary on 7/25/15.
//


#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include "chemhelp_defines.h"

using namespace std;


int main(int argc, char** argv)
{
    if (argc == 1)
    {
        cout << "error: no arguments provided" << endl;
        cout << "usage: chemhelp [options/flags] [arguments]" << endl;
        cout << "use `chemhelp --help` for more info" << endl;
        return 1;
    }

    cout << fixed << setprecision(RESULT_PRECISION);

    using std::placeholders::_1;

    // This maps the "short" commands (ones that are prefixed with "-") to their functions

    unordered_map<string, function<void(bool)>> shortCommandMap = {
        {string(CMD_MMASS), bind(performMolarMass, argc, argv, _1)},
        {string(CMD_STOICH), bind(performStoich, argc, argv, _1)},
        {string(CMD_BALANCE), bind(performBalance, argc, argv, _1)},
        {string(CMD_LIMITING_REACTANT), bind(performLimitingReactant, argc, argv, _1)}
    };

    // This maps the "long" commands (ones that are prefixed with "--") to their functions

    unordered_map<string, function<void()>> longCommandMap = {
        {string(CMD_HELP), outputHelp},
        {string(CMD_VERSION), outputVersion}
    };

    string rawArg = ARG_CMD;

    if (rawArg.substr(0, 2) == LONG_CMD_MARKER)
    {
        // The command starts with "--" and is a "long" command. Use the map with the function binding to perform
        // it.

        string command = rawArg.substr(2);
        if (longCommandMap.find(command) == longCommandMap.end())
        {
            cout << "error: unknown command \"" << command << "\"" << endl;
            cout << "use `chemhelp --help` for a list of valid commands" << endl;
            return 1;
        }
        longCommandMap[command]();
    }
    else if (rawArg.substr(0, 1) == SHORT_CMD_MARKER)
    {
        // The command starts with "-"

        bool verboseMode = false;

        // Get the whole command string and split it into a list of chars, each of which is a flag

        string commandFlagsStr = rawArg.substr(1);
        vector<char> commandFlags(commandFlagsStr.begin(), commandFlagsStr.end());

        // Check to see if a verbose flag is present. If so, remove it from the list of flags as we
        // are done with it

        auto verboseFlagIt = find(commandFlags.begin(), commandFlags.end(), CMD_VERBOSE_FLAG[0]);

        if (verboseFlagIt != commandFlags.end())
        {
            verboseMode = true;
            commandFlags.erase(verboseFlagIt);
        }

        // There should only be one flag left because only flags that indicate what action to perform are left.
        // Since they conflict in terms of what to do and what parameters to pass, only one flag can be provided.
        // If one flag is provided then use the function map with the bindings to perform it. Otherwise throw an
        // error

        if (commandFlags.size() == 1)
        {
            string command(1, commandFlags[0]);
            if (shortCommandMap.find(command) == shortCommandMap.end())
            {
                cout << "error: unknown command \"" << command << "\"" << endl;
                cout << "use `chemhelp --help` for a list of valid commands" << endl;
                return 1;
            }
            shortCommandMap[command](verboseMode);
        }
        else if (commandFlags.size() > 1)
        {
            cout << "error: too many options provided" << endl;
            cout << "use `chemhelp --help` for further info" << endl;
        }
        else if (commandFlags.size() < 1)
        {
            cout << "error: no option provided" << endl;
            cout << "use `chemhelp --help` for further info" << endl;
        }
    }
    else
    {
        // No command (prefixed with either "-" or "--") was provided

        cout << "error: incorrect format provided" << endl;
        cout << "use `chemhelp --help` for more info" << endl;
    }

    return 0;
}