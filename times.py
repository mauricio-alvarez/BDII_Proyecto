import matplotlib.pyplot as plt
import numpy as np

# Insert

# N = 10 
# Scan all: 2 microseconds
# Hash File: 6650 microseconds
# N = 100
# Scan all: 4 microseconds
# Hash File: 165407 microseconds
# N = 1000
# Scan all: 32 microseconds
# Hash File: 2738617 microseconds
# N = 10000
# Scan all: 45 microseconds
# Hash File: 24893107 microseconds

# Hash File (Insertion)
x1 = np.array([10, 100, 1000, 10000])
y1 = np.array([358, 373, 2610, 383])

# (Searh)
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
plt.savefig('chart.png')