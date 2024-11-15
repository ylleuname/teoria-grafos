def lergrafo(nome_arquivo):
    with open(nome_arquivo, 'r') as arquivo:
        num_vertices = int(arquivo.readline().strip())
            
        # Cria uma matriz de adjacência inicializada com zeros
        matriz_adjacencia = [[0 for _ in range(num_vertices)] for _ in range(num_vertices)]

        for linha in arquivo:
            u, v = map(int, linha.strip().split())
            # Ajusta os índices para começar em 0
            u -= 1
            v -= 1

            # Marca a existência da aresta (grafo não direcionado)
            matriz_adjacencia[u][v] = 1
            matriz_adjacencia[v][u] = 1

        return matriz_adjacencia

def main():
    # Lê o grafo do arquivo e obtém a matriz de adjacência
    nome_arquivo = "teste.txt"  # Substitua pelo nome do seu arquivo
    grafo = lergrafo(nome_arquivo)
 
    if grafo:
        print("Grafo lido com sucesso:")
        for i in range(len(grafo)):
            print(f"Vértice {i+1}: {grafo[i]}")

if __name__ == "__main__":
    main()