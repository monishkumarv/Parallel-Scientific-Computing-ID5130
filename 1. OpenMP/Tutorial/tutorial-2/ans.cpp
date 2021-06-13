#include <bits/stdc++.h>
using namespace std;
#include <omp.h>

#define PI 3.14159265358

double f(double x)
{
  return (sin(x)/(2*x*x*x));
}

int main(int argc, char* argv[])
{
    long thread_count = 1;

    if(argc == 2){
        thread_count = strtol(argv[1], NULL, 10);
    }
    else{
        cout<<"\n A command line arguement other than the name of the executable is required...exiting program..";
        return 1;
    }


    int n = 32;
    double a = 1, b = PI;
    double h = (b-a)/n;
    double my_area, net_area = 0;

    #pragma omp parallel for num_threads(thread_count) reduction(+ : net_area)
        for (int i = 0; i < n; i++)
        {
            my_area = 0.5 * h * (f(a + (i+1)*h) + f(a + i*h));
            
            // TODO:.... why are we getting different answer if have the 'cout<<' statement
            cout<<endl<<omp_get_thread_num();  

            net_area += my_area;
        }
        
    cout<<endl<<"Area = "<<net_area;
    return 0;
}
