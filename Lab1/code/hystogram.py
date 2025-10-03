import matplotlib.pyplot as plt
import numpy as np
from matplotlib import colors

data = []
lim = 201

with open("ddice.txt") as file:
    for i in range(lim-1):
        data.append(float(file.readline()))
data = np.array(data)

def hyst():
    norm = plt.Normalize(min(data), max(data))
    cmap = plt.cm.viridis
    # Создание гистограммы
    plt.figure(figsize=(10, 6))
    bars = plt.bar(range(1, lim), data, 
                color=cmap(norm(data)),
                edgecolor='black', 
                linewidth=1.2)
    
    plt.xticks(
    range(1, lim, 7),
    range(1, lim, 7),       
    ha='right',           # Выравнивание по правому краю
    fontsize=9,           # Размер шрифта
    fontweight='bold'     # Жирность шрифта
    )
    # Настройка внешнего вида
    plt.title('Гистограмма для DDice', fontsize=14, fontweight='bold')
    plt.xlabel('Значение', fontsize=12)
    plt.ylabel('Вероятность', fontsize=12)
    plt.grid(axis='y', alpha=0.3)
    plt.ylim(0, max(data) * 1.1)
    plt.tight_layout()
    plt.savefig("ddice.png")

hyst()
