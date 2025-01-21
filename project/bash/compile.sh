#!/bin/bash

ABS_PATH=$(pwd)

SOURCE_DIR="$ABS_PATH/src"
HEADER_DIR="$ABS_PATH/headers"


cat > rootlogon.C << EOF
{
    gSystem->AddIncludePath("-I$HEADER_DIR");

    if (!gSystem->AccessPathName("$SOURCE_DIR/Event_cxx.so")) {
        gSystem->Load("$SOURCE_DIR/Event_cxx.so");
    }

    if (!gSystem->AccessPathName("$SOURCE_DIR/Trajectory_cxx.so")) {
        gSystem->Load("$SOURCE_DIR/Trajectory_cxx.so");
    }
    
}
EOF

root -b -l -q "$SOURCE_DIR/Event.cxx+" || true
root -b -l -q "$SOURCE_DIR/Trajectory.cxx+" || true
root -b -l -q "$SOURCE_DIR/HitPoint.cxx+" || true
root -b -l -q "$SOURCE_DIR/myPoint.cxx+" || true

rm rootlogon.C

