set term postscript eps color solid enhanced "Helvetica" 20

##### Fitting: B #####
f0ipr(x) = a1 + a2 * x + a3 * x**2
fit [1:0] f0ipr(x) 'b.dat' using 1:2 via a1, a2, a3
f0ip(x) = (x>0) ? f0ipr(x) : (1/0)

f0ear(x) = a4 + a5 * x + a6 * x**2
fit [0:-1] f0ear(x) 'b.dat' using 1:2 via a4, a5, a6
f0ea(x) = (x<0) ? f0ear(x) : (1/0)

##### Fitting: B #####
f1ipr(x) = a7 + a8 * x + a9 * x**2
fit [1:0] f1ipr(x) 'b.dat' using 1:2 via a7, a8, a9
f1ip(x) = (x>0) ? f1ipr(x) : (1/0)

f1ear(x) = a10 + a11 * x + a12 * x**2
fit [0:-1] f1ear(x) 'b.dat' using 1:2 via a10, a11, a12
f1ea(x) = (x<0) ? f1ear(x) : (1/0)

##### Fitting: B #####
f2ipr(x) = a13 + a14 * x + a15 * x**2
fit [1:0] f2ipr(x) 'b.dat' using 1:2 via a13, a14, a15
f2ip(x) = (x>0) ? f2ipr(x) : (1/0)

f2ear(x) = a16 + a17 * x + a18 * x**2
fit [0:-1] f2ear(x) 'b.dat' using 1:2 via a16, a17, a18
f2ea(x) = (x<0) ? f2ear(x) : (1/0)

##### Fitting: B #####
f3ipr(x) = a19 + a20 * x + a21 * x**2
fit [1:0] f3ipr(x) 'b.dat' using 1:2 via a19, a20, a21
f3ip(x) = (x>0) ? f3ipr(x) : (1/0)

f3ear(x) = a22 + a23 * x + a24 * x**2
fit [0:-1] f3ear(x) 'b.dat' using 1:2 via a22, a23, a24
f3ea(x) = (x<0) ? f3ear(x) : (1/0)

##### Fitting: B #####
f4ipr(x) = a25 + a26 * x + a27 * x**2
fit [1:0] f4ipr(x) 'b.dat' using 1:2 via a25, a26, a27
f4ip(x) = (x>0) ? f4ipr(x) : (1/0)

f4ear(x) = a28 + a29 * x + a30 * x**2
fit [0:-1] f4ear(x) 'b.dat' using 1:2 via a28, a29, a30
f4ea(x) = (x<0) ? f4ear(x) : (1/0)

##### Fitting: B #####
f5ipr(x) = a31 + a32 * x + a33 * x**2
fit [1:0] f5ipr(x) 'b.dat' using 1:2 via a31, a32, a33
f5ip(x) = (x>0) ? f5ipr(x) : (1/0)

f5ear(x) = a34 + a35 * x + a36 * x**2
fit [0:-1] f5ear(x) 'b.dat' using 1:2 via a34, a35, a36
f5ea(x) = (x<0) ? f5ear(x) : (1/0)

##### Fitting: B #####
f6ipr(x) = a37 + a38 * x + a39 * x**2
fit [1:0] f6ipr(x) 'b.dat' using 1:2 via a37, a38, a39
f6ip(x) = (x>0) ? f6ipr(x) : (1/0)

f6ear(x) = a40 + a41 * x + a42 * x**2
fit [0:-1] f6ear(x) 'b.dat' using 1:2 via a40, a41, a42
f6ea(x) = (x<0) ? f6ear(x) : (1/0)

##### Fitting: B #####
f7ipr(x) = a43 + a44 * x + a45 * x**2
fit [1:0] f7ipr(x) 'b.dat' using 1:2 via a43, a44, a45
f7ip(x) = (x>0) ? f7ipr(x) : (1/0)

f7ear(x) = a46 + a47 * x + a48 * x**2
fit [0:-1] f7ear(x) 'b.dat' using 1:2 via a46, a47, a48
f7ea(x) = (x<0) ? f7ear(x) : (1/0)

##### Fitting: B #####
f8ipr(x) = a49 + a50 * x + a51 * x**2
fit [1:0] f8ipr(x) 'b.dat' using 1:2 via a49, a50, a51
f8ip(x) = (x>0) ? f8ipr(x) : (1/0)

f8ear(x) = a52 + a53 * x + a54 * x**2
fit [0:-1] f8ear(x) 'b.dat' using 1:2 via a52, a53, a54
f8ea(x) = (x<0) ? f8ear(x) : (1/0)

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
plot 'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a6,a3) w p lt rgb "#000000" lw 2 pt 4, f0ip(x) w l lt rgb "#000000" lw 2 notitle, f0ea(x) w l lt rgb "#000000" lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a12,a9) w p lt rgb "#0000FF" lw 2 pt 6, f1ip(x) w l lt rgb "#0000FF" lw 2 notitle, f1ea(x) w l lt rgb "#0000FF" lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a18,a15) w p lt rgb "#DC143C" lw 2 pt 8, f2ip(x) w l lt rgb "#DC143C" lw 2 notitle, f2ea(x) w l lt rgb "#DC143C" lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a24,a21) w p lt rgb "#00C957" lw 2 pt 10, f3ip(x) w l lt rgb "#00C957" lw 2 notitle, f3ea(x) w l lt rgb "#00C957" lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a30,a27) w p lt rgb "#800080" lw 2 pt 12, f4ip(x) w l lt rgb "#800080" lw 2 notitle, f4ea(x) w l lt rgb "#800080" lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a36,a33) w p lt rgb "#FF00FF" lw 2 pt 14, f5ip(x) w l lt rgb "#FF00FF" lw 2 notitle, f5ea(x) w l lt rgb "#FF00FF" lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a42,a39) w p lt rgb "#EE7600" lw 2 pt 1, f6ip(x) w l lt rgb "#EE7600" lw 2 notitle, f6ea(x) w l lt rgb "#EE7600" lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a48,a45) w p lt rgb "#000080" lw 2 pt 2, f7ip(x) w l lt rgb "#000080" lw 2 notitle, f7ea(x) w l lt rgb "#000080" lw 2 notitle, \
     'b.dat' u 1:2 t sprintf('%s (%4.2f,%5.2f)','B',a54,a51) w p lt rgb "#556B2F" lw 2 pt 3, f8ip(x) w l lt rgb "#556B2F" lw 2 notitle, f8ea(x) w l lt rgb "#556B2F" lw 2 notitle