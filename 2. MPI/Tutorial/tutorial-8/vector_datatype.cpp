#include <bits/stdc++.h>
#include <stdio.h>
#include <mpi.h>

using namespace std;
#include <sys/time.h>

#define print_array(a, n)             \
    cout << "Array '" << #a << "': "; \
    for (int i = 0; i < n; i++)       \
        cout << a[i] << " ";          \
    cout << endl;

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    int myid, size;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int a[4][4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int count = 8, blocklength = 1, stride = 2;

    MPI_Datatype newtype;
    MPI_Type_vector(count, blocklength, stride, MPI_INT, &newtype);
    MPI_Type_commit(&newtype);

    if (myid == 0)
    {
        MPI_Send(&a[0][1], 1, newtype, 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        // int b[count * blocklength];
        // MPI_Recv(&b, count * blocklength, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // print_array(b, count * blocklength);

        int b[4][4] = {11, 21, 31, 41, 51, 61, 71, 81, 91, 101, 111, 121, 131, 141, 151, 161};
        MPI_Recv(&b, 1, newtype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                cout << b[i][j]<<",";
            }
        }
    }

    MPI_Finalize();

    return 0;
}
