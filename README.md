# rename-cpp [![C++ CI](https://github.com/edmBernard/rename-cpp/workflows/C++%20CI/badge.svg?branch=master)](https://github.com/edmBernard/rename-cpp/actions)
Minimalist command line tool to track time spend

## Dependencies

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

KoTH-snake depend on:
- [cxxopts](https://github.com/jarro2783/cxxopts): Command line argument parsing
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library

```
./vcpkg install cxxopts fmt
```

## Build instructions

```bash
mkdir build
cd build
# configure make with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

## Disclaimer

It's a toy project. Your comments are welcome, if you spot error or improvement.
