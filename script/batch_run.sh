#!/bin/bash
set -e

ROOT=`git rev-parse --show-toplevel`

cd $ROOT
rm -rf ./build || true
mkdir build
cd build
cmake -DBOOST_GRAPH=TRUE ..
make

GRAPH_FILE=$1          # graph file path

########################################
# Modify parameters to fit your case.  #
########################################
for ((i=0; i<=10; i++))
do

GED_FILE=$2$i    # file path
########################################

GRAPH_REMAP_FIEL=$GRAPH_FILE".remap"   # graph map
GED_REMAP_FIEL=$GED_FILE".remap"       # ged map
GED_LOG=$GED_REMAP_FIEL".log"          # geds' literals check log

REMAP="./exp-checkGED 3 $GRAPH_FILE $GED_FILE"
BOOST_VF2="nohup ./exp-checkGED 1 $GRAPH_REMAP_FIEL $GED_REMAP_FIEL >$GED_LOG 2>&1"

echo "  Begin remap Graph file and GEDs file..."
echo $REMAP
eval $REMAP

echo "  Running validation for GEDs..."
echo $BOOST_VF2
eval $BOOST_VF2

done
