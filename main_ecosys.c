#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>

#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000
  


int main(void) {

	srand(time(NULL));
	
	// Test pour le bouger_animaux();

  Animal *a1 = creer_animal(17, 14, 12.0);
  assert(a1->x >= 0 && a1->x < SIZE_X); assert(a1->y >= 0 && a1->y < SIZE_Y);

  printf("Test de la fonction bouger_animaux() :\n");
  printf("Animal avant de le bouger : %d, %d, %.2f, [%d,%d]\n", a1->x, a1->y, a1->energie, a1->dir[0], a1->dir[1]);
  while((a1->x == 17) && (a1->y == 14))
    bouger_animaux(a1);
  printf("Animal après l'avoir bouger : %d, %d, %.2f, [%d,%d]\n", a1->x, a1->y, a1->energie, a1->dir[0], a1->dir[1]);

  printf("\n");
  
   // Test pour le reproduce(); (on est supposés avoir 8 fois le meme animal apres avoir appliqué 3 fois la fonction reproduce)

  for(int i = 0; i < 3; i++){
    reproduce(&a1, 1);
  }
  printf("Test de la fonction reproduce() 3 fois de suite : \n");
  Animal *temp = a1;
  while(temp){
    printf("%d, %d, %.2f, [%d,%d]\n", a1->x, a1->y, a1->energie, a1->dir[0], a1->dir[1]);
    temp = temp->suivant;
  }

  printf("\n");
  a1 = liberer_liste_animaux(a1);

	



 	// Initialisation du monde avec des herbes (valeurs aléatoires entre 0 et 2)
    int monde[SIZE_X][SIZE_Y] = {0}; 
    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
            monde[i][j] = rand() % 3; // 0 (pas d'herbe), 1 ou 2 (quantités d'herbe)
        }
    }
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



  //Lancement de la simulation.

  	int cpt = 0;
    while ((liste_proie != NULL || liste_predateur != NULL) && cpt < 100) {
        // Affichage de l'état actuel
        printf("Iteration %d - Proies: %d, Prédateurs: %d\n", cpt, compte_animal_it(liste_proie), compte_animal_it(liste_predateur));
        afficher_ecosys(liste_proie, liste_predateur);  // Affichage de l'écosystème

        // Pause de 1 seconde (1 000 000 microsecondes)
        usleep(1000000);

        clear_screen();

        // Mise à jour des proies et prédateurs
        rafraichir_proies(&liste_proie, monde);
        rafraichir_predateurs(&liste_predateur, &liste_proie);

        // Mise à jour de l'état du monde (par exemple, croissance de l'herbe)
        rafraichir_monde(monde);

        cpt++;  // Incrémentation du compteur d'itérations
    }

    // Libération des listes d'animaux
    liste_proie = liberer_liste_animaux(liste_proie);
    liste_predateur = liberer_liste_animaux(liste_predateur);

    printf("La simulation est terminée !\n");

    return 0;
}

