# ID3, Reduced Error Pruing, and Random Forests - BITS F464 Assigment 2

## Introduction

There are several methods to classify data from a dataset. One notable method is to use decision trees which are recursively generated data structures based on the training data.

For this assignment, the task is to classify whether a given instance of a person's details can be used to deduce if they earn more or less than 50,000 USD per annum.

## Installation

This project has dependencies on the SQLite3 library and SQLiteCpp for interfacing SQL with C++ code. The project has been built with CMake. The libraries can be downloaded with:

```
sudo apt install libsqlite3-dev cmake
```

The SQLiteCpp must be built from source. Instructions for doing so can be found here: [https://github.com/SRombauts/SQLiteCpp/](https://github.com/SRombauts/SQLiteCpp/)

Once the library has been installed, clone or unzip the files to your favorite folder and run:

```
mkdir build
cd build
cmake ..
make -j4
```

## Code Base

random_dataset
build_random_trees
rebuild_forest
libsqlite3-0
