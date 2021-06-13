#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

#include <mpi.h>
#include "utils_v0.h"

void ParallelSGD(double **x, double y[], int m, int n, int myid, int p, double alpha = 0.001)
{
    double w[n] = {0}, gradient[n];
    double loss;

    // Opening a csv file:
    std::ofstream myfile;
    myfile.open("loss.csv");
    myfile << "epoch,loss" << endl;

    int epoch = 0;
    while (epoch <= 5000)
    {
        // Draw random sample with replacement...
        int random_index = rand() % m;

        double y_hat = getPrediction(x, random_index, w, n);
        loss = getLoss(x, y, m, n, w);

        for (int j = 0; j < n; j++)
        {
            gradient[j] = (y_hat - y[random_index]) * x[random_index][j];
            w[j] = w[j] - alpha * gradient[j];
        }

        myfile << epoch << "," << loss << endl;
        epoch++;
    }

    // -------------------------------------------------------------------------------------------------------------------- //

    double weights[n];
    MPI_Reduce(w, weights, n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myid == 0)
    {
        for (int j = 0; j < n; j++)
        {
            weights[j] /= p;
        }

        // Display the final answer:
        cout << "\nThe calculated weights(theta) are as follows:" << endl;
        display_1D_Array(weights, n);

        // stats:
        cout << "\nEpochs : " << epoch - 1;
        cout << "\nLoss : " << loss << endl;
    }
}

double kernel(int myid, int p, MPI_Status status)
{
    long long m = 10000;
    int n = 10;
    int rows = m / p;

    // m = no. of training data, n = no. of features + 1
    double **X = Create_2D_Array(m, n, 0);
    double X_buf[m][n], Y[m];
    double err_mean = 0.0, err_std = 1.0;

    if (myid == 0)
    {

        // Get the below values from user:
        double weights[n] = {1.0, -2.0, 8.0, 4.0, -9.0, 5.0, 7.0, 2.0, -3.0, 6.0};
        // Generate Toy Data:   x[i] = i, y[i] = x.w + err and err ~ N(0,1)
        genToyData(X, Y, weights, m, n, err_mean, err_std);
        cout << "\nOriginal weights used: ";
        display_1D_Array(weights, n);

        // Copying X to X_buf:
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                X_buf[i][j] = X[i][j];
            }
        }
    }

    double **local_X = Create_2D_Array(rows, n, 0);
    double local_X_buf[rows][n], local_Y[rows];

    MPI_Scatter(X_buf, (rows * n), MPI_DOUBLE, local_X_buf, (rows * n), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(Y, rows, MPI_DOUBLE, local_Y, rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < n; j++)
        {
            local_X[i][j] = local_X_buf[i][j];
        }
    }

    ParallelSGD(local_X, local_Y, rows, n, myid, p);

    // stats:
    if (myid == 0)
    {
        cout << "\nLength of training data (m) = " << m;
        cout << "\nNo. of features in the data (n) = " << n;
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
        cout << "------------------------------------------------------------------------------------------------\n\n";
    }

    MPI_Finalize();
    return 0;
}
