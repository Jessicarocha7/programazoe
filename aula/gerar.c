#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void gerar_string(char *destino, int tamanho) {
    const char *letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < tamanho; i++) {
        destino[i] = letras[rand() % 26];
    }
    destino[tamanho] = '\0';
}


void gerar_valor(char *tipo, char *resultado) {
    if (strcmp(tipo, "int") == 0) {
        sprintf(resultado, "%d", rand() % 100);
    } else if (strcmp(tipo, "float") == 0) {
        float valor = (float)rand() / (float)(RAND_MAX / 100.0);
        sprintf(resultado, "%.2f", valor);
    } else if (strcmp(tipo, "bool") == 0) {
        sprintf(resultado, "%s", rand() % 2 ? "true" : "false");
    } else if (strcmp(tipo, "string") == 0) {
        gerar_string(resultado, 8); 
    }
}


time_t gerar_timestamp_aleatorio(struct tm *inicial, struct tm *final) {
    time_t timestamp_inicial = mktime(inicial);
    if (timestamp_inicial == -1) {
        printf("Data inicial inválida.\n");
        return -1;
    }

    time_t timestamp_final = mktime(final);
    if (timestamp_final == -1) {
        printf("Data final inválida.\n");
        return -1;
    }

    if (timestamp_final < timestamp_inicial) {
        printf("Data final menor que data inicial.\n");
        return -1;
    }

    time_t intervalo = timestamp_final - timestamp_inicial;
    time_t timestamp_aleatorio = timestamp_inicial + (rand() % (intervalo + 1));

    return timestamp_aleatorio;
}

int main() {
    srand(time(NULL)); 

    
    struct tm data_inicial = {0};
    printf("Digite a data inicial (ano mes dia hora min seg): ");
    scanf("%d %d %d %d %d %d",
        &data_inicial.tm_year,
        &data_inicial.tm_mon,
        &data_inicial.tm_mday,
        &data_inicial.tm_hour,
        &data_inicial.tm_min,
        &data_inicial.tm_sec);

    data_inicial.tm_year -= 1900;  
    data_inicial.tm_mon -= 1;

    
    struct tm data_final = {0};
    printf("Digite a data final (ano mes dia hora min seg): ");
    scanf("%d %d %d %d %d %d",
        &data_final.tm_year,
        &data_final.tm_mon,
        &data_final.tm_mday,
        &data_final.tm_hour,
        &data_final.tm_min,
        &data_final.tm_sec);

    data_final.tm_year -= 1900;
    data_final.tm_mon -= 1;

   
    char sensores[2][32] = {"sensor1", "sensor2"};
    char tipos[2][16] = {"int", "float"};

    FILE *arquivo = fopen("leituras_brutas.txt", "w");
    if (!arquivo) {
        printf("Erro ao criar arquivo.\n");
        return 1;
    }

    for (int i = 0; i < 2; i++) {  
        for (int j = 0; j < 2000; j++) {  
            time_t ts = gerar_timestamp_aleatorio(&data_inicial, &data_final);
            if (ts == -1) {
                fclose(arquivo);
                return 1;
            }
            char valor[32];
            gerar_valor(tipos[i], valor);
            fprintf(arquivo, "%ld %s %s\n", ts, sensores[i], valor);
        }
    }

    fclose(arquivo);
    printf("Arquivo 'leituras_brutas.txt' criado com sucesso!\n");

    return 0;
}