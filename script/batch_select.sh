#!/bin/bash
set -e

ROOT=`git rev-parse --show-toplevel`

cd $ROOT
rm -rf ./build || true
mkdir build
cd build
cmake -DBOOST_GRAPH=TRUE ..
make

T_FILE=$1".vali_m"$2
T_FILE_TMP=$T_FILE".tmp"

for ((i=0; i<=54; i++))
do

GED_FILE=$1$i".remap"    # file path

RES_FILE=$GED_FILE".vali_m"$2

SELECT="./exp-checkGED 4 $GED_FILE $2"

echo $SELECT
eval $SELECT

cat $RES_FILE >> $T_FILE_TMP
rm $RES_FILE

done

cat $T_FILE_TMP > $T_FILE
rm $T_FILE_TMP
