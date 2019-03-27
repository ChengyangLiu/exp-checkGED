#!/bin/bash
set -e

ROOT=`git rev-parse --show-toplevel`

cd $ROOT
rm -rf ./build || true
mkdir build
cd build
cmake -DBOOST_GRAPH=TRUE ..
make

GED_FILE=$1    # file path

RES_FILE=$GED_FILE".vali_m"$2

SELECT="./exp-checkGED 4 $GED_FILE $2"

echo $SELECT
eval $SELECT
