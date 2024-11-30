#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

float p_ch_dir=0.5;
float p_reproduce_proie=0.3;
float p_reproduce_predateur=0.3;
int temps_repousse_herbe=-15;

/* PARTIE 1*/
/* Fourni: Part 1, exercice 3, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 3, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 5, question 1 */
void ajouter_animal(int x, int y, float energie, Animal **liste_animal) {
	assert ((x>=0) && (x<SIZE_X));
	assert ((y>=0) && (y<SIZE_Y));
	Animal *new =creer_animal(x,y,energie);
	*liste_animal=ajouter_en_tete_animal(*liste_animal,new); // on utilise *liste_animal car *liste_animal pointe vers le premier élement de la liste 
}  

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
    // Si la liste est vide, on ne fait rien
    if (*liste == NULL) {
        return;
    }
    
    // Si l'animal courant de la liste est celui qu'on veut enlever
    if (*liste == animal) {
        Animal *temp = *liste;            // Stocker l'animal à supprimer
        *liste = (*liste)->suivant;       // Mettre à jour la liste 
        free(temp);                       // Libère la mémoire de l'animal
    } else {
        // Appel sur le suivant seulement si l'animal n'est pas trouvé
        enlever_animal(&((*liste)->suivant), animal);
    }
}


/* A Faire. Part 1, exercice 5, question 2 */
Animal* liberer_liste_animaux(Animal *liste) {
	Animal *tmp;
	while(liste){
		tmp=liste->suivant;
		free(liste);
		liste=tmp;
	}

  return NULL;
}

/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_it(Animal *la) {
  unsigned int cpt=0;
  while (la) {
    cpt++;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 4, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... corrigé.*/
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }


  // On ajoute les proies
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  // On ajoute les predateurs
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { 
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  // On affiche le tableau
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
  
}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
  Animal *a = la;
  
	while(a){
		if (rand()/(float)RAND_MAX < p_ch_dir){
			a->dir[0] = rand()%3 - 1;
			a->dir[1] = rand()%3 - 1;
		}
		
		a->x = (a->x + a->dir[0] + SIZE_X) % SIZE_X; // Avoir le vrai modulo : ((x % 7) + 7) % 7;
		a->y = (a->y + a->dir[1] + SIZE_Y) % SIZE_Y;
		a = a->suivant;
	}
}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **la, float p_reproduce) {
  Animal *a = la ? *la : NULL;

  while(a){
    if(rand()/(float)RAND_MAX < p_reproduce){
      ajouter_animal(a->x, a->y, a->energie/2.0, la);
      a->energie = a->energie/2.0;
    }

    a = a->suivant;
  }
}


/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
  bouger_animaux(*liste_proie);
  Animal *temp = liste_proie ? *liste_proie : NULL;

  while(temp){
    temp->energie-=1;
    if (temp->energie < 0){
      enlever_animal(liste_proie, temp); // on le supprime si son energie est negative
    }
    

    else if (monde[temp->x][temp->y] > 0){ // si il y'a de l'herbe à la position de la proie
      temp->energie += monde[temp->x][temp->y]; // la proie gagne de l'énergie 
      monde[temp->x][temp->y] = temps_repousse_herbe; // Réinitialisation de l'herbe dans monde
    }

    temp = temp->suivant;
  }
  reproduce(liste_proie, p_reproduce_proie);
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
  Animal *temp = l;
  while(temp){
    if ((temp->x == x) && (temp->y == y))
      return temp;
    
    temp = temp->suivant;
  }

  return NULL;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
  bouger_animaux(*liste_predateur);
  Animal *tmp = liste_predateur ? *liste_predateur : NULL;

  while(tmp){
    tmp->energie-=1; // baisser l'energie de 1

    if (tmp->energie < 0){
      enlever_animal(liste_predateur, tmp); // on le supprime si son energie est negative
    }
    else {
      Animal *proieAmanger = animal_en_XY(*liste_proie, tmp->x, tmp->y); // on vérifie si y'a une proie
      if (proieAmanger != NULL){
        tmp->energie += proieAmanger->energie; // le prédateur recupere l'energie de la proie
        enlever_animal(liste_proie, proieAmanger); // puis on supprime la proie
      }
    }

    tmp = tmp->suivant;
  }

  reproduce(liste_predateur, p_reproduce_predateur);
}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
  for(int i = 0; i < SIZE_X; i++)
    for(int j = 0; j < SIZE_Y; j++)
      monde[i][j]++;
}





