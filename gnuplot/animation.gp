set terminal pngcairo size 720,720
#set terminal png enhanced size 720,720

# Format of the axis
if(exist("n") == 0) n = 0
set xrange [-5.0:5.0]
set yrange [-5.0:5.0]
set xlabel "x" font ", 24"
set ylabel "y" font ", 24"
set xtics 2.5 font ", 18"
set ytics 2.5 font ", 18"
set format x "%.1f"
set format y "%.1f"
set size square

inputFile  = sprintf("../result/result%05d.dat", n)
outputFile = sprintf("image/result%05d.png", n)

print outputFile
set output outputFile

al = 0.2

#set cbrange [-1.0:1.0]
#set cblabel "theta/pi" font ", 24"
#set cbtics 0.5 font ", 18"
#set palette defined (-1.0 "green", -0.5 "blue", 0.0 "red", 0.5 "yellow", 1.0 "green")

plot inputFile using 1:2:(0.1) with circles fill solid lc rgb "#999999" notitle,\
     inputFile using 1:2:($3*al):($4*al) with vectors lw 2.0 lc rgb "black" notitle

#plot inputFile using 1:2:(0.1):(atan2($4, $3)/pi) with circles fill solid lc palette notitle,\

clear

n=n+1
if(n <= 500) reread
