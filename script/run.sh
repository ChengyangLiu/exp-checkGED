#!/bin/bash

ROOT=`git rev-parse --show-toplevel`

GRAPH_FILE=../data/dbpedia_label39      # graph file path
GED_FILE=../data/dbpedia_label39.ged    # ged file path
GED_PART=1    # parts number

cd $ROOT
cd build
cmake ..
make

echo "delivery into "$GED_PART" parts."
./exp-checkGED $GRAPH_FILE $GED_FILE $GED_PART
echo "completed!"

for ((i=0; i<$GED_PART; i++))
do
  GED_PART_FILE=$GED_FILE$i
  GED_PART_FILE_LOG=$GED_FILE$i".log"
  nohup ./exp-checkGED $GRAPH_FILE $GED_PART_FILE >$GED_PART_FILE_LOG 2>&1 &
done
