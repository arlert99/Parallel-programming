import numpy as np
import sys

def generate(size):
    A = np.random.randint(1, 10, size=(size, size))
    B = np.random.randint(1, 10, size=(size, size))
    for name, mat in [("matrix_A", A), ("matrix_B", B)]:
        filename = f"{name}_{size}.txt"
        with open(filename, 'w') as f:
            f.write(f"{size}\n")
            for row in mat:
                f.write(" ".join(map(str, row)) + "\n")
        print(f"Generated {filename}")

if __name__ == "__main__":
    size = int(sys.argv[1])
    generate(size)