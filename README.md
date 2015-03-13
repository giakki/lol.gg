# lol.gg

### Download Champion builds form champion.gg

## Building the project

Dependencies:

 * [boost](http://www.boost.org/)
 * [libcurl](http://curl.haxx.se/libcurl/)
 * [gumbo-parser](https://github.com/google/gumbo-parser)

The build was tested on Visual Studio 2013, clang 3.5 and gcc 4.9

### Linux: 

 * Make sure `-lcurl` and `-lgumbo` are available.
 * `make` or `make CXX=g++`, if you prefer gcc over clang

### Windows:

 * Change the project library directories to where your dependencies are located.
 * The VS project includes Debug, Release and MinSize configurations.



### License: MIT
