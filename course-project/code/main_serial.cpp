#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
#include "utils.h"

// double getPrediction(double **x, int index, double w[], int n)
// {
//     double pred = 0.0;

//     for (int j = 0; j < n; j++)
//     {
//         pred += w[j] * x[index][j];
//     }

//     return pred;
// }

// double getLoss(double **x, double y[], int m, int n, double w[])
// {
//     double loss = 0.0;

//     for (int i = 0; i < m; i++)
//     {
//         double y_hat = getPrediction(x, i, w, n);
//         loss += pow((y_hat - y[i]), 2);
//     }

//     return (loss / (2 * m));
// }
// 
// // Generate a random floating point number b/w min & max
// double genRandom(double min, double max)
// {
//     double range = (max - min);
//     double div = RAND_MAX / range;
//     return min + (rand() / div);
// }

// // Generate Toy Data:   x[i] = i, y[i] = 2x[i] - 1 + err and err ~ N(0,0.5)
// void genToyData(double **x, double y[], int m, int n, double err_mean, double err_std)
// {
//     // Get the below values from user:
//     double w[n] = {1.0, -2.0, 5};

//     random_device rd;
//     mt19937 gen(rd());
//     normal_distribution<double> dist(err_mean, err_std);

//     // Opening a csv file:
//     std::ofstream myfile;
//     myfile.open("train_data.csv");
//     myfile << "x1,y" << endl;

//     for (int i = 0; i < m; i++)
//     {
//         for (int j = 0; j < n; j++)
//         {
//             // Intercept:
//             if (j == 0)
//                 x[i][0] = 1.0;
//             // Random x values:
//             else
//                 x[i][j] = genRandom(-1.0, 1.0);
//         }

//         // Adding some error/uncertainity to y values:
//         y[i] = getPrediction(x, i, w, n) + dist(gen);

//         // Save data to excel:
//         myfile << x[i][1] << "," << y[i] << endl;
//     }
// }

void SGD(double **x, double y[], int m, int n, double alpha = 0.01, double tolerance = 10e-4)
{
    double w[n] = {0}, gradient[n];

    // Opening a csv file:
    std::ofstream myfile;
    myfile.open("loss.csv");
    myfile << "epoch,loss" << endl;

    int epoch = 0;
    while (epoch <= 1000)
    {
        // Draw random sample with replacement...
        int random_index = rand() % m; // this will be m/p in parallel

        double y_hat = getPrediction(x, random_index, w, n);
        double loss = getLoss(x, y, m, n, w);

        cout << "\nEpoch: " << epoch;
        cout << "\nLoss = " << loss;
        cout << "\nTheta:";
        display_1D_Array(w, n);
        myfile << epoch << "," << loss << endl;

        for (int j = 0; j < n; j++)
        {
            gradient[j] = (y_hat - y[random_index]) * x[random_index][j];
            w[j] = w[j] - alpha * gradient[j];
        }

        // Decreasing learning rate:
        // alpha = alpha / 1.1;

        epoch++;
    }

    cout << endl;
    for (int i = 0; i < m; i++)
    {
        // cout << y[i] << "," << getPrediction(x, i, w, n) << endl;
    }
}

int main(int argc, char *argv[])
{

    // TODO:
    // n cant be greater than 2 for now (theta0, theta1)........................///    user-input   ///
    // Get the above values from user

    // m = no. of training data, n = no. of features + 1
    int m = 10e3, n = 3;
    double **X = Create_2D_Array(m, n, 0);
    double Y[m];
    double err_mean = 0.0, err_std = 1.0;

    double weights[n] = {1.0, -2.0, 5};
    genToyData(X, Y, weights, m, n, err_mean, err_std);

    // // Display X:
    // display_2D_Array(X, m, n);

    // // Display Y:
    // display_1D_Array(Y, m);

    SGD(X, Y, m, n);

    return 0;
}