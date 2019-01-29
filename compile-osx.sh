#!/bin/bash

# Includes and libs paths
include_cplex="/Applications/CPLEX_Studio128/cplex/include"
include_concert="/Applications/CPLEX_Studio128/concert/include"
lib_cplex="/Applications/CPLEX_Studio128/cplex/lib/x86-64_osx/static_pic/"
lib_concert="/Applications/CPLEX_Studio128/concert/lib/x86-64_osx/static_pic/"
libs="-lilocplex -lconcert -lcplex -lm -lpthread"

# Check if arg is passed
if [ $# -gt 0 ]; then

    if [ $# != 2 ]; then
        echo "Invalid number of arguments! Two needed!"
        exit 2
    fi
    
    # Check if given source file exists
    if [ -f $1 ]; then
        echo "File: $1"
    else
        echo "File $1 does not exists!"
        exit 3
    fi

    # Try to compile the given file
    if g++ $1 -o $2 -I $include_cplex -I $include_concert -DIL_STD -L $lib_cplex -L $lib_concert $libs ; then
        echo "Compilation successful!"
    else
        echo "Compilation failed!"
    fi

else
    echo "No args given! Please provide the name of the source file to compile."
    exit 1
fi