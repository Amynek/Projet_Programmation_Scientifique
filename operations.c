// KADDOURI AMYNE 22103988

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct
{
  int nrow;
  int ncol;
  float* mat_alloc;
  float** mat;
} Matrix;
typedef struct
{
  int size;
  float* vect;
} Vector;

// Creation d'une matrice en simple precision
void mat_create(Matrix* Mat,int nrow,int ncol){
	// Dimensions
	Mat -> nrow = nrow;
	Mat -> ncol = ncol;
	// Allocation dynamique
	Mat -> mat_alloc = (float *) calloc(nrow*ncol,sizeof(float));
	Mat -> mat = (float **) malloc(nrow*sizeof(float *));

	for(int i=0;i<nrow;i++) Mat -> mat[i] = &Mat->mat_alloc[i*ncol];
}

// Creation d'un vecteur en simple precision
void vect_create(Vector* Vect,int size){
	Vect -> size = size; // Dimension
	Vect -> vect = (float *) malloc(size*sizeof(float)); // Allocation dynamique
}

// Liberation de la memoire prise par la matrice Mat
void mat_free(Matrix Mat){
	free (Mat.mat_alloc);
	free (Mat.mat);	
}

// Liberation de la memoire prise par le vecteur Vect
void vect_free(Vector Vect){
	free (Vect.vect);	
}

// Initialisation de la matrice A
void mat_init_A(Matrix* Mat){
	for(int i=0;i<Mat->nrow;i++){
	
		Mat -> mat[i][i] = 2;// Diagonale
	}
	for(int i=1;i<Mat->nrow-1;i++){
		Mat -> mat[i][i+1] = -1;// Diagonale superieure
		Mat -> mat[i][i-1] = -1;// Diagonale inferieure
	}
	// Bords de la matrice
	Mat -> mat[0][1] = -1;
	Mat -> mat[Mat->nrow-1][Mat->nrow-2] = -1;
}
// Initialisation du vecteur b
void vect_init_b(Vector* Vect,float h){
	for(int i=0;i<Vect->size;i++){
		Vect -> vect[i] = h*h;
	}
}

// Produit d'une matrice tridiagonale et d'un veteur
Vector prod_mat_vect_Tri(Matrix Mat, Vector Vect){
	// La matrice Mat est supposee n x n tridiagonale et le Vecteur Vect de taille n
	// Matrice tridiagonale, on ignore donc les 0 dans notre produit, les operations
	// ne se font que sur les diagonales
	Vector Vect_out;
	int size = Mat.ncol;
	float value;

	Vect_out.size = size;
	Vect_out.vect = (float *) malloc(size*sizeof(float));
	// Ligne 1 : 
	Vect_out.vect[0] = Mat.mat[0][0] * Vect.vect[0] + Mat.mat[0][1] * Vect.vect[1];
	// Ligne n :
	Vect_out.vect[size-1] = Mat.mat[size-1][size-2] * Vect.vect[size-2] + Mat.mat[size-1][size-1] * Vect.vect[size-1];

	// Lignes 2 a n-1 :
	for(int i=1;i<size-1;i++){
			value = 0;
			for(int j=i-1;j<i+2;j++){
				value += Mat.mat[i][j] * Vect.vect[j];
			}
			Vect_out.vect[i] = value;
		}

	return Vect_out;
}

// Difference entre Vect1 et Vect2
Vector diff_vect(Vector Vect1, Vector Vect2){
	int size = Vect1.size;

	Vector Vect_out;
	Vect_out.size = size;
	Vect_out.vect = (float *) malloc(size*sizeof(float));
	
	for(int i=0;i<size;i++) Vect_out.vect[i] = Vect1.vect[i] - Vect2.vect[i];

	return Vect_out;
}

// Norme 1 d'un vecteur
// Positif = 0 si une valeur du vecteur est negative
float norme_1(Vector Vect,int positif){
	float norme = 0;
	if(positif){ // Toutes les valeurs du vecteur sont positives 
		for(int i=0;i<Vect.size;i++) norme += Vect.vect[i];
	}
	else{ // Au moins une valeur du vecteur est négative
		for(int i=0;i<Vect.size;i++) norme += fabs(Vect.vect[i]);
	}
	return norme;
}
