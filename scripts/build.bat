mingw32-make rebuild_cache
cmake -G "MinGW Makefiles" -B build

cd build
mingw32-make
cd example_app
example_app
cd ../../