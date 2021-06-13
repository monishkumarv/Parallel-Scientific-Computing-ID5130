#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	
    int a = 5;
    int c;

    #pragma acc data copyout(c)
    {
        c = 2 * a;
    }
	
    printf("\nc = %d \n\n", c);

}
