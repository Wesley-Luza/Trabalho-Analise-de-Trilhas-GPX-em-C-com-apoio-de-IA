/*
PRIMEIRO PROMPT:
Estou realizando um trabalho surpresa para a materia de Algoritmos e programação II, 
do professor Cassandro Emmer, sobre "Analise de Trilhas GPX em C com apoio de IA".
Preciso que você gere um código em C que leia um arquivo GPX (formato XML usado para dados de GPS), 
extraia os pontos de trilha (latitude, longitude, elevação) e calcule a distância total percorrida, 
o ganho e a perda de elevação acumulados. Fiquei com o item A, o qual encaminhei em anexo.
Faça com os seguintes requisitos:
1. Linguagem C padrão, parsing via tratamento de strings (sem libs externas).
2. Organização modular com funções bem definidas.
3. Implementar fórmula de Haversine para distância (R=6371000m).
4. Calcular ganho e perda de elevação acumulados comparando ponto a ponto.
5. Saída formatada: Distância em km (3 casas), Elevações em m (2 casas).

Gere o código comentado e pronto para compilar.

Decisões de Projeto:
- Optei por não usar bibliotecas de XML (como libxml2) para facilitar a compilação e 
atender ao requisito de tratamento de strings.
- A struct 'TrkPt' foi criada para manter os dados organizados em memória.
- A fórmula de Haversine foi encapsulada em uma função própria para manter a clareza do código.

SEGUNDO PROMPT: (COMO RODAR)
Estou fazendo pelo vscode, como rodo nesse caso? (para windows)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Definições conforme o enunciado
#define RAIO_TERRA 6371000.0 // Em metros
#define MAX_LINHA 1024       // Tamanho do buffer de leitura
#define MAX_PONTOS 10000     // Tamanho máximo do array (ajustável)
#define PI 3.14159265358979323846

// Estrutura para armazenar um ponto da trilha
typedef struct {
    double lat;
    double lon;
    double ele;
} TrkPt;

// --- Protótipos das Funções (Organização Modular) ---
double toRadians(double grau);
double calcularDistanciaHaversine(TrkPt p1, TrkPt p2);
void lerArquivoGPX(const char *nomeArquivo, TrkPt *pontos, int *qtdPontos);
void processarTrilha(TrkPt *pontos, int qtdPontos, double *distTotalKm, double *ganhoEle, double *perdaEle);

// --- Função Principal ---
int main() {
    // Alocação estática simples para a prova (ou dinâmica se preferir, mas estática é mais segura pro tempo)
    TrkPt pontos[MAX_PONTOS];
    int qtdPontos = 0;
    
    double distanciaTotalKm = 0.0;
    double ganhoElevacao = 0.0;
    double perdaElevacao = 0.0;

    printf("Iniciando analise do arquivo A.gpx...\n");

    // 1. Parsing
    lerArquivoGPX("A.gpx", pontos, &qtdPontos);

    if (qtdPontos < 2) {
        printf("Erro: Pontos insuficientes encontrados no arquivo.\n");
        return 1;
    }

    printf("Total de pontos lidos: %d\n", qtdPontos);

    // 2. Cálculos
    processarTrilha(pontos, qtdPontos, &distanciaTotalKm, &ganhoElevacao, &perdaElevacao);

    // 3. Relatório Final (Conforme solicitado: km com 3 casas, metros com 2 casas)
    printf("\n--- Relatorio Final ---\n");
    printf("Distancia Total: %.3f km\n", distanciaTotalKm);
    printf("Ganho de Elevacao: %.2f m\n", ganhoElevacao);
    printf("Perda de Elevacao: %.2f m\n", perdaElevacao);

    return 0;
}

// --- Implementação das Funções ---

// Converte graus para radianos
double toRadians(double grau) {
    return grau * (PI / 180.0);
}

// Calcula a distância entre dois pontos usando a fórmula de Haversine
// Dica 1 do enunciado
double calcularDistanciaHaversine(TrkPt p1, TrkPt p2) {
    double lat1 = toRadians(p1.lat);
    double lon1 = toRadians(p1.lon);
    double lat2 = toRadians(p2.lat);
    double lon2 = toRadians(p2.lon);

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dLon / 2) * sin(dLon / 2);
    
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return RAIO_TERRA * c; // Retorna em metros
}

// Função de Parsing Robusto (Lê o arquivo e extrai lat, lon, ele)
void lerArquivoGPX(const char *nomeArquivo, TrkPt *pontos, int *qtdPontos) {
    FILE *file = fopen(nomeArquivo, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[MAX_LINHA];
    *qtdPontos = 0;
    
    // Variáveis temporárias para montar o ponto atual
    double tempLat = 0, tempLon = 0, tempEle = 0;
    int lendoPonto = 0; // Flag para saber se estamos dentro de uma tag <trkpt>

    while (fgets(linha, MAX_LINHA, file)) {
        // Procura início de um ponto de trilha
        char *ptrTrkpt = strstr(linha, "<trkpt");
        
        if (ptrTrkpt) {
            // Extrai lat e lon da string (Tratamento de Strings)
            // O formato no arquivo é: lon="-51.551410" lat="-28.862960"
            // Vamos buscar as substrings "lat=" e "lon=" independentemente da ordem
            
            char *ptrLat = strstr(linha, "lat=\"");
            char *ptrLon = strstr(linha, "lon=\"");

            if (ptrLat) sscanf(ptrLat, "lat=\"%lf\"", &tempLat);
            if (ptrLon) sscanf(ptrLon, "lon=\"%lf\"", &tempLon);
            
            lendoPonto = 1; // Estamos dentro de um bloco trkpt
        }

        // Procura elevação
        if (lendoPonto) {
            char *ptrEle = strstr(linha, "<ele>");
            if (ptrEle) {
                // Avança o ponteiro para depois de <ele> e lê o número
                sscanf(ptrEle, "<ele>%lf", &tempEle);
            }
        }

        // Procura fim do ponto </trkpt> e salva na estrutura
        if (strstr(linha, "</trkpt>") && lendoPonto) {
            pontos[*qtdPontos].lat = tempLat;
            pontos[*qtdPontos].lon = tempLon;
            pontos[*qtdPontos].ele = tempEle;
            (*qtdPontos)++;
            lendoPonto = 0;
        }
    }

    fclose(file);
}

// Realiza a iteração sobre os pontos para acumular distâncias e elevações
void processarTrilha(TrkPt *pontos, int qtdPontos, double *distTotalKm, double *ganhoEle, double *perdaEle) {
    double distTotalMetros = 0.0;
    *ganhoEle = 0.0;
    *perdaEle = 0.0;

    for (int i = 0; i < qtdPontos - 1; i++) {
        // 1. Distância (Dica 1)
        distTotalMetros += calcularDistanciaHaversine(pontos[i], pontos[i+1]);

        // 2. Elevação (Dica 2)
        double deltaEle = pontos[i+1].ele - pontos[i].ele;
        
        if (deltaEle > 0) {
            *ganhoEle += deltaEle;
        } else {
            *perdaEle += fabs(deltaEle); // Acumula valor absoluto
        }
    }

    *distTotalKm = distTotalMetros / 1000.0; // Converter para KM
}