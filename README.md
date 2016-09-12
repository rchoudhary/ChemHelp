# ChemHelp
This is a simple command line tool to perform common chemistry functions. It is meant to be used as supplementary tool, as it is not capable of tackling large, complex problems. It can perform the trivial calculations that are part of a problem and can be used to help check your work.

# Supported Operating Systems
Mac OSX

(Windows if you have cmake and cygwin)

#Building
To build, simply download all the files, `cd` into the directory and run `./build.sh`. That's it! CMake and make take care of the rest.

# Installation
To install, while the working directory is still the folder with all the files, run `install.sh`. This will copy the necessary `.dylib` to `/usr/local/lib` and the executable and the help page to `/usr/local/bin`.

# Uninstallation
To uninstall, `cd` into the directory with all the files and run `uninstall.sh`. This will remove the `.dylib` from `/usr/local/lib` and the executable and the help page from `/usr/local/bin`.

# Uses
##Input format
Elements should be entered as written, e.g. Calcium is `Ca`, Iron is `Fe`, etc. *Element names are indeed case sensitive!*
Compounds are entered as normal but subscripts should be entered as regular digits, e.g. `CO2`, `H2O`, etc.
Subgroups can be entered as well with the subgroup within parenthesis `(...)` and the subgroup count following the closing parenthesis, e.g. `Mg(NO3)2`.

<b>Note: While charges of elements/compounds are supported, none of chemhelp's functions do anything with them, so they can be left off</b>
## Calculating molar masses
The program can be used to calculate the molar mass of an element, a list of elements, or a compound using the `-m` option. This can be done as such:

    > chemhelp -m Ca
    40.078 g/mol
    > chemhelp -m H2O
    18.015 g/mol
To get the molar masses of more than one element, simply group them together as if they were a compound and run the program in verbose mode with the `-v` flag:

    > chemhelp -mv HOCaPS
    
    =========================================
    |   elem|  count|  mol. mass|      total|
    =========================================
    |      H|      1|      1.008|      1.008|
    |      O|      1|     15.999|     15.999|
    |      S|      1|     32.065|     32.065|
    |     Ca|      1|     40.078|     40.078|
    |      P|      1|     30.974|     30.974|
    =========================================
    
    120.124 g/mol
Just ignore the total at the end.
##Performing stoichiometry
Simply use the `-s` option as such:

    > chemhelp -s [compound 1] [amount 1] [unit 1 (g|mol)] [compound 2] [unit 2 (g|mol)] [OPTIONAL: ratio (x:y)]
*Note that `unit 1` and `unit 2` need not be the same.* The `ratio` is optional and must be expressed as `x:y` where `x` and `y` are both integers. *Also, compounds 1 and 2 can both be elements if desired.*

Example:

    > chemhelp -s H2O 50 g H mol 1:2
    5.551 mol
##Balancing a reaction
To balance a reaction, simply use this format:

    > chemhelp -b [reactants (a+b+c...)]=[products (x+y+z...)]

<b>Important: if you wish to space the reactants/products like so: `C2H4 + O2 = CO2 + H2O`, they must be surrounded with quotes: `"C2H4 + O2 = CO2 + H2O"` or must not contain spaces: `C2H4+O2=CO2+H2O`</b>

Example:

    > chemhelp -b "C2H4 + O2 = CO2 + H2O"
    1, 3, 2, 2
    
The output is simply the coefficients listed in a row.

For the curious, you can also run it in verbose mode to see how the coefficients were solved for:

    > chemhelp -bv "C2H4 + O2 = CO2 + H2O"
    
    balancing equation: A[C2H4] + B[O2] -> C[CO2] + D[H2O]
    elements being examined: C H O
    num variables: 4
    num equations: 3
    
            A   B   C   D
       C[   2   0   1   0 ] <- starting equation
       H[   4   0   0   2 ]
       O[   0   2   2   1 ]
    
    equation 0: 2A = C
        A = 24  <- arbitrarily set
        C = 48  <- newly determined

    equation 1: 4A = 2D
        A = 24
        D = 48  <- newly determined

    equation 2: 2B = 2C + D
        C = 48
        D = 48
        B = 72  <- newly determined
    
    unsimplified coeffs: A = 24, B = 72, C = 48, D = 48
    divide all values by 24
    simplified coeffs: A = 1, B = 3, C = 2, D = 2

    final element tallies
    =========================
    |       |  react|   prod|
    |   elem|   side|   side|
    |========================
    |      C|      2|      2|
    |      H|      4|      4|
    |      O|      6|      6|
    =========================
    
    1, 3, 2, 2
    
##Limiting reactant

To find the limiting rectant of an equation, simply use this format:

    > chemhelp -l [equation]
    
Note: the equation provided must be enclosed in quotes if it contains spaces and <i>must be balanced beforehand!</i>

The program will use the first product as the target product to test against and will prompt the user to enter the amount of each reactant in either grams (g) or moles (mol). The whole output looks like this:

    > chemhelp -l "4NH3+5O2=4NO+6H2O"
    enter amounts of reactants (-1 if not given) in the format "X mol" or "X g"
    NOTE: this won't give the right answer unless the equation is balanced!
    >>> 4NH3: 2 g
    0.117 mols (3.524 g) of NO
    >>> 5O2: 4 g
    0.100 mols (3.001 g) of NO
    the limiting reactant is O2

#Full Command List

    usage: chemhelp [options/flags] [arguments]
    
    Options:
    (only ONE option at a time can be specified)
      -m            calculate molar mass of compound
                        usage: chemhelp -m [compound]
      -s            performs stoichiometry between two compounds
                        usage: chemhelp -s [compound 1] [amount 1]
                            [unit 1 (g, mol)] [compound 2] [unit 2]
                            [OPTIONAL: ratio (x:y)]
      -b            balance a chemical equation
                        usage: chemhelp -b [reactants (a+b+c+...)] =
                            [products (d+e+f+...)]
                        NOTE: if you type spaces into the equation,
                        the equation MUST be surrounded by quotes ("")
      -l            finds the limiting reactant of a reaction. prompts
                    for the amount of each reactant.
                        usage: chemhelp -l [equation]
                        NOTE: equation must be balanced beforehand
    
    Special Options:
    (no flags can be specified alongside these options)
      --help        shows this page
      --version     shows version number and author
      
    Flags:
    (as many flags can be specified as desired)
      -v            show all output and/or work
                        usages: chemhelp -[opt]v [req args to [opt]]
