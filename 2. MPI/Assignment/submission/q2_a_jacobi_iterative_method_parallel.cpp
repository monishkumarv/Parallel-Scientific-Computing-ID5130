#include <bits/stdc++.h>
#include <stdio.h>
#include <mpi.h>

using namespace std;
#include <sys/time.h>

double q(double x, double y)
{
    return (2 * (2 - pow(x, 2) - pow(y, 2)));
}

double phi(double x, double y)
{
    return ((pow(x, 2) - 1) * (pow(y, 2) - 1));
}

// changing it from 21 to 20, so that n/p is an integer (for p=2, delta = 0.1)...
#define N 210
void SendRecvGhost(double x_prev[][N], int n, int rows, int myid, int p)
{
    if (myid % 2 == 0)
    {
        if (myid != 0)
        {
            MPI_Send(&x_prev[1][0], n, MPI_DOUBLE, myid - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&x_prev[0][0], n, MPI_DOUBLE, myid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (myid != (p - 1))
        {
            MPI_Send(&x_prev[(rows + 2) - 2][0], n, MPI_DOUBLE, myid + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&x_prev[(rows + 2) - 1][0], n, MPI_DOUBLE, myid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else
    {
        if (myid != 0)
        {
            MPI_Recv(&x_prev[0][0], n, MPI_DOUBLE, myid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&x_prev[1][0], n, MPI_DOUBLE, myid - 1, 0, MPI_COMM_WORLD);
        }

        if (myid != (p - 1))
        {
            MPI_Recv(&x_prev[(rows + 2) - 1][0], n, MPI_DOUBLE, myid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&x_prev[(rows + 2) - 2][0], n, MPI_DOUBLE, myid + 1, 0, MPI_COMM_WORLD);
        }
    }
}

double kernel(int myid, int size, MPI_Status status)
{

    // Initialization:
    int n = N;    // changing it from 21 to 20, so that n/p is an integer (for p=2, delta = 0.1)...
    int p = size; // use p = 2 threads...
    float delta = 2.0 / (n - 1);

    int rows = (n - 2) / p;                     // no. of allocated to each thread
    double x[rows + 2][N], x_prev[rows + 2][N]; // first and last row contains ghost rows...

    for (int i = 0; i < rows + 2; i++)
    {
        for (int j = 0; j < n; j++)
        {
            x_prev[i][j] = 0;
        }
    }

    // Jacobi iterative method:

    double ii, jj, local_error, error = 1;
    int iterations = 0;

    while (error > pow(10, -2))
    {
        // MPI Communication: (send/recv of ghost rows)
        SendRecvGhost(x_prev, n, rows, myid, p);

        local_error = 0.0;
        for (int i = 1; i < (rows + 2) - 1; i++)
        {
            for (int j = 1; j < n - 1; j++)
            {
                ii = -1 + ((rows * myid) + i) * delta;
                jj = -1 + j * delta;

                double bij = pow(delta, 2) * q(ii, jj);

                x[i][j] = 0.25 * (x_prev[i + 1][j] + x_prev[i - 1][j] + x_prev[i][j + 1] + x_prev[i][j - 1] + bij);

                local_error += abs((phi(ii, jj) - x[i][j]) / phi(ii, jj));
            }
        }

        for (int i = 1; i < (rows + 2) - 1; i++)
            for (int j = 1; j < n - 1; j++)
            {
                x_prev[i][j] = x[i][j];
            }

        MPI_Allreduce(&local_error, &error, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        iterations++;
        if (myid == 0)
        {
            cerr << error << endl;
        }
    }

    double dfdx[n][n] = {0};
    MPI_Gather(&x[1][0], (rows * n), MPI_DOUBLE, &dfdx[1][0], (rows * n), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    // Display and save output:
    if (myid == 0)
    {
        std::ofstream myfile;
        myfile.open("q2_a_jacobi_iterative_method_parallel.csv");
        myfile << "x,y,Analytical solution,Numerical solution\n";

        for (int i = 1; i < n - 1; i++)
        {
            for (int j = 1; j < n - 1; j++)
            {
                ii = -1 + i * delta;
                jj = -1 + j * delta;
                cout << phi(ii, jj) << " " << dfdx[i][j] << endl;
                myfile << ii << "," << jj << "," << phi(ii, jj) << "," << dfdx[i][j] << endl;
            }
        }

        cout << "\nDelta = " << delta;
        cout << "\nn = " << n;
        cout << "\nThreads (p) = " << size;
        cout << "\nIterations = " << iterations;
        cout << "\nError: " << error << endl;
    }
    return end;
}

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);
    double start = MPI_Wtime();

    int myid, size;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    double end = kernel(myid, size, status);
    if (myid == 0)
        printf("\nTotal time taken  = %fs\n", (end - start));

    MPI_Finalize();

    return 0;
}