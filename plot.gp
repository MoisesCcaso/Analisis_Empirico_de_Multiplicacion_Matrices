set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output "grafico.png"

set title "Standard vs Strassen"
set xlabel "Tamaño de matriz (n)"
set ylabel "Tiempo de ejecución (microsegundos)"

# ===== ESCALAS =====
set autoscale fix
set logscale y

# ===== GRILLA =====
set grid xtics ytics back lc rgb "#b0b0b0" lw 1 dt 2
set grid mxtics mytics lc rgb "#e0e0e0" lw 0.5 dt 3

# ===== MÁRGENES (REEMPLAZO DE OFFSETS) =====
set bmargin 4
set tmargin 2
set lmargin 8
set rmargin 2

# ===== LEYENDA =====
set key top left box opaque

# ===== ESTILO =====
set style line 1 lc rgb "#1f77b4" lw 2 pt 7 ps 1.2
set style line 2 lc rgb "#d62728" lw 2 pt 5 ps 1.2

# ===== TICS =====
set xtics nomirror
set ytics nomirror
set mxtics 2
set mytics 2

# ===== PLOT =====
plot "resultados.dat" using 1:2 with linespoints ls 1 title "Standard Multiplication", \
     "resultados.dat" using 1:3 with linespoints ls 2 title "Strassen Multiplication"

set output
