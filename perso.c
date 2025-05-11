#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "perso.h"
#include "utilis.h"

// Tableau des attaques spéciales pour chaque personnage
SpecialAttack special_attacks[MAX_PERSOS] = {
    {"Éclair Foudroyant", 2, 5, "Décharge un éclair électrique puissant", 0},    // Pikachu: dégâts moyens, CD court
    {"Gum Gum Pistol", 3, 6, "Attaque élastique puissante", 0},                  // Luffy: attaque forte, CD standard
    {"Coup de Bicycle", 2, 4, "Frappe acrobatique précise", 0},                  // Salah: plus faible mais recharge rapide
    {"Super Splash", 4, 6, "Attaque aquatique massive", 0},                      // El Primo: puissant mais long CD
    {"Puissance Ogre", 3, 5, "Déchaîne la force primitive", 0},                  // Shrek: équilibré
    {"Champions League Strike", 2, 4, "Frappe précise enroulée", 0},             // CR7: rapide mais moins puissant
    {"Santoryu Ougi", 3, 6, "Triple attaque éclair", 0},                         // Zoro: puissant, CD normal
    {"Bubble Beam", 2, 4, "Canon à bulles paralysant", 0},                       // Bob: faible mais rapide
    {"Kamehameha", 4, 6, "Attaque énergétique ", 0},                       // Goku: très fort mais long CD
    {"Muffin Assault", 1, 4, "Muffins empoisonnés ", 0}  // Muffin Man: faible mais très rapide
};


Perso** creer_persos_disponibles() {
    // Allocation du tableau de pointeurs vers les personnages
    Perso** persos = malloc(sizeof(Perso*) * MAX_PERSOS);
    if (!persos) {
        perror("malloc persos"); // Super important de vérifier si le malloc a fonctionné
        exit(1);                 // Sinon on quitte 
    }

    const char* noms[MAX_PERSOS] = {"Pikachu", "Luffy", "Salah", "El Primo", "Shrek", "CR7", "Zoro", "Bob l'éponge", "Goku", "Muffin Man"};
    
    // Les stats pour chaque personnage [PV, att, def, agi, vit]
    // On a équilibré pour que chaque perso ait exactement 200 points au total
    int stats[MAX_PERSOS][5] = {
        {40, 30, 30, 50, 50}, // Pikachu - rapide et agile mais moins robuste
        {50, 50, 40, 30, 30}, // Luffy - fort et résistant mais moins mobile
        {40, 30, 30, 50, 50}, // Salah - agile et rapide comme une flèche
        {60, 50, 40, 25, 25}, // El Primo - tank avec PV élevés et forte attaque
        {60, 30, 50, 30, 30}, // Shrek - robuste avec bonne défense
        {40, 30, 30, 50, 50}, // CR7 - agile et véloce comme un vrai footballeur
        {45, 50, 40, 35, 30}, // Zoro - attaquant puissant et équilibré
        {35, 35, 35, 45, 50}, // Bob l'éponge - rapide et polyvalent
        {50, 50, 30, 35, 35}, // Goku - fort avec bonne vitesse
        {40, 35, 35, 40, 50}  // Muffin Man - rapide et surprenant
    };
    
    // Création et initialisation de chaque personnage
    for (int i = 0; i < MAX_PERSOS; i++) {
        persos[i] = malloc(sizeof(Perso));
        persos[i]->nom = strdup(noms[i]);  // strdup fait un malloc+strcpy, pratique!
        persos[i]->PVmax = stats[i][0];
        persos[i]->PV = persos[i]->PVmax;  // Au début, PV = PVmax logiquement
        persos[i]->att = stats[i][1];
        persos[i]->def = stats[i][2];
        persos[i]->agi = stats[i][3];
        persos[i]->vit = stats[i][4];
        persos[i]->special = &special_attacks[i];  // Association de l'attaque spéciale
        persos[i]->special_cd = 0;                 // Cooldown initial à 0
        persos[i]->shield_active = 0;              // Pas de bouclier au début
        
        // Initialisation de la jauge avec une valeur aléatoire entre 40 et 50
        persos[i]->jauge = 40 + (rand() % 11);  // Entre 40 et 50 inclus
    }
    
    return persos;  // On retourne le tableau de pointeurs
}

/**
 * Libère la mémoire allouée pour tous les personnages
 * Super important d'éviter les fuites mémoire
 */
void liberer_persos(Perso** persos) {
    for (int i = 0; i < MAX_PERSOS; i++) {
        free(persos[i]->nom);  // D'abord libérer le nom (alloué avec strdup)
        free(persos[i]);       // Puis le personnage lui-même
    }
    free(persos);  // Et enfin le tableau de pointeurs
}

/**
 * Affiche le menu de sélection des personnages
 * Interface utilisateur propre et claire, c'est agréable!
 */
void afficher_menu_personnages(Perso** persos) {
    clear_terminal();  // On efface l'écran pour une interface propre
    printf("\n%s=== CHOIX DES PERSONNAGES ===%s\n", BOLD CYAN, RESET);
    
    // Affichage de tous les personnages avec leurs stats principales
    for (int i = 0; i < MAX_PERSOS; i++) {
        printf("%s%2d)%s %s%-12s%s PV:%s%3d%s ATK:%s%2d%s DEF:%s%2d%s AGI:%s%2d%s VIT:%s%2d%s %s\n",
            BOLD, i+1, RESET,
            BOLD YELLOW, persos[i]->nom, RESET,
            GREEN, persos[i]->PVmax, RESET,
            RED, persos[i]->att, RESET,
            BLUE, persos[i]->def, RESET,
            CYAN, persos[i]->agi, RESET,
            MAGENTA, persos[i]->vit, RESET,
            persos[i]->special->description);
    }
    
    printf("%s=============================%s\n", BOLD CYAN, RESET);
}

/**
 * Détermine si un personnage esquive une attaque
 * Le système d'esquive ajoute un élément de hasard, j'adore ça!
 */
int tenter_esquive(Perso* def) {
    // La chance d'esquive dépend de l'agilité et de la vitesse
    // J'ai ajusté la formule pour que ce soit environ 15-40% selon les stats
    double chance_esquive = (def->vit + def->agi) / 250.0;
    
    // Tirage aléatoire entre 0 et 1
    double tirage = (double)rand() / RAND_MAX;
    
    // Si le tirage est inférieur à la chance, c'est réussi!
    if (tirage < chance_esquive) {
        printf(BOLD GREEN "%s esquive l'attaque !\n" RESET, def->nom);
        return 1; // Esquive réussie
    }
    
    return 0; // Esquive échouée
}


int calculer_degats(Perso* atk, Perso* def) {
    // Formule de base: attaque - défense/2 avec un minimum de 1
    int base = atk->att - (def->def / 2);
    
    // S'assurer qu'il y a toujours au moins 2 dégâts de base
    if (base < 2) base = 2;
    
    // Si le bouclier est actif, on divise les dégâts par 2
    if (def->shield_active) {
        base /= 2;
        printf("%s bloque partiellement l'attaque avec son bouclier!\n" RESET, def->nom);
    }
    
    // On ajoute une variation aléatoire pour plus de fun
    // Entre -15% et +15% de variation
    int var = base / 6;  // Augmenté pour plus de variation
    if (var < 1) var = 1;  // Au moins 1 point de variation
    int rnd = (rand() % (2*var+1)) - var;
    
    int degats = base + rnd;
    
    // S'assurer que les dégâts sont au moins de 1
    if (degats < 1) degats = 1;
    
    return degats;
}
