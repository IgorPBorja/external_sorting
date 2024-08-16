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
plt.scatter(m_values, averages, color='blue', label='Average over 10 different data arrays')

# Box plot for standard deviation
plt.errorbar(m_values, averages, yerr=std_devs, fmt='o', color='red', label='Standard deviation (as error bars)')

# Adding labels and title
plt.xlabel(r'Memory size $m$')
plt.ylabel(r'Metric $\beta(m, 0)$')
# FIXME is it better with this scale that starts at 0, or the natural scale that starts close to 1.9x?
plt.ylim(0, 5.0)
plt.xticks([5 * k for k in range(1, 20 + 1)])
plt.title(r'Plot of relative (to memory size) average run length $\beta(m, 0)$ from the initial distribution of runs')
plt.legend()

# Display the plot
plt.show()
