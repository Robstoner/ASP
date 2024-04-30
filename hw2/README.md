# Tema 2 ASP

## Edge-detection algorithm pe o imagine pgm paralelizat folosind MPI

### Secvential (hw2.c)

Am implementat un algoritm de edge-detection primit in cerinta pe o imagine pgm.
Algoritmul este secvential si citeste imaginea din fisierul de intrare, aplica filtrul de edge-detection si scrie imaginea rezultata in fisierul de iesire.

Se poate rula cu comanda:
```bash
make hw2
./hw2 input_file niter
```

### Paralelizare (hw2_mpi.c)

Am paralelizat algoritmul folosind MPI. Am impartit imaginea in coloane si am trimis cate o bucata din imagine fiecarui proces. Fiecare proces aplica filtrul de edge-detection pe bucata primita si trimite rezultatul inapoi procesului 0 care il scrie in fisierul de iesire.

Ele comunica intre ele, trimitandu-si coloanele de pixeli vecine (halo) pentru a putea aplica filtrul corect.

Se poate rula cu comanda:
```bash
make all
mpirun --oversusbcribe -np P ./hw2_mpi input_file niter
```

P trebuie sa fie un numar care sa se imparta exact la numarul de pixeli de pe coloane al imaginii. Pe imaginea de test, am folosit 4 procese.

