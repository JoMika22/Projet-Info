/*
 * utils.h
 * Utilitaires pour le jeu de combat
 * Macros de couleurs et fonctions d'animation
 */

 #ifndef UTILS_H
 #define UTILS_H
 
 #include <stdio.h>
 
 #define RESET "\033[0m"
 #define RED "\033[31m"
 #define GREEN "\033[32m"
 #define YELLOW "\033[33m"
 #define BLUE "\033[34m"
 #define MAGENTA "\033[35m"
 #define CYAN "\033[36m"
 #define BOLD "\033[1m"
 
 void clear_terminal();
 void animation_combat_epique(const char* attaquant, const char* defenseur);
 void afficher_barre_sante(int PV, int PVmax);
 int lire_entree_valide(int min, int max);
 
 #endif 