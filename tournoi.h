#ifndef TOURNOI_H
#define TOURNOI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "perso.h"

// Structure d'un tournoi
typedef struct {
    char nom[50];
    int nb_participants;
    struct Perso** participants;
    int* points;          // Points pour chaque participant
    int* victoires;       // Nombre de victoires
    int* defaites;        // Nombre de défaites
    int* matchs_joues;    // Nombre de matchs joués
    int format;           // 1 = poule uniquement
} Tournoi;

// Prototypes du mode tournoi
void mode_tournoi(struct Perso** roster);
Tournoi* creer_tournoi(struct Perso** roster, struct Perso* joueur);
void afficher_classement(Tournoi* t);
void simuler_match(struct Perso* p1, struct Perso* p2, int* vainqueur, int afficher);
void executer_tournoi_poule(Tournoi* t);
void liberer_tournoi(Tournoi* t);
void organiser_journee_matchs(Tournoi* t, int journee);

#endif // TOURNOI_H