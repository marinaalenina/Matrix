#include <iostream>
#include <fstream>
#include "Matrix.cpp"

using namespace std;


int main() {
    int n;
    double a;

    // Ввод с клавиатуры
 
    //cout << "Input the number of equations: ";
    //cin >> n;
    //cout << "Input the coefficients for the equation:" << endl;
    //cout << "a, b, c, ... - coefficients" << endl;
    //for (int i = 0; i < n; i++) {
    //    char coeff = 'a';
    //    for (int j = 0; j < n + 1; j++) {
    //        if (j == n)
    //            cout << "answer = ";
    //        else
    //            cout << coeff << " = ";
    //        cin >> a;
    //        M.set_matrix_el(i, j, a);
    //        coeff++;
    //    }
    //    cout << endl;
    //}

    // Чтение данных из файла

    ifstream fin;
    fin.open("input.txt");
    fin >> n;
    Matrix M(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            fin >> a;
            M.set_matrix_el(i, j, a);
        }
    }
    M.print();
    M.print_equations();
    //M.Preob();
    M.print();
    M.Itera();
    double* answer = M.calculate();
    for (int i = 0; i < n; i++)
        cout <<"X" << i + 1 << " = " << answer[i] << "\t";
    cout << endl;
}