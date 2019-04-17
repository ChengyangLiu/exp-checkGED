# exp-checkGED

[![CircleCI](https://circleci.com/gh/ChengyangLiu/exp-checkGED.svg?style=svg)](https://circleci.com/gh/ChengyangLiu/exp-checkGED)
[![Release](https://img.shields.io/github/release/ChengyangLiu/exp-checkGED.svg)](https://github.com/ChengyangLiu/exp-checkGED/releases)
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-red.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)

exp-checkGED is a tool to validate GEDs or GFDs.
GED or GFD is a structure consisting with a graph pattern and a relation between X and Y, which is X->Y, each of them is a set of literals.

## Remap graph and GEDs
Remap graphs and GEDs (or GFDs) because **continous** vertices with ids starting from **zero** are necessary.

### Command:

```JavaScript
$./script/remap.sh $GRAPH_FILE_PATH $GED_FILE_PATH
```
Please use absolute path.

### Preparations:

- *$GRAPH_FILE_PATH* contains vertex file and edge file. *g.v* and *g.e* are necessary in this path. e.g.
```JavaScript
$GRAPH_FILE_PATH=~/data/g
```

- *$GED_FILE_PATH* contains GEDs (or GFDs) file. *g.ged* is necessary in this path. e.g.
```JavaScript
$GED_FILE_PATH=~/data/g.ged
```

### Result:

It produces *g.remap.v*, *g.remap.e*, *g.ged.remap*.

- *g.remap.*\*: mappings of original graph, whose ids are started from 0 and continous.

- *g.ged.remap*: mappings of original GEDs, whose ids are started frome 0 and continous.

## Validate GEDs
Check whether GEDs(or GFDs) are correct mainly using graph isomorphism algorithm **vf2**.

### Command:

```JavaScript
$./script/run.sh $GRAPH_FILE_PATH $GED_FILE_PATH
```
Please use absolute path and **remap files**.

### Preparations:

- *$GRAPH_FILE_PATH* contains vertex file and edge file. *g.remap.v* and *g.remap.e* are necessary in this path. e.g.
```JavaScript
$GRAPH_FILE_PATH=~/data/g.remap
```

- *$GED_FILE_PATH* contains GEDs (or GFDs) file. *g.ged.remap* is necessary in this path. e.g.
```JavaScript
$GED_FILE_PATH=~/data/g.ged.remap
```

### Result:

It produces *g.ged.remap.log*, *g.ged.remap.map* and *g.ged.remap.vali*.

- *g.ged.remap.log*: validation-GEDs running log.

- *g.ged.remap.map*: mappings between GED's pattern and graph, which uses algorithm vf2.

- *g.ged.remap.vali*: result file containing correct GEDs.
