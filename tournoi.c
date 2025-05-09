#include "tournoi.h"
#include "affichage.h"
#include "combat.h"
#include "utilis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Mode tournoi principal
void mode_tournoi(Perso** tab) {
    clear_terminal();
    printf(BOLD YELLOW "\n=== MODE TOURNOI ===\n" RESET);
    
    // Affichage et selection du personnage
    afficher_menu_personnages(tab);
    printf(BOLD CYAN "\nChoisis ton personnage [1-%d] : " RESET, MAX_PERSOS);
    int choix = lire_entree_valide(1, MAX_PERSOS);
    
    // Copie du personnage joueur
    Perso* perso_joueur = malloc(sizeof(Perso));
    if (!perso_joueur) { 
        perror("malloc perso joueur"); 
        exit(1); 
    }
    Perso* base = tab[choix-1];
    perso_joueur->nom = strdup(base->nom);
    perso_joueur->PVmax = base->PVmax;
    perso_joueur->PV = base->PVmax;
    perso_joueur->att = base->att;
    perso_joueur->def = base->def;
    perso_joueur->agi = base->agi;
    perso_joueur->vit = base->vit;
    perso_joueur->special = base->special;
    perso_joueur->special_cd = 0;
    perso_joueur->shield_active = 0;
    
    // Creation et execution du tournoi
    Tournoi* tournoi = creer_tournoi(tab, perso_joueur);
    executer_tournoi_poule(tournoi);

    // Nettoyage
    free(perso_joueur->nom);
    free(perso_joueur);
}

// Creation du tournoi
Tournoi* creer_tournoi(Perso** tab, Perso* joueur) {
    Tournoi* t = malloc(sizeof(Tournoi));
    if (!t) { perror("malloc tournoi"); exit(1); }
    strcpy(t->nom, "Tournoi des Champions");
    t->nb_participants = 4;  // Fixé à 4 participants
    t->format = 1; // poule uniquement
    t->participants = malloc(sizeof(Perso*) * t->nb_participants);
    t->points       = calloc(t->nb_participants, sizeof(int));
    t->victoires    = calloc(t->nb_participants, sizeof(int));
    t->defaites     = calloc(t->nb_participants, sizeof(int));
    t->matchs_joues = calloc(t->nb_participants, sizeof(int));
    if (!t->participants || !t->points || !t->victoires || !t->defaites || !t->matchs_joues) {
        perror("malloc participants/stats"); 
        exit(1);
    }
    // Joueur humain en index 0
    t->participants[0] = joueur;
    // Tirage IA
    int perso_utilise[MAX_PERSOS] = {0};
    for (int i = 0; i < MAX_PERSOS; i++) {
        if (strcmp(tab[i]->nom, joueur->nom) == 0) { perso_utilise[i] = 1; break; }
    }
    for (int i = 1; i < t->nb_participants; i++) {
        int r;
        do { r = rand() % MAX_PERSOS; } while (perso_utilise[r]);
        perso_utilise[r] = 1;
        Perso* src = tab[r];
        Perso* p = malloc(sizeof(Perso));
        if (!p) { 
            perror("malloc perso tournoi"); 
            exit(1); }
        p->nom = strdup(src->nom);
        p->PVmax = src->PVmax;
        p->PV    = src->PVmax;
        p->att   = src->att;
        p->def   = src->def;
        p->agi   = src->agi;
        p->vit   = src->vit;
        p->special = src->special;
        p->special_cd = 0;
        p->shield_active = 0;
        t->participants[i] = p;
    }
    return t;
}

// Affichage du classement
void afficher_classement(Tournoi* t) {
    clear_terminal();
    printf(BOLD YELLOW "\n=== CLASSEMENT DU TOURNOI %s ===\n\n" RESET, t->nom);
    int idx[t->nb_participants];
    for (int i = 0; i < t->nb_participants; i++) idx[i] = i;
    // Tri descriptif
    for (int i = 0; i < t->nb_participants - 1; i++) {
        for (int j = 0; j < t->nb_participants - i - 1; j++) {
            if (t->points[idx[j]] < t->points[idx[j+1]]) {
                int tmp = idx[j]; idx[j] = idx[j+1]; idx[j+1] = tmp;
            }
        }
    }
    // Tableau de classement
    printf("+-----------------+----+-----+-----+-----+\n");
    printf("|     Joueur      | MJ | Vic | Def | Pts |\n");
    printf("+-----------------+----+-----+-----+-----+\n");
    for (int i = 0; i < t->nb_participants; i++) {
        int id = idx[i];
        const char* color = (id == 0) ? BOLD GREEN : "";
        printf("| %s%-15s%s | %2d | %3d | %3d | %3d |\n",
               color, t->participants[id]->nom, RESET,
               t->matchs_joues[id], t->victoires[id], t->defaites[id], t->points[id]);
    }
    printf("+-----------------+----+-----+-----+-----+\n");
}

