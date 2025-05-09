#include "affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utilis.h" 



// Fonction qui affiche le menu principal et retourne le choix de l'utilisateur
int afficher_menu_principal() {
    clear_terminal(); // Efface le terminal 
    printf(BOLD YELLOW "=== MENU PRINCIPAL ===\n" RESET); 
    printf(BOLD CYAN "1) Combat Classique 2v2\n" RESET); // Option 1
    printf(BOLD GREEN "2) Mode Tournoi\n" RESET); // Option 2
    printf(BOLD RED "3) Quitter\n" RESET); // Option 3
    printf(BOLD YELLOW "======================\n" RESET); 
    printf(BOLD MAGENTA "Fais ton choix : " RESET); 

    return lire_entree_valide(1, 3); // Lit et retourne un choix valide entre 1 et 3
}

// Fonction qui affiche les options de difficulté
void afficher_difficulte() {
    clear_terminal(); // Efface le terminal 
    printf(BOLD YELLOW "\nChoisis la difficulté :\n" RESET); 
    printf(BOLD GREEN "1) Facile\n" RESET); // Option 1 : Facile
    printf(BOLD CYAN "2) Normal\n" RESET); // Option 2 : Normal
    printf(BOLD RED "3) Difficile\n" RESET); // Option 3 : Difficile
}

// Fonction qui permet à l'utilisateur de choisir une difficulté et retourne son choix
int choisir_difficulte() {
    afficher_difficulte(); 
    return lire_entree_valide(1, 3); // Lit et retourne un choix valide entre 1 et 3
}

// Fonction qui affiche les dégâts infligés par un attaquant à un défenseur
void afficher_degats(Perso* attaquant, Perso* defenseur, int degats) {
    printf(BOLD "%s" RESET " inflige " BOLD RED "%d" RESET " dégâts à " BOLD "%s" RESET "\n", 
           attaquant->nom, degats, defenseur->nom); // Affiche les noms et les dégâts
}

// Fonction qui affiche l'utilisation d'une attaque spéciale
void afficher_special(Perso* attaquant, Perso* defenseur, int degats) {
    printf(BOLD MAGENTA "%s" RESET " utilise " BOLD YELLOW "%s" RESET " et inflige " BOLD RED "%d" RESET " dégâts à " BOLD "%s" RESET "!\n", 
           attaquant->nom, attaquant->special->nom, degats, defenseur->nom); // Affiche les détails de l'attaque spéciale
}

// Fonction qui affiche un message lorsque le défenseur esquive une attaque
void afficher_esquive(Perso* defenseur) {
    printf(BOLD GREEN "%s esquive l'attaque avec succès!" RESET "\n", defenseur->nom); // Affiche le nom du défenseur
}

// Fonction qui affiche un message lorsque le personnage active un bouclier
void afficher_shield(Perso* perso) {
    printf(BOLD CYAN "%s active un bouclier" RESET "\n", perso->nom); // Affiche le nom du personnage
}

// Fonction qui affiche un message lorsque le personnage est mis KO
void afficher_ko(Perso* perso) {
    printf(BOLD RED "%s KO!" RESET "\n", perso->nom); // Affiche le nom du personnage
}

// Fonction qui affiche un message de victoire pour une équipe
void afficher_victoire(Equipe* equipe) {
    printf("\n" BOLD GREEN "Victoire de l'équipe %s!" RESET "\n", equipe->nom_equipe); // Affiche le nom de l'équipe gagnante
}





