## What's exp-checkGED
exp-checkGED is a tool to validate GEDs or GFDs and then produces all correction.
GED or GFD is a structure consisting with a graph pattern and a relation between X and Y, which is X->Y, each of them is a set of literals.

## How to run exp-checkGED

#### Command

```JavaScript
$./script/run.sh $GRAPH_FILE_PATH $GED_FILE_PATH
```
Please use absolute path.

#### Preparations

- *$GRAPH_FILE_PATH* contains vertex file and edge file. *g.v* and *g.e* are necessary in this path. e.g.
```JavaScript
$GRAPH_FILE_PATH=~/data/g
```

- *$GED_FILE_PATH* contains GED (or GFD) file. *g.ged* is necessary in this path. e.g.
```JavaScript
$GED_FILE_PATH=~/data/g.ged
```

#### Result

It produces *g.remap.v*, *g.remap.e*, *g.ged.remap*, *g.ged.remap.log*, *g.ged.remap.map* and *g.ged.remap.vali*.

- *g.remap.*\*: mappings of original graph, whose numbers are started from 0 and continous.

- *g.ged.remap*: mappings of original GEDs, whose numbers are started frome 0 and continous.

- *g.ged.remap.log*: validation-GEDs running log.

- *g.ged.remap.map*: mappings between GED's pattern and graph, which uses algorithm vf2.

- *g.ged.remap.vali*: correct GEDs file.

## How to select top-k matched GEDs (or GFDs)

#### Command

```JavaScript
$./script/select.sh $GED_FILE_PATH $K
```
Please use absolute path.

#### Preparations

- *$GED_FILE_PATH* contains GED (or GFD) file. Please use remap file. *g.ged.remap*, *g.ged.remap.log* and *g.ged.remap.map* are necessary in this path. e.g.
```JavaScript
$GED_FILE_PATH=~/data/g.ged.remap
```

- *$K* is an integer number, which means selecting right GEDs whose number of isomorphism matches is more than k. e.g.
```JavaScript
$K=5
```

#### Result

It produces *g.ged.remap.vali_m$K*.
