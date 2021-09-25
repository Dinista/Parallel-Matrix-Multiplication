# Parallel-Matrix-Multiplication

## Introduction

 A simple sequential and parallel algorithm for random matrix multiplication. The objective of this project is to learn how to use the <a href="https://en.wikipedia.org/wiki/POSIX_Threads">Pthreads</a> library and also compare parallel and sequential code in order to interpret the <a href="https://en.wikipedia.org/wiki/Speedup">Speedup</a> metric seeking for improvement in parallel code.

An portuguese article was written analyzing the performance of the parallel implementation considering the <i>Speedup</i> metric. The article can be found in the <i>/docs</i> folder. 


<b> First project of the class Concurrent Programming.</b>


## The probleam

The matrix multiplication problem is embarrassingly parallel, which means there is no need for interthread communications. It's also a data parallelism problem, so each thread will perform the same task, but on different data.

### How it works

Basically the matrix will be divided equally between threads. Each thread it's gonna perform the multiplication of it respective block, after all threads are done, then each part will be united together forming the final result matrix.

For example, considering having the multiplication:

<div>
<table>
  <tr>
    <td>1</td>
    <td>5</td>
    <td>9</td>
    <td>2</td>
  </tr>
  <tr>
    <td>6</td>
    <td>10</td>
    <td>13</td>
    <td>3</td>
  </tr>
  <tr>
    <td>7</td>
    <td>11</td>
    <td>14</td>
    <td>4</td>
  </tr>
  <tr>
    <td>8</td>
    <td>12</td>
    <td>0</td>
    <td>15</td>
  </tr>
</table>
<p >
X
</p>
<table style="font-size: 24px">
  <tr>
    <td>1</td>
    <td>5</td>
    <td>9</td>
    <td>2</td>
  </tr>
  <tr>
    <td>6</td>
    <td>10</td>
    <td>13</td>
    <td>3</td>
  </tr>
  <tr>
    <td>7</td>
    <td>11</td>
    <td>14</td>
    <td>4</td>
  </tr>
  <tr>
    <td>8</td>
    <td>12</td>
    <td>0</td>
    <td>15</td>
  </tr>
</table>
</p>

</div>

## How to use
