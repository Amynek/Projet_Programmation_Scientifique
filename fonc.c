// KADDOURI AMYNE 22103988
#include "decl.h"

void main(){
	int nmin=5;
	int nmax=705;
	int pas=50;
	int lmax=(nmax-nmin)/pas; // Nombre de subdivisions de l'intervalle [nmin;nmax]
	float err_approx[lmax]; 
	for(int l=0;l<lmax+1;l++){
		int n = nmin + l*pas;
		float h = 1.0/(n+1);

//============= Definition des matrices A, b et x =============//
	// Matrice A :
		Matrix A;
		Matrix ABis;

	// On cree une seconde matrice A car les valeurs de l'originale seront ecrasees par sgesv_
		mat_create(&A,n,n);
		mat_create(&ABis,n,n);
		
	// Vecteur b et x :
		Vector b;
		Vector x;
		vect_create(&b,n);
		vect_create(&x,n);
		

//============= Initialisation des matrices A, b et x =============//
	// A doit etre enregistree en memoire par colonnes du aux specifications fortran
	// Nous devrions initialiser A^T or A symetrique donc A=A^T 
		mat_init_A(&A);
		mat_init_A(&ABis);	

	// On pose x=b, le vecteur x sera modifie en sortie de sgesv
		vect_init_b(&b,h);
		vect_init_b(&x,h);

//============= Resolution du systeme =============//
		int one=1; 
		int IPIV[n];
		int INFO;
		
		sgesv_(&n,&one,A.mat_alloc,&n,IPIV,x.vect,&n,&INFO);	

	// Liberation de la memoire
		mat_free(A);

//============== Calcul de la norme 1 de b ==============//
	// Tous les b_i sont positifs
		float norme1_b = norme_1(b,1);

//============= Calcul de la norme 1 de Ax-b =============//
	// Declaration et calcul du vecteur Ax-b :
		Vector Ax;
		Vector Axb;
		vect_create(&Ax,n);
		vect_create(&Axb,n);

		Ax = prod_mat_vect_Tri(ABis, x);
	// Liberation de la memoire
		mat_free(ABis);
		vect_free(x); 

		Axb = diff_vect(Ax,b); 
	// Liberation de la memoire
		vect_free(Ax);
		vect_free(b);

	// ||Ax-b||_1
		float norme1_Axb = norme_1(Axb,0);
		vect_free(Axb);

//============= Stockage de l'erreur d'approximation =============//
		err_approx[l] = norme1_Axb/norme1_b;

	}

//===== Creation et ecriture du fichier contenant les resultats obtenus =====//	
	FILE* pFile = fopen("output_fonc.txt","w");
	
	if(pFile == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	for(int i=0;i<lmax+1;i++) fprintf(pFile,"%d%23.16e\n",nmin+i*pas,err_approx[i]);
	printf("Le fichier output_fonc.txt contenant les resultats a ete ecrit avec succes.\n");
	fclose(pFile);
}
