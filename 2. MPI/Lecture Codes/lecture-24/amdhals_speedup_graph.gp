set term postscript enhanced font "Times, 20"
set output "amdahl_speedup.eps"
set size 1,1
set key top left

a = 100
b = 1000
c = 0.25

n = 5000
sigma1 = a*n + b
phi1 = c*n*n
f1 = sigma1/(sigma1 + phi1)
psi1(p) = 1.0/(f1 + (1-f1)/p)

n = 10000
sigma2 = a*n + b
phi2 = c*n*n
f2 = sigma2/(sigma2 + phi2)
psi2(p) = 1.0/(f2 + (1-f2)/p)

set xlabel 'No. of processors (p)'
set ylabel 'Maximum Speedup, Psi(n, p)'

plot [p=1:20] psi1(p) w p pt 2 title 'n = 5000',\
     psi2(p) w l lt 1 title 'n = 10000'

