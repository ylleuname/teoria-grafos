#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <float.h> // Para usar FLT_MAX
#include <limits.h> // Para INT_MAX

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

// Estrutura para uso do Dijkstra nos grafos ponderados
typedef struct {
    int vertice;
    float distancia;
} Nodo;

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


// PARTE 2 DO TRABALHO ******************************************

// Liberar matriz de adjacência para pesos - QUESTÃO 1
void liberar_matriz_pesos(float** matriz, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

float** alocar_matriz_pesos(int num_vertices) {
    float** matriz = (float**)malloc(num_vertices * sizeof(float*));
    if (!matriz) {
        fprintf(stderr, "Erro ao alocar memória para matriz\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_vertices; i++) {
        matriz[i] = (float*)calloc(num_vertices, sizeof(float));
        if (!matriz[i]) {
            fprintf(stderr, "Erro ao alocar memória para linha %d da matriz\n", i);
            exit(EXIT_FAILURE);
        }
    }
    return matriz;
}

float** ler_grafo_ponderado(const char* nome_arquivo, int* num_vertices, int* num_arestas) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    // Ler número de vértices
    if (fscanf(arquivo, "%d", num_vertices) != 1) {  
        fprintf(stderr, "Erro ao ler o número de vértices\n");
        exit(EXIT_FAILURE);
    }
    printf("Número de vértices lido: %d\n", *num_vertices);

    float** grafo = alocar_matriz_pesos(*num_vertices);
    *num_arestas = 0;

    // Ler arestas e pesos
    int u, v;
    float peso;
    while (fscanf(arquivo, "%d %d %f", &u, &v, &peso) == 3) {
        printf("Lendo aresta: %d -> %d (peso: %.2f)\n", u, v, peso);
        u--; v--;

        if (u < 0 || u >= *num_vertices || v < 0 || v >= *num_vertices) {
            fprintf(stderr, "Erro: vértice fora do intervalo! (%d, %d)\n", u, v);
            exit(EXIT_FAILURE);
        }

        if (grafo[u][v] == 0.0) {
            (*num_arestas)++;
        }
        grafo[u][v] = peso;
        grafo[v][u] = peso;
        printf("Total de arestas até agora: %d\n", *num_arestas);
    }

    fclose(arquivo);
    return grafo;
}



// Função para imprimir a matriz de adjacência ponderada
void imprimir_matriz_pesos(float** grafo, int num_vertices) {
    printf("Matriz de Adjacência com Pesos:\n");
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            printf("%.2f ", grafo[i][j]);
        }
        printf("\n");
    }
}

// QUESTÃO 2
// ---------- Dijkstra

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <time.h>

void dijkstra(float** grafo, int num_vertices, int inicio, int destino, float* distancia, int* predecessor) {
    bool* visitado = (bool*)calloc(num_vertices, sizeof(bool));

    for (int i = 0; i < num_vertices; i++) {
        distancia[i] = FLT_MAX; // Inicializa com "infinito"
        predecessor[i] = -1;    // Sem predecessor
    }

    distancia[inicio] = 0;

    for (int count = 0; count < num_vertices - 1; count++) {
        int u = -1;
        for (int v = 0; v < num_vertices; v++) {
            if (!visitado[v] && (u == -1 || distancia[v] < distancia[u])) {
                u = v;
            }
        }

        if (u == -1) break; // Não há mais vértices alcançáveis

        visitado[u] = true;

        for (int v = 0; v < num_vertices; v++) {
            if (grafo[u][v] != 0.0 && distancia[u] != FLT_MAX && distancia[u] + grafo[u][v] < distancia[v]) {
                distancia[v] = distancia[u] + grafo[u][v];
                predecessor[v] = u;
            }
        }
    }

    free(visitado);
}

void reconstruir_caminho(int destino, int* predecessor, int* caminho, int* tamanho_caminho) {
    int atual = destino;
    *tamanho_caminho = 0;

    while (atual != -1) {
        caminho[(*tamanho_caminho)++] = atual;
        atual = predecessor[atual];
    }

    // Inverter o caminho (para ter do início ao destino)
    for (int i = 0; i < *tamanho_caminho / 2; i++) {
        int temp = caminho[i];
        caminho[i] = caminho[*tamanho_caminho - 1 - i];
        caminho[*tamanho_caminho - 1 - i] = temp;
    }
}

