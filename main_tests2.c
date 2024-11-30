#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ecosys.h"


int main(){
	srand(time(NULL));

	// Création des listes de proies et de prédateurs (20 par liste)

	Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;

    // Création des proies
    for (int i = 0; i < 20; i++) {
        int x = rand() % SIZE_X; // on s'assure que x et y sont entre 0 et size_x
        int y = rand() % SIZE_Y;
        ajouter_animal(x, y, 50.0, &liste_proie);  
    }

    // Création des prédateurs
    for (int i = 0; i < 20; i++) {
        int x = rand() % SIZE_X;
        int y = rand() % SIZE_Y;
        ajouter_animal(x, y, 100.0, &liste_predateur); 
	}
	
	// Affichage du nombre de proies et de prédateurs
    printf("Nombre de proies: %d\n", compte_animal_it(liste_proie));
    printf("Nombre de prédateurs: %d\n", compte_animal_it(liste_predateur));
    
     // Affichage de l'état de l'écosystème (les listes d'animaux)
    afficher_ecosys(liste_proie,liste_predateur);
    
    // test de la fonction enlever_animal
    enlever_animal(&liste_proie,liste_proie);
    enlever_animal(&liste_proie,liste_proie->suivant);
    enlever_animal(&liste_predateur,liste_predateur);
    enlever_animal(&liste_predateur,liste_predateur->suivant);
	afficher_ecosys(liste_proie,liste_predateur);

	// Test des fonctions de lecture et d'écriture dans les fichiers

	Animal *liste_proies_lue = NULL;
	Animal *liste_predateurs_lue = NULL;
	ecrire_ecosys("liste_ecosys.txt", liste_proie, liste_predateur);
	lire_ecosys("liste_ecosys.txt", &liste_proies_lue, &liste_predateurs_lue);


	afficher_ecosys(liste_proies_lue, liste_predateurs_lue); printf("\n");

	// verification que les listes lues et écrits sont les memes
	Animal *proies_ecrits=liste_proie;
	printf("Proies écrits :\n");
    while (proies_ecrits) {
  		printf("%d, %d, %f\n", proies_ecrits->x, proies_ecrits->y, proies_ecrits->energie);
  		proies_ecrits= proies_ecrits->suivant;
    }
    printf("\n");
	
	Animal *proies_lus = liste_proies_lue;
	printf("Proies lus :\n");
    while (proies_lus) {
  		printf("%d, %d, %f\n", proies_lus ->x, proies_lus ->y, proies_lus ->energie);
  		proies_lus  = proies_lus ->suivant;
    }
    printf("\n");
    
    Animal *predateurs_ecrits=liste_predateur;
	printf("Prédateurs écrits :\n");
    while (predateurs_ecrits) {
  		printf("%d, %d, %f\n", predateurs_ecrits->x, predateurs_ecrits->y, predateurs_ecrits->energie);
  		predateurs_ecrits= predateurs_ecrits->suivant;
    }
    printf("\n");
    
  
	Animal *predeateurs_lus = liste_predateurs_lue;
  	printf("\nPrédateurs lus :\n");
  	while (predeateurs_lus ) {
  		printf("%d, %d, %f\n", predeateurs_lus ->x, predeateurs_lus ->y, predeateurs_lus ->energie);
  		predeateurs_lus  = predeateurs_lus ->suivant;
    }
  
	liste_proie = liberer_liste_animaux(liste_proie);
	liste_predateur = liberer_liste_animaux(liste_predateur);
	liste_proies_lue = liberer_liste_animaux(liste_proies_lue);
	liste_predateurs_lue = liberer_liste_animaux(liste_predateurs_lue);

	return 0;
}
