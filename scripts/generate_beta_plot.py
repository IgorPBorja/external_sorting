import matplotlib.pyplot as plt
import numpy as np

from pathlib import Path

# Initialize lists to hold m values and their corresponding averages and standard deviations
m_values = []
averages = []
std_devs = []

# Define your input data
with open(Path(__file__).parents[1] / "data" / "beta.txt", "r") as f:
    data = f.read()

# Split the data into lines and process each pair
lines = data.strip().split("\n")

for i in range(0, len(lines), 2):
    m = int(lines[i].rstrip(":"))  # Extract the value of m
    a_values = list(map(float, lines[i+1].split()))  # Extract the a1, ..., a10 values

    # Calculate the average and standard deviation
    avg_a = np.mean(a_values)
    std_a = np.std(a_values)

    # Append to the lists
    m_values.append(m)
    averages.append(avg_a)
    std_devs.append(std_a)

# Plotting
plt.figure(figsize=(10, 6))

# Scatter plot for m vs average(a)
plt.plot(m_values, averages, color='blue', label=f'Média sobre 10 arquivos diferentes')

# Box plot for standard deviation
plt.errorbar(m_values, averages, yerr=std_devs, fmt='o', color='red', label='Desvio padrão (no formato de error bars)')

# Adding labels and title
plt.xlabel(r'Tamanho de memória $m$')
plt.ylabel(r'Métrica $\beta(m, 0)$')
# FIXME is it better with this scale that starts at 0, or the natural scale that starts close to 1.9x?
plt.xticks(m_values)
plt.ylim(1.9, 2.1)
plt.title(r'Plot de tamanho médio relativo (ao tamanho de memória) das runs $\beta(m, 0)$'
          + '\n a partir da distribuição inicial (seleção natural)')
plt.legend()

# Display the plot
plt.savefig("data/beta.png")
