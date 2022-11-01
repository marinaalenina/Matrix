#include <iostream>
#include <math.h>
#define e 0.005

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

	double get_matrix_el(int i, int j) {
		if (size > 0)
			return matrix[i][j];
		else
			return 0;
	}

	void set_matrix_el(int i, int j, double value) {
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
		mat.print();
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
			if (abs(mat.get_matrix_el(i + 1, i + 1)) != 1) {
				for (int p = i + 1; p < size + 1; p++) {
					if (matrix[i + 1][0] > 0)
						mat.set_matrix_el(i + 1, p, matrix[i + 1][p] / matrix[i + 1][i + 1]);
					else
						mat.set_matrix_el(i + 1, p, matrix[i + 1][p] / (-matrix[i + 1][i + 1]));
				}
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

	//void Preob() {
	//	double temp = 0;
	//	Matrix mat(size);
	//	for (int i = 0; i < size; i++) {
	//		temp = matrix[i][i] * (-1);
	//		for (int j = 0; j < size + 1; j++) {
	//			mat.set_matrix_el(i, j, matrix[i][j] / temp);
	//			if (i == j)
	//				mat.set_matrix_el(i, j, 0);
	//			if(j == size)
	//				mat.set_matrix_el(i, j, mat.get_matrix_el(i, j) * (-1));
	//		}
	//	}
	//}
	double Norma(double** matrix) {
		double sum = 0, max = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				sum += fabs(matrix[i][j]);
				if (sum > max)
					max = sum;
			}
			sum = 0;
		}
		return max;
	}

	double Pogr(double** matrix) {
		double eps = 0;
		double norm = Norma(matrix);
		eps = ((1 - norm) / norm) * e;
		return eps;
	}

	void Itera() {
		double* x = new double[size];
		double* x0 = new double[size];
		double* E = new double[size];
		double max = 0, per = 0;
		double temp = 0;
		Matrix mat(size);
		for (int i = 0; i < size; i++) {
			temp = matrix[i][i] * (-1);
			for (int j = 0; j < size + 1; j++) {
				mat.set_matrix_el(i, j, matrix[i][j] / temp);
				if (i == j)
					mat.set_matrix_el(i, j, 0);
				if (j == size)
					mat.set_matrix_el(i, j, mat.get_matrix_el(i, j) * (-1));
			}
		}
		per = Pogr(mat.matrix);
		for (int i = 0; i < size; i++)
			x0[i] = mat.get_matrix_el(i, size);
		int counter = 0;
		do {
			for (int i = 0; i < size; i++) {
				x[i] = 0;
				for (int j = 0; j < size; j++) {
					x[i] += mat.get_matrix_el(i, j) * x0[j];
				}
				x[i] += mat.get_matrix_el(i, size);
				E[i] = fabs(x[i] - x0[i]);
			}
			max = 0;
			for (int i = 0; i < size; i++) {
				if (max < E[i])
					max = E[i];
				x0[i] = x[i];
			}
			cout << endl << "max = " << max;
			counter++;
		} while (max > per);
		cout << endl << "Count iter: " << counter << endl;
		for (int i = 0; i < size; i++)
			cout << "x" << i + 1 << "=" << x[i] << " " << endl;
		delete[] x;
		delete[] x0;
		delete[] E;
	}
};