#include <bits/stdc++.h>
using namespace std;
#include <omp.h>

#define print_array(a, n)             \
    cout << "Array '" << #a << "': "; \
    for (int i = 0; i < n; i++)       \
        cout << a[i] << " ";          \
    cout << endl;

int main(int argc, char *argv[])
{
    long thread_count = 1;

    int n = 51;
    int a[n];

    // generate an array of random numbers...
    srand(0);
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 1000;
    }
    print_array(a, n);

    int temp;

    for (int pass = 0; pass < n; pass++)
    {
        if (pass % 2)
        {
#pragma omp parallel for num_threads(thread_count) default(none) private(temp) shared(a, n)
            for (int i = 1; i < n; i += 2)
            {
                if (a[i - 1] > a[i])
                {
                    temp = a[i];
                    a[i] = a[i - 1];
                    a[i - 1] = temp;
                }
            }
        }
        else
        {
#pragma omp parallel for num_threads(thread_count) default(none) private(temp) shared(a, n)
            for (int i = 1; i < n - 1; i += 2)
            {
                if (a[i] > a[i + 1])
                {
                    temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
            }
        }
    }

    print_array(a, n);

    return 0;
}
