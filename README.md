# Extract Event Information
This is a conversion layer between the unpacked Analysis Trees from GRSISort and a csv file for use in a python based analysis program

## Table of Contents
  * [Installation](#installation)

# Installation
0. Requires GRSISort 4.X.X
1. Get the code, either via tarball or from github

I use ssh keys with github so the below command reflects that.
```
git clone git@github.com:cnatzke-TRIUMF/extract-event-information.git
```
2. Build program using standard cmake process, e.g.
```
mkdir myBuild && cd myBuild
cmake ..
make
make install # if you would like to install the programs elsewhere.
```
3. Do science.
