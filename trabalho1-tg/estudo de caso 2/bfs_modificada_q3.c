#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// questão 1 - leitura do gravo
int** alocar_matriz(int num_vertices) {
    int** matriz = (int**)malloc(num_vertices * sizeof(int*));
    for (int i = 0; i < num_vertices; i++) {
        matriz[i] = (int*)calloc(num_vertices, sizeof(int));
    }
    return matriz;
}

void liberar_matriz(int** matriz, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int** ler_grafo(const char* nome_arquivo, int* num_vertices, int* num_arestas) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fscanf(arquivo, "%d", num_vertices);
    int** grafo = alocar_matriz(*num_vertices);
    *num_arestas = 0;

    int u, v;
    while (fscanf(arquivo, "%d %d", &u, &v) != EOF) {
        u--; v--;  // Ajustar para índices baseados em 0
        if (grafo[u][v] == 0) {
            (*num_arestas)++;
        }
        grafo[u][v] = 1;
        grafo[v][u] = 1;
    }

    fclose(arquivo);
    return grafo;
}

// questão 2 - informações do grafo
int* calcular_graus(int** grafo, int num_vertices) {
    int* graus = (int*)calloc(num_vertices, sizeof(int));
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            graus[i] += grafo[i][j];
        }
    }
    return graus;
}

void salvar_informacoes_grafo(const char* nome_arquivo, int num_vertices, int num_arestas, int* graus) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao salvar informações do grafo");
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "%d\n%d\n", num_vertices, num_arestas);
    for (int i = 0; i < num_vertices; i++) {
        fprintf(arquivo, "%d %d\n", i + 1, graus[i]);
    }

    fclose(arquivo);
}

// Função modificada de busca em largura para retornar o maior nível
int busca_em_largura(int** grafo, int num_vertices, int vertice_inicial) {
    bool* visitados = (bool*)calloc(num_vertices, sizeof(bool));
    int* niveis = (int*)malloc(num_vertices * sizeof(int));
    int* fila = (int*)malloc(num_vertices * sizeof(int));

    if (!visitados || !niveis || !fila) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_vertices; i++) {
        niveis[i] = -1;  // Indica que o vértice não foi visitado
    }

    // Inicializa BFS
    int inicio_fila = 0, fim_fila = 0;
    fila[fim_fila++] = vertice_inicial;
    visitados[vertice_inicial] = true;
    niveis[vertice_inicial] = 0;
    int nivel_maximo = 0;

    while (inicio_fila < fim_fila) {
        int atual = fila[inicio_fila++];
        for (int vizinho = 0; vizinho < num_vertices; vizinho++) {
            if (grafo[atual][vizinho] == 1 && !visitados[vizinho]) {
                visitados[vizinho] = true;
                niveis[vizinho] = niveis[atual] + 1;
                fila[fim_fila++] = vizinho;
                if (niveis[vizinho] > nivel_maximo) {
                    nivel_maximo = niveis[vizinho];
                }
            }
        }
    }

    free(visitados);
    free(niveis);
    free(fila);

    return nivel_maximo;
}

int main() {
    int num_vertices, num_arestas;
    const char* nome_arquivo_grafo = "as_graph.txt";

    // Ler grafo da matriz de adjacência
    int** grafo = ler_grafo(nome_arquivo_grafo, &num_vertices, &num_arestas);
    printf("Grafo carregado com %d vértices e %d arestas.\n", num_vertices, num_arestas);
    
    while (1) {
        int vertice_inicial;
        printf("Digite o vértice inicial para a busca em largura (1 a %d, ou 0 para sair): ", num_vertices);
        scanf("%d", &vertice_inicial);

        if (vertice_inicial == 0) {
            break;
        }

        if (vertice_inicial < 1 || vertice_inicial > num_vertices) {
            printf("Vértice inválido. Tente novamente.\n");
            continue;
        }

        // Convertendo para índice baseado em 0
        vertice_inicial--;

        int nivel_maximo = busca_em_largura(grafo, num_vertices, vertice_inicial);
        printf("A maior distância do vértice %d é %d.\n", vertice_inicial + 1, nivel_maximo);
    }
    liberar_matriz(grafo, num_vertices);


    return 0;
}
