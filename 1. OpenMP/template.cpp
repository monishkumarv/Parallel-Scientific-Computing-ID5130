#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
#include <omp.h>
#include <sys/time.h>

#define print_array(a, n)             \
    cout << "Array '" << #a << "': "; \
    for (int i = 0; i < n; i++)       \
        cout << a[i] << " ";          \
    cout << endl;

double clock_time()
{
    struct timeval Tp;
    int stat;
    stat = gettimeofday(&Tp, NULL);
    if (stat != 0)
        printf("Error return from gettimeofday: %d", stat);
    return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

void kernel()
{
    long thread_count = 1;

    float a[3][3];

    //
    // start code here...
    //
}

int main()
{
    double start = 0.0, end = 0.0;
    start = clock_time();
    kernel();
    end = clock_time();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}
