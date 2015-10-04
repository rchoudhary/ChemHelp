//
// Created by Rutvik Choudhary on 7/25/15.
//

#pragma once


#define PROGRAM_NAME "ChemHelp"
#define AUTHOR "Rutvik Choudhary"
#define VERSION "0.1.0"


#define RESULT_PRECISION 3
#define UNIT_MOL "mol"
#define UNIT_GRAMS "g"


#define ARG_CMD argv[1]
#define SHORT_CMD_MARKER "-"
#define LONG_CMD_MARKER "--"

#define CMD_VERBOSE_FLAG "v"

#define CMD_HELP "help"
#define CMD_VERSION "version"


#define CMD_MMASS "m"
#define CMD_MMASS_NUMARGS 3
#define ARG_MMASS_COMPOUND argv[2]


#define CMD_STOICH "s"
#define CMD_STOICH_DEF_NUMARGS 7
#define CMD_STOICH_SPEC_NUMARGS 8
#define ARG_STOICH_FROM_COMPOUND argv[2]
#define ARG_STOICH_FROM_AMT argv[3]
#define ARG_STOICH_FROM_UNIT argv[4]
#define ARG_STOICH_TO_COMPOUND argv[5]
#define ARG_STOICH_TO_UNIT argv[6]
#define ARG_STOICH_SPEC_RATIO argv[7]
#define STOICH_RATIO_DELIM ':'


#define CMD_BALANCE "b"
#define CMD_BALANCE_NUMARGS 3
#define ARG_BALANCE_REACTION argv[2]
#define EQUATION_DELIM "+"
#define REACT_PROD_SPLIT "="


#define CMD_LIMITING_REACTANT "l"
#define CMD_LIMITING_NUMARGS 3
#define ARG_LIMITING_REACTION argv[2]


void performMolarMass(int argc, char** argv, bool verbose);
void performStoich(int argc, char** argv, bool verbose);
void performBalance(int argc, char** argv, bool verbose);
void performLimitingReactant(int argc, char** argv, bool verbose);
void outputHelp();
void outputVersion();