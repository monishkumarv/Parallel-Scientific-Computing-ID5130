#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
#include <sys/time.h>

int main()
{
    long long int n = 25;

    float xl = 0, xu = 3, x;
    float f[n], f_dash[n], h = (xu - xl) / (n - 1);

    for (int i = 0; i < n; i++)
    {
        x = xl + (i * h);
        f[i] = sin(5 * x);
    }

    f_dash[0] = (f[1] - f[0]) / h;
    f_dash[1] = (f[2] - f[1]) / h;

    f_dash[n - 2] = (f[n - 2] - f[n - 3]) / h;
    f_dash[n - 1] = (f[n - 1] - f[n - 2]) / h;

    for (int i = 2; i < n - 2; i++)
    {
        f_dash[i] = (-f[i + 2] + 8 * f[i + 1] - 8 * f[i - 1] + f[i - 2]) / (12 * h);
    }

    // Display output:
    std::ofstream myfile;
    myfile.open("q1_a_serial_code.csv");
    myfile << "x,Numerical solution,Analytical solution\n";

    for (int i = 0; i < n; i++)
    {
        x = xl + (i * h);
        cout << "Numerical solution: " << f_dash[i] << " Analytical solution: " << 5 * cos(5 * x) << endl;
        myfile << x << "," << f_dash[i] << "," << 5 * cos(5 * x) << endl;
    }
    myfile.close();

    return 0;
}
