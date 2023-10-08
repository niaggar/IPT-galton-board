set terminal pngcairo color font"Times, 20" size 900,600
set output "Histogram.png"
set grid
set ylabel "Position X"
set xlabel "# of particles"

set key top left
plot "histogram.dat" using 1:2 w p pt 5 lc 2 t "Histogram"
unset terminal