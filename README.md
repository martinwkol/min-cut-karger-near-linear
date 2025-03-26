# min-cut-karger-near-linear  

This repository contains a C++ implementation of a Monte Carlo algorithm for computing a minimum cut of a weighted, undirected graph in near-linear time **O(m log(n)^4)** (where **m** is the number of edges and **n** is the number of vertices).  

The algorithm is based on the paper *["A Simple Algorithm for Minimum Cuts in Near-Linear Time"](https://arxiv.org/abs/1908.11829)* by Nalin Bhardwaj, Antonio Molina Lovett, and Bryce Sandlund (2020).  

## Installation & Compilation  

Prerequisites:  
- `g++` compiler  
- `make` build system  

Compilation:  
```sh
make
```

## Usage  

The program expects a weighted, undirected graph as input via the console and outputs the weight of the minimum cut along with the corresponding vertices.  

### Input Format  
```
<numVertices> <numEdges>
<endpoint0> <endpoint1> <weight>
...
```
- **numVertices**: Number of vertices in the graph  
- **numEdges**: Number of edges  
- Each following line represents an edge with:  
  - `endpoint0`: First vertex of the edge  
  - `endpoint1`: Second vertex of the edge  
  - `weight`: Weight of the edge  

### Output Format  
```
<minCutWeight>
<vertex_1>
<vertex_2>
...
```
- **minCutWeight**: Weight of the computed minimum cut  
- The following lines list the vertices belonging to one of the two partitions of the minimum cut.  

### Example  
#### Input:  
```
4 5
0 1 2
1 2 3
2 3 4
3 0 1
0 2 5
```
#### Output:  
```
3
0
1
```