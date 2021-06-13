#!/bin/bash
#PBS -o logfile.log
#PBS -e errorfile.err
#PBS -l walltime=02:00:00
#PBS -l select=1:ncpus=1
tpdir=`echo $PBS_JOBID | cut -f 1 -d .`
tempdir=$HOME/scratch/job$tpdir

mkdir -p $tempdir
cd $tempdir
cp -R $PBS_O_WORKDIR/* .

mpicxx q2_a_jacobi_iterative_method_parallel.cpp
mpiexec -n 1 ./a.out >output.txt
mpiexec -n 2 ./a.out >>output.txt
mpiexec -n 4 ./a.out >>output.txt
mpiexec -n 8 ./a.out >>output.txt
mpiexec -n 16 ./a.out >>output.txt

rm a.out
mv ../job$tpdir $PBS_O_WORKDIR/.