void encontrar_distancia_caminho(const char* nome_arquivo, int inicio, int destino) {
    int num_vertices, num_arestas;
    float** grafo_ponderado = ler_grafo_ponderado(nome_arquivo, &num_vertices, &num_arestas);

    float* distancia_ponderado = (float*)malloc(num_vertices * sizeof(float));
    int* predecessor_ponderado = (int*)malloc(num_vertices * sizeof(int));
    int* caminho = (int*)malloc(num_vertices * sizeof(int));
    int tamanho_caminho;

    if (inicio < 1 || inicio > num_vertices || destino < 1 || destino > num_vertices) {
        printf("Erro: Vertices de inicio ou destino invalidos.\n");
        liberar_matriz_pesos(grafo_ponderado, num_vertices);
        free(distancia_ponderado);
        free(predecessor_ponderado);
        free(caminho);
        return;
    }

    inicio--; // Ajustar para índice baseado em 0
    destino--;

    dijkstra(grafo_ponderado, num_vertices, inicio, destino, distancia_ponderado, predecessor_ponderado);
    reconstruir_caminho(destino, predecessor_ponderado, caminho, &tamanho_caminho);

    if (tamanho_caminho == 0) {
        printf("Nao existe caminho entre %d e %d.\n", inicio + 1, destino + 1);
    } else {
        printf("Distancia entre %d e %d: %.2f\n", inicio + 1, destino + 1, distancia_ponderado[destino]);
        printf("Caminho: ");
        for (int i = 0; i < tamanho_caminho; i++) {
            printf("%d ", caminho[i] + 1); // Imprimir vértices + 1
        }
        printf("\n");
    }

    liberar_matriz_pesos(grafo_ponderado, num_vertices);
    free(distancia_ponderado);
    free(predecessor_ponderado);
    free(caminho);
}


// Função principal
int main() {
    const char* nome_arquivo = "teste.txt";
    const char* bfs_saida = "bfs_arvore.txt";
    const char* dfs_saida = "dfs_arvore.txt";
    const char* info_saida = "informacoes_grafo.txt";
    const char* componentes_saida = "componentes_conexos.txt"; 


    int num_vertices, num_arestas;
    /*int** grafo = ler_grafo(nome_arquivo, &num_vertices, &num_arestas);

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

    printf("Processamento concluido!\n");*/

    // Grafo com peso ****************************************
    const char* nome_arquivo_entrada = "teste.txt";
    const char* nome_arquivo_saida = "informacoes_grafo_ponderado.txt";

    int num_verticesp,  num_arestasp;
    int opcao;
    printf("Escolha o tipo de grafo:\n");
    printf("1 - Grafo não ponderado\n");
    printf("2 - Grafo ponderado\n");
    printf("Opção: ");
    scanf("%d", &opcao);
    
    if (opcao == 1) {
        int** grafo = ler_grafo(nome_arquivo, &num_vertices, &num_arestas);
        printf("Grafo não ponderado carregado com %d vértices e %d arestas.\n", num_vertices, num_arestas);
        liberar_matriz(grafo, num_vertices);
    } else if (opcao == 2) {
        float** grafop = ler_grafo_ponderado(nome_arquivo, &num_verticesp, &num_arestasp);
        printf("Grafo ponderado carregado com %d vértices e %d arestas.\n", num_verticesp, num_arestasp);
        imprimir_matriz_pesos(grafop, num_verticesp);
          
        int origem, destino;
        printf("Digite o vértice de origem: ");
        scanf("%d", &origem);
        printf("Digite o vértice de destino: ");
        scanf("%d", &destino);
        
        origem--; destino--;  // Ajustar para índice base 0
    
        encontrar_distancia_caminho(nome_arquivo, origem, destino);
        liberar_matriz_pesos(grafop, num_vertices);
    } else {
        printf("Opção inválida!\n");
    }
      
    return 0;
}