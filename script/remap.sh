#!/bin/bash
################################################################################
# Use: rewrite graph and GEDs, make vertices start from id 0 and continous.
# Author: Liucy
# Date: 4.16,2019
# Input: $graph file path; $GED file path
# Output: .remap files
################################################################################
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
