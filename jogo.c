// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // Para a função usleep (pausa entre turnos)

// Definição das entidades
#define VAZIO 0
#define CACADOR 1
#define LOBO 2
#define OVELHA 3

// Protótipos das funções
void imprimirTabuleiro(int *tab, int tam);
void inicializarTabuleiro(int *tab, int tam);
void processarTurno(int *tab, int tam);

int main() {
    int tam;
    int *tabuleiro;

    // Semente para números aleatórios
    srand(time(NULL));

    printf("Digite o tamanho do tabuleiro: ");
    if (scanf("%d", &tam) != 1 || tam <= 0) {
        printf("Tamanho inválido.\n");
        return 1;
    }

    // Alocação Dinâmica (malloc)
    tabuleiro = (int *)malloc(tam * sizeof(int));

    if (tabuleiro == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    inicializarTabuleiro(tabuleiro, tam);

    // Loop de Autoplay
    for (int i = 0; i < 50; i++) { // Executa 50 turnos
        system("clear || cls"); // Limpa o terminal (Linux/Windows)
        printf("Turno: %d\n", i + 1);
        imprimirTabuleiro(tabuleiro, tam);
        processarTurno(tabuleiro, tam);
        usleep(1000000); // Pausa de 0.5 segundos para visualização
    }

    // Liberação da memória
    free(tabuleiro);
    return 0;
}

void inicializarTabuleiro(int *tab, int tam) {
    for (int i = 0; i < tam; i++) {
        int r = rand() % 10;
        if (r == 1) tab[i] = CACADOR;
        else if (r == 2) tab[i] = LOBO;
        else if (r == 3) tab[i] = OVELHA;
        else tab[i] = VAZIO;
    }
}

void imprimirTabuleiro(int *tab, int tam) {
    for (int i = 0; i < tam; i++) {
        if (tab[i] == CACADOR) printf("[ C ] ");
        else if (tab[i] == LOBO) printf("[ L ] ");
        else if (tab[i] == OVELHA) printf("[ O ] ");
        else printf("[   ] ");
    }
    printf("\n");
}

void processarTurno(int *tab, int tam) {
    // Criamos uma cópia para não mover a mesma peça várias vezes no mesmo turno
    int *proximo = (int *)malloc(tam * sizeof(int));
    for (int i = 0; i < tam; i++) proximo[i] = tab[i];

    for (int i = 0; i < tam; i++) {
        if (tab[i] == VAZIO) continue;

        int direcao = (rand() % 2 == 0) ? 1 : -1; // Move para esquerda ou direita
        int nova_pos = i + direcao;

        if (nova_pos >= 0 && nova_pos < tam) {
            int eu = tab[i];
            int alvo = tab[nova_pos];

            if (alvo == VAZIO) {
                proximo[nova_pos] = eu;
                proximo[i] = VAZIO;
            } 
            // Regra: Caçador mata Lobo
            else if (eu == CACADOR && alvo == LOBO) {
                proximo[nova_pos] = CACADOR;
                proximo[i] = VAZIO;
            }
            // Regra: Lobo mata Ovelha
            else if (eu == LOBO && alvo == OVELHA) {
                proximo[nova_pos] = LOBO;
                proximo[i] = VAZIO;
            }
            // Regra: Ovelha se salva com o Caçador (ela some do perigo)
            else if (eu == OVELHA && alvo == CACADOR) {
                proximo[i] = VAZIO;
            }
            // Regra: Reprodução
            else if (eu == alvo) {
                // Tenta colocar um novo na posição atual se houver espaço
                int pos_filho = i - direcao; 
                if (pos_filho >= 0 && pos_filho < tam && proximo[pos_filho] == VAZIO) {
                    proximo[pos_filho] = eu;
                }
            }
        }
    }

    for (int i = 0; i < tam; i++) tab[i] = proximo[i];
    free(proximo);
}
