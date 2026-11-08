#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <iomanip>
#include <mpi.h>

bool readFlatMatrix(const std::string &filename, std::vector<int> &data, int &N)
{
    std::ifstream in(filename);
    if (!in.is_open())
        return false;

    in >> N;
    data.resize(N * N);
    for (int i = 0; i < N * N; ++i)
    {
        if (!(in >> data[i]))
            return false;
    }
    return true;
}

void writeFlatMatrix(const std::string &filename, const std::vector<int> &data, int N)
{
    std::ofstream out(filename);
    out << N << '\n';
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            out << data[i * N + j] << ' ';
        }
        out << '\n';
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 4)
    {
        if (rank == 0)
        {
            std::cerr << "Usage: " << argv[0] << " <file_A> <file_B> <file_result>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    std::string fileA = argv[1];
    std::string fileB = argv[2];
    std::string fileRes = argv[3];

    int N = 0;
    std::vector<int> A, B;

    if (rank == 0)
    {
        std::cout << "Reading matrices from files..." << std::endl;
        if (!readFlatMatrix(fileA, A, N) || !readFlatMatrix(fileB, B, N))
        {
            std::cerr << "Error reading matrices." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::cout << "Matrix size: " << N << "x" << N << std::endl;
        std::cout << "Processes used: " << size << std::endl;
    }

    if (N % size != 0)
    {
        if (rank == 0)
        {
            std::cerr << "Error: Matrix size " << N << " is not divisible by process count " << size << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int rows_per_proc = N / size;
    std::vector<int> local_A(rows_per_proc * N);
    std::vector<int> local_C(rows_per_proc * N);

    if (rank != 0)
    {
        B.resize(N * N);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    auto start = std::chrono::high_resolution_clock::now();

    MPI_Bcast(B.data(), N * N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(A.data(), rows_per_proc * N, MPI_INT,
                local_A.data(), rows_per_proc * N, MPI_INT,
                0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_proc; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            long long sum = 0;
            for (int k = 0; k < N; ++k)
            {
                sum += static_cast<long long>(local_A[i * N + k]) * B[k * N + j];
            }
            local_C[i * N + j] = static_cast<int>(sum);
        }
    }

    std::vector<int> C;
    if (rank == 0)
    {
        C.resize(N * N);
    }
    MPI_Gather(local_C.data(), rows_per_proc * N, MPI_INT,
               C.data(), rows_per_proc * N, MPI_INT,
               0, MPI_COMM_WORLD);

    auto end = std::chrono::high_resolution_clock::now();

    if (rank == 0)
    {
        std::chrono::duration<double> duration = end - start;
        std::cout << "Multiplication time: " << std::fixed << std::setprecision(4) << duration.count() << " seconds" << std::endl;

        writeFlatMatrix(fileRes, C, N);
        std::cout << "Result written to " << fileRes << std::endl;

        std::cout << "RESULT_CSV:" << N << "," << size << "," << duration.count() << std::endl;
    }

    MPI_Finalize();
    return 0;
}