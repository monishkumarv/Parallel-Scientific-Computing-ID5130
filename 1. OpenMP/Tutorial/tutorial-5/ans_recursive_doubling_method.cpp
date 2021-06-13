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
    double t1, t2;
    long thread_count = 1;

    int n = 4; ////////////////////////////////////////////////////////////////////////////////////////
    float a[n] = {-1, -1, -1, -1}, b[n] = {2, 2, 2, 2}, c[n] = {-1, -1, -1, -1};
    float y[n] = {1, 2, 3, 4};

    a[1] = 0;
    c[n] = 0;

    t1 = omp_get_wtime();

    for (int k = 1; k <= ceil(log2(n)); k++)
    {
        for (int i = 1; i < n; i++)
        {

            int q = i - pow(2, k - 1);
            int w = i + pow(2, k - 1);

            float alpha = -1 * (a[i] / b[q]);
            float beta = -1 * (a[i] / b[w]);

            a[i] = c[i] = y[i] = 0;
            b[i] = 1;

            if (i >= (pow(2, k) + 1))
            {
                a[i] = alpha * a[q];
            }

            if (i <= (n - pow(2, k)))
            {
                c[i] = alpha * c[w];
            }

            b[i] = alpha * c[q] + b[i] + beta * a[w];

            y[i] = alpha * y[q] + y[i] + beta * y[w];
        }

        print_array(a, n);
    }

    for (int i = 0; i < n; i++)
    {
        cout << "x[" << i << "] = " << (y[i] / b[i]) << endl;
    }

    t2 = omp_get_wtime();

    cout << "\nTime of execution: " << t2 - t1;

    return 0;
}
