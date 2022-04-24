# Small Basic

Small Basic interpreter written in C++.
A treewalk interpreter to interpret and produce output from a subset of Small Basic,
shares syntax with Small Basic although some features have been omitted due to structure.
Unlike the original Small Basic implementation this version also features maps!

## Installation

Here is how to install:

```shell
git clone https://github.com/Callum-A/small-basic.git
cd small-basic
mkdir build
make all
```

## Example usage
Here is some example usages:
```shell
# Normal node
./build/sb path_to_file.sb

# Debug mode
./build/sb path_to_file.sb --debug

# Symbol table
./build/sb path_to_file.sb --sym

# Breakpoints, lines 1, 2, 3
./build/sb path_to_file.sb 1 2 3

# All flags at once
./build/sb path_to_file.sb --debug --sym 1 2 3
```

## Run Tests

Ensure a Small Basic executale is located in the `build` folder and then
run the following command from the project root.
```shell
python ./src/test/main.py
```