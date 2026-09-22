#include <iostream>   
#include <fstream>    
#include <vector>
#include <chrono>     
#include <string>     
#include <iomanip> 

class Matrix {
    int _size = 0;
    std::vector<std::vector<int>> _data;

    public:
    Matrix() = default;
    Matrix(int size): _size(size), _data(size, std::vector<int>(size, 0)) {}

    std::vector<int>& operator[](int i) {
        return _data[i];
    }

    const std::vector<int>& operator[](int i) const {
        return _data[i];
    }

    int size() const{
        return _size;
    }

    Matrix operator*(const Matrix& other) const {
        if (_size != other._size) {
            std::cout << "Need the same size\n";
            return Matrix();
        }
        Matrix prod(_size);
        for (int i = 0; i < _size; ++i)
            for (int j = 0; j < _size; ++j) {
                int s = 0;
                for (int k = 0; k < _size; ++k)
                    s += (*this)[i][k] * other[k][j];
                prod[i][j] = s;
            }
        return prod;
    }

    bool read(std::istream& in) {
        if (!(in >> _size)) return false;
        _data.assign(_size, std::vector<int>(_size, 0));
        for (int i = 0; i < _size; ++i)
            for (int j = 0; j < _size; ++j)
                if (!(in >> _data[i][j])) return false;
        return true;
    }

    void write(std::ostream& out) const {
        out << _size << '\n';
        for (int i = 0; i < _size; ++i) {
            for (int j = 0; j < _size; ++j)
                out << _data[i][j] << ' ';
            out << '\n';
        }
    }

    bool readFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in.is_open()) 
            {
                return false;
            }
        return read(in);
    }

    void writeToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cerr << "Cannot open file for writing: " << filename << std::endl;
            return;
        }
        write(out);
    }
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <file_A> <file_B> <file_result>" << std::endl;
        return 1;
    }

    std::string fileA = argv[1];
    std::string fileB = argv[2];
    std::string fileRes = argv[3];

    Matrix A, B;
    
    std::cout << "Reading matrices from files..." << std::endl;
    if (!A.readFromFile(fileA) || !B.readFromFile(fileB)) {
        std::cerr << "Error reading matrices. Check file format." << std::endl;
        return 1;
    }

    if (A.size() != B.size()) {
        std::cerr << "Matrix sizes mismatch!" << std::endl;
        return 1;
    }

    int N = A.size();
    std::cout << "Matrix size: " << N << "x" << N << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    
    Matrix C = A * B;
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Multiplication time: " << std::fixed << std::setprecision(4) << duration.count() << " seconds" << std::endl;
    
    C.writeToFile(fileRes);
    std::cout << "Result written to " << fileRes << std::endl;

    std::cout << "RESULT_CSV:" << N << "," << duration.count() << std::endl;

    return 0;
}
   