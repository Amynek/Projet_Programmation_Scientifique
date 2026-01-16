// KADDOURI AMYNE 22103988
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void sgesv_(int *, int *, float *, int *, int *, float *, int *, int *); 

void main(){
	int nmin=5;
	int nmax=705;
	int pas=50;
	int lmax=(nmax-nmin)/pas;
	float err_approx[lmax]; 
	for(int l=0;l<lmax+1;l++){
		int n = nmin + l*pas;
		float h = 1.0/(n+1);

//============= Definition des matrices A, b et x =============//
	// Matrice A :
		float* mat_allocA = (float *) calloc(n*n,sizeof(float));
		float** mat_A = (float **) malloc(n*sizeof(float *));
		for(int i=0;i<n;i++) mat_A[i] = &mat_allocA[i*n]; 

	// On cree une seconde matrice A car les valeurs de l'originale seront ecrasees par sgesv_
		float* mat_allocABis = (float *) calloc(n*n,sizeof(float));
		float** mat_ABis = (float **) malloc(n*sizeof(float *));
		for(int i=0;i<n;i++) mat_ABis[i] = &mat_allocABis[i*n];

	// Vecteurs b et x :
		float* mat_b = (float *) malloc(n*sizeof(float));
		float* mat_x = (float *) malloc(n*sizeof(float));
		

//============= Initialisation des matrices A, b et x =============//
	// A doit etre enregistree en memoire par colonnes du aux specifications fortran
	// Nous devrions initialiser A^T or A symetrique donc A=A^T 
		for(int i=0;i<n;i++){
			mat_A[i][i] = 2;
			mat_ABis[i][i] = 2;

	//(On pose x=b, x sera modifie en sortie de sgesv)
			mat_b[i]=h*h;
			mat_x[i]=h*h;
		}
		for(int i=1;i<n-1;i++){
			mat_A[i][i+1] = -1;
			mat_ABis[i][i+1] = -1;

			mat_A[i][i-1] = -1;
			mat_ABis[i][i-1] = -1;
		}
		mat_A[0][1] = -1;
		mat_ABis[0][1] = -1;
		mat_A[n-1][n-2] = -1;
		mat_ABis[n-1][n-2] = -1;


//============= Resolution du systeme =============//
		int one=1; 
		int IPIV[n];
		int INFO;
		
		sgesv_(&n,&one,mat_allocA,&n,IPIV,mat_x,&n,&INFO);	

	//Liberation de la memoire
		free(mat_allocA);
		free(mat_A);

//============== Calcul de la norme 1 de b ==============//
		float norme1_b = 0;
	// Tous les b_i sont positifs : fabs non necessaire
		for(int i=0;i<n;i++) norme1_b += mat_b[i]; 

//============= Calcul de la norme 1 de Ax-b =============//
	// Declaration du vecteur Ax-b :
		float* mat_Axb = (float *) malloc(n*sizeof(float));

	// A est tridiagonale, pour le produit Matrice - Vecteur
	// on utilisera seulement les elements de A != 0
		
	// Ligne 1 : 
		mat_Axb[0] = mat_ABis[0][0] * mat_x[0] + mat_ABis[0][1] * mat_x[1] - mat_b[0];  
	// Ligne n :
		mat_Axb[n-1] = mat_ABis[n-1][n-2] * mat_x[n-2] + mat_ABis[n-1][n-1] * mat_x[n-1] - mat_b[n-1];
	// Lignes 2 a n-1 :
		for(int i=1;i<n-1;i++){
			float value = 0;
			for(int j=i-1;j<i+2;j++){ 
				value += mat_ABis[i][j] * mat_x[j];
			}
			mat_Axb[i] = value - mat_b[i];
		}

	// ||Ax-b||_1
		float norme1_Axb = 0;
		for(int i=0;i<n;i++) norme1_Axb += fabs(mat_Axb[i]); 		
	
//============= Liberation de la memoire =============//
		free(mat_allocABis);
		free(mat_ABis);
		free(mat_b);
		free(mat_x);
		free(mat_Axb);

//============= Stockage de l'erreur d'approximation =============//
		err_approx[l] = norme1_Axb/norme1_b;

	}

//===== Creation et ecriture du fichier contenant les resultats obtenus =====//	
	FILE* pFile = fopen("output_mono.txt","w");
	
	if(pFile == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	for(int i=0;i<lmax+1;i++) fprintf(pFile,"%d%23.16e\n",5+i*pas,err_approx[i]);
	printf("Le fichier output_mono.txt contenant les resultats a ete ecrit avec succes.\n");
	fclose(pFile);
}
