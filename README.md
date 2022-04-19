# Pdp-Procedural_Planet_Generation

## Required libraries
ImageMagick 7 : https://github.com/ImageMagick/ImageMagick
Libnoise : https://github.com/qknight/libnoise

## How to build

```
mkdir build
cd build
cmake ../src
make -j6
./planet_generation
```
A few warnings could be raised depending on your OS, however the executable should be working

## Operating system tested

- [x] Mac OS X 10.15
- [x] Windows 10, WSL2