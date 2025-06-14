#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct {
    int timestamp;
    char valor[32];
} Leitura;


time_t converter_para_timestap(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t;

    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t timestamp = mktime(&t);
    if (timestamp == -1) {
        printf("Data inválida. Tente novamente.\n");
        return -1;
    } else {
        return timestamp;
    }
}

int main() {
    char nome_sensor[32];
    int dia, mes, ano, hora, min, seg;

    printf("Digite o nome do sensor (ex: sensor1): ");
    scanf("%s", nome_sensor);

    printf("Digite a data e hora da consulta (DD MM AAAA HH MM SS): ");
    if (scanf("%d %d %d %d %d %d", &dia, &mes, &ano, &hora, &min, &seg) != 6) {
        printf("Formato inválido!\n");
        return 1;
    }

    int alvo = (int)converter_para_timestap(dia, mes, ano, hora, min, seg);
    if (alvo == -1) {
        return 1;
    }

    char nome_arquivo[64];
    sprintf(nome_arquivo, "%s.txt", nome_sensor);

    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo do sensor.\n");
        return 1;
    }

    Leitura leituras[2000];
    int total = 0;

    while (fscanf(f, "%d %s", &leituras[total].timestamp, leituras[total].valor) == 2) {
        total++;
    }

    fclose(f);

    // Busca binária 
    int esq = 0, dir = total - 1, meio;
    int melhor_dif = 999999999;
    Leitura melhor;

    while (esq <= dir) {
        meio = (esq + dir) / 2;
        int ts = leituras[meio].timestamp;
        int dif = abs(ts - alvo);

        if (dif < melhor_dif) {
            melhor = leituras[meio];
            melhor_dif = dif;
        }

        if (ts < alvo)
            esq = meio + 1;
        else
            dir = meio - 1;
    }

    printf("\nLeitura mais próxima:\n");
    printf("Timestamp: %d\n", melhor.timestamp);
    printf("Valor: %s\n", melhor.valor);

    return 0;
}