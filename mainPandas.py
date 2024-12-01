from collections import deque
import pandas as pd

def ler_grafo(nome_arquivo, chunk_size=1000):
    with open(nome_arquivo, 'r') as arquivo:
        # Lê a primeira linha e extrai o número de vértices
        num_vertices = int(arquivo.readline().strip())
        matriz_adjacencia = [[0 for _ in range(num_vertices)] for _ in range(num_vertices)]
        num_arestas = 0

    # Usa pandas para processar as demais linhas em chunks
    colunas = ['u', 'v']
    for chunk in pd.read_csv(nome_arquivo, delim_whitespace=True, skiprows=1, names=colunas, chunksize=chunk_size, header=None):
        for _, row in chunk.iterrows():
            u, v = row['u'] - 1, row['v'] - 1

            # Marca a existência de conexão entre os vértices
            if matriz_adjacencia[u][v] == 0:  # Evita contar arestas duplicadas
                num_arestas += 1
            matriz_adjacencia[u][v] = 1
            matriz_adjacencia[v][u] = 1

    return num_vertices, num_arestas, matriz_adjacencia


def print_grafo(grafo, num_vertices, num_arestas):
    #matriz de adjacência
    #if grafo:
        #print("\nMariz de Adjacencia:")
        #print(f"Número de vértices: {num_vertices}")
        #print(f"Número de arestas: {num_arestas}")
        
        # Exibe a matriz de adjacência
        #print("\nMatriz de Adjacência:")
        #for i, linha in enumerate(grafo):
        #    print(f"Vértice {i+1}: {linha}")  

    #lista de adjacência
    #essa lista mostra as conexões que um vertíce x possui.
    lista_adjacencia = [[] for _ in range(num_vertices)]
    for u in range(num_vertices):
        for v in range(num_vertices):
            if grafo[u][v] == 1:  
                lista_adjacencia[u].append(v + 1)  
        
        # Exibe a lista de adjacência
    #print("\nLista de Adjacencia:")
    #for i, vizinhos in enumerate(lista_adjacencia):
    #    print(f"Vértice {i+1}: {vizinhos}")

def componentes_conexos(grafo, num_vertices):
    visitados = [False] * num_vertices
    componentes = []

    def busca_prof(v, componente):
        visitados[v] = True
        componente.append(v + 1)  
        for vizinho in range(num_vertices):
            if grafo[v][vizinho] == 1 and not visitados[vizinho]:
                busca_prof(vizinho, componente)

    for v in range(num_vertices):
        if not visitados[v]:
            componente_atual = []
            busca_prof(v, componente_atual)
            componentes.append(componente_atual)


    return componentes

# Funções para a questão 2
def calcular_graus(grafo, num_vertices):
    graus = []
    for u in range(num_vertices):
        grau = sum(grafo[u]) 
        graus.append(grau)
    return graus

def salvar_informacoes_grafo(nome_arquivo_saida, num_vertices, num_arestas, graus):
    with open(nome_arquivo_saida, 'w') as arquivo:
        arquivo.write(f"{num_vertices}\n")
        arquivo.write(f"{num_arestas}\n")
        for i, grau in enumerate(graus):
            arquivo.write(f"{i + 1} {grau}\n")

def representacaoGrafo(grafo, num_vertices, num_arestas):
    graus = calcular_graus(grafo, num_vertices)
    nome_arquivo_saida = "informacoes_grafo.txt"
    salvar_informacoes_grafo(nome_arquivo_saida, num_vertices, num_arestas, graus)
    print(f"\nInformações do grafo salvas no arquivo: {nome_arquivo_saida}")

# Funções para a questão 4
def salvar_arvore_busca(nome_arquivo_saida, arvore):
    with open(nome_arquivo_saida, 'w') as arquivo:
        arquivo.write("Vertice Pai Nivel\n")
        for vertice, (pai, nivel) in enumerate(arvore):
            arquivo.write(f"{vertice + 1} {pai + 1 if pai != -1 else 'None'} {nivel}\n")

def busca_em_largura(grafo, num_vertices):
    visitados = [False] * num_vertices
    arvore = [(-1, -1)] * num_vertices  # (pai, nível)
    fila = deque()

    for inicio in range(num_vertices):
        if not visitados[inicio]:
            # Inicia a BFS a partir de um novo componente
            fila.append(inicio)
            visitados[inicio] = True
            arvore[inicio] = (-1, 0)  # Raiz do componente

            while fila:
                atual = fila.popleft()
                for vizinho in range(num_vertices):
                    if grafo[atual][vizinho] == 1 and not visitados[vizinho]:
                        visitados[vizinho] = True
                        fila.append(vizinho)
                        arvore[vizinho] = (atual, arvore[atual][1] + 1)

    salvar_arvore_busca("bfs_arvore.txt", arvore)

def busca_em_profundidade(grafo, num_vertices):
    visitados = [False] * num_vertices
    arvore = [(-1, -1)] * num_vertices  # (pai, nível)

    def dfs(v, pai, nivel):
        visitados[v] = True
        arvore[v] = (pai, nivel)
        for vizinho in range(num_vertices):
            if grafo[v][vizinho] == 1 and not visitados[vizinho]:
                dfs(vizinho, v, nivel + 1)

    for inicio in range(num_vertices):
        if not visitados[inicio]:
            dfs(inicio, -1, 0)  # Inicia uma nova DFS para um componente desconectado

    salvar_arvore_busca("dfs_arvore.txt", arvore)


def main():
    # questão 1
    nome_arquivo = "teste.txt"
    num_vertices, num_arestas, grafo = ler_grafo(nome_arquivo)
    # questão 2
    representacaoGrafo(grafo, num_vertices, num_arestas)
    # questão 3
    print_grafo(grafo, num_vertices, num_arestas)
    # questão 4
    vertice_inicial = 1  
    #print(f"\nO vértice inicial para as buscas será: {vertice_inicial}")
    busca_em_largura(grafo, num_vertices)
    busca_em_profundidade(grafo, num_vertices)
    # questão 5
    componentes = componentes_conexos(grafo, num_vertices)
    #print("\nComponentes Conexos:")
    #print(f"Quantidade de componentes conexos: {len(componentes)}")
    #for i, componente in enumerate(componentes):
    #    print(f"Componente {i+1} tem {len(componente)} vertices : {componente}")
if __name__ == "__main__":
    main()
