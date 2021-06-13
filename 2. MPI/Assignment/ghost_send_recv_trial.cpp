// .........................................................................................................................................

// int n = N;
// int p = size;
// int rows = (n - 2) / p;
// double b[rows + 2][N];

// for (int i = 0; i < rows + 2; i++)
// {
//     for (int j = 0; j < n; j++)
//     {
//         b[i][j] = myid;
//     }
// }

// int count = (n - 2) / 2, blocklength = 1, stride = 2;

// MPI_Datatype alternate_data;
// MPI_Type_vector(count, blocklength, stride, MPI_DOUBLE, &alternate_data);
// MPI_Type_commit(&alternate_data);

// int isRed = 1;
// SendRecvGhost(b, n, rows, myid, p, alternate_data, isRed);

// if (myid == 1)
// {
//     for (int i = 0; i < rows + 2; i++)
//     {
//         for (int j = 0; j < n; j++)
//         {
//             cout << b[i][j] << ",";
//         }
//         cout << endl;
//     }
// }

// .........................................................................................................................................