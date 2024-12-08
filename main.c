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

// Estrutura para lista de adjacência
typedef struct No {
    int vertice;
    struct No* prox;
} No;

typedef struct ListaAdj {
    No* cabeca;
} ListaAdj;

// Função para alocar e inicializar a lista de adjacência
ListaAdj* alocar_lista_adj(int num_vertices) {
    ListaAdj* lista = (ListaAdj*)malloc(num_vertices * sizeof(ListaAdj));
    for (int i = 0; i < num_vertices; i++) {
        lista[i].cabeca = NULL;
    }
    return lista;
}

// Função para liberar a memória da lista de adjacência
void liberar_lista_adj(ListaAdj* lista, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        No* atual = lista[i].cabeca;
        while (atual) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(lista);
}

// Função para adicionar uma aresta na lista de adjacência
void adicionar_aresta_lista(ListaAdj* lista, int u, int v) {
    No* novo_u = (No*)malloc(sizeof(No));
    novo_u->vertice = v;
    novo_u->prox = lista[u].cabeca;
    lista[u].cabeca = novo_u;

    No* novo_v = (No*)malloc(sizeof(No));
    novo_v->vertice = u;
    novo_v->prox = lista[v].cabeca;
    lista[v].cabeca = novo_v;
}

// Função para imprimir a lista de adjacência
void imprimir_lista_adj(ListaAdj* lista, int num_vertices) {
    printf("Lista de Adjacência:\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("%d: ", i + 1);
        No* atual = lista[i].cabeca;
        while (atual) {
            printf("%d ", atual->vertice + 1);
            atual = atual->prox;
        }
        printf("\n");
    }
}

// Função para ler o grafo e construir a matriz de adjacência
int** ler_grafo_matriz(const char* nome_arquivo, int* num_vertices, int* num_arestas) {
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

// Função para ler o grafo e construir a lista de adjacência
ListaAdj* ler_grafo_lista(const char* nome_arquivo, int* num_vertices, int* num_arestas) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fscanf(arquivo, "%d", num_vertices);
    ListaAdj* grafo = alocar_lista_adj(*num_vertices);
    *num_arestas = 0;

    int u, v;
    while (fscanf(arquivo, "%d %d", &u, &v) != EOF) {
        u--; v--;  // Ajustar para índices baseados em 0
        if (grafo[u].cabeca == NULL || grafo[v].cabeca == NULL) {
            (*num_arestas)++;
        }
        adicionar_aresta_lista(grafo, u, v);
    }

    fclose(arquivo);
    return grafo;
}

// Função para imprimir a matriz de adjacência
void imprimir_matriz_adj(int** grafo, int num_vertices) {
    printf("Matriz de Adjacência:\n");
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            printf("%d ", grafo[i][j]);
        }
        printf("\n");
    }
}

// questão 4 - busca em profundidade e largura
void busca_em_largura(int** grafo, int num_vertices, const char* nome_arquivo) {
    bool* visitados = (bool*)calloc(num_vertices, sizeof(bool));
    int* niveis = (int*)malloc(num_vertices * sizeof(int));
    int* pais = (int*)malloc(num_vertices * sizeof(int));
    int* fila = (int*)malloc(num_vertices * sizeof(int));

    if (!visitados || !niveis || !pais || !fila) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa arrays
    for (int i = 0; i < num_vertices; i++) {
        niveis[i] = -1;  // Indica que o vértice não foi visitado
        pais[i] = -1;    // Sem pai inicialmente
    }

    // Processa todos os componentes desconexos
    for (int componente = 0; componente < num_vertices; componente++) {
        if (!visitados[componente]) {
            // Inicializa fila para a BFS
            int inicio_fila = 0, fim_fila = 0;
            fila[fim_fila++] = componente;
            visitados[componente] = true;
            niveis[componente] = 0;

            while (inicio_fila < fim_fila) {
                int atual = fila[inicio_fila++];
                for (int vizinho = 0; vizinho < num_vertices; vizinho++) {
                    if (grafo[atual][vizinho] == 1 && !visitados[vizinho]) {
                        visitados[vizinho] = true;
                        niveis[vizinho] = niveis[atual] + 1;
                        pais[vizinho] = atual;
                        fila[fim_fila++] = vizinho;
                    }
                }
            }
        }
    }

    // Salva os resultados no arquivo
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        exit(EXIT_FAILURE);
    }
    fprintf(arquivo, "Vertice Pai Nivel\n");
    for (int i = 0; i < num_vertices; i++) {
        fprintf(arquivo, "%d %d %d\n", i + 1, pais[i] + 1, niveis[i]);
    }
    fclose(arquivo);

    // Libera memória
    free(visitados);
    free(niveis);
    free(pais);
    free(fila);
}

