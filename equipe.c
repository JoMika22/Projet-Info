
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "equipe.h"
 #include "perso.h"
 #include "utilis.h"
 
 // Création d'une équipe joueur
 Equipe* creer_equipe_joueur(Perso** persos) {
     Equipe* e = malloc(sizeof(Equipe));
     if (!e) {
         perror("malloc equipe");
         exit(1);
     }
     printf("Nom de l'équipe : ");
     fgets(e->nom_equipe, MAX_TEAM_NAME, stdin);
     e->nom_equipe[strcspn(e->nom_equipe, "\n")] = 0;
     e->taille = 2;
     e->membres = malloc(sizeof(Perso*) * 2);
     if (!e->membres) {
         perror("malloc membres equipe");
         exit(1);
     }
     
     afficher_menu_personnages(persos);
     // Ensure unique `Perso` instances for each team member
     for (int i = 0; i < 2; i++) {
         printf("Sélection perso %d [1-%d] : ", i+1, MAX_PERSOS);
         int choix = lire_entree_valide(1, MAX_PERSOS);
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
 
 // Création d'une équipe IA
 Equipe* creer_equipe_IA(Perso** persos, int diff) {
     Equipe* e = malloc(sizeof(Equipe));
     if (!e) {
         perror("malloc equipe IA");
         exit(1);
     }
     
     strncpy(e->nom_equipe, "IA", MAX_TEAM_NAME-1);
     e->nom_equipe[MAX_TEAM_NAME-1] = '\0';
     e->taille = 2;
     e->membres = malloc(sizeof(Perso*) * 2);
     if (!e->membres) {
         perror("malloc membres equipe IA");
         free(e);
         exit(1);
     }
     
     int used[MAX_PERSOS] = {0};
     for (int i = 0; i < 2; i++) {
         int r;
         do { r = rand() % MAX_PERSOS; } while (used[r]);
         used[r] = 1;
         
         // Boost stats selon la difficulté
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
 
 // Affichage d'une équipe
 void afficher_equipe(Equipe* e) {
     printf("\n-- Équipe %s --\n", e->nom_equipe);
     for (int i = 0; i < e->taille; i++) {
         Perso* p = e->membres[i];
         printf("%s (PV:%d/%d ATK:%d DEF:%d VIT:%d)\n",
                p->nom, p->PV, p->PVmax, p->att, p->def, p->vit);
     }
 }
 
 // Libération d'une équipe
 void liberer_equipe(Equipe* e) {
     if (!e) return; // Safeguard against null pointer

     for (int i = 0; i < e->taille; i++) {
         if (e->membres[i]) {
             if (e->membres[i]->nom) {
                 printf("Freeing team member name: %p\n", (void*)e->membres[i]->nom);
                 free(e->membres[i]->nom);
                 e->membres[i]->nom = NULL;
             }
             printf("Freeing team member: %p\n", (void*)e->membres[i]);
             free(e->membres[i]);
             e->membres[i] = NULL;
         }
     }

     if (e->membres) {
         printf("Freeing team members array: %p\n", (void*)e->membres);
         free(e->membres);
         e->membres = NULL;
     }

     printf("Freeing team structure: %p\n", (void*)e);
     free(e);
     e = NULL;
 }