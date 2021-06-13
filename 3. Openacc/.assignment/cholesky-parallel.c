#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TYPE		float
#define N		10
#define SMALLVALUE	0.001

void initmult(TYPE mat[][N], int iimult, int jjmult) {

	#pragma acc parallel loop present(mat[0:N][0:N])
	for (int ii = 0; ii < N; ++ii)
		for (int jj = 0; jj < N && jj < ii; ++jj)
			mat[ii][jj] = mat[jj][ii] = (ii * iimult + jj * jjmult) / (float)N / N;
	
}

void zero(TYPE mat[][N]) {
	initmult(mat, 0, 0);
}

void init(TYPE mat[][N]) {
	initmult(mat, 1, 1);
}

void printMat(TYPE a[][N]) {
	for (int ii = 0; ii < N; ++ii) {
		printf("[");
		for (int jj = 0; jj < N; ++jj)
			printf("%.2f ", a[ii][jj]);
		printf("]; \n");
	}
}


void cholesky(TYPE a[][N]) {

	#pragma acc parallel loop present(a[0:N][0:N])
	for (int ii = 0; ii < N; ++ii) {

		#pragma acc loop									// i > j elements
		for (int jj = 0; jj < ii; ++jj) {
			for (int kk = 0; kk < jj; ++kk) {
				a[ii][jj] -= a[ii][kk] * a[jj][kk];					
			}
			a[ii][jj] /= (a[jj][jj] > SMALLVALUE ? a[jj][jj] : 1);
		}

		#pragma acc loop									// i == j elements
		for (int kk = 0; kk < ii; ++kk)
			a[ii][ii] -= a[ii][kk] * a[ii][kk];
		
		if (a[ii][ii] > 0) a[ii][ii] = sqrt(a[ii][ii]);
	}
}

int main() {

    struct timespec ts1, ts2;
	TYPE a[N][N];

	clock_gettime(CLOCK_REALTIME, &ts1);
	
	#pragma acc data create(a) copyout(a)
	{
		init(a);
		cholesky(a);
	}
    
	printf("N = %d\n",N);
	printMat(a);

	clock_gettime(CLOCK_REALTIME, &ts2);
    if (ts2.tv_nsec < ts1.tv_nsec)
    {
        ts2.tv_nsec += 1000000000;
        ts2.tv_sec--;
    }

    printf("\nTime taken : %ld.%09ld\n", (long)(ts2.tv_sec - ts1.tv_sec), ts2.tv_nsec - ts1.tv_nsec);

	return 0;
}
