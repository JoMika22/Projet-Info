/*
 * utils.c
 * Implémentation des utilitaires pour le jeu de combat
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // Include for strchr function
#include "utilis.h"


void clear_terminal() {
    printf("\033[H\033[J");
}

// Affiche une barre de santé graphique
void afficher_barre_sante(int PV, int PVmax) {
    int largeur = 10; 
    int remplissage = (PV * largeur) / PVmax;
    printf("[");
    for (int i = 0; i < largeur; i++) {
        if (i < remplissage) printf("🟩");
        else printf("⬜"); 
    }
    printf("] %d/%d\n", PV, PVmax);
}

// Ensure input validation handles multi-character inputs
int lire_entree_valide(int min, int max) {
    int choix;
    char buffer[100];
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erreur de lecture. Reessayez.\n");
            continue;
        }
        // Check if input is a valid integer and contains no extra characters
        if (sscanf(buffer, "%d", &choix) != 1 || strchr(buffer, ' ') || strchr(buffer, '\n') == NULL || buffer[strlen(buffer)-1] != '\n') {
            printf("Choix invalide. Reessayez.\n");
            continue;
        }
        // Check if input is within range
        if (choix < min || choix > max) {
            printf("Choix invalide. Reessayez.\n");
            continue;
        }
        return choix;
    }
}

// Animation de combat épique entre deux personnages
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

    // Frame 9 - Défenseur étourdi
    clear_terminal();
    printf("\n\n");
    printf("             %s   O %s         @_@   \n", color_atk, color_def);
    printf("             %s  /|\\%s        /|\\   \n", color_atk, color_def);
    printf("             %s  / \\%s        / \\   \n", color_atk, color_def);
    printf("                       %s*WHAM!*%s\n", YELLOW, RESET);
    printf("             %s%s%s frappe %s%s%s\n", color_atk, attaquant, RESET, color_def, defenseur, RESET);
    fflush(stdout);
    usleep(400000);

    // Frame 10 - Étoiles au-dessus du défenseur
    clear_terminal();
    printf("\n\n");
    printf("             %s   O %s      *  *  *  \n", color_atk, color_def);
    printf("             %s  /|\\%s         @_@   \n", color_atk, color_def);
    printf("             %s  / \\%s        / \\   \n", color_atk, color_def);
    fflush(stdout);
}