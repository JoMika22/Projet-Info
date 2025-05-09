// Inclusion des bibliothèques nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#include "affichage.h"
#include "combat.h"
#include "tournoi.h"
#include "perso.h" 

#define MAX_PERSOS 10
#define MAX_TEAM_NAME 50

// Fonction principale du programme
// Gère le menu principal et les différents modes de jeu
int main() {
    srand(time(NULL)); // Initialisation de la graine pour les nombres aléatoires
    int choix = afficher_menu_principal(); // Affiche le menu principal et récupère le choix de l'utilisateur
    while (choix != 3) { // Boucle principale tant que l'utilisateur ne choisit pas de quitter
        switch(choix) {
            case 1: { // Mode combat classique
                int diff = choisir_difficulte(); // Choix de la difficulté
                Perso** persos = creer_persos_disponibles(); // Création des personnages disponibles
                assert(persos != NULL && "Erreur: creer_persos_disponibles a retourné NULL");

                Equipe* joueur = creer_equipe_joueur(persos); // Création de l'équipe du joueur
                Equipe* ia = creer_equipe_IA(persos, diff); // Création de l'équipe IA

                assert(joueur != NULL && "Erreur: creer_equipe_joueur a retourné NULL");
                assert(ia != NULL && "Erreur: creer_equipe_IA a retourné NULL");

                afficher_equipe(joueur); // Affiche les détails de l'équipe du joueur
                afficher_equipe(ia); // Affiche les détails de l'équipe IA

                printf("\nAppuie sur Entrée pour commencer le combat...");
                getchar();

                tour_de_combat(joueur, ia); // Lance le combat entre les deux équipes

                printf("\nAppuie sur Entrée pour continuer...");
                getchar();

                liberer_equipe(joueur); // Libère la mémoire allouée pour l'équipe du joueur
                liberer_equipe(ia); // Libère la mémoire allouée pour l'équipe IA
                liberer_persos(persos); // Libère la mémoire allouée pour les personnages
                break;
            }
            case 2: { // Mode tournoi
                Perso** roster = creer_persos_disponibles(); // Création des personnages disponibles pour le tournoi
                assert(roster != NULL && "Erreur: creer_persos_disponibles a retourné NULL");

                mode_tournoi(roster); // Lance le mode tournoi
                liberer_persos(roster); // Libère la mémoire allouée pour les personnages
                break;
            }
            case 3:
                printf(BOLD GREEN "\nMerci d'avoir joué! À bientôt!\n" RESET); // Message de fin
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n"); // Gestion des choix invalides
        }
        if (choix != 3) {
            choix = afficher_menu_principal(); // Réaffiche le menu principal si l'utilisateur ne quitte pas
        }
    }

    return 0; // Fin du programme
}