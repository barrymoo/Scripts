set term postscript eps color solid enhanced "Helvetica" 20

##### Fitting: A #####
f0ipr(x) = a1 + a2 * x + a3 * x**2
fit [1:0] f0ipr(x) 'a.dat' using 1:2 via a1, a2, a3
f0ip(x) = (x>0) ? f0ipr(x) : (1/0)

f0ear(x) = a4 + a5 * x + a6 * x**2
fit [0:-1] f0ear(x) 'a.dat' using 1:2 via a4, a5, a6
f0ea(x) = (x<0) ? f0ear(x) : (1/0)

##### Fitting: B #####
f1ipr(x) = a7 + a8 * x + a9 * x**2
fit [1:0] f1ipr(x) 'b.dat' using 1:2 via a7, a8, a9
f1ip(x) = (x>0) ? f1ipr(x) : (1/0)

f1ear(x) = a10 + a11 * x + a12 * x**2
fit [0:-1] f1ear(x) 'b.dat' using 1:2 via a10, a11, a12
f1ea(x) = (x<0) ? f1ear(x) : (1/0)

set out 'test.eps'
set key top right
set size ratio 1.0
set pointsize 1.25
set xzeroaxis
set xrange [-1:1] noreverse
#set yrange [-0.5:9.5]
set xtics 0.2
set ytics 1
set xlabel "{/Symbol D}N"
set ylabel "[E({/Symbol D}N)-E(0)] / eV"
plot 'a.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','A',a3,a6) w p lt 0 lw 2 pt 1, f0ip(x) w l lt 0 lw 2 notitle, f0ea(x) w l lt 0 lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a9,a12) w p lt 1 lw 2 pt 2, f1ip(x) w l lt 1 lw 2 notitle, f1ea(x) w l lt 1 lw 2 notitle