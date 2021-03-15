## Usage

Compile
```shell
$ gcc -o main.o -fopenmp pi_monte_carlo.c my_rand.c my_rand.h 
```

Run
```shell
$ chmod +x main.o
$ ./main.o <num_threads> <num_iters>
```

EG

```shell
$ ./main.o 10 100000000
Estimated pi: 3.117674e+00
```