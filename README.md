# The Kore Programming Language

A pet project for me to learn about programming language implementation.

**Features**

* Statically typed
* Type inference
* Automatic memory management
* No nulls, optionals instead
* Support for different backends

**Focus on**

* Developer productivity
* Batteries included
* Explicitness
* Preferably only one way to do something
* Modularity and code splitting

## Code Structure

All code related to the compiler frontend can be found in `korec/frontend` with
all abstract syntax tree code in `korec/frontend/ast`.

Each compiler backend uses the AST of the parser to generate code for a target
(x86, bytecode, etc.) and can be found in `korec/backend`.

## Build

**Requirements**

* [CMake](https://cmake.org/) >= 3.8
* Optionally [ninja](https://ninja-build.org/)

```shell
$ cmake . -G Ninja
$ ninja
```

Omit `-G Ninja` if you are not using [ninja](https://ninja-build.org/).

Currently, a `korec` binary, which reflects the current state of the frontend,
is built and put in `bin/` along with a binary for running all tests (found in
`tests/`).

## TODO

* [ ] Preliminary language design (see `kore.grammar`)
* [ ] Scanner
* [ ] Parser
* [ ] Typechecker
* [ ] First compiler backend (bytecode runtime)

## Inspiration

* Python
* C#
* Swift
* Go
