set term pdfcairo enhanced dashed font 'Helvetica,16' size 15cm,8cm dashed
set output "prec.pdf"

set multiplot layout 2,1

set key left

set xrange [1e-3:1e3]
set log x

set ytics 2
unset xtics
set lmargin 6
set bmargin 0
set yrange [-4.5:4.5]
plot 'prec.csv' u ($1):(($3-$2)/$2*100) w l notitle lc rgb "red"

set ytics .05
set xtics
set lmargin 6
set tmargin 0
set bmargin 3
set yrange [-0.2:0.03]
plot 'prec.csv' u ($1):(($4-$2)/$2*100) w l notitle lc rgb "red"
