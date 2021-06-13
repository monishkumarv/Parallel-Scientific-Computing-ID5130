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

double kernel(int myid, int size, MPI_Status status)
{

    int n = 20;
    float delta = 2.0 / (n - 1);

    double x[n][n], x_prev[n][n];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            x_prev[i][j] = 0.0;
        }
    }

    // Jacobi iterative method:

    double ii, jj, error = 1;
    int iterations = 0;

    while (error > pow(10, -2))
    {
        error = 0.0;
        for (int i = 1; i < n - 1; i++)
        {
            for (int j = 1; j < n - 1; j++)
            {
                ii = -1 + i * delta;
                jj = -1 + j * delta;

                double bij = pow(delta, 2) * q(ii, jj);

                x[i][j] = 0.25 * (x_prev[i + 1][j] + x_prev[i - 1][j] + x_prev[i][j + 1] + x_prev[i][j - 1] + bij);

                error += abs((phi(ii, jj) - x[i][j]) / phi(ii, jj));
            }
        }

        for (int i = 1; i < n - 1; i++)
            for (int j = 1; j < n - 1; j++)
            {
                x_prev[i][j] = x[i][j];
            }

        iterations++;
    }

    double end = MPI_Wtime();

    // Display and save output:
    if (myid == 0)
    {
        std::ofstream myfile;
        myfile.open("jacobi_iterative_method.csv");
        myfile << "x,y,Analytical solution,Numerical solution\n";

        for (int i = 1; i < n - 1; i++)
        {
            for (int j = 1; j < n - 1; j++)
            {
                ii = -1 + i * delta;
                jj = -1 + j * delta;
                cout << phi(ii, jj) << " " << x[i][j] << endl;
                myfile << ii << "," << jj << "," << phi(ii, jj) << "," << x[i][j] << endl;
            }
        }

        cout << "\nNo. of itereations before convergence: " << iterations << endl;
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
