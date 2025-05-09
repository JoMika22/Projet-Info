#ifndef COMBAT_H
#define COMBAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "perso.h" 
#include "equipe.h" 

// Constantes
#define MAX_PERSOS 10
#define MAX_TEAM_NAME 50

// Fonctions pour le combat
void appliquer_action(Perso* atk, Perso* def, int action);
int choisir_action_joueur(Perso* p);
int choisir_action_ia(Perso* p);
void tour_de_combat(Equipe* j, Equipe* ia);

#endif // COMBAT_H
