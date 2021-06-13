set term postscript enhanced font "Times, 20"
set output "overhead_speedup.eps"
set size 1,1
set key top left

a = 100
b = 1000
c = 0.25

n = 5000

#maximum speedup proposed by Amdahl

sigma1 = a*n + b
phi1 = c*n*n
f1 = sigma1/(sigma1 + phi1)
psi1(p) = 1.0/(f1 + (1-f1)/p)

# actual speed up obtained by including the overheads. 
kappa(p) = log(n)*(10000*log(p)+(n/10))
sigma2 = a*n + b
phi2 = c*n*n
psi2(p) = (sigma2+phi2)/(sigma2 + phi2/p + kappa(p))

set xlabel 'No. of processors (p)'
set ylabel 'Speedup, Psi(n, p)'

plot [p=1:20] psi1(p) w p pt 2  title 'No Overheads',\
              psi2(p) w l lt 1  title 'With Overheads'

