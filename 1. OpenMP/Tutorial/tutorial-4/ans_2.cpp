#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
#include <omp.h>

#define print_array(a, n)             \
    cout << "Array '" << #a << "': "; \
    for (int i = 0; i < n; i++)       \
        cout << a[i] << " ";          \
    cout << endl;

float u(float x)
{
    return (7 - x * tan(x));
}

float u_dash(float x)
{
    return ((-1 * x) / (cos(x) * cos(x)) - tan(x));
}

int main()
{
    double t1, t2;
    long thread_count = 1;

    float a = -1, b = 1, h = 0.001;
    int n = (b - a) / h;

    float ux[n + 1], dudx[n + 1];

    for (int i = 0; i <= n; i++)
    {
        float x = a + i * h;
        ux[i] = u(x);
    }

    t1 = omp_get_wtime();

#pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i <= n; i++)
    {
        if (i == 0)
        {
            dudx[i] = (ux[i + 1] - ux[i]) / h;
        }
        else if (i == n)
        {
            dudx[i] = (ux[i] - ux[i - 1]) / h;
        }
        else
        {
            dudx[i] = (ux[i + 1] - ux[i - 1]) / (2 * h);
        }
    }

    float error = 0;
    for (int i = 0; i <= n; i++)
    {
        float x = a + i * h;
        error += abs(dudx[i] - u_dash(x));
        // cout << dudx[i] << " " << u_dash(x) << endl;
    }

    cout << "\nCalculation error: " << error;

    t2 = omp_get_wtime();
    cout << "\nTime of execution: " << t2 - t1;

    return 0;
}
