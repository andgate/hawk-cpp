# Hawk
Hawk is a languaged designed to function very similar to C++, but borrowing elements from Haskell's type system and clean syntax style. Hawk is intended to strike a balance between clarity and control. Check out the prototype code for examples of how a Hawk program should look. Currently the compiler needs to be built, which you can find in hawkc. As time goes on, this will eventually contain the base toolchain for development with Hawk 


# Toolchain
Compiler: hkc
  - Helpful compiler errors, that follow a standard for an ide to manage.

Debugger: hdb

Project Manager: nest

Project Package: egg

Documentation Generator: owl

Linter: eagle

Standard Libary: Core

Main repository: flock.io

X-Plat GUI library: peacock

IDE: 

# Building
Need to build boost for yourself, static and multi-threaded.
To build hawk, go into the root directory and create a folder called "build".
On the command line, enter the build directory and use cmake to setup the build system.
"cmake .."
