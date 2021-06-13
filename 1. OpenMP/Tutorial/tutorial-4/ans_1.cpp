#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
#include <omp.h>

#define print_array(a, n)             \
    cout << "Array '" << #a << "': "; \
    for (int i = 0; i < n; i++)       \
        cout << a[i] << " ";          \
    cout << endl;

int main()
{

    int n = 100;
    float a[n][n], b[n][n], c[n][n];

    srand(0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            a[i][j] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1000));
            b[i][j] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1000));
        }
    }

    double t1, t2;
    t1 = omp_get_wtime();

    long thread_count = 8z;

    #pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            c[i][j] = a[i][j] + b[i][j];
        }
    }

    t2 = omp_get_wtime();

    // Time increases as thread count increases...
    cout << "Execution time: " << t2 - t1;

    return 0;
}
