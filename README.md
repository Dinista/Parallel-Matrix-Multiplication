# Parallel-Matrix-Multiplication

## Introduction

 A simple sequential and parallel algorithm for random matrix multiplication. The objective of this project is to learn how to use the <a href="https://en.wikipedia.org/wiki/POSIX_Threads">Pthreads</a> library and also compare parallel and sequential code in order to interpret the <a href="https://en.wikipedia.org/wiki/Speedup">Speedup</a> metric seeking for improvement in parallel code.

An portuguese article was written analyzing the performance of the parallel implementation considering the <i>Speedup</i> metric. The article can be found in the <i>/docs</i> folder. 


<b> First project of the class Concurrent Programming.</b>


## The probleam

The matrix multiplication problem is embarrassingly parallel, which means there is no need for interthread communications. It's also a data parallelism problem, so each thread will perform the same task, but on different data.

### How it works

Basically the matrix will be divided equally between threads. Each thread it's gonna perform the multiplication of it respective block, after all threads are done, then each part will be united together forming the final result matrix.

## How to use
Compile the sequential code:
```console

gcc mm_seq.c -o mm_seq

``` 
Compile the parallel code:
```console

gcc mm_par.c -o mm_par -lpthread

``` 

To execute:
```console
./mm -m <matrix size>
./mm -m <matrix size> -t <numbers of threads>
``` 

Example:
```console
./mm -m 512 -t 4
``` 


