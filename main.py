def ler_grafo(nome_arquivo):
    with open(nome_arquivo, 'r') as arquivo:
        # lê a primeira linha e extrai o número de vértices e arestas
        num_vertices, num_arestas = map(int, arquivo.readline().strip().split())
        matriz_adjacencia = [[0 for _ in range(num_vertices)] for _ in range(num_vertices)]

        for linha in arquivo:
            u, v = map(int, linha.strip().split())
            u -= 1 
            v -= 1

            # marca a existência de conexão entre os vértices
            matriz_adjacencia[u][v] = 1
            matriz_adjacencia[v][u] = 1

        return num_vertices, num_arestas, matriz_adjacencia

def print_grafo(grafo, num_vertices, num_arestas):
    #matriz de adjacência
    if grafo:
        print("\nMariz de Adjacencia:")
        print(f"Número de vértices: {num_vertices}")
        print(f"Número de arestas: {num_arestas}")
        
        # Exibe a matriz de adjacência
        print("\nMatriz de Adjacência:")
        for i, linha in enumerate(grafo):
            print(f"Vértice {i+1}: {linha}")  

    #lista de adjacência
    #essa lista mostra as conexões que um vertíce x possui.
    lista_adjacencia = [[] for _ in range(num_vertices)]
    for u in range(num_vertices):
        for v in range(num_vertices):
            if grafo[u][v] == 1:  
                lista_adjacencia[u].append(v + 1)  
        
        # Exibe a lista de adjacência
    print("\nLista de Adjacencia:")
    for i, vizinhos in enumerate(lista_adjacencia):
        print(f"Vértice {i+1}: {vizinhos}")

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


def main():
    # questão 1
    nome_arquivo = "teste.txt"
    num_vertices, num_arestas, grafo = ler_grafo(nome_arquivo)
    # questão 3
    print_grafo(grafo, num_vertices, num_arestas)
    # questão 5
    componentes = componentes_conexos(grafo, num_vertices)
    print("\nComponentes Conexos:")
    print(f"Número de componentes conexos: {len(componentes)}")
    for i, componente in enumerate(componentes):
        print(f"Componente {i+1} tem {len(componente)} vertices : {componente}")
if __name__ == "__main__":
    main()
