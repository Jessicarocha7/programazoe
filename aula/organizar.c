#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 128
#define MAX_SENSOR 50
#define MAX_LEITURAS 2000

typedef struct {
    int timestamp;
    char valor[32];
} Leitura;

int main() {
    char nome_arquivo[] = "leituras_brutas.txt";

    printf("Abrindo o arquivo '%s'\n", nome_arquivo);

    FILE *entrada = fopen(nome_arquivo, "r");
    if (!entrada) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    printf("Arquivo aberto com sucesso!\n");

    char sensores[MAX_SENSOR][32];
    int total_sensores = 0;
    int *contagens = calloc(MAX_SENSOR, sizeof(int)); 
    if (!contagens) {
        printf("Erro de alocação de memória.\n");
        fclose(entrada);
        return 1;
    }

   
    Leitura **dados = malloc(MAX_SENSOR * sizeof(Leitura *));
    if (!dados) {
        printf("Erro de alocação de memória.\n");
        free(contagens);
        fclose(entrada);
        return 1;
    }
    for (int i = 0; i < MAX_SENSOR; i++) {
        dados[i] = malloc(MAX_LEITURAS * sizeof(Leitura));
        if (!dados[i]) {
            printf("Erro de alocação de memória.\n");
            for (int j = 0; j < i; j++) free(dados[j]);
            free(dados);
            free(contagens);
            fclose(entrada);
            return 1;
        }
    }

    char linha[MAX_LINHA];
    int linhas_lidas = 0;

    while (fgets(linha, MAX_LINHA, entrada)) {
        linhas_lidas++;

        int timestamp;
        char nome[32], valor[32];

        if (sscanf(linha, "%d %31s %31s", &timestamp, nome, valor) != 3) {
            printf("Erro ao ler dados na linha %d\n", linhas_lidas);
            continue;
        }

        int indice = -1;
        for (int i = 0; i < total_sensores; i++) {
            if (strcmp(sensores[i], nome) == 0) {
                indice = i;
                break;
            }
        }

        if (indice == -1) {
            if (total_sensores >= MAX_SENSOR) {
                printf("Número máximo de sensores atingido! Parando leitura.\n");
                break;
            }
            strcpy(sensores[total_sensores], nome);
            indice = total_sensores;
            total_sensores++;
        }

        if (contagens[indice] >= MAX_LEITURAS) {
            printf("Número máximo de leituras para sensor %s atingido! Ignorando entrada.\n", sensores[indice]);
            continue;
        }

        int pos = contagens[indice]++;
        dados[indice][pos].timestamp = timestamp;
        strcpy(dados[indice][pos].valor, valor);
    }

    fclose(entrada);

    printf("\nTotal de sensores encontrados: %d\n", total_sensores);
    for (int i = 0; i < total_sensores; i++) {
        printf("Sensor %d: %s, Leituras: %d\n", i + 1, sensores[i], contagens[i]);
    }

    for (int i = 0; i < MAX_SENSOR; i++) {
        free(dados[i]);
    }
    free(dados);
    free(contagens);

    return 0;
}
