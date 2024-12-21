#!/bin/bash

ABS_PATH=$(pwd)

SOURCE_DIR="$ABS_PATH/simulation"

export LD_LIBRARY_PATH="$SOURCE_DIR:$LD_LIBRARY_PATH"

root -b -l -q "$SOURCE_DIR/Event.cxx+"         || true
root -b -l -q "$SOURCE_DIR/Trajectory.cxx+"    || true
root -b -l -q "$SOURCE_DIR/HitPoint.cxx+"      || true
#root -b -l -q "$SOURCE_DIR/TestMain.cxx+"


#ABS_PATH=$(pwd)
#
#SOURCE_DIR="$ABS_PATH/simulation"
#
#SOURCES=$(find "$SOURCE_DIR" -type f -name '*.cxx')
#
#
#
#if [[ -z "${SOURCES}" ]]; then # -z returns true if variable is empty
#    echo "No .cxx files found in ${SOURCE_DIR}!"
#    exit 1
#fi
#
##echo "Running ROOT macros in batch mode..."
#
#for SOURCE in ${SOURCES}; do
#    echo "Compiling $SOURCE..."
#    root -b -l -q "$SOURCE"
#
#    if [[ $? -ne 0 ]]; then # '$?' = last command exit code, '-ne' = not equal
#        echo "Execution failed for $SOURCE!"
#        exit 1
#    fi
#
#done
#
#echo "All files compiled."
