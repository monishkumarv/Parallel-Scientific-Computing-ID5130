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
void SendRecvGhost(double x[][N], int n, int rows, int myid, int p, MPI_Datatype alternate_data, int isRed)
{

    int upper_ghost_start_pos = isRed ? 1 : 2;
    int lower_ghost_start_pos = (rows % 2) ? upper_ghost_start_pos : (3 - upper_ghost_start_pos);

    if (myid % 2 == 0)
    {
        if (myid != 0)
        {
            MPI_Send(&x[1][upper_ghost_start_pos], 1, alternate_data, myid - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&x[0][upper_ghost_start_pos], 1, alternate_data, myid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (myid != (p - 1))
        {
            MPI_Send(&x[(rows + 2) - 2][lower_ghost_start_pos], 1, alternate_data, myid + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&x[(rows + 2) - 1][lower_ghost_start_pos], 1, alternate_data, myid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else
    {
        if (myid != 0)
        {
            MPI_Recv(&x[0][upper_ghost_start_pos], 1, alternate_data, myid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&x[1][upper_ghost_start_pos], 1, alternate_data, myid - 1, 0, MPI_COMM_WORLD);
        }

        if (myid != (p - 1))
        {
            MPI_Recv(&x[(rows + 2) - 1][lower_ghost_start_pos], 1, alternate_data, myid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&x[(rows + 2) - 2][lower_ghost_start_pos], 1, alternate_data, myid + 1, 0, MPI_COMM_WORLD);
        }
    }
}

double kernel(int myid, int size, MPI_Status status)
{
    // Initialization:
    int n = N;    // changing it from 21 to 20, so that n/p is an integer (for p=2, delta = 0.1)...
    int p = size; // use p = 2 threads...
    float delta = 2.0 / (n - 1);

    int rows = (n - 2) / p; // no. of rows allocated to each thread
    double x[rows + 2][N];  // first and last row contains ghost rows...

    for (int i = 0; i < rows + 2; i++)
    {
        for (int j = 0; j < n; j++)
        {
            x[i][j] = 0.0;
        }
    }

    int count = (n - 2) / 2, blocklength = 1, stride = 2;

    MPI_Datatype alternate_data;
    MPI_Type_vector(count, blocklength, stride, MPI_DOUBLE, &alternate_data);
    MPI_Type_commit(&alternate_data);

    // Red-black method:

    double ii, jj, local_error, error = 1;
    int isRed, iterations = 0;

    while (error > pow(10, -2))
    {
        local_error = 0.0;

        // Updating Red points...
        isRed = 1;
        SendRecvGhost(x, n, rows, myid, p, alternate_data, isRed);
        for (int i = 1; i < (rows + 2) - 1; i++)
        {
            for (int j = 1; j < n - 1; j++)
            {
                if ((i + j) % 2 == 1)
                {
                    ii = -1 + ((rows * myid) + i) * delta;
                    jj = -1 + j * delta;
                    double bij = pow(delta, 2) * q(ii, jj);

                    x[i][j] = 0.25 * (x[i + 1][j] + x[i - 1][j] + x[i][j + 1] + x[i][j - 1] + bij);

                    // double err_ij = abs((phi(ii, jj) - x[i][j]) / phi(ii, jj));
                    // local_error = local_error > err_ij ? local_error : err_ij;

                    local_error += abs((phi(ii, jj) - x[i][j]) / phi(ii, jj));
                }
            }
        }

        // Updating Black points...
        isRed = 0;
        SendRecvGhost(x, n, rows, myid, p, alternate_data, isRed);
        for (int i = 1; i < (rows + 2) - 1; i++)
        {
            for (int j = 1; j < n - 1; j++)
            {
                if ((i + j) % 2 == 0)
                {
                    ii = -1 + ((rows * myid) + i) * delta;
                    jj = -1 + j * delta;
                    double bij = pow(delta, 2) * q(ii, jj);

                    x[i][j] = 0.25 * (x[i + 1][j] + x[i - 1][j] + x[i][j + 1] + x[i][j - 1] + bij);

                    // double err_ij = abs((phi(ii, jj) - x[i][j]) / phi(ii, jj));
                    // local_error = local_error > err_ij ? local_error : err_ij;

                    local_error += abs((phi(ii, jj) - x[i][j]) / phi(ii, jj));
                }
            }
        }

        MPI_Allreduce(&local_error, &error, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        // MPI_Allreduce(&local_error, &error, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
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
        myfile.open("q2_b_red_black.csv");
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