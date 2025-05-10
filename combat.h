#ifndef COMBAT_H
#define COMBAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "perso.h" // Ensure consistent definitions for Perso and SpecialAttack
#include "equipe.h" // Use centralized definition of Equipe

// Constantes
#define MAX_PERSOS 10
#define MAX_TEAM_NAME 50

// Fonctions pour le combat
int appliquer_action(Perso* atk, Perso* def, int action);
int choisir_action_joueur(Perso* p);
int choisir_action_ia(Perso* p);
int choisir_cible(Equipe* equipe);
void tour_de_combat(Equipe* j, Equipe* ia);

// Nouvelle fonction pour afficher les personnages et leurs PV
void afficher_persos_et_pv(Equipe* joueur, Equipe* ia);
void gerer_jauges(Equipe* equipe) ;

#endif // COMBAT_H