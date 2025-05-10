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
// Modification de la fonction appliquer_action pour ajuster la gestion des jauges
int appliquer_action(Perso* atk, Perso* def, int action) {
    if (def->PV <= 0) {
        printf(BOLD RED "%s est déjà KO et ne peut pas être attaqué. Choisissez une autre cible.\n" RESET, def->nom);
        return -1;
    }

    atk->shield_active = 0; // Réinitialise l'état du bouclier de l'attaquant avant toute action
    
    // Récupérer un pointeur vers l'équipe du personnage actif et l'équipe adverse
    Equipe *equipe_atk = NULL;
    // Note: Dans une implémentation complète, il faudrait trouver les équipes correspondantes
    
    switch(action) {
        case 1: { // Cas où le personnage effectue une attaque normale
            animation_combat_epique(atk->nom, def->nom); // Affiche une animation
            
            // Ajustement de la jauge pour l'attaque normale
            atk->jauge -= 10; // Réduit de 10 points au lieu de vider complètement
            
            if (!tenter_esquive(def)) {
                int dmg = calculer_degats(atk, def);
                def->PV -= dmg;
                if (def->PV < 0) def->PV = 0;
                printf("%s inflige %d dégâts à %s\n", atk->nom, dmg, def->nom);
                
                // Ajustement des jauges après l'attaque
                def->jauge += 10; // Le défenseur gagne de la jauge quand il est attaqué
                
                // Si l'équipe de l'attaquant est connue, augmenter la jauge des coéquipiers
                if (equipe_atk != NULL) {
                    for (int i = 0; i < equipe_atk->taille; i++) {
                        if (equipe_atk->membres[i] != atk && equipe_atk->membres[i]->PV > 0) {
                            equipe_atk->membres[i]->jauge += 5; // Les coéquipiers gagnent de la jauge
                            if (equipe_atk->membres[i]->jauge > 50) equipe_atk->membres[i]->jauge = 50;
                        }
                    }
                }
            } else {
                printf("%s esquive l'attaque!\n", def->nom);
                // Si esquive, l'attaquant récupère un peu de jauge
                atk->jauge += 5;
            }
            break;
        }
        case 2: // Cas où le personnage active un bouclier
            printf("%s active un bouclier\n", atk->nom);
            atk->shield_active = 1; // Active le bouclier pour réduire les dégâts des prochaines attaques
            
            // Gain de jauge pour l'activation du bouclier
            atk->jauge += 5; // Légère augmentation de jauge
            break;
            
        case 3: // Cas où le personnage utilise une attaque spéciale
            if (atk->special_cd > 0) { // Vérifie si l'attaque spéciale est en cooldown
                printf("Spécial indisponible (cd:%d)\n", atk->special_cd);
                return 0; // Si l'attaque spéciale n'est pas disponible, on sort de la fonction
            }
            
            // Ajustement de la jauge pour l'attaque spéciale
            atk->jauge -= 20; // Coût plus élevé mais pas vidage complet
            
            animation_combat_epique(atk->nom, def->nom); // Affiche une animation pour l'attaque spéciale
            printf("%s utilise %s\n", atk->nom, atk->special->nom);
            
            if (atk->special->effets == 1) { // Si l'effet spécial est un bouclier
                atk->shield_active = 1; // Active un bouclier spécial
            } else { // Sinon, l'effet spécial inflige des dégâts doublés
                int dmg2 = calculer_degats(atk, def) * 2;
                def->PV -= dmg2;
                if (def->PV < 0) def->PV = 0; // Empêche les PV d'être négatifs
                printf("Spécial inflige %d dégâts à %s\n", dmg2, def->nom);
                
                // Ajustement des jauges après l'attaque spéciale
                def->jauge += 15; // Le défenseur gagne plus de jauge quand il est touché par un spécial
                
                // Si l'équipe de l'attaquant est connue, augmenter la jauge des coéquipiers
                if (equipe_atk != NULL) {
                    for (int i = 0; i < equipe_atk->taille; i++) {
                        if (equipe_atk->membres[i] != atk && equipe_atk->membres[i]->PV > 0) {
                            equipe_atk->membres[i]->jauge += 10; // Les coéquipiers gagnent plus de jauge pour un spécial
                            if (equipe_atk->membres[i]->jauge > 50) equipe_atk->membres[i]->jauge = 50;
                        }
                    }
                }
            }
            atk->special_cd = atk->special->cooldown_max; // Réinitialise le cooldown de l'attaque spéciale
            break;
            
        default:
            break; // Si l'action n'est pas reconnue, ne fait rien
    }
    
    // Limiter la jauge entre 0 et 50
    if (atk->jauge < 0) atk->jauge = 0;
    if (atk->jauge > 50) atk->jauge = 50;
    
    if (def->jauge < 0) def->jauge = 0;
    if (def->jauge > 50) def->jauge = 50;

    return 1; 
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

// Nouvelle fonction pour choisir une cible dans l'équipe adverse
int choisir_cible(Equipe* equipe) {
    printf("\nChoisissez une cible :\n");
    for (int i = 0; i < equipe->taille; i++) {
        if (equipe->membres[i]->PV > 0) { // Affiche uniquement les membres encore en vie
            printf("%d) %s (PV: %d/%d)\n", i + 1, equipe->membres[i]->nom, equipe->membres[i]->PV, equipe->membres[i]->PVmax);
        }
    }
    return lire_entree_valide(1, equipe->taille) - 1; // Retourne l'index de la cible
}

// Nouvelle fonction pour afficher la jauge d'attaque
void afficher_jauge_attaque(int jauge, int max_jauge) {
    int largeur = 10; // Largeur de la jauge
    int remplissage = (jauge * largeur) / max_jauge; // Calcul proportionnel
    printf("[");
    for (int i = 0; i < largeur; i++) {
        if (i < remplissage) printf("🟨"); // Carrés jaunes pour la jauge
        else printf("⬜"); // Carrés vides
    }
    printf("] %d/%d\n", jauge, max_jauge); // Affiche aussi les valeurs numériques
}

// Modification de la fonction afficher_persos_et_pv pour inclure la jauge d'attaque
void afficher_persos_et_pv(Equipe* joueur, Equipe* ia) {
    printf("\n" BOLD GREEN "=== Équipe Joueur ===\n" RESET);
    for (int i = 0; i < joueur->taille; i++) {
        Perso* p = joueur->membres[i];
        printf("%s (PV: ", p->nom);
        afficher_barre_sante(p->PV, p->PVmax);
        printf("Jauge: ");
        afficher_jauge_attaque(p->jauge, 50); // Jauge max fixée à 50
    }

    printf("\n" BOLD RED "=== Équipe IA ===\n" RESET);
    for (int i = 0; i < ia->taille; i++) {
        Perso* p = ia->membres[i];
        printf("%s (PV: ", p->nom);
        afficher_barre_sante(p->PV, p->PVmax);
        printf("Jauge: ");
        afficher_jauge_attaque(p->jauge, 50); // Jauge max fixée à 50
    }
}

// Nouvelle fonction pour gérer la progression des jauges
void gerer_jauges(Equipe* equipe) {
    for (int i = 0; i < equipe->taille; i++) {
        Perso* p = equipe->membres[i];
        if (p->PV > 0) { // Seulement pour les personnages vivants
            p->jauge += 1; // Progression constante minimale
            
            // Bonus de progression basé sur la vitesse (mais moins impactant)
            int bonus = p->vit / 25;  // Valeur réduite pour un impact moindre
            p->jauge += bonus;
            
            if (p->jauge > 50) p->jauge = 50; // Limite à 50
        }
    }
}

// Modification de la fonction tour_de_combat dans combat.c
// Voici la section à modifier:

void tour_de_combat(Equipe* j, Equipe* ia) {
    while (1) {
        // Efface le terminal pour un affichage propre
        system("clear");

        // Affiche l'état actuel des deux équipes
        afficher_persos_et_pv(j, ia);

        // Gère les jauges pour les deux équipes
        gerer_jauges(j);
        gerer_jauges(ia);

        // Détermine le personnage actif avec la jauge la plus élevée
        Perso* actif = NULL;
        Equipe* equipe_actif = NULL;
        Equipe* equipe_adverse = NULL;

        for (int i = 0; i < j->taille; i++) {
            if (j->membres[i]->jauge >= 50 && j->membres[i]->PV > 0) {
                actif = j->membres[i];
                equipe_actif = j;
                equipe_adverse = ia;
                break;
            }
        }
        if (!actif) {
            for (int i = 0; i < ia->taille; i++) {
                if (ia->membres[i]->jauge >= 50 && ia->membres[i]->PV > 0) {
                    actif = ia->membres[i];
                    equipe_actif = ia;
                    equipe_adverse = j;
                    break;
                }
            }
        }

        if (!actif) continue;

        // Ceci permet de conserver une partie de la jauge après l'action
        actif->jauge = 25;  // Modifié: au lieu de mettre à 0, on conserve une partie de la jauge

        // Pour les attaques et attaques spéciales, la jauge sera ajustée dans appliquer_action
        // Cette valeur de 25 est la base à laquelle on retire les coûts des actions

        if (equipe_actif == j) {
            printf("\nC'est le tour de %s\n", actif->nom);
            int action = choisir_action_joueur(actif);
            int cible_idx, resultat_action;
            Perso* cible;
            
            do {
                cible_idx = choisir_cible(equipe_adverse);
                cible = equipe_adverse->membres[cible_idx];
                
                // Appliquer l'action et vérifier le résultat
                resultat_action = appliquer_action(actif, cible, action);
                
                // Si la cible est morte, proposer de rechoisir
                if (resultat_action == -1) {
                    printf("Veuillez choisir une autre cible.\n");
                    sleep(1); // Petit délai pour lire le message
                }
            } while (resultat_action == -1); // Recommencer si la cible n'est pas valide
        } else {
            printf("\nC'est le tour de l'IA (%s)\n", actif->nom);
            int action = choisir_action_ia(actif);
            
            // Trouver une cible valide (avec PV > 0)
            Perso* cible = NULL;
            int resultat_action;
            
            do {
                // Chercher une cible valide parmi les joueurs encore en vie
                cible = NULL;
                for (int i = 0; i < j->taille; i++) {
                    if (j->membres[i]->PV > 0) {
                        cible = j->membres[i];
                        break;
                    }
                }
                
                // Si aucune cible valide n'est trouvée, c'est que tous les joueurs sont KO
                if (cible == NULL) {
                    printf("L'IA n'a plus de cible valide!\n");
                    break;
                }
                
                // Essayer d'appliquer l'action
                resultat_action = appliquer_action(actif, cible, action);
            } while (resultat_action == -1); // Recommencer si la cible n'est pas valide
        }

        int joueur_ko = 1, ia_ko = 1;
        for (int i = 0; i < j->taille; i++) if (j->membres[i]->PV > 0) joueur_ko = 0;
        for (int i = 0; i < ia->taille; i++) if (ia->membres[i]->PV > 0) ia_ko = 0;

        if (joueur_ko || ia_ko) {
            if (joueur_ko) printf("\nVictoire de l'IA!\n");
            if (ia_ko) printf("\nVictoire du joueur!\n");
            break;
        }
        
        // Petit délai pour voir ce qui se passe
        sleep(1);
    }
}