#!/bin/bash
set -e

ROOT=`git rev-parse --show-toplevel`

cd $ROOT
rm -rf ./build || true
mkdir build
cd build
cmake -DBOOST_GRAPH=TRUE ..
make

GRAPH_FILE=$1      # graph file path
GED_FILE=$2        # ged file path

GED_LOG=$GED_FILE".log"          # geds' literals check log

BOOST_VF2="nohup ./exp-checkGED 1 $GRAPH_FILE $GED_FILE >$GED_LOG 2>&1 &"

echo "  Running validation for GEDs..."
echo $BOOST_VF2
eval $BOOST_VF2
