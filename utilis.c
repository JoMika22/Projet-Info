#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include "utilis.h"
 
 // Cette fonction efface l'écran du terminal en utilisant les séquences d'échappement ANSI

void clear_terminal() {
     printf("\033[H\033[J");  
 }
 
 // Affiche une barre de santé graphique
 // C'est génial d'avoir une représentation visuelle des points de vie!
void afficher_barre_sante(int PV, int PVmax) {
    int largeur = 10;  // On pourrait peut-être augmenter ça pour avoir une barre plus détaillée?
    int remplissage = (PV * largeur) / PVmax;  // Calcul proportionnel, bien pensé
    printf("[");
    for (int i = 0; i < largeur; i++) {
        if (i < remplissage) printf("🟩");  // Les carrés verts, c'est trop stylé!
        else printf("⬜");  // J'adore qu'on utilise des emojis dans un programme C
    }
    printf("] %d/%d\n", PV, PVmax);  // Affiche aussi les valeurs numériques, pratique
}
 
 // Cette fonction est vraiment importante pour éviter les bugs d'entrée utilisateur
 // Elle vérifie que l'entrée est bien un nombre dans l'intervalle demandé
int lire_entree_valide(int min, int max) {
    int choix;
    char buffer[100];
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erreur de lecture. Reessayez.\n");
            continue;
        }

        // Vérifie que l'entrée contient uniquement des chiffres
        int is_valid = 1;
        for (size_t i = 0; i < strlen(buffer) - 1; i++) { // -1 pour ignorer le '\n'
            if (buffer[i] < '0' || buffer[i] > '9') {
                is_valid = 0;
                break;
            }
        }

        if (!is_valid || buffer[strlen(buffer) - 1] != '\n') {
            printf("Choix invalide. Reessayez.\n");
            continue;
        }

        // Convertit l'entrée en entier
        buffer[strlen(buffer) - 1] = '\0'; // Remplace '\n' par '\0'
        choix = atoi(buffer);

        // Vérifie que l'entier est dans l'intervalle
        if (choix < min || choix > max) {
            printf("Choix invalide. Reessayez.\n");
            continue;
        }

        return choix;
    }
}
 

 void animation_combat_epique(const char* attaquant, const char* defenseur) {
     const char* color_atk = BOLD BLUE;  
     const char* color_def = BOLD RED;
 
     // Frame 1 - Les personnages se font face
     clear_terminal();
     printf("\n\n");
     printf("    %s    O       %s     O    \n", color_atk, color_def); 
     printf("    %s   /|\\      %s    /|\\   \n", color_atk, color_def);  
     printf("    %s   / \\      %s    / \\   \n", color_atk, color_def);  
     printf("\n");
     printf("    %s%s%s        %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);  
     usleep(700000);  
 
     // Frame 2 - Attaquant avance
     clear_terminal();
     printf("\n\n");
     printf("        %s    O   %s     O    \n", color_atk, color_def);
     printf("        %s   /|\\  %s    /|\\   \n", color_atk, color_def);
     printf("        %s   / \\  %s    / \\   \n", color_atk, color_def);
     printf("\n");
     printf("        %s%s%s    %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);
     usleep(500000);  
 
     // Frame 3 - Attaquant continue d'avancer
     clear_terminal();
     printf("\n\n");
     printf("            %s    O%s     O    \n", color_atk, color_def);
     printf("            %s   /|\\%s    /|\\   \n", color_atk, color_def);
     printf("            %s   / \\%s    / \\   \n", color_atk, color_def);
     printf("\n");
     printf("            %s%s%s%s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);
     usleep(500000);
 
     // Frame 4 - Préparation du coup
     clear_terminal();
     printf("\n\n");
     printf("             %s   O %s    O    \n", color_atk, color_def);
     printf("             %s  /|>%s    /|\\   \n", color_atk, color_def); 
     printf("             %s  / \\%s    / \\   \n", color_atk, color_def);
     printf("\n");
     printf("             %s%s%s vs %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);
     usleep(400000);  
 
     // Frame 5 - Impact du coup
     clear_terminal();
     printf("\n\n");
     printf("             %s   O %s    O    \n", color_atk, color_def);
     printf("             %s  /|=>%s   /|\\   \n", color_atk, color_def);  
     printf("             %s  / \\%s    / \\   \n", color_atk, color_def);
     printf("\n");
     printf("             %s%s%s vs %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);
     usleep(300000);
 
     // Frame 6 - Impact avec effet
     clear_terminal();
     printf("\n\n");
     printf("             %s   O %s    O*   \n", color_atk, color_def);  
     printf("             %s  /|=>%s   /|\\   \n", color_atk, color_def);
     printf("             %s  / \\%s    / \\   \n", color_atk, color_def);
     printf("                  %s   *BANG!*%s\n", YELLOW, RESET); 
     printf("             %s%s%s vs %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);
     usleep(200000);  
 
     // Frame 7 - Défenseur recule
     clear_terminal();
     printf("\n\n");
     printf("             %s   O %s     O*   \n", color_atk, color_def);
     printf("             %s  /|>%s     /|\\   \n", color_atk, color_def);  
     printf("             %s  / \\%s     / \\   \n", color_atk, color_def);
     printf("                   %s  *POW!*%s\n", YELLOW, RESET);
     printf("             %s%s%s vs %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);
     usleep(200000);
 
     // Frame 8 - Défenseur recule plus
     clear_terminal();
     printf("\n\n");
     printf("             %s   O %s       O*   \n", color_atk, color_def);
     printf("             %s  /|\\%s      /|\\   \n", color_atk, color_def);
     printf("             %s  / \\%s      / \\   \n", color_atk, color_def);
     printf("                     %s*BOOM!*%s\n", YELLOW, RESET);  
     printf("             %s%s%s vs %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);
     usleep(300000);
 
     clear_terminal();
     printf("\n\n");
     printf("             %s   O %s         @_@   \n", color_atk, color_def);  
     printf("             %s  /|\\%s        /|\\   \n", color_atk, color_def);
     printf("             %s  / \\%s        / \\   \n", color_atk, color_def);
     printf("                       %s*WHAM!*%s\n", YELLOW, RESET);
     printf("             %s%s%s frappe %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
     fflush(stdout);
     usleep(400000);
 
 
     clear_terminal();
     printf("\n\n");
     printf("             %s   O %s      *  *  *  \n", color_atk, color_def);  
     printf("             %s  /|\\%s         @_@   \n", color_atk, color_def);
     printf("             %s  / \\%s        / \\   \n", color_atk, color_def);
     fflush(stdout);
     // Pas de usleep à la fin, ça permet d'enchaîner  avec la suite du jeu
 }