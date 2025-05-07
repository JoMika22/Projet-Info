/*
 * personnages.c
 * Implémentation des fonctions liées aux personnages
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "perso.h"
 #include "utilis.h"
 
 SpecialAttack special_attacks[MAX_PERSOS] = {
     {"Éclair Foudroyant", 3, 5, "Décharge un éclair électrique puissant", 0},
     {"Gum Gum Pistol", 3, 6, "Attaque élastique puissante", 0},
     {"Coup de Bicycle", 3, 4, "Frappe acrobatique dévastatrice", 0},
     {"Super Splash", 3, 5, "Attaque aquatique massive", 0},
     {"Puissance Ogre", 3, 6, "Déchaîne la force primitive", 0},
     {"Champions League Strike", 3, 4, "Frappe précise enroulée", 0},
     {"Santoryu Ougi", 3, 5, "Triple attaque éclair", 0},
     {"Bubble Beam", 3, 4, "Canon à bulles paralysant", 0},
     {"Kamehameha", 3, 7, "Attaque énergétique ultime", 0},
     {"Muffin Assault", 3, 3, "Muffins empoisonnés", 0}
 };
 
 Perso** creer_persos_disponibles() {
     Perso** persos = malloc(sizeof(Perso*) * MAX_PERSOS);
     if (!persos) { 
         perror("malloc persos"); 
         exit(1); 
     }
     const char* noms[MAX_PERSOS] = {"Pikachu","Luffy","Salah","El Primo","Shrek","CR7","Zoro","Bob l'éponge","Goku","Muffin Man"};
     int stats[MAX_PERSOS][5] = {
         {80, 25, 15, 50, 35}, // Pikachu
         {90, 45, 40, 10, 15}, // Luffy
         {75, 20, 10, 55, 40}, // Salah
         {100, 45, 30, 15, 10}, // El Primo
         {95, 25, 45, 15, 20}, // Shrek
         {75, 25, 10, 50, 40}, // CR7
         {85, 50, 30, 20, 15}, // Zoro
         {60, 15, 30, 40, 55}, // Bob l’éponge
         {90, 50, 20, 25, 15}, // Goku
         {65, 20, 25, 45, 45}  // Muffin Man
     };

     for (int i = 0; i < MAX_PERSOS; i++) {
         persos[i] = malloc(sizeof(Perso));
         persos[i]->nom = strdup(noms[i]);
         persos[i]->PVmax = stats[i][0];
         persos[i]->PV = persos[i]->PVmax;
         persos[i]->att = stats[i][1];
         persos[i]->def = stats[i][2];
         persos[i]->agi = stats[i][3];
         persos[i]->vit = stats[i][4];
         persos[i]->special = &special_attacks[i];
         persos[i]->special_cd = 0;
         persos[i]->shield_active = 0;
     }
     return persos;
 }
 
 // Libération des personnages
 void liberer_persos(Perso** persos) {
     for (int i = 0; i < MAX_PERSOS; i++) {
         free(persos[i]->nom);
         free(persos[i]);
     }
     free(persos);
 }
 
 void afficher_menu_personnages(Perso** persos) {
     clear_terminal();
     printf("\n=== CHOIX DES PERSONNAGES ===\n");
     for (int i = 0; i < MAX_PERSOS; i++) {
         printf("%2d) %s  PV:%d ATK:%d DEF:%d VIT:%d\n",
                i+1,
                persos[i]->nom,
                persos[i]->PVmax,
                persos[i]->att,
                persos[i]->def,
                persos[i]->vit);
     }
     printf("============================\n");
 }
 
 // Tentative d'esquive
 int tenter_esquive(Perso* def) {
     double chance_esquive = (def->vit + def->agi) / 200.0;
     double tirage = (double)rand() / RAND_MAX;
     if (tirage < chance_esquive) {
         printf(BOLD GREEN "%s esquive l'attaque avec succès!\n" RESET, def->nom);
         return 1; // Esquive réussie
     }
     return 0; // Esquive échouée
 }
 
 // Calcul des dégâts
 int calculer_degats(Perso* atk, Perso* def) {
     int base = atk->att * 2 - def->def;
     if (base < 1) base = 1;
     if (def->shield_active) base /= 2;
     int var = base / 10;
     int rnd = (rand() % (2*var+1)) - var;
     return base + rnd;
 }