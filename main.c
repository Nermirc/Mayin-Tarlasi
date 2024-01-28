#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAILLE_TABLEAU 8
#define NORMALE 7
#define GAGNER 1
#define PERDU -1
#define CONTINUE 0

int soustab[TAILLE_TABLEAU + 2][TAILLE_TABLEAU + 2];
char tab[TAILLE_TABLEAU + 2][TAILLE_TABLEAU + 2];
int nb_mine, y = 0;
int d_count = 0;

// Fonksiyonların bildirimleri
void creer_soustableau();
void initializer_tableau();
void afficher_soustableau();
void afficher_tableau();
void placer_mines();
int verifiermatchgagner();
void explorer(int ligne, int colonne);
int creuser(int ligne, int colonne);

int main() {
    int ligne, colonne;
    int ilerleme;
    int yeniden;
    char action;

    printf("Mayin tarlasi oyunumuza hosgeldiniz.\n");
    creer_soustableau();
    initializer_tableau();

    do {
        afficher_tableau();
        printf("\nLutfen su formatta giriniz. c,ligne,colonne veya d,ligne,colonne veya e,ligne,colonne\n");
        scanf(" %c,%d,%d", &action, &ligne, &colonne);

        if (action == 'c') {
            ilerleme = creuser(ligne, colonne);
        } else if (action == 'd') {
            if (nb_mine > d_count) {
                d_count++;
                tab[ligne][colonne] = 'D';
                ilerleme = verifiermatchgagner();
            }
        } else if (action == 'e') {
            if (d_count > 0) {
                d_count--;
            }
            tab[ligne][colonne] = '#';
            ilerleme = creuser(ligne, colonne);
        }

        if (ilerleme == PERDU) {
            printf("Oyunu kaybettin mal\n");
            afficher_soustableau();
            printf("Oyunu yeniden oynamak istiyor musunuz?[1-Evet][0-Hayir]\n");
            scanf("%d", &yeniden);

            switch (yeniden) {
                case 0:
                    printf("Tesekkurler gorusuruz\n");
                    return 0;
                case 1:
                    d_count = 0;
                    ilerleme = CONTINUE;
                    creer_soustableau();
                    initializer_tableau();
                    break;
                default:
                    printf("Yanlis bir deger girdiniz.\n");
                    break;
            }
        }

        if (ilerleme == GAGNER) {
            printf("Bravo harika bir is cikardin.\n");
             afficher_soustableau();
            printf("Oyunu yeniden oynamak istiyor musunuz?[1-Evet][0-Hayir]\n");
            scanf("%d", &yeniden);

            switch (yeniden) {
                case 0:
                    printf("Tesekkurler gorusuruz\n");
                    return 0;
                case 1:
                    d_count = 0;
                    ilerleme = CONTINUE;
                    creer_soustableau();
                    initializer_tableau();
                    break;
                default:
                    printf("Yanlis bir deger girdiniz.\n");
                    break;
            }
        }
    } while (1);

    return 0;
}

void creer_soustableau() {
    int i, j;
    for (i = 0; i <= TAILLE_TABLEAU + 1; i++) {
        for (j = 0; j <= TAILLE_TABLEAU + 1; j++) {
            soustab[i][j] = 0;
        }
    }

    for (i = 0; i <= TAILLE_TABLEAU + 1; i++) {
        soustab[i][0] = 1;
        soustab[i][TAILLE_TABLEAU + 1] = 1;
    }

    for (j = 0; j <= TAILLE_TABLEAU; j++) {
        soustab[0][j] = 1;
        soustab[TAILLE_TABLEAU + 1][j] = 1;
    }

    nb_mine = NORMALE;
    placer_mines();
}

void initializer_tableau() {
    int i, j;
    for (i = 0; i <= TAILLE_TABLEAU + 1; i++) {
        for (j = 0; j <= TAILLE_TABLEAU + 1; j++) {
            tab[i][j] = '#';
        }
    }

    for (i = 0; i <= TAILLE_TABLEAU + 1; i++) {
        tab[i][0] = '*';
        tab[i][TAILLE_TABLEAU + 1] = '*';
    }

    for (j = 0; j <= TAILLE_TABLEAU; j++) {
        tab[0][j] = '*';
        tab[TAILLE_TABLEAU + 1][j] = '*';
    }
}
void afficher_soustableau() {
    int i, j;

    for (i = 1; i <= TAILLE_TABLEAU; i++) {
        for (j = 1; j <= TAILLE_TABLEAU; j++) {
            if (soustab[i][j] == -1) {
                printf(" * ");
            } else {
                printf(" %d ", soustab[i][j]);
            }
        }
        printf("\n");
    }
}
void afficher_tableau() {
    int i, j;

    printf("\n");
    for (i = 1; i <= TAILLE_TABLEAU; i++) {
        printf("%3d", i);
    }
    printf("\n");

    for (i = 1; i <= TAILLE_TABLEAU; i++) {
        for (j = 1; j <= TAILLE_TABLEAU; j++) {
            printf("%3c", tab[i][j]);
        }
        printf("  |%d", i);
        printf("\n");
    }

    for (i = 1; i <= TAILLE_TABLEAU; i++) {
        printf("---");
    }
}

void placer_mines() {
    srand(time(NULL));
    nb_mine = NORMALE;
    int i, j, k, p, ligne, colonne;

    for (i = 1; i <= nb_mine; i++) {
        ligne = 1 + rand() % TAILLE_TABLEAU;
        colonne = 1 + rand() % TAILLE_TABLEAU;
        if (soustab[ligne][colonne] == -1) {
            i--;
        }

        soustab[ligne][colonne] = -1;
        for (k = -1; k <= 1; k++) {
            for (p = -1; p <= 1; p++) {
                if (soustab[ligne][colonne] == -1) {
                    if (soustab[ligne + k][colonne + p] != -1) {
                        soustab[ligne + k][colonne + p]++;
                    }
                }
            }
        }
    }
}

int creuser(int ligne, int colonne) {
    int x = 0;
    if (soustab[ligne][colonne] == -1) {
        x = PERDU;
    } else if (soustab[ligne][colonne] == 0) {
        tab[ligne][colonne] = ('0' + soustab[ligne][colonne]);
        explorer(ligne, colonne);
        x = verifiermatchgagner();
        x = CONTINUE;
    } else {
        explorer(ligne, colonne);
    }

    return x;
}

void explorer(int ligne, int colonne) {
    int i, j;

    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            tab[ligne + i][colonne + j] = '0' + soustab[ligne + i][colonne + j];

            if (soustab[ligne + i][colonne + i] > 0 && tab[ligne + i][colonne + j] == '#') {
                tab[ligne + i][colonne + j] = '0' + soustab[ligne + i][colonne + j];
            } else if (soustab[ligne + i][colonne + j] == 0 && tab[ligne + i][colonne + j] == '#') {
                explorer(ligne + i, colonne + j);
            }
        }
    }
}


int verifiermatchgagner() {
    int i, j;
    int status;
    int y = 0;

    for (i = 1; i <= TAILLE_TABLEAU; i++) {
        for (j = 1; j <= TAILLE_TABLEAU; j++) {
            if (tab[i][j] == 'D') {
                if (soustab[i][j] == -1) {
                    y++;
                }
            }
        }
    }

    if (y == NORMALE) {
        status = GAGNER;
    } else {
        status = CONTINUE;
    }

    return status;
}

