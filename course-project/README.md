# Parallel Stochastic Gradient Descent Using MPI

A simple implementation of "Parallelized Stochastic Gradient Descent" (Zinkevich et al., 2010) with C++ and MPI.

An example of Linear Regression is demonstrated by encoding the closed form of objective function (least squres) and gradient.

### Usage:

```
cd ./code

mpicxx main_parallel.cpp

mpiexec -n <no-of-threads> ./a.out >output.txt
```

```
utils.h contain the helper funtions used in `main_parallel.cpp`

To play around with model parameters modify `sgd_params.h`
```

### Note:

Running the codebase creates two `.csv` files,

1. train_data.csv
2. loss.csv