void busca_em_profundidade_rec(int** grafo, int num_vertices, int atual, bool* visitados, int* niveis, int* pais, int nivel) {
    visitados[atual] = true;
    niveis[atual] = nivel;

    for (int vizinho = 0; vizinho < num_vertices; vizinho++) {
        if (grafo[atual][vizinho] && !visitados[vizinho]) {
            pais[vizinho] = atual;
            busca_em_profundidade_rec(grafo, num_vertices, vizinho, visitados, niveis, pais, nivel + 1);
        }
    }
}

void busca_em_profundidade(int** grafo, int num_vertices, int inicio, const char* nome_arquivo) {
    bool* visitados = (bool*)calloc(num_vertices, sizeof(bool));
    int* niveis = (int*)malloc(num_vertices * sizeof(int));
    int* pais = (int*)malloc(num_vertices * sizeof(int));

    for (int i = 0; i < num_vertices; i++) {
        niveis[i] = -1;
        pais[i] = -1;
    }

    for (int componente = 0; componente < num_vertices; componente++) {
        if (!visitados[componente]) {
            busca_em_profundidade_rec(grafo, num_vertices, componente, visitados, niveis, pais, 0);
        }
    }

    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao salvar árvore de busca em profundidade");
        exit(EXIT_FAILURE);
    }
    fprintf(arquivo, "Vertice Pai Nivel\n");
    for (int i = 0; i < num_vertices; i++) {
        fprintf(arquivo, "%d %d %d\n", i + 1, pais[i] + 1, niveis[i]);
    }
    fclose(arquivo);

    free(visitados);
    free(niveis);
    free(pais);
}

// questão 5 - componentes conexos
int bfs_component(int** grafo, int num_vertices, bool* visitados, int inicio, FILE* arquivo, int* num_componentes) {
    int* fila_bfs = (int*)malloc(num_vertices * sizeof(int));
    if (!fila_bfs) {
        fprintf(stderr, "Erro ao alocar memória para fila.\n");
        exit(EXIT_FAILURE);
    }

    int tamanho_componente = 0; // Contador de vértices no componente
    int inicio_fila = 0, fim_fila = 0;
    fila_bfs[fim_fila++] = inicio;
    visitados[inicio] = true;

    fprintf(arquivo, "Componente %d: ", ++(*num_componentes));

    while (inicio_fila < fim_fila) {
        int atual = fila_bfs[inicio_fila++];
        fprintf(arquivo, "%d ", atual + 1);
        tamanho_componente++; // Incrementa o tamanho do componente

        for (int vizinho = 0; vizinho < num_vertices; vizinho++) {
            if (grafo[atual][vizinho] && !visitados[vizinho]) {
                fila_bfs[fim_fila++] = vizinho;
                visitados[vizinho] = true;
            }
        }
    }

    fprintf(arquivo, "\n");
    free(fila_bfs);
    return tamanho_componente;
}

