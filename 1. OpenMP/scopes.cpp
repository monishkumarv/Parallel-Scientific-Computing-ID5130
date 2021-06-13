#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
#include <omp.h>

int main(int argc, char *argv[])
{
    long thread_count = 1;

    int x = 10, y = 20;

    cout << "\nActual values...\n";
    printf("x = %d\n", x);
    printf("y = %d\n", y);

#pragma omp parallel private(x) firstprivate(y) lastprivate(i) num_threads(3)
    {
        printf("thread %d: x = %d\n", omp_get_thread_num(), x);
        printf("thread %d: y = %d\n", omp_get_thread_num(), y);
    }

    int i, a[i], b[i] = {1, 2, 3, 4};
    cout << i;

// https://stackoverflow.com/questions/15304760/how-are-firstprivate-and-lastprivate-different-than-private-clauses-in-openmp
#pragma omp parallel
    {
#pragma omp for lastprivate(i)
        for (i = 0; i < 4 - 1; i++)
            a[i] = b[i] + b[i + 1];
    }
    a[i] = b[i];

    return 0;
}
