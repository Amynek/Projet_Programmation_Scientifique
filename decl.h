// KADDOURI AMYNE 22103988
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structure d'une matrice en simple precision
typedef struct
{
  int nrow;
  int ncol;
  float* mat_alloc;
  float** mat;
} Matrix;
// Structure d'un vecteur en simple precision
typedef struct
{
  int size;
  float* vect;
} Vector;

// Resolution de systeme lineaire (Bibliotheque LAPACK):
void sgesv_(int *, int *, float *, int *, int *, float *, int *, int *); 

// Creation d'une matrice remplie de 0 selon les caracteristiques de la structure Matrix
void mat_create(Matrix *, int, int);
// Creation d'un vecteur selon les caracteristiques de la structure Vector
void vect_create(Vector *, int);

// Liberation de l'espace pris par une matrice
void mat_free(Matrix);
// Liberation de l'espace pris par un vecteur 
void vect_free(Vector);

// Initialisation de la matrice A :
void mat_init_A(Matrix *);
// Initialisation du vecteur b :
void vect_init_b(Vector *,float);

// Produit d'une matrice n x n tridiagonale et d'un vecteur de taille n
// prod_mat_vect_Tri(Matrix Mat, Vector Vect) => Mat x Vect = Vect_out
// Renvoie Vect_out
Vector prod_mat_vect_Tri(Matrix, Vector);

// Difference entre 2 vecteurs de taille n 
// Vect3 = diff_vect(Vector Vect1, Vector Vect2) => Vect1 - Vect2 = Vect3
// Renvoie Vect3
Vector diff_vect(Vector, Vector);

// Calcul de la norme 1 d'un vecteur ( norme1 = ||Vector||_{1} )
// Si l'une de ses valeurs est negative : deuxieme argument d'entree = 0
// Renvoie norme1
float norme_1(Vector, int);