void encontrar_componentes_conexos(int** grafo, int num_vertices, const char* nome_arquivo) {
    bool* visitados = (bool*)calloc(num_vertices, sizeof(bool));
    if (!visitados) {
        fprintf(stderr, "Erro ao alocar memória para array de visitados.\n");
        exit(EXIT_FAILURE);
    }

    int num_componentes = 0;
    int maior_tamanho = 0; // Tamanho do maior componente
    int componente_maior = 0; // Índice do maior componente

    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        free(visitados);
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "Componentes Conexos:\n");

    for (int i = 0; i < num_vertices; i++) {
        if (!visitados[i]) {
            int tamanho_atual = bfs_component(grafo, num_vertices, visitados, i, arquivo, &num_componentes);
            if (tamanho_atual > maior_tamanho) {
                maior_tamanho = tamanho_atual;
                componente_maior = num_componentes;
            }
        }
    }

    fprintf(arquivo, "Maior componente é o número %d com %d vértices.\n", componente_maior, maior_tamanho);

    fclose(arquivo);
    free(visitados);
}

// ESTUDO DE CASO ************************************************
// calculo de uso de memoria
size_t calcular_memoria_matriz(int num_vertices) {
    return num_vertices * num_vertices * sizeof(int);
}

size_t calcular_memoria_lista(int num_vertices, int num_arestas) {
    size_t memoria_vertices = num_vertices * sizeof(No*);
    size_t memoria_arestas = 2 * num_arestas * (sizeof(int) + sizeof(No*));
    return memoria_vertices + memoria_arestas;
}

// calculo do tempo
double calcular_tempo(clock_t inicio, clock_t fim) {
    return ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

double bytes_para_mb(size_t bytes) {
    return (double)bytes / (1024 * 1024);
}


// Função principal
int main() {
    const char* nome_arquivo = "collaboration_graph.txt";
    const char* bfs_saida = "bfs_arvore.txt";
    const char* dfs_saida = "dfs_arvore.txt";
    const char* info_saida = "informacoes_grafo.txt";
    const char* componentes_saida = "componentes_conexos.txt"; 

    int num_vertices, num_arestas;
    int** grafo = ler_grafo(nome_arquivo, &num_vertices, &num_arestas);

    int* graus = calcular_graus(grafo, num_vertices);
    //salvar_informacoes_grafo(info_saida, num_vertices, num_arestas, graus);

    //busca_em_largura(grafo, num_vertices, bfs_saida);
    //busca_em_profundidade(grafo, num_vertices, 1, dfs_saida);

    // Chamada para encontrar componentes conexos
    encontrar_componentes_conexos(grafo, num_vertices, componentes_saida);

    clock_t inicio_matriz = clock();
    int** grafo_matriz = ler_grafo_matriz(nome_arquivo, &num_vertices, &num_arestas);
    clock_t fim_matriz = clock();
    //imprimir_matriz_adj(grafo_matriz, num_vertices);
    size_t memoria_matriz = calcular_memoria_matriz(num_vertices);
    printf("Memoria usada pela matriz de adjacencia: %.2f MB\n", bytes_para_mb(memoria_matriz));
    printf("Tempo de execução para matriz de adjacência: %.6f segundos\n", calcular_tempo(inicio_matriz, fim_matriz));

    clock_t inicio_lista = clock();
    ListaAdj* grafo_lista = ler_grafo_lista(nome_arquivo, &num_vertices, &num_arestas);
    clock_t fim_lista = clock();
    //imprimir_lista_adj(grafo_lista, num_vertices);
    size_t memoria_lista = calcular_memoria_lista(num_vertices, num_arestas);
    printf("Memoria usada pela lista de adjacencia: %.2f MB\n", bytes_para_mb(memoria_lista));
    printf("Tempo de execução para lista de adjacência: %.6f segundos\n", calcular_tempo(inicio_lista, fim_lista));

    // Libera memória
    liberar_matriz(grafo_matriz, num_vertices);
    liberar_lista_adj(grafo_lista, num_vertices);

    liberar_matriz(grafo, num_vertices);
    free(graus);

    printf("Processamento concluido!\n");
    return 0;
}
