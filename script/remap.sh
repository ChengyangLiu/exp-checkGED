#!/bin/bash
set -e

ROOT=`git rev-parse --show-toplevel`

cd $ROOT
#rm -rf ./build || true
#mkdir build
cd build
#cmake -DBOOST_GRAPH=TRUE ..
#make

GRAPH_FILE=$1    # graph file path
GED_FILE=$2      # file path

REMAP="./exp-checkGED 3 $GRAPH_FILE $GED_FILE"

echo "  Begin remap Graph file and GEDs file..."
echo $REMAP
eval $REMAP
