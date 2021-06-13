#include <bits/stdc++.h>
#include <stdio.h>
#include <mpi.h>

using namespace std;
#include <sys/time.h>

int main(int argc, char **argv)
{
    int myid, size, tag = 100;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // int a[4][4] = {{0, 0, 0, 0},
    //                {1, 2, 3, 4},
    //                {2, 4, 6, 8},
    //                {3, 6, 9, 12}};
    // int b[4] = {1, 2, 3, 4};

    int a[4][4], b[4], c[4];
    if (myid == 0)
    {

        for (int i = 0; i < 4; i++)
        {
            b[i] = i + 1;
            for (int j = 0; j < 4; j++)
            {
                a[i][j] = i * (j + 1);
            }
        }
    }

    // ----------------------------------- Use processor (p) = 2 -----------------------------------

    MPI_Bcast(b, 4, MPI_INT, 0, MPI_COMM_WORLD);

    int a_id[2][4];
    MPI_Scatter(a, 8, MPI_INT, a_id, 8, MPI_INT, 0, MPI_COMM_WORLD);

    int c_id[2] = {0, 0};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            c_id[i] += a_id[i][j] * b[j];
        }
    }

    MPI_Gather(c_id, 2, MPI_INT, c, 2, MPI_INT, 0, MPI_COMM_WORLD);

    if (myid == 0)
    {
        cout << "a x b = c = [" << c[0] << "," << c[1] << "," << c[2] << "," << c[3] << "]" << endl;
    }

    // ----------------------------------------------------------------------------------------------

    int x[4] = {1, 2, 5, 4}, y[4];
    MPI_Allreduce(x, y, 4, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    for (int i = 0; i < size; i++)
    {
        MPI_Reduce(x, y, 4, MPI_INT, MPI_SUM, i, MPI_COMM_WORLD);
    }
    cout << myid << " : " << y[0] << "," << y[1] << "," << y[2] << "," << y[3] << endl;

    // ----------------------------------------------------------------------------------------------

    MPI_Finalize();

    return 0;
}
