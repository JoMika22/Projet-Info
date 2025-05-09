#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipe.h"
#include "perso.h"
#include "utilis.h"

// Fonction pour créer une équipe pour le joueur
// Permet au joueur de nommer son équipe et de choisir deux personnages parmi une liste
Equipe* creer_equipe_joueur(Perso** persos) {
    // Allocation mémoire pour l'équipe
    Equipe* e = malloc(sizeof(Equipe));
    if (!e) {
        perror("malloc equipe");
        exit(1);
    }

    // Saisie du nom de l'équipe
    printf("Nom de l'équipe : ");
    fgets(e->nom_equipe, MAX_TEAM_NAME, stdin);
    e->nom_equipe[strcspn(e->nom_equipe, "\n")] = 0;

    // Initialisation de la taille de l'équipe
    e->taille = 2;

    // Allocation mémoire pour les membres de l'équipe
    e->membres = malloc(sizeof(Perso*) * 2);
    if (!e->membres) {
        perror("malloc membres equipe");
        exit(1);
    }

    // Affichage du menu des personnages disponibles
    afficher_menu_personnages(persos);

    // Boucle pour sélectionner deux personnages
    for (int i = 0; i < 2; i++) {
        printf("Sélection perso %d [1-%d] : ", i+1, MAX_PERSOS);
        int choix = lire_entree_valide(1, MAX_PERSOS);

        // Clonage du personnage sélectionné
        Perso* original = persos[choix-1];
        Perso* clone = malloc(sizeof(Perso));
        if (!clone) {
            perror("malloc clone personnage");
            exit(1);
        }
        *clone = *original;
        clone->nom = strdup(original->nom);
        if (!clone->nom) {
            perror("strdup clone->nom");
            free(clone);
            exit(1);
        }
        e->membres[i] = clone;
    }
    return e;
}

// Fonction pour créer une équipe contrôlée par l'IA
// Génère une équipe avec deux personnages aléatoires et ajuste leurs statistiques selon la difficulté
Equipe* creer_equipe_IA(Perso** persos, int diff) {
    // Allocation mémoire pour l'équipe IA
    Equipe* e = malloc(sizeof(Equipe));
    if (!e) {
        perror("malloc equipe IA");
        exit(1);
    }

    // Nom par défaut pour l'équipe IA
    strncpy(e->nom_equipe, "IA", MAX_TEAM_NAME-1);
    e->nom_equipe[MAX_TEAM_NAME-1] = '\0';

    // Initialisation de la taille de l'équipe
    e->taille = 2;

    // Allocation mémoire pour les membres de l'équipe
    e->membres = malloc(sizeof(Perso*) * 2);
    if (!e->membres) {
        perror("malloc membres equipe IA");
        free(e);
        exit(1);
    }

    // Tableau pour éviter les doublons
    int used[MAX_PERSOS] = {0};

    // Boucle pour sélectionner deux personnages aléatoires
    for (int i = 0; i < 2; i++) {
        int r;
        do { r = rand() % MAX_PERSOS; } while (used[r]);
        used[r] = 1;

        // Clonage du personnage sélectionné
        Perso* base = persos[r];
        Perso* clone = malloc(sizeof(Perso));
        if (!clone) {
            perror("malloc clone personnage");
            for (int j = 0; j < i; j++) {
                free(e->membres[j]->nom);
                free(e->membres[j]);
            }
            free(e->membres);
            free(e);
            exit(1);
        }

        *clone = *base;
        clone->nom = strdup(base->nom);
        if (!clone->nom) {
            perror("strdup clone->nom");
            free(clone);
            for (int j = 0; j < i; j++) {
                free(e->membres[j]->nom);
                free(e->membres[j]);
            }
            free(e->membres);
            free(e);
            exit(1);
        }

        // Ajustement des statistiques selon la difficulté
        double factor = 1.0 + 0.2 * (diff - 1);
        clone->PVmax = (int)(clone->PVmax * factor);
        clone->PV = clone->PVmax;
        clone->att = (int)(clone->att * factor);
        clone->def = (int)(clone->def * factor);
        clone->agi = (int)(clone->agi * factor);
        clone->vit = (int)(clone->vit * factor);
        clone->special_cd = 0;
        clone->shield_active = 0;
        e->membres[i] = clone;
    }
    return e;
}

// Fonction pour afficher les détails d'une équipe
// Affiche le nom de l'équipe et les statistiques de chaque membre
void afficher_equipe(Equipe* e) {
    printf("\n-- Équipe %s --\n", e->nom_equipe);
    for (int i = 0; i < e->taille; i++) {
        Perso* p = e->membres[i];
        printf("%s (PV:%d/%d ATK:%d DEF:%d VIT:%d)\n",
               p->nom, p->PV, p->PVmax, p->att, p->def, p->vit);
    }
}

// Fonction pour libérer la mémoire allouée à une équipe
// Libère la mémoire de chaque membre de l'équipe, puis celle de l'équipe elle-même
void liberer_equipe(Equipe* e) {
    if (!e) return;

    // Libération de la mémoire des membres
    for (int i = 0; i < e->taille; i++) {
        if (e->membres[i]) {
            if (e->membres[i]->nom) {
                free(e->membres[i]->nom);
                e->membres[i]->nom = NULL;
            }
            free(e->membres[i]);
            e->membres[i] = NULL;
        }
    }

    // Libération de la mémoire de l'équipe
    if (e->membres) {
        free(e->membres);
        e->membres = NULL;
    }

    free(e);
    e = NULL;
}