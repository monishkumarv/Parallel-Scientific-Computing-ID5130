#include <bits/stdc++.h>
using namespace std;

#ifdef _OPENMP
    #include <omp.h>
#endif

void Hello(){

    #ifdef _OPENMP
        int thread_rank = omp_get_thread_rank();
        int total_threads = omp_get_num_threads();
    #else
        int thread_rank = 0;
        int total_threads = 1;
    #endif
    

    // Instead of the below cout line, create a whole string using concatenation..and then print...
    cout<<"\n    Hello from thread '" << thread_rank <<"' out of totally " << total_threads << " threads";
}

int main(int argc, char* argv[])                         // argv and argc are how command line arguments are passed to main()
{                                   // The variables are named argc (argument count) and argv (argument vector) by convention
    long thread_count = 1;

    if(argc == 2){
        thread_count = strtol(argv[1], NULL, 10);
        // cout<<argv[0];                                // ...1st arguement is always the name of the executable
    }
    else{
        cout<<"\n A command line arguement other than the name of the executable is required...exiting program..";
        return 1;
    }

    #ifdef _OPENMP
        cout<<"\nParallel Code: (coz you used -fopenmp while compiling)";
    #else
        cout<<"\nSerial Code: (coz you didn't use -fopenmp while compiling)";
    #endif

    #pragma omp parallel num_threads(thread_count)
    Hello();
    
    cout<<endl;
    return 0;
}
