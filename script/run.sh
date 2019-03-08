#!/bin/bash

ROOT=`git rev-parse --show-toplevel`

GRAPH_FILE=../data/dbpedia_label39      # graph file path
GED_FILE=../data/dbpedia_label39.ged    # ged file path
GED_PART=64    # parts number

cd build
cmake ..
make

echo $GRAPH_FILE
echo $GED_FILE
echo $GED_PART

./exp-checkGED $GRAPH_FILE $GED_FILE $GED_PART

for ((i=0; i<$GED_PART; i++))
do
  GED_PART_FILE=$GED_FILE$i
  ./exp-checkGED $GRAPH_FILE $GED_PART_FILE
  echo $GED_PART_FILE
done
