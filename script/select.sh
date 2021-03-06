#!/bin/bash
################################################################################
# Use: select right GEDs whose number of isomorphism matches is more than k
# Author: Liucy
# Date: 4.16,2019
# Input: $GED file path; $K
# Output: result file
################################################################################
set -e

ROOT=`git rev-parse --show-toplevel`

cd $ROOT
#rm -rf ./build || true
#mkdir build
cd build
#cmake -DBOOST_GRAPH=TRUE ..
#make

GED_FILE=$1".remap.reduced"    # file path
T_FILE=$1".vali_m"$2
RES_FILE=$GED_FILE".vali_m"$2

SELECT="./exp-checkGED 4 $GED_FILE $2"

echo $SELECT
eval $SELECT

cat $RES_FILE > $T_FILE
rm $RES_FILE
