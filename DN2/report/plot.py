import matplotlib.pyplot as plt

def plot_epsilon_vs_time(data, title="FPTAS Runtime vs Epsilon", save_path=None):
    if not data:
        raise ValueError("Data must not be empty.")
    
    data.sort()
    epsilons, times = zip(*data)

    plt.figure(figsize=(8, 5))
    plt.plot(epsilons, times, marker='o', linestyle='-', color='blue')
    plt.xlabel("Epsilon (ε)")
    plt.ylabel("Time (seconds)")
    plt.title(title)
    plt.grid(True)
    plt.tight_layout()

    if save_path:
        plt.savefig(save_path)
    else:
        plt.show()

data = [
    (0.10, 0.58324),
    (0.20, 0.60114),
    (0.30, 0.60394),
    (0.40, 0.60614),
    (0.50, 0.60483),
    (0.60, 0.55744),
    (0.70, 0.52758),
    (0.80, 0.49613),
    (0.90, 0.46998),
    (1.00, 0.45214),
    (1.10, 0.42886),
    (1.20, 0.41427),
    (1.30, 0.39493),
    (1.40, 0.38685),
    (1.50, 0.37025),
    (1.60, 0.35601),
    (1.70, 0.34534),
    (1.80, 0.33583),
    (1.90, 0.33855),
    (2.00, 0.33254),
    (2.10, 0.31832),
    (2.20, 0.30979),
    (2.30, 0.30201),
    (2.40, 0.29433),
    (2.50, 0.29080),
    (2.60, 0.28431),
    (2.70, 0.27066),
    (2.80, 0.26499),
    (2.90, 0.25929),
    (3.00, 0.25698)
]

plot_epsilon_vs_time(data, save_path="plot.png")