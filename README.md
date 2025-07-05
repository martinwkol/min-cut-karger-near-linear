# min-cut-karger-near-linear

This repository contains a C++ implementation of a Monte Carlo algorithm for computing a minimum cut of a weighted, undirected graph in near-linear time **O(m log(n)^4)**, where **m** is the number of edges and **n** is the number of vertices.

The algorithm is based on the paper [*"A Simple Algorithm for Minimum Cuts in Near-Linear Time"*](https://arxiv.org/abs/1908.11829) by Nalin Bhardwaj, Antonio Molina Lovett, and Bryce Sandlund (2020), and is further explained in my [**bachelor thesis**](./docs/thesis.pdf).

---

## 🔧 Quick Start

If you just want to build and run the program (no tests, no presets):

```sh
mkdir build
cd build
cmake ..
make
./min_cut < ../input/i1.txt
````

This will:

* Compile the project in **Release** mode by default
* Create a `min_cut` binary in the `build/` directory
* Run the program on the example input `i1.txt`

---

## 🛠 Advanced Build Options (Presets, Testing, Debugging)

### Prerequisites

* CMake

  * **v3.21+** for building with presets
  * **v3.16+** for manual configuration
* A C++ compiler (e.g., `g++`, `clang++`)

### Building with CMake Presets (Recommended for Development)

This project supports [CMake presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) via `CMakePresets.json`.

Presets:

* `release`:

  * `CMAKE_BUILD_TYPE=Release`
  * `BUILD_TESTING=Off`
* `debug`:

  * `CMAKE_BUILD_TYPE=Debug`
  * `BUILD_TESTING=On`
* `asan`:

  * `CMAKE_BUILD_TYPE=Asan`
  * `BUILD_TESTING=On` (Address Sanitizer enabled)

Build example:

```sh
cmake --preset release
cmake --build --preset release
```

Binary will be placed in `build/release/`.

### Running Tests

Tests use [Catch2](https://github.com/catchorg/Catch2). If `BUILD_TESTING=On`, you can run them with:

```sh
ctest --preset debug
```

or

```sh
ctest --preset asan
```

---

## 🚀 Usage

### Input Overview

The program expects a weighted, undirected graph as input. The first line specifies a parameter `d`, which controls the success probability of the algorithm:

```
Success probability = 1 − O(1/n^d)
```

where `n` is the number of vertices.

### Input Format

```
<d>
<numVertices> <numEdges>
<endpoint0> <endpoint1> <weight>
...
```

* **d**: Exponent controlling the success probability
* **numVertices**: Number of vertices
* **numEdges**: Number of edges
* Each edge line:

  * `endpoint0` and `endpoint1`: Edge endpoints (= Vertex IDs in the range 0..(numVertices - 1))
  * `weight`: Edge weight

### Output Format

```
<minCutWeight>
<vertex_1>
<vertex_2>
...
```

* **minCutWeight**: Weight of the minimum cut
* Vertices on one side of the partition

### Running the Program

Once built, run the binary with an input file:

```sh
./min_cut < input/i1.txt
```

If built with presets (e.g., release):

```sh
cd build/release
./min_cut < ../../input/i1.txt
```

### Example

#### Input (`input/i1.txt`)

```
1
6 7
0 1 3.0
0 2 2.0
1 2 1.0
1 3 2.0
2 4 2.0
3 5 3.0
4 5 3.0
```

#### Output

```
4
4
5
3
```

---

## 📁 Example Input Files

The `input/` directory includes several examples:

* `i1.txt`
* `i2.txt`
* `i3.txt`
* `i4.txt`
* `i5.txt`

These demonstrate the required format and can be used for quick testing.
