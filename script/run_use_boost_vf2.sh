#!/bin/bash
set -e

ROOT=`git rev-parse --show-toplevel`

GRAPH_FILE=$1      # graph file path
GED_FILE=$2        # ged file path

GRAPH_REMAP_FIEL=$GRAPH_FILE".remap"   # graph map
GED_REMAP_FIEL=$GED_FILE".remap"       # ged map

GED_LOG=$GED_REMAP_FIEL".log"    # geds' literals check log

cd $ROOT
#rm -rf ./build || true
#mkdir build
cd build
#cmake -DBOOST_GRAPH=TRUE ..
#make

REMAP="./exp-checkGED 3 $GRAPH_FILE $GED_FILE"
BOOST_VF2="nohup ./exp-checkGED 1 $GRAPH_REMAP_FIEL $GED_REMAP_FIEL >$GED_LOG 2>&1 &"

echo "#### Begin remap Graph file and GEDs file... ####"
echo $REMAP
eval $REMAP
echo "#### Completed! ####"

echo "#### Begin validate GEDs... ####"
echo $BOOST_VF2
eval $BOOST_VF2
echo "#### Running in Background ####"
