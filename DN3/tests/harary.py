import networkx as nx

def generate_harary_graph(k, n):
    G = nx.Graph()
    G.add_nodes_from(range(n))

    for i in range(n):
        for j in range(1, k // 2 + 1):
            G.add_edge(i, (i + j) % n)

    if k % 2 == 1 and n % 2 == 0:
        for i in range(n // 2):
            G.add_edge(i, (i + n // 2) % n)

    return G

if __name__ == "__main__":
    k = 3
    n = 500
    output_path = "harary.graph"

    G = generate_harary_graph(k, n)
    with open(output_path, "w") as f:
        for u, v in G.edges():
            f.write(f"{u} {v}\n")

    print(f"Graph H_{{{k},{n}}} written to '{output_path}' with {G.number_of_edges()} edges.")