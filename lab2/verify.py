import numpy as np
import sys
import os

def read_matrix_from_file(filename):
    if not os.path.exists(filename):
        print(f"Ошибка: Файл {filename} не найден")
        return None
    
    with open(filename, 'r') as f:
        lines = f.readlines()
        
    n = int(lines[0].strip())

    matrix = []
    for i in range(1, n + 1):
        row = list(map(int, lines[i].split()))
        matrix.append(row)
        
    return np.array(matrix, dtype=np.int64)

def verify(matrix_size, threads_list=(1, 2, 4, 8)):
    print(f"Верификация для матриц {matrix_size}x{matrix_size}")
    
    file_A = f"matrix_A_{matrix_size}.txt"
    file_B = f"matrix_B_{matrix_size}.txt"
    
    A = read_matrix_from_file(file_A)
    B = read_matrix_from_file(file_B)
    
    if A is None or B is None:
        print("Не удалось прочитать исходные матрицы")
        return
    
    C_numpy = np.dot(A, B)
    
    all_passed = True
    for t in threads_list:
        file_C = f"result_{t}.txt"
        C_cpp = read_matrix_from_file(file_C)
        
        if C_cpp is None:
            print(f"{file_C}: файл отсутствует")
            all_passed = False
            continue
        
        if np.array_equal(C_cpp, C_numpy):
            print(f"{file_C}: совпадает с numpy")
        else:
            diff = np.abs(C_cpp - C_numpy)
            max_diff = np.max(diff)
            n_errors = np.sum(diff > 0)
            print(f"{file_C}: не совпадает, макс. расхождение {max_diff}, ошибок {n_errors}")
            all_passed = False
    
    if all_passed:
        print("Верификация пройдена для всех потоков")
    else:
        print("Верификация не пройдена")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        size = int(sys.argv[1])
    else:
        size = 200
        print("Размер не указан, проверка 200x200")
    
    verify(size)