void ecrire_ecosys(const char *nom_fichier, Animal *liste_proie, Animal *liste_predateur){
	FILE *f = fopen(nom_fichier, "w");
	if (f == NULL){
		printf("Erreur lors de l'ouverture.");
		exit(1);
	}

	fprintf(f, "<proie>\n");
	Animal *tmp = liste_proie;
	while(tmp){
		fprintf(f, "x=%d y=%d dir=[%d,%d] e=%f\n", tmp->x, tmp ->y, tmp->dir[0], tmp->dir[1], tmp->energie);
		tmp = tmp->suivant;
	}
	fprintf(f,"</proie>\n");

	fprintf(f,"<predateur>\n");
	tmp = liste_predateur;
	while(tmp){
		fprintf(f, "x=%d y=%d dir=[%d,%d] e=%f\n", tmp->x, tmp ->y, tmp->dir[0], tmp->dir[1], tmp->energie);
		tmp = tmp->suivant;
	}
	fprintf(f, "</predateur>\n");
	
	fclose(f);
}

void lire_ecosys(const char *nom_fichier, Animal **liste_proie, Animal **liste_predateur){
	FILE *f = fopen(nom_fichier, "r");
	if (f == NULL){
		printf("Erreur lors de l'ouverture.");
		exit(1); 
	}

	char buffer[256]; 
	fgets(buffer, 256, f);  // Lit la première ligne du fichier
	assert(strncmp(buffer, "<proie>", 7) == 0);  // Vérifie que le format commence avec "<proie>"

	int x_lu, y_lu, dir0, dir1; float e_lu;  
	fgets(buffer, 256, f);  

	// Boucle pour lire chaque proie jusqu'à rencontrer "</proie>"
	while(strncmp(buffer, "</proie>", 8) != 0){
		// Extrait les données de la proie depuis la ligne courante
		sscanf(buffer, "x=%d y=%d dir=[%d,%d] e=%f\n", &x_lu, &y_lu, &dir0, &dir1, &e_lu);
		Animal *a = creer_animal(x_lu, y_lu, e_lu);
		a->dir[0] = dir0; 
		a->dir[1] = dir1; 
		a->suivant = *liste_proie;
		*liste_proie = a; // mise à jour de la liste
		fgets(buffer, 256, f);  // Lit la ligne suivante pour la prochaine itération
	}

	// Lit une ligne supplémentaire pour atteindre la fin de la section des proies
	fgets(buffer, 256, f);
	assert(strncmp(buffer, "<predateur>", 11) == 0);  // Vérifie que le format continue avec "<predateur>"

	fgets(buffer, 256, f); 

	// Boucle pour lire chaque prédateur jusqu'à rencontrer "</predateur>"
	while(strncmp(buffer, "</predateur>", 12) != 0){
		// Extrait les données du prédateur depuis la ligne courante
		sscanf(buffer, "x=%d y=%d dir=[%d,%d] e=%f\n", &x_lu, &y_lu, &dir0, &dir1, &e_lu);
		// Crée un nouvel animal avec les coordonnées et l'énergie extraites
		Animal *a = creer_animal(x_lu, y_lu, e_lu);
		a->dir[0] = dir0;  
		a->dir[1] = dir1;  
		a->suivant = *liste_predateur;
		*liste_predateur = a; // mise à jour de la liste
		fgets(buffer, 256, f);  // Lit la ligne suivante pour la prochaine itération
	}

	fclose(f); 
}

