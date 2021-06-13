// #include <iostream>
// #include <bits/stdc++.h>
// using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TYPE		float
#define N		1000
#define SMALLVALUE	0.001

void init_A(TYPE A[][N], int num_gangs) {

	#pragma acc parallel loop present(A[0:N][0:N]) num_gangs(num_gangs)
	for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(i==j)
            {
                if(i == 0 || i == N-1)
                    A[i][j] = 1;
                else
                    A[i][j] = 4;
            }

            else if (abs(i-j) == 1)
            {
                if(i == 0 || i == N-1)
                    A[i][j] = 2;
                else
                    A[i][j] = 1;
            }
            else
            {
                A[i][j] = 0;
            }
        }
        
    }
}

void init_b(TYPE b[], TYPE f[], int num_gangs) {

    float xl = 0, xu = 3;
    float h = (xu - xl) / (N - 1);

    b[0] = (1 / h) * (-2.5 * f[0] + 2 * f[1] + 0.5 * f[2]);
    b[N - 1] = (1 / h) * (2.5 * f[N - 1] - 2 * f[N - 2] - 0.5 * f[N - 3]);

	#pragma acc parallel loop present(b[0:N], f[0:N]) num_gangs(num_gangs)
    for (int i = 1; i < N - 1; i++)
    {
        b[i] = (3 / h) * (f[i + 1] - f[i - 1]);
    }
}

void printMatrix(TYPE a[][N]) {
	for (int ii = 0; ii < N; ++ii) {
		printf("[");
		for (int jj = 0; jj < N; ++jj)
			printf("%.2f ", a[ii][jj]);
		printf("]; \n");
	}
}

void printVector(TYPE a[]) {
	printf("\n[");
	for (int ii = 0; ii < N; ++ii)
		printf("%.2f ", a[ii]);
	printf("] \n");
}


void LU_Decompose(TYPE a[][N], TYPE b[], TYPE x[], int num_gangs) {

    // LU decomposition phase:
    #pragma acc parallel loop present(a[0:N][0:N], b[0:N], x[0:N]) num_gangs(num_gangs)
    for (int k = 0; k < N; k++)
    {
        for (int j = k+1; j < N; j++)
        {
            a[j][k] /= a[k][k];
            for (int i = k+1; i < N; i++)
            {
                a[i][j] -= a[i][k] * a[k][j];
            }
            
        }        
    }

    // Foward substitution phase:
    #pragma acc parallel loop present(a[0:N][0:N], b[0:N]) num_gangs(num_gangs)
    for (int i = 0; i < N; i++)
    {
        TYPE w = b[i];
        #pragma acc loop reduction(-:w)
        for (int j = 0; j < i; j++)
            w -= a[i][j] * b[j];
        b[i] = w;
    }

    // Backwoard substitution phase:
    #pragma acc parallel loop present(a[0:N][0:N], b[0:N], x[0:N]) num_gangs(num_gangs)
    for (int i = N - 1; i >= 0; i--)
    {
        TYPE w = b[i];
        #pragma acc loop reduction(-:w)
        for (int j = i + 1; j < N; j++)
            w -= a[i][j] * x[j];
        x[i] = w / a[i][i];
    }

    
}

int main(int argc, char* argv[]) {

    // Get num_gangs from use:
    int num_gangs = 10;
    if(argc == 2){
        num_gangs = strtol(argv[1], NULL, 10);
    }
    else{
        printf("\nNumber of gangs not specified...\n");
        return 1;
    }

    // Initialise f(x)
    float f[N], xl = 0, xu = 3;
    float h = (xu - xl) / (N - 1);
	for (int i = 0; i < N; ++i)
    { 
        f[i] = sin(5 * (xl + (i * h)));
    }

    // LU Decomposition:
	TYPE a[N][N], b[N], x[N];

    struct timespec ts1, ts2;
	clock_gettime(CLOCK_REALTIME, &ts1);
	
	#pragma acc data create(a,b,x) copyin(f) copyout(x)
	{
		init_A(a,num_gangs);
        init_b(b,f,num_gangs);
        LU_Decompose(a,b,x,num_gangs);
	}
    
	printf("N = %d\n",N);

    // Display output: (run as '.cpp' file)
    // std::ofstream myfile;
    // myfile.open("lu-decomposition.csv");
    // myfile << "x,Numerical solution,Analytical solution\n";

    // for (int i = 0; i < N; i++)
    // {
    //     float xl = 0, xu = 3;
    //     float h = (xu - xl) / (N - 1);
    //     float xx = xl + (i * h);
    //     printf("Numerical solution: %f Analytical solution: %f\n", x[i], (5 * cos(5 * xx)));
    //     // myfile << xx << "," << x[i] << "," << 5 * cos(5 * xx) << endl;
    // }
    // myfile.close();

    // Display time:
	clock_gettime(CLOCK_REALTIME, &ts2);
    if (ts2.tv_nsec < ts1.tv_nsec)
    {
        ts2.tv_nsec += 1000000000;
        ts2.tv_sec--;
    }
    printf("\nTime taken : %ld.%09ld\n", (long)(ts2.tv_sec - ts1.tv_sec), ts2.tv_nsec - ts1.tv_nsec);

	return 0;
}
