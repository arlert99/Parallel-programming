import numpy as np
import sys
import os

def read_matrix_from_file(filename):
    if not os.path.exists(filename):
        print(f"Ошибка: Файл {filename} не найден!")
        return None
    
    with open(filename, 'r') as f:
        lines = f.readlines()
        
    n = int(lines[0].strip())

    matrix = []
    for i in range(1, n + 1):
        row = list(map(int, lines[i].split()))
        matrix.append(row)
        
    return np.array(matrix, dtype=np.int64)

def verify(matrix_size):
    print(f"--- Верификация для матриц размером {matrix_size}x{matrix_size} ---")
    
    file_A = f"matrix_A_{matrix_size}.txt"
    file_B = f"matrix_B_{matrix_size}.txt"
    file_C_cpp = f"result_{matrix_size}.txt"
    
    A = read_matrix_from_file(file_A)
    B = read_matrix_from_file(file_B)
    C_cpp = read_matrix_from_file(file_C_cpp)
    
    if A is None or B is None or C_cpp is None:
        print("Не удалось прочитать файлы. Проверьте пути и названия.")
        return

    print(f"Матрицы прочитаны. Размер A: {A.shape}, B: {B.shape}, C_cpp: {C_cpp.shape}")

    print("Вычисление эталона через numpy...")
    C_numpy = np.dot(A, B)

    if np.array_equal(C_cpp, C_numpy):
        print("Результаты C++ и Python (numpy) совпадают")
        print("Верификация пройдена.")
    else:
        print("Результаты не совпадают!")
        diff = np.abs(C_cpp - C_numpy)
        max_diff = np.max(diff)
        error_indices = np.where(diff > 0)
        
        print(f"Максимальное расхождение: {max_diff}")
        print(f"Количество несовпадающих элементов: {len(error_indices[0])}")
        
        print("Примеры ошибок (строка, столбец): C++ vs NumPy")
        for i in range(min(5, len(error_indices[0]))):
            r, c = error_indices[0][i], error_indices[1][i]
            print(f"  [{r}][{c}]: {C_cpp[r][c]} vs {C_numpy[r][c]}")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        size = int(sys.argv[1])
        verify(size)
    else:
        print("Размер не указан. Проверяем 200x200 по умолчанию.")
        print("Чтобы указать размер, запустите: python verify.py 400")
        verify(200)