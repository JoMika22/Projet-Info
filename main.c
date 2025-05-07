#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#include "affichage.h"
#include "combat.h"
#include "tournoi.h"
#include "perso.h" // Use centralized definition of Perso and SpecialAttack

#define MAX_PERSOS 10
#define MAX_TEAM_NAME 50


int main() {
    srand(time(NULL));
    int choix = afficher_menu_principal();
    while (choix != 3) {
        switch(choix) {
            case 1: {
                int diff = choisir_difficulte();
                Perso** persos = creer_persos_disponibles();
                assert(persos != NULL && "Erreur: creer_persos_disponibles a retourné NULL");

                Equipe* joueur = creer_equipe_joueur(persos);
                Equipe* ia = creer_equipe_IA(persos, diff);

                assert(joueur != NULL && "Erreur: creer_equipe_joueur a retourné NULL");
                assert(ia != NULL && "Erreur: creer_equipe_IA a retourné NULL");

                afficher_equipe(joueur);
                afficher_equipe(ia);

                printf("\nAppuie sur Entrée pour commencer le combat...");
                getchar();

                tour_de_combat(joueur, ia);

                printf("\nAppuie sur Entrée pour continuer...");
                getchar();

                liberer_equipe(joueur);
                liberer_equipe(ia);
                liberer_persos(persos);
                break;
            }
            case 2: {
                Perso** roster = creer_persos_disponibles();
                assert(roster != NULL && "Erreur: creer_persos_disponibles a retourné NULL");

                mode_tournoi(roster);
                liberer_persos(roster);
                break;
            }
            case 3:
                printf(BOLD GREEN "\nMerci d'avoir joué! À bientôt!\n" RESET);
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
        if (choix != 3) {
            choix = afficher_menu_principal();
        }
    }

    return 0;
}