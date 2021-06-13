#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

#include <mpi.h>
#include "utils.h" // custom helper functions..

void ParallelSGD(double x[][N], double y[], int m, int n, int myid, int p)
{
    // init zero weights...
    double w[n] = {0}, gradient[n];
    double loss;

    // Opening a csv file:
    std::ofstream myfile;
    myfile.open("loss.csv");
    myfile << "epoch,loss" << endl;

    int epoch = 0;
    while (epoch <= EPOCHS)
    {
        // Draw random sample 'with replacement'...
        int random_index = rand() % m;

        double y_hat = getPrediction(x, random_index, w, n);
        loss = getLoss(x, y, m, n, w);

        for (int j = 0; j < n; j++)
        {
            gradient[j] = (y_hat - y[random_index]) * x[random_index][j];
            w[j] -= ALPHA * gradient[j];
        }

        // Save the loss at every epoch in a csv file...
        myfile << epoch << "," << loss << endl;
        epoch++;
    }

    // Gather all etimated weights together:
    double weights[n];
    MPI_Reduce(w, weights, n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myid == 0)
    {
        // Taking average of weights calculated by each thread..
        for (int j = 0; j < n; j++)
        {
            weights[j] /= p;
        }

        // Display the final answer:
        cout << "\nThe estimated weights(theta) are as follows:" << endl;
        display_1D_Array(weights, n);

        // Display stats:
        cout << "\nEpochs : " << epoch - 1;
        cout << "\nLoss : " << loss << endl;
    }
}

double kernel(int myid, int p, MPI_Status status)
{
    int rows = M / p;
    double X[M][N], Y[M];
    double local_X[rows][N], local_Y[rows];

    if (myid == 0)
    {
        // Generate Toy Data:   x[i] and y[i] = (x * w) + err and err ~ N(0,1)
        genToyData(X, Y, WEIGHTS, M, N, NOISE_MEAN, NOISE_STD);

        cout << "\nOriginal weights used: ";
        display_1D_Array(WEIGHTS, N);
    }

    // Scatter data among p threads...
    MPI_Scatter(X, (rows * N), MPI_DOUBLE, local_X, (rows * N), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(Y, rows, MPI_DOUBLE, local_Y, rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    ParallelSGD(local_X, local_Y, rows, N, myid, p);

    // Display stats:
    if (myid == 0)
    {
        cout << "\nLength of training data (m) = " << M;
        cout << "\nNo. of features in the data (n) = " << N;
        cout << "\nThreads (p) = " << p;
    }

    double end = MPI_Wtime();
    return end;
}

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    double start = MPI_Wtime();

    int myid, size;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    double end = kernel(myid, size, status);
    if (myid == 0)
    {
        printf("\nTotal time taken  = %fs\n", (end - start));
    }

    MPI_Finalize();
    return 0;
}
