#ifndef PERSONNAGES_H
#define PERSONNAGES_H

#define MAX_PERSOS 10

// Structure d'une attaque spéciale
typedef struct SpecialAttack {
    char* nom;
    int cooldown_max;
    char* description;
    int effets; // 0 = dégâts, 1 = bouclier
} SpecialAttack;

// Structure d'un personnage
typedef struct Perso {
    char* nom;
    int PV;
    int PVmax;
    int att;
    int def;
    int agi;
    int vit;
    SpecialAttack* special;
    int special_cd;
    int shield_active;
    int jauge; // Field to track the attack gauge
} Perso;

// Prototypes des fonctions
Perso** creer_persos_disponibles();
void liberer_persos(Perso** persos);
void afficher_menu_personnages(Perso** persos);
int tenter_esquive(Perso* def);
int calculer_degats(Perso* atk, Perso* def);


#endif /* PERSONNAGES_H */