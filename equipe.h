/*
 * equipe.h
 * Gestion des équipes de personnages
 */

 #ifndef EQUIPE_H
 #define EQUIPE_H
 
 #include "perso.h" // Ensure consistent definitions for Perso and SpecialAttack
 
 #define MAX_TEAM_NAME 50
 
 // Structure d'une équipe (pour le 2v2)
 typedef struct {
     char nom_equipe[MAX_TEAM_NAME];
     Perso** membres;
     int taille;
 } Equipe;
 
 // Prototypes des fonctions
 Equipe* creer_equipe_joueur(Perso** persos);
 Equipe* creer_equipe_IA(Perso** persos, int diff);
 void afficher_equipe(Equipe* e);
 void liberer_equipe(Equipe* e);
 
 #endif /* EQUIPE_H */