// Simuler un match p1 vs p2
void simuler_match(Perso* p1, Perso* p2, int* vainqueur, int afficher) {
    // Reset stats
    p1->PV = p1->PVmax; p2->PV = p2->PVmax;
    p1->special_cd = 0; p2->special_cd = 0;
    p1->shield_active = p2->shield_active = 0;

    if (afficher) {
        // Interaction joueur vs IA
        printf(BOLD YELLOW "\n=== MATCH: %s vs %s ===\n" RESET, p1->nom, p2->nom);
        while (p1->PV > 0 && p2->PV > 0) {
            printf("\n[Tour] " BOLD GREEN "%s" RESET " PV: ", p1->nom);
            afficher_barre_sante(p1->PV, p1->PVmax);
            printf(BOLD RED "%s" RESET " PV: ", p2->nom);
            afficher_barre_sante(p2->PV, p2->PVmax);

            p1->shield_active = 0;
            int action_j = choisir_action_joueur(p1);
            appliquer_action(p1, p2, action_j);
            if (p2->PV <= 0) { printf(BOLD GREEN "%s a gagne!\n" RESET, p1->nom); *vainqueur = 0; break; }

            p2->shield_active = 0;
            int action_ia = choisir_action_ia(p2);
            appliquer_action(p2, p1, action_ia);
            if (p1->PV <= 0) { printf(BOLD RED "%s a gagne!\n" RESET, p2->nom); *vainqueur = 1; break; }

            if (p1->special_cd > 0) p1->special_cd--;
            if (p2->special_cd > 0) p2->special_cd--;
        }
    } 
    else {
        // Randomly choose the winner for AI matches
        printf("\nMatch: %s vs %s\n", p1->nom, p2->nom);
        *vainqueur = rand() % 2; // Randomly selects 0 or 1

    }

}

// Organisation des matchs d'une journee (poule fixe 4 joueurs)
void organiser_journee_matchs(Tournoi* t, int journee) {
    int vainqueur;
    switch(journee) {
        case 1:
            simuler_match(t->participants[0], t->participants[1], &vainqueur, 1);
            t->matchs_joues[0]++; t->matchs_joues[1]++;
            if (vainqueur==0) { t->victoires[0]++; t->defaites[1]++; t->points[0]+=3; }
            else            { t->victoires[1]++; t->defaites[0]++; t->points[1]+=3; }

            simuler_match(t->participants[2], t->participants[3], &vainqueur, 0);
            t->matchs_joues[2]++; t->matchs_joues[3]++;
            if (vainqueur==0) { t->victoires[2]++; t->defaites[3]++; t->points[2]+=3; }
            else            { t->victoires[3]++; t->defaites[2]++; t->points[3]+=3; }
            break;
        case 2:
            simuler_match(t->participants[0], t->participants[2], &vainqueur, 1);
            t->matchs_joues[0]++; t->matchs_joues[2]++;
            if (vainqueur==0) { t->victoires[0]++; t->defaites[2]++; t->points[0]+=3; }
            else            { t->victoires[2]++; t->defaites[0]++; t->points[2]+=3; }

            simuler_match(t->participants[1], t->participants[3], &vainqueur, 0);
            t->matchs_joues[1]++; t->matchs_joues[3]++;
            if (vainqueur==0) { t->victoires[1]++; t->defaites[3]++; t->points[1]+=3; }
            else            { t->victoires[3]++; t->defaites[1]++; t->points[3]+=3; }
            break;
        case 3:
            simuler_match(t->participants[0], t->participants[3], &vainqueur, 1);
            t->matchs_joues[0]++; t->matchs_joues[3]++;
            if (vainqueur==0) { t->victoires[0]++; t->defaites[3]++; t->points[0]+=3; }
            else            { t->victoires[3]++; t->defaites[0]++; t->points[3]+=3; }

            simuler_match(t->participants[1], t->participants[2], &vainqueur, 0);
            t->matchs_joues[1]++; t->matchs_joues[2]++;
            if (vainqueur==0) { t->victoires[1]++; t->defaites[2]++; t->points[1]+=3; }
            else            { t->victoires[2]++; t->defaites[1]++; t->points[2]+=3; }
            break;
    }
}

