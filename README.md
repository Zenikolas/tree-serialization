# Welcome to Tree serialization project!
The aim of the project is to provide mechanism for serialization of multi-dimensional
 trees to stream either stdout or file and de-serialise them back

## Values
Trees can holds three different types of values:
  - integer
  - floating number
  - string

## Install
The project uses `cmake` for building sources, just run:
```
mkkdir build; cd build
cmake ../
cmake --build . -j8
```
For running tests run:
```
ctest
```
  