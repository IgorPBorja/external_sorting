import re
import glob
import os
import matplotlib.pyplot as plt

from pathlib import Path

def path_pattern(method: str):
    input_p = str(Path(__file__).parents[1] / "data" / "alpha" / (method + "*"))
    output_p = str(Path(__file__).parents[1] / "data" / f"{method}.pdf")
    return input_p, output_p

# METHODS = ["polyphasic", "balanced", "cascade"]
METHODS = ["balanced"]
NAMES = {
    "polyphasic": "Ordenação polifásica",
    "balanced": "Ordenação balanceada por p caminhos",
    "cascade": "Ordenação em cascata",
}

for met in METHODS:
    input_p, output_p = path_pattern(met)
    regex = r"[0-9]+"
    data = {}
    for path in sorted(glob.glob(input_p)):
        basepath = os.path.basename(path)
        match = re.search(regex, basepath)
        num_files = int(basepath[match.start():match.end()])
        with open(path, "r") as f:
            lines = f.readlines()
        r_values = [int(line.split(":")[0].strip()) for line in lines]
        alpha_values = [float(line.split(":")[1].strip()) for line in lines]
        data[num_files] = (r_values, alpha_values)
    for num_files in sorted(data.keys()):
        r_values, alpha_values = data[num_files]
        plt.plot(r_values, alpha_values, label=f"Com {num_files} arquivos (total)")
    plt.xlabel(r"$r$: quantidade de runs")
    plt.ylabel(r"$\alpha(r)$: média de escritas por elemento")
    plt.legend()
    plt.title(NAMES[met])
    plt.savefig(output_p)
