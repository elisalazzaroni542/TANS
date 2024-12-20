#!/bin/bash

ABS_PATH=$(pwd)

SOURCE_DIR="$ABS_PATH/simulation"


SOURCES=$(find "$SOURCE_DIR" -type f -name '*.cxx') 

# Pass cxx and libraries to the compiler
CXXFLAGS=$(root-config --cflags)
LDFLAGS=$(root-config --libs)

CXXFLAGS="$CXXFLAGS -I$SOURCE_DIR"


if [[ -z "${SOURCES}" ]]; then     # -z returns true if variable is empty
    echo "No .cxx files found in ${SOURCE_DIR}!"
    exit 1
fi

echo "Compiling simulation source files..."
g++ ${CXXFLAGS} ${SOURCES} ${LDFLAGS} #-o ${.OUTPUT}




if [[ $? -ne 0 ]]; then # '$?' = last command exit code, '-ne' = not equal 
    echo "Compilation failed!"
    exit 1
fi



echo "Compilation successful."