// Execution du tournoi en format poule
void executer_tournoi_poule(Tournoi* t) {
    clear_terminal();
    printf(BOLD YELLOW "\n=== TOURNOI %s (FORMAT POULE) ===\n" RESET, t->nom);
    printf("\nParticipants :\n");
    for (int i = 0; i < t->nb_participants; i++) {
        if (i == 0) {
            printf(BOLD GREEN "%s (TOI)" RESET "\n", t->participants[i]->nom);
        } else {
            printf("%s\n", t->participants[i]->nom);
        }
    }
    printf("\nAppuie sur Entree pour commencer le tournoi..."); 
    getchar();
    afficher_classement(t);
    printf("\nAppuie sur Entree pour commencer les matchs..."); 
    getchar();

    // Déclaration de l'array idx pour stocker les indices des participants
    int idx[t->nb_participants];

    // Initialisation des indices des participants pour le tri
    for (int i = 0; i < t->nb_participants; i++) {
        idx[i] = i; // Chaque participant est initialement à sa position d'origine
    }

    int nb_j = t->nb_participants - 1;
    for (int j=1; j<=nb_j; j++) {
        clear_terminal();
        printf(BOLD YELLOW "\n=== JOURNEE %d ===\n" RESET, j);
        organiser_journee_matchs(t, j);
        afficher_classement(t);
        printf("\nAppuie sur Entree pour continuer..."); getchar();
    }

    clear_terminal(); afficher_classement(t);

    // Tri des participants par points décroissants (tri à bulles)
    for (int i = 0; i < t->nb_participants - 1; i++) {
        for (int j = 0; j < t->nb_participants - i - 1; j++) {
            // Si le participant actuel a moins de points que le suivant, on échange leurs indices
            if (t->points[idx[j]] < t->points[idx[j + 1]]) {
                int tmp = idx[j];
                idx[j] = idx[j + 1];
                idx[j + 1] = tmp;
            }
        }
    }

    // Affichage du podium des trois premiers participants
    printf("\n          %s🏆%s\n", BOLD YELLOW, RESET); // Trophée pour le champion
    printf("       %s%s%s\n", BOLD MAGENTA, t->participants[idx[0]]->nom, RESET); // Nom du champion
    printf("       %s🥇%s\n", BOLD YELLOW, RESET); // Médaille d'or
    printf("    _____________\n    |     |     |\n");
    printf(" %s%s%s |     | %s%s%s\n", BOLD CYAN, t->participants[idx[1]]->nom, RESET, BOLD GREEN, t->participants[idx[2]]->nom, RESET); // Noms des 2ème et 3ème
    printf(" %s🥈%s  |     |  %s🥉%s\n", BOLD CYAN, RESET, BOLD GREEN, RESET); // Médailles d'argent et de bronze
    printf("_____|     |_____\n");

    // Affichage des résultats détaillés pour les trois premiers participants
    printf(BOLD MAGENTA "\nChampion: %s avec %d points!\n" RESET,
           t->participants[idx[0]]->nom, t->points[idx[0]]); // Points du champion
    printf(BOLD CYAN    "2eme: %s avec %d points\n" RESET,
           t->participants[idx[1]]->nom, t->points[idx[1]]); // Points du 2ème
    printf(BOLD GREEN   "3eme: %s avec %d points\n" RESET,
           t->participants[idx[2]]->nom, t->points[idx[2]]); // Points du 3ème

    // Vérification si le joueur est le champion ou affichage de sa position
    if (idx[0] == 0) {
        printf(BOLD YELLOW "\nFelicitations! Tu es champion du tournoi!\n" RESET); // Message de félicitations si le joueur est le champion
    } else {
        int place = 0;
        // Recherche de la position du joueur dans le classement
        for (int i = 0; i < t->nb_participants; i++) {
            if (idx[i] == 0) {
                place = i + 1; // Position du joueur trouvée
                break;
            }
        }
        printf(BOLD CYAN "\nTu as termine a la %deme place.\n" RESET, place); // Affichage de la position du joueur
    }

    // Message pour revenir au menu principal
    printf("\nAppuie sur Entree pour revenir au menu principal..."); getchar();

    // Libération de la mémoire allouée pour le tournoi
    liberer_tournoi(t); // Nettoyage des ressources allouées dynamiquement
    return;
}

// Liberation memoire tournoi
void liberer_tournoi(Tournoi* t) {
    // IA participants sauf index 0
    for (int i=1; i<t->nb_participants; i++) {
        free(t->participants[i]->nom);
        free(t->participants[i]);
    }
    free(t->participants);
    free(t->points);
    free(t->victoires);
    free(t->defaites);
    free(t->matchs_joues);
    free(t);
}