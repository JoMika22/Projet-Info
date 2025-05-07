#include "affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utilis.h" // Include the header for lire_entree_valide



// Menus et interfaces
int afficher_menu_principal() {
    clear_terminal();
    printf(BOLD YELLOW "=== MENU PRINCIPAL ===\n" RESET);
    printf(BOLD CYAN "1) Combat Classique 2v2\n" RESET);
    printf(BOLD GREEN "2) Mode Tournoi\n" RESET);
    printf(BOLD RED "3) Quitter\n" RESET);
    printf(BOLD YELLOW "======================\n" RESET);
    printf(BOLD MAGENTA "Fais ton choix : " RESET);

    return lire_entree_valide(1, 3);
}

void afficher_difficulte() {
    clear_terminal();
    printf(BOLD YELLOW "\nChoisis la difficulté :\n" RESET);
    printf(BOLD GREEN "1) Facile\n" RESET);
    printf(BOLD CYAN "2) Normal\n" RESET);
    printf(BOLD RED "3) Difficile\n" RESET);
}

int choisir_difficulte() {
    afficher_difficulte();
    return lire_entree_valide(1, 3);
}


void afficher_degats(Perso* attaquant, Perso* defenseur, int degats) {
    printf(BOLD "%s" RESET " inflige " BOLD RED "%d" RESET " dégâts à " BOLD "%s" RESET "\n", 
           attaquant->nom, degats, defenseur->nom);
}

void afficher_special(Perso* attaquant, Perso* defenseur, int degats) {
    printf(BOLD MAGENTA "%s" RESET " utilise " BOLD YELLOW "%s" RESET " et inflige " BOLD RED "%d" RESET " dégâts à " BOLD "%s" RESET "!\n", 
           attaquant->nom, attaquant->special->nom, degats, defenseur->nom);
}

void afficher_esquive(Perso* defenseur) {
    printf(BOLD GREEN "%s esquive l'attaque avec succès!" RESET "\n", defenseur->nom);
}

void afficher_shield(Perso* perso) {
    printf(BOLD CYAN "%s active un bouclier" RESET "\n", perso->nom);
}

void afficher_ko(Perso* perso) {
    printf(BOLD RED "%s KO!" RESET "\n", perso->nom);
}

void afficher_victoire(Equipe* equipe) {
    printf("\n" BOLD GREEN "Victoire de l'équipe %s!" RESET "\n", equipe->nom_equipe);
}





