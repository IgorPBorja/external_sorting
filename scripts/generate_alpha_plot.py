import re
import glob
import os
import matplotlib.pyplot as plt
import argparse

from pathlib import Path

def path_pattern(method: str):
    input_p = str(Path(__file__).parents[1] / "data" / "alpha" / (method + "*"))
    output_p = str(Path(__file__).parents[1] / "data" / f"{method}.png")
    return input_p, output_p

def parse_methods():
    parser = argparse.ArgumentParser()
    parser.add_argument("--all", action="store_true", help="Plot all graphs")
    parser.add_argument("--balanced", action="store_true", help="Plot graph for p-way balanced sort")
    parser.add_argument("--polyphasic", action="store_true", help="Plot graph for polyphasic sort")
    parser.add_argument("--cascade", action="store_true", help="Plot graph for cascade sort")
    args = parser.parse_args()
    methods = []
    if args.all:
        return ["balanced", "polyphasic", "cascade"]
    elif args.balanced:
        methods.append("balanced")
    elif args.polyphasic:
        methods.append("polyphasic")
    elif args.cascade:
        methods.append("cascade")
    print(f"Running methods: ", ", ".join(methods) if methods else None)
    return methods

METHODS = parse_methods()

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
