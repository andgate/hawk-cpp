# Hawk
Hawk is a languaged designed to function very similar to C++, but borrowing elements from Haskell's type system and clean syntax style. Hawk is intended to strike a balance between clarity and control. Check out the prototype code for examples of how a Hawk program should look. Currently the compiler needs to be built, which you can find in hawkc. As time goes on, this will eventually contain the base toolchain for development with Hawk 


### Building
Requires Flex, Bison, Boost, LLVM, and GTest.
To build hawk, run the following command in the root directory.
```Bash
mkdir build && cd build && cmake ..
```

#

### Symbol Cheatsheet
Hawk syntax mostly avoids the use of reserved keywords. Instead, hawk makes use of operator symbols to declare variables, functions, and data types.

__Module System__

| Symbol | Meaning |
|   ---: | :---    |
|     :: | Module  |
|     -> | Import module |
|     => | Import module as Qualified |

__Expression System__

| Symbol | Meaning |
|   ---: | :---    |
|      $ | Declare Variable |
|      # | Declare Value    |
|      = | Assignment |
|     := | Define function |
|      ^ | Return |

__Type System__

| Symbol | Meaning |
|   ---: | :---    |
|     :- | Record Datatype (Object) or Algebraic Datatype (enum) |
|     :~ | Typeclass |
|     :+ | Implementation |
|     <: | Subtype of |
|      ! |  Immutable |
|     \* | Pointer   |
|      & | Reference |


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

IDE: Sky
