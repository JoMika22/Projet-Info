#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdio.h>
#include <unistd.h>
#include "perso.h" // Ensure consistent definitions for Perso and SpecialAttack
#include "equipe.h"
#include "tournoi.h"

// Macros pour les couleurs
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"


// Menus et interfaces
int afficher_menu_principal();
void afficher_difficulte();
int choisir_difficulte();

// Affichage combat
void afficher_action(Perso* perso, int action);
void afficher_degats(Perso* attaquant, Perso* defenseur, int degats);
void afficher_special(Perso* attaquant, Perso* defenseur, int degats);
void afficher_esquive(Perso* defenseur);
void afficher_shield(Perso* perso);
void afficher_ko(Perso* perso);
void afficher_victoire(Equipe* equipe);



#endif // AFFICHAGE_H