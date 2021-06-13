#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
#include "sgd_params.h"

void init_2D_Array(double array[][N], int rows, double init_value)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < N; j++)
        {
            array[i][j] = init_value;
        }
    }
}

void display_1D_Array(double array[], int rows)
{
    cout << endl;
    for (size_t i = 0; i < rows; ++i)
    {
        cout << i << ": " << array[i] << endl;
    }
}

void display_2D_Array(double array[][N], int rows, int cols)
{
    cout << endl;
    for (size_t i = 0; i < rows; ++i)
    {
        cout << i << ": ";
        for (size_t j = 0; j < cols; ++j)
        {
            cout << array[i][j] << ", ";
        }
        cout << endl;
    }
}

// Generate a random floating point number b/w min & max...
double genRandom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

// Calculate prediction...
double getPrediction(double x[][N], int index, double w[], int n)
{
    double pred = 0.0;

    for (int j = 0; j < n; j++)
    {
        pred += w[j] * x[index][j];
    }

    return pred;
}

// Calculate loss function...
double getLoss(double x[][N], double y[], int m, int n, double w[])
{
    double loss = 0.0;

    for (int i = 0; i < m; i++)
    {
        double y_hat = getPrediction(x, i, w, n);
        loss += pow((y_hat - y[i]), 2);
    }

    return (loss / (2 * m));
}

// Generate dummy data...
void genToyData(double x[][N], double y[], double w[], int m, int n, double err_mean, double err_std)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> dist(err_mean, err_std);

    // Opening a csv file:
    std::ofstream myfile;
    myfile.open("train_data.csv");
    myfile << "x,y" << endl;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // Intercept:
            if (j == 0)
                x[i][0] = 1.0;
            // Random x values:
            else
                x[i][j] = genRandom(-10.0, 10.0);
            // Save data to csv:
            myfile << x[i][j] << ",";
        }

        // Adding some error/uncertainity to y values:
        y[i] = getPrediction(x, i, w, n) + dist(gen);

        // Save data to csv:
        myfile << y[i] << endl;
    }
}
