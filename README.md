# Arhitectura sistemelor paralele

## Laborator 1

Rezolvarea unei functii si plotarea ei in C.

Am folosit gnuplot pentru a genera graficul, creand o librarie `gplot` care sa usureze generarea de grafice.
Apoi am creat un Makefile pentru a usura compilarea si rularea programului impreuna cu libraria.

Pentru a rula programul, aveti nevoie de **gnuplot** instalat pe sistem.

Rulati `make all` pentru a compila programul, apoi `./vanderpol output_file.png` pentru a rula programul si a genera graficul.
Dupa ce ati rulat programul, puteti rula `make clean` pentru a sterge fisierele generate de compilare, sau `make cleanall` pentru a sterge si executabilul creat.