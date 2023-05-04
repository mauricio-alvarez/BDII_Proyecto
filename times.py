import matplotlib.pyplot as plt
import numpy as np

# Hash File
# Insertion [358, 373, 2610, 383]
# Search [340, 261, 390, 402]

# Hash File (Insertion)
x1 = np.array([10, 100, 1000, 10000])
y1 = np.array([358, 373, 2610, 383])

# Hash File (Search)
x2 = np.array([10, 100, 1000, 10000])
y2 = np.array([340, 261, 390, 402])

plt.xscale("log")

plt.title("Tiempos de inserción y búsqueda")
plt.xlabel("Número de registros en el archivo")
plt.ylabel("Tiempo de ejecución (microsegundos)")

plt.plot(x1, y1, marker = 's', color = 'green')
plt.plot(x2, y2, marker = 'o', color = 'blue')

plt.legend(["Insert", "Search"])

plt.grid()
plt.savefig('chart2.png')