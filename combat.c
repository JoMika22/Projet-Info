#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "combat.h"
#include "affichage.h"
#include "perso.h" 
#include "equipe.h"
#include "utilis.h" 
// Cette fonction gère les différentes actions qu'un personnage peut effectuer pendant un combat.
// Les actions possibles sont : attaquer, activer un bouclier ou utiliser une attaque spéciale.
void appliquer_action(Perso* atk, Perso* def, int action) {
    atk->shield_active = 0; // Réinitialise l'état du bouclier de l'attaquant avant toute action
    switch(action) {
        case 1: { // Cas où le personnage effectue une attaque normale
            animation_combat_epique(atk->nom, def->nom); // Affiche une animation
            if (tenter_esquive(def)) { // Vérifie si le défenseur esquive l'attaque
                printf("%s évite complètement l'attaque!\n", def->nom);
                break; // Si l'esquive réussit, l'attaque est annulée
            }
            int dmg = calculer_degats(atk, def); // Calcule les dégâts infligés par l'attaquant
            def->PV -= dmg; // Réduit les points de vie du défenseur
            if (def->PV < 0) def->PV = 0; // Empêche les PV d'être négatifs
            printf("%s inflige %d dégâts à %s\n", atk->nom, dmg, def->nom);
            break;
        }
        case 2: // Cas où le personnage active un bouclier
            printf("%s active un bouclier\n", atk->nom);
            atk->shield_active = 1; // Active le bouclier pour réduire les dégâts des prochaines attaques
            break;
        case 3: // Cas où le personnage utilise une attaque spéciale
            if (atk->special_cd > 0) { // Vérifie si l'attaque spéciale est en cooldown
                printf("Spécial indisponible (cd:%d)\n", atk->special_cd);
                return; // Si l'attaque spéciale n'est pas disponible, on sort de la fonction
            }
            animation_combat_epique(atk->nom, def->nom); // Affiche une animation pour l'attaque spéciale
            printf("%s utilise %s\n", atk->nom, atk->special->nom);
            if (atk->special->effets == 1) { // Si l'effet spécial est un bouclier
                atk->shield_active = 1; // Active un bouclier spécial
            } else { // Sinon, l'effet spécial inflige des dégâts doublés
                int dmg2 = calculer_degats(atk, def) * 2;
                def->PV -= dmg2;
                if (def->PV < 0) def->PV = 0; // Empêche les PV d'être négatifs
                printf("Spécial inflige %d dégâts à %s\n", dmg2, def->nom);
            }
            atk->special_cd = atk->special->cooldown_max; // Réinitialise le cooldown de l'attaque spéciale
            break;
        default:
            break; // Si l'action n'est pas reconnue, ne fait rien
    }
}

// Fonction pour choisir une action pour le joueur
// Cette fonction affiche les options disponibles et s'assure que le joueur choisit une action valide.
int choisir_action_joueur(Perso* p) {
    int action;
    do {
        printf("Actions : 1.Attaque  2.Shield  3.Special (cd:%d) > ", p->special_cd);
        action = lire_entree_valide(1, 3); // Lit une entrée valide entre 1 et 3
        if (action == 3 && p->special_cd > 0) { // Vérifie si l'attaque spéciale est en cooldown
            printf("Special pas dispo\n");
        } else {
            break; // Si l'action est valide, sort de la boucle
        }
    } while (1);
    return action; // Retourne l'action choisie
}

// Fonction pour choisir une action pour l'IA
// Cette fonction utilise des probabilités pour rendre les choix de l'IA moins prévisibles.
int choisir_action_ia(Perso* p) {
    if (p->special_cd == 0 && rand() % 3 == 0) return 3; // Utilise l'attaque spéciale avec une probabilité de 1/3
    if (rand() % 5 == 0) return 2; // Active un bouclier avec une probabilité de 1/5
    return 1; // Sinon, effectue une attaque normale
}

// Fonction pour gérer un tour de combat
// Cette fonction alterne les actions entre le joueur et l'IA jusqu'à ce qu'un des deux perde tous ses membres.
void tour_de_combat(Equipe* j, Equipe* ia) {
    clear_terminal(); // Efface le terminal pour une meilleure lisibilité
    int idx_j = 0, idx_ia = 0; // Indices pour suivre les membres actifs des équipes
    while (idx_j < j->taille && idx_ia < ia->taille) {
        if (!j->membres[idx_j] || !ia->membres[idx_ia]) { // Vérifie que les membres des équipes sont valides
            printf("Erreur: Membre invalide dans l'équipe.\n");
            break; // Si un membre est invalide, arrête le combat
        }

        Perso* pj = j->membres[idx_j]; // Récupère le membre actif de l'équipe du joueur
        Perso* pi = ia->membres[idx_ia]; // Récupère le membre actif de l'équipe de l'IA
        printf("\n[Tour] " BOLD GREEN "%s" RESET " PV: ", pj->nom);
        afficher_barre_sante(pj->PV, pj->PVmax); // Affiche la barre de santé du joueur
        printf(BOLD RED "%s" RESET " PV: ", pi->nom);
        afficher_barre_sante(pi->PV, pi->PVmax); // Affiche la barre de santé de l'IA

        pj->shield_active = pi->shield_active = 0; // Réinitialise les boucliers des deux personnages
        int act_j = choisir_action_joueur(pj); // Le joueur choisit une action
        appliquer_action(pj, pi, act_j); // Applique l'action du joueur
        if (pi->PV == 0) { // Vérifie si le personnage de l'IA est KO
            printf(BOLD RED "%s KO!\n" RESET, pi->nom);
            idx_ia++; // Passe au membre suivant de l'équipe de l'IA
            if (idx_ia >= ia->taille) break; // Si tous les membres de l'IA sont KO, le joueur gagne
            continue;
        }

        int act_i = choisir_action_ia(pi); // L'IA choisit une action
        appliquer_action(pi, pj, act_i); // Applique l'action de l'IA
        if (pj->PV == 0) { // Vérifie si le personnage du joueur est KO
            printf(BOLD RED "%s KO!\n" RESET, pj->nom);
            idx_j++; // Passe au membre suivant de l'équipe du joueur
            if (idx_j >= j->taille) break; // Si tous les membres du joueur sont KO, l'IA gagne
        }

        if (pj->special_cd > 0) pj->special_cd--; // Réduit le cooldown de l'attaque spéciale du joueur
        if (pi->special_cd > 0) pi->special_cd--; // Réduit le cooldown de l'attaque spéciale de l'IA
    }
    if (idx_j < j->taille) printf("\n" BOLD GREEN "Victoire Joueur!\n" RESET); // Le joueur gagne
    else printf("\n" BOLD RED "Victoire IA!\n" RESET); // L'IA gagne
}