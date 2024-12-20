# WAGWAN

### Graph Drawing competition 2025

#### Table of Contents
- [Setup and usage guide](#####setup-and-usage-guide)
- [Structure of the repository](#####Structure-of-the-repo)
- [Table of Results](#####table-of-results)

##### Setup and usage guide
This repo uses  [OGDF Library](https://ogdf.uos.de/) (c). Once cloned, this has to be installed. The folder ogdf includes the source files. Enter the folder an refere to `ogdf`installation guide with the default configration.

When that is done, run make on the root folder. `wagwan.out` should be generated.

To run the program:

`./wagwan.out <input_file> <output_file> <mode(=colling_technique)>`

* input_file: must be an existing file with .json file extension in the competition file format
* output_file: the file where the resulted configuration would be written to, can be existing or none existing (!Warning: if the file already exists its contents would be overwritten)
* mode: parameter in the range of the values 0, 1, 2 (0 by defaults) to specify cooling technique used. [0=>for linear decay, 1=>exponential decay, 2=>logarithmic decay]


##### Structure of the repo

- src/, algorithm/ and include/ : are all working directories that include source code.
- ogdf/ : includes source files for ogdf library, check [Setup](###setup) for mor e details.
- resources/ : includes graph used as input for testing
- outputs/ : includes output result configurations computed and given from wagwan simulated annealing.

### Table of Results
This section presents the results in a tabular format.


Best results optained so far:

* 2024 graphs

Graph |  initial | Linear | Logarithmic | Exponential
------------- | ----------------| ------------ | ------------- | -----------
Graph1   | x | 4 | 3 | x 
Graph2   | x | 0 | 6 | 12
Graph3   | x | 8 | 8 | 5 
Graph4   | x | 7 | 17 | 13
Graph5   | x | 10 | 13 | 26
Graph6   | x | 8 | 4 | 5 
Graph7   | x | 10 | 5 | 18
Graph10  | 1789 | 26 | | 
Graph11  | 997  | 8 | | 
Graph12  | 1799 | 29| |
Graph13  | 1406 | 656 | | 
Graph14  | 3238 | 18 | | 
Graph15  | 1382 | 69 | |
