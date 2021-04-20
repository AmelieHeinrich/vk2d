mingw32-make rebuild_cache
cmake -G "MinGW Makefiles"

mingw32-make
cd example_app
example_app
cd ../