#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "combat.h"
#include "affichage.h"
#include "perso.h" // Use centralized definition of Perso and SpecialAttack
#include "equipe.h" // Use centralized definition of Equipe
#include "utilis.h" // Include the header for lire_entree_valide

// Fonction pour appliquer une action
void appliquer_action(Perso* atk, Perso* def, int action) {
    atk->shield_active = 0;
    switch(action) {
        case 1: {
            animation_combat_epique(atk->nom, def->nom); // Utilisation de l'animation épique
            if (tenter_esquive(def)) {
                printf("%s évite complètement l'attaque!\n", def->nom);
                break;
            }
            int dmg = calculer_degats(atk, def);
            def->PV -= dmg;
            if (def->PV < 0) def->PV = 0;
            printf("%s inflige %d dégâts à %s\n", atk->nom, dmg, def->nom);
            break;
        }
        case 2:
            printf("%s active un bouclier\n", atk->nom);
            atk->shield_active = 1;
            break;
        case 3:
            if (atk->special_cd > 0) {
                printf("Spécial indisponible (cd:%d)\n", atk->special_cd);
                return;
            }
            animation_combat_epique(atk->nom, def->nom); // Utilisation de l'animation épique
            printf("%s utilise %s\n", atk->nom, atk->special->nom);
            if (atk->special->effets == 1) {
                atk->shield_active = 1;
            } else {
                int dmg2 = calculer_degats(atk, def) * 2;
                def->PV -= dmg2;
                if (def->PV < 0) def->PV = 0;
                printf("Spécial inflige %d dégâts à %s\n", dmg2, def->nom);
            }
            atk->special_cd = atk->special->cooldown_max;
            break;
        default:
            break;
    }
}

// Add null pointer checks and replace recursion with a loop in choisir_action_joueur
int choisir_action_joueur(Perso* p) {
    int action;
    do {
        printf("Actions : 1.Attaque  2.Shield  3.Special (cd:%d) > ", p->special_cd);
        action = lire_entree_valide(1, 3);
        if (action == 3 && p->special_cd > 0) {
            printf("Special pas dispo\n");
        } else {
            break;
        }
    } while (1);
    return action;
}

// Fonction pour choisir une action IA
int choisir_action_ia(Perso* p) {
    if (p->special_cd == 0 && rand() % 3 == 0) return 3;
    if (rand() % 5 == 0) return 2;
    return 1;
}

// Add bounds and null pointer checks in tour_de_combat
void tour_de_combat(Equipe* j, Equipe* ia) {
    clear_terminal(); // Efface le terminal avant de commencer un tour de combat
    int idx_j = 0, idx_ia = 0;
    while (idx_j < j->taille && idx_ia < ia->taille) {
        if (!j->membres[idx_j] || !ia->membres[idx_ia]) {
            printf("Erreur: Membre invalide dans l'équipe.\n");
            break;
        }

        Perso* pj = j->membres[idx_j];
        Perso* pi = ia->membres[idx_ia];
        printf("\n[Tour] " BOLD GREEN "%s" RESET " PV: ", pj->nom);
        afficher_barre_sante(pj->PV, pj->PVmax);
        printf(BOLD RED "%s" RESET " PV: ", pi->nom);
        afficher_barre_sante(pi->PV, pi->PVmax);

        pj->shield_active = pi->shield_active = 0;
        int act_j = choisir_action_joueur(pj);
        appliquer_action(pj, pi, act_j);
        if (pi->PV == 0) {
            printf(BOLD RED "%s KO!\n" RESET, pi->nom);
            idx_ia++;
            if (idx_ia >= ia->taille) break;
            continue;
        }

        int act_i = choisir_action_ia(pi);
        appliquer_action(pi, pj, act_i);
        if (pj->PV == 0) {
            printf(BOLD RED "%s KO!\n" RESET, pj->nom);
            idx_j++;
            if (idx_j >= j->taille) break;
        }

        if (pj->special_cd > 0) pj->special_cd--;
        if (pi->special_cd > 0) pi->special_cd--;
    }
    if (idx_j < j->taille) printf("\n" BOLD GREEN "Victoire Joueur!\n" RESET);
    else printf("\n" BOLD RED "Victoire IA!\n" RESET);
}