#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

void bfs(int** grafo, int num_vertices, int origem, int* mais_distante, int* max_dist) {
    bool* visitados = (bool*)calloc(num_vertices, sizeof(bool));
    int* niveis = (int*)malloc(num_vertices * sizeof(int));
    int* fila = (int*)malloc(num_vertices * sizeof(int));

    if (!visitados || !niveis || !fila) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_vertices; i++) {
        niveis[i] = -1; // Inicializa os níveis como -1 (não visitado)
    }

    // Configurações iniciais da BFS
    int inicio_fila = 0, fim_fila = 0;
    fila[fim_fila++] = origem;
    visitados[origem] = true;
    niveis[origem] = 0;

    *max_dist = 0;  // Inicializa a maior distância como 0
    *mais_distante = origem; // Inicializa o vértice mais distante como a origem

    while (inicio_fila < fim_fila) {
        int atual = fila[inicio_fila++];
        for (int vizinho = 0; vizinho < num_vertices; vizinho++) {
            if (grafo[atual][vizinho] && !visitados[vizinho]) {
                visitados[vizinho] = true;
                niveis[vizinho] = niveis[atual] + 1;
                fila[fim_fila++] = vizinho;

                // Atualiza a maior distância e o vértice mais distante
                if (niveis[vizinho] > *max_dist) {
                    *max_dist = niveis[vizinho];
                    *mais_distante = vizinho;
                }
            }
        }
    }

    free(visitados);
    free(niveis);
    free(fila);
}

// Função para calcular o diâmetro do grafo usando BFS Duplo
int bfs_duplo(int** grafo, int num_vertices) {
    int mais_distante, max_dist;

    // Primeira BFS a partir de um vértice arbitrário (neste caso, o vértice 0)
    bfs(grafo, num_vertices, 0, &mais_distante, &max_dist);

    // Segunda BFS a partir do vértice mais distante encontrado na primeira BFS
    bfs(grafo, num_vertices, mais_distante, &mais_distante, &max_dist);

    return max_dist; // Retorna a distância máxima encontrada, que é o diâmetro aproximado
}

int main() {
    const char* nome_arquivo = "as_graph.txt";
    int num_vertices, num_arestas;
    int** grafo = ler_grafo(nome_arquivo, &num_vertices, &num_arestas);

    int diametro = bfs_duplo(grafo, num_vertices);

    // Exibir o diâmetro aproximado do grafo
    printf("O diâmetro aproximado do grafo é: %d\n", diametro);

    // Liberar memória
    liberar_matriz(grafo, num_vertices);

    return 0;
}