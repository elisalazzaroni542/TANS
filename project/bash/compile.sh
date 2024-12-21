#!/bin/bash

ABS_PATH=$(pwd)

SOURCE_DIR="$ABS_PATH/src"
HEADER_DIR="$ABS_PATH/headers"

export LD_LIBRARY_PATH="$SOURCE_DIR:$LD_LIBRARY_PATH"

cat > rootlogon.C << EOF
{
    gSystem->AddIncludePath("-I$HEADER_DIR");
}
EOF

root -b -l -q "$SOURCE_DIR/Event.cxx+" || true
root -b -l -q "$SOURCE_DIR/Trajectory.cxx+" || true
root -b -l -q "$SOURCE_DIR/HitPoint.cxx+" || true
#root -b -l -q "$SOURCE_DIR/TestMain.cxx+" || true

rm rootlogon.C

