set term pdfcairo enhanced dashed font 'Helvetica,18' size 12cm,10cm dashed
set output "log.pdf"

set key left

set xrange [0:1]
set yrange [0:1.1]
set xlabel "M/L"

sigma = 0.0430357
plot log(1+x)/log(2) ti "log_2(1+M/L)" lc rgb "red" lw 2, x ti "M/L" lc rgb "dark-green", x+sigma ti "M/L + {/Symbol s}" lc rgb "blue"
