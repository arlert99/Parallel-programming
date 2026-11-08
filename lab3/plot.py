import csv
import matplotlib.pyplot as plt

# Читаем данные из CSV
data = []
with open('results_mpi.csv', 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        data.append((int(row['size']), int(row['threads']), float(row['time'])))

sizes = sorted(set(d[0] for d in data))
threads_list = [1, 2, 4, 8]

# --- ГРАФИК 1: Время от количества процессов ---
plt.figure(figsize=(10, 6))
for size in sizes:
    times = [next(d[2] for d in data if d[0] == size and d[1] == t) for t in threads_list]
    plt.plot(threads_list, times, marker='o', label=f'N={size}')

plt.title('MPI: Execution Time vs Number of Processes')
plt.xlabel('Number of Processes')
plt.ylabel('Time (seconds)')
plt.xticks(threads_list)
plt.yscale('log')
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig('graph_time_mpi.png', dpi=150)
plt.close()

# --- ГРАФИК 2: Ускорение от количества процессов ---
plt.figure(figsize=(10, 6))
for size in sizes:
    times = [next(d[2] for d in data if d[0] == size and d[1] == t) for t in threads_list]
    t1 = times[0]
    speedups = [t1 / t for t in times]
    plt.plot(threads_list, speedups, marker='s', label=f'N={size}')

plt.plot(threads_list, threads_list, 'k--', alpha=0.4, label='Ideal Speedup')

plt.title('MPI: Speedup vs Number of Processes')
plt.xlabel('Number of Processes')
plt.ylabel('Speedup S = T1 / Tn')
plt.xticks(threads_list)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig('graph_speedup_mpi.png', dpi=150)
plt.close()

print("Графики построены: graph_time_mpi.png, graph_speedup_mpi.png")