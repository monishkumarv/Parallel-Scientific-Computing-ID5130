#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

#define TYPE double

#define M 1000 // no. of training data
#define N 10   // no. of explanatory variables (regressors) + 1

// Model parameters (weights):
TYPE WEIGHTS[N] = {1.0, -2.0, 8.0, 4.0, -9.0, 5.0, 7.0, 2.0, -3.0, 6.0};

// Mean and Std of noise:
#define NOISE_MEAN 0.0
#define NOISE_STD 1.0

// Model hyperparameters:
#define ALPHA 0.001
#define EPOCHS 5000
