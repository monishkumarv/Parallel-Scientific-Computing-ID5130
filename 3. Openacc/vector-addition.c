#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 300

void init(int matrix[][N])
{
    #pragma acc parallel loop present(matrix[0:N][0:N])
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = i * N + j;
        }
    }
}

void add(int c[][N], int a[][N], int b[][N])
{
    #pragma acc parallel loop present (a[0:N][0:N], b[0:N][0:N], c[0:N][0:N]) collapse(2)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
} 

int main()
{
    int a[N][N], b[N][N], c[N][N];

    #pragma acc data create(a,b,c) copyout(c)
    {
        init(a);
        init(b);
        add(c, a, b);
    }
    
    printf("%d\n", c[100][100]);

    return 0;
}
