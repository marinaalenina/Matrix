#include <iostream>

using namespace std;


class Matrix {
public:
	int size;
	double** matrix;

	Matrix() {
		size = 0;
		matrix = nullptr;
	}

	Matrix(int size) {
		this->size = size;
		matrix = (double**) new double* [size];
		for (int i = 0; i < size; i++)
			matrix[i] = (double*) new double[size + 1];
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size + 1; j++)
				matrix[i][j] = 0;
	}

	~Matrix() {
		if (size > 0)
			for (int i = 0; i < size; i++)
				delete[] matrix[i];
			delete[] matrix;
	}

	int get_matrix_el(int i, int j) {
		if (size > 0)
			return matrix[i][j];
		else
			return 0;
	}

	void set_matrix_el(int i, int j, int value) {
		if ((i < 0) || (i >= size))
			return;
		if ((j < 0) || (j >= size + 1))
			return;
		matrix[i][j] = value;
	}

	void print() {
		cout << "Matrix: " << endl;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size + 1; j++)
				cout << matrix[i][j] << "\t";
			cout << endl;
		}
		cout << "---------------------" << endl << endl;
	}
	
	void print_equations() {
		cout << "Equations: " << endl;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size + 1; j++) {
				if (size == j)
					cout << " = " << matrix[i][j] << endl;
				else if (j == 0 && matrix[i][j] != 0)
					cout << matrix[i][j] << " * x" << j + 1;
				else if (matrix[i][j] > 0)
					cout << " + " << matrix[i][j] << " * x" << j + 1;
				else if (matrix[i][j] < 0)
					cout << " - " << -matrix[i][j] << " * x" << j + 1;
			}
		}
		cout << "---------------------" << endl << endl;
	}

	double* calculate() {
		Matrix mat(size);
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size + 1; j++)
				mat.set_matrix_el(i, j, matrix[i][j]);
		mat.print();
		double* ans = new double[size];
		for (int i = 0; i < size + 1; i++) {
			if (matrix[0][0] > 0)
				mat.set_matrix_el(0, i, matrix[0][i] / matrix[0][0]);
			else
				mat.set_matrix_el(0, i, matrix[0][i] / (-matrix[0][0]));
		}
		for (int i = 0; i < size - 1; i++) {
			for (int k = 1; k < size - i; k++) {
				for (int j = i; j < size + 1; j++) {
					double value = mat.get_matrix_el(i, j) * (-abs(matrix[i + k][i])) + matrix[i + k][j];
					mat.set_matrix_el(i + k, j, value);
					if (value != 0)
						matrix[i + k][j] = value;
				}
				mat.print();
			}
		}
		double s = 0;
		int j = 0;
		for (int i = size - 1; i >= 0; i--) {
			for (s = 0, j = i + 1; j < size + 1; j++)
				s += mat.get_matrix_el(i, j) * ans[j];
			ans[i] = (mat.get_matrix_el(i, size) - s) / mat.get_matrix_el(i, i);
		}
		return ans;
	}

	double* iter() {
		double** E;
		E = new double* [size];
		for (int i = 0; i < size; i++) {
			E[i] = new double[size + 1];
		}
		double** A;
		A = new double* [size];
		for (int i = 0; i < size; i++) {
			A[i] = new double[size];
		}
		Matrix mat(size);
		double* main_koef;
		main_koef = new double[size];
		double* B;
		B = new double[size];

		//Заполнение единичной матрицы
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size + 1; j++) {
				if (i == j)
					E[i][j] = 1;
				else
					E[i][j] = 0;
			}
		}

		for (int i = 0; i < size; i++) {
			main_koef[i] = matrix[i][i];
		}

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size + 1; j++) {
				A[i][j] = matrix[i][j] / main_koef[i];
			}
		}

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size + 1; j++) {
				A[i][j] = E[i][j] - A[i][j];
			}
		}

		for (int i = 0; i < size; i++) {
			B[i] = matrix[i][size] / main_koef[i];
		}

		double* x_prev = new double[size];
		double* x_next = new double[size];
		for (int i = 0; i < size; i++) {
			x_prev[i] = 1;
			x_next[i] = 0;
		}
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size + 1; j++) {
				x_next[i] += A[i][j] * x_prev[j];
			}
			x_next[i] += B[i];
		}
		int counter = 0;
		while (counter < 1000) { //abs(Norma(x_next) - abs(Norma(x_prev))) >= 0.0001
			double* tmp_x;
			tmp_x = new double[size];
			for (int i = 0; i < size; i++) {
				tmp_x[i] = x_next[i];
			}

			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size + 1; j++) {
					x_next[i] += A[i][j] * x_prev[j];
				}
				x_next[i] += B[i];
			}

			for (int i = 0; i < size; i++) {
				x_prev[i] = tmp_x[i];
			}
			counter++;
		}
		return x_next;
	}
};