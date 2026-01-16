# ---- CONFIGURATION DE BASE ----
reset
set encoding utf8
set datafile separator " "

# Titres
set title "Courbe des erreurs d'approximation en fonction de n"
set xlabel "n"
set ylabel "Erreur"

set grid lw 1 # Grille

# Creation du fichier image : courbe.png
set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output "courbe.png"

# ---- TRACÉ ----
plot "output.txt" using 1:2 with lines lw 2 lc rgb "blue" title "||A_{n}x-b||_{1}/||b||_{1}"

# Fin de l'ecriture
set output
