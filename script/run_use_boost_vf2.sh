#!/bin/bash

ROOT=`git rev-parse --show-toplevel`

GRAPH_FILE=$1      # graph file path
GED_FILE=$2        # ged file path

GRAPH_REMAP_FIEL=$GRAPH_FILE".remap"   # graph map
GED_REMAP_FIEL=$GED_FILE".remap"       # ged map
GED_MAP_GRAPH=$GED_REMAP_FIEL".map"    # the mapping relations from ged to graph

GED_LOG=$GED_REMAP_FIEL".log"    # geds' literals check log

cd $ROOT
cd build
cmake ..
make

REMAP="./exp-checkGED 3 $GRAPH_FILE $GED_FILE"
BOOST_VF2="nohup ./exp-checkGED 1 $GRAPH_REMAP_FIEL $GED_REMAP_FIEL >$GED_MAP_GRAPH 2>&1"
CHECK_LITERAL="nohup ./exp-checkGED 4 $GRAPH_REMAP_FIEL $GED_REMAP_FIEL >$GED_LOG 2>&1"

echo "#### Begin remap Graph file and GEDs file... ####"
echo $REMAP
eval $REMAP
echo "#### Completed! ####"

echo "#### Begin use boost_vf2... ####"
echo $BOOST_VF2
eval $BOOST_VF2
echo "#### Completed! ####"

echo "#### Begin check GEDs' literals... ####"
echo $CHECK_LITERAL
eval $CHECK_LITERAL
echo "#### Completed! ####"
