# README
## __Studying the speedup on matrix multiplication achieved by multithreading and multiprocessing__


matmul.c implements matrix multiplication in various modes (single threaded, multi-threading and multi-processing) and shows the speedup achieved by using the above said methods

### __Compiling__

```
gcc -pthread matmul.c -o matmul
```

### __Usage__

```
./matmul --ar <rows_in_A>  --ac <cols_in_A> --br <rows_in_B>  --bc <cols_in_B> [--interactive]
```

+ _--ar_ : Specifies number of rows in matrix A
+ _--ac_ : Specifies number of columns in matrix A 
+ _--br_ : Specifies number of rows in matrix B 
+ _--bc_ : Specifies number of columns in matrix B
+ _--interactive_ : Run the program in interactive, i.e., user can give the input for matrices A and B. In interactive mode the user is asked for input for each mode and product of the given inputs is also displayed.
+ _matmul -h_ :shows te usage of options and parameters

_By default the program generates random matrices of the given dimensions and displays the speedup and time taken  only anad not the result of multiplication_


### __Example Outputs__

#### __Interactive Mode__
```
./matmul --ar 3 --ac 3 --br 3 --bc 3 --interactive
Enter A:
1 2 3
4 5 6
7 8 9
Enter B:
1 0 0
0 1 0
0 0 1
Result:
1 2 3 
4 5 6 
7 8 9 
Enter A:
1 2 3
4 5 6
7 8 9    
Enter B:
1 0 0
0 1 0
0 0 1
Result:
1 2 3 
4 5 6 
7 8 9 
Enter A:
1 2 3
4 5 6
7 8 9
Enter B:
1 0 0
0 1 0
0 0 1
Result:
1 2 3 
4 5 6 
7 8 9 
Time taken for single threaded: 1 us
Time taken for multi process: 1 us
Time taken for multi threaded: 1 us
Speedup for multi process : 1 x
Speedup for multi threaded : 1 x
```

#### __Non-interactive Mode__

```
./matmul --ar 100 --ac 100 --br 100 --bc 100
Time taken for single threaded: 10 us
Time taken for multi process: 14 us
Time taken for multi threaded: 14 us
Speedup for multi process : 0.71 x
Speedup for multi threaded : 0.71 x
```

```
./matmul --ar 300 --ac 300 --br 300 --bc 300
Time taken for single threaded: 329 us
Time taken for multi process: 54 us
Time taken for multi threaded: 15 us
Speedup for multi process : 6.09 x
Speedup for multi threaded : 21.93 x
```

```
./matmul --ar 500 --ac 500 --br 500 --bc 500
Time taken for single threaded: 2365 us
Time taken for multi process: 133 us
Time taken for multi threaded: 57 us
Speedup for multi process : 17.78 x
Speedup for multi threaded : 41.49 x
```

