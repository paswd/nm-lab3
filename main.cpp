#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include "types.h"

using namespace std;

const string INCORRECT_SELECTION = "Неверная опция";

/*
========================
INTERPOLATION POLYNOMIAL
========================
*/

TNum InterpolationFunc(TNum x) {
	return 1. / tan(x);
}
TNum LagrangeMethod(TNum x, vector <TNum> xi) {
	TNum res = 0.;
	for (size_t i = 0; i < xi.size(); i++) {
		TNum y = InterpolationFunc(xi[i]);
		for (size_t j = 0; j < xi.size(); j++) {
			if (i != j) {
				y *= (x - xi[j]) / (xi[i] - xi[j]);
			}
		}
		res += y;
	}
	return abs(InterpolationFunc(x) - res);
}
TNum NewtonMethod(TNum x, vector <TNum> xi) {
	size_t size = xi.size();
	vector <vector <TNum>> arr(size);
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			if (i == 0) {
				arr[i].push_back(InterpolationFunc(xi[j]));
			} else {
				if (!(i + j < size)) {
					break;
				}
				arr[i].push_back((arr[i - 1][j + 1] - arr[i - 1][j]) / (xi[i + j] - xi[j]));
			}
		}
	}
	TNum res = 0.;
	TNum dx = 1.;
	for (size_t i = 0; i < size; i++) {
		res += arr[i][0] * dx;
		dx *= x - xi[i];
	}
	return abs(InterpolationFunc(x) - res);
}

void InterpolationPoly(void) {
	TNum x = PI / 3.;
	vector <TNum> a({PI/8., 2.*PI/8., 3.*PI/8., 4.*PI/8.});
	vector <TNum> b({PI/8., 5.*PI/16., 3.*PI/8., PI/2.});

	cout << "Метод Лагранжа:" << endl;
	cout << "А)\t" <<  LagrangeMethod(x, a) << endl;
	cout << "Б)\t" <<  LagrangeMethod(x, b) << endl;

	cout << "Метод Ньютона:" << endl;
	cout << "А)\t" <<  NewtonMethod(x, a) << endl;
	cout << "Б)\t" <<  NewtonMethod(x, b) << endl;
}


/*
===========
CUBE SPLINE
===========
*/
vector <TNum> CubeSplineSub(size_t size, vector <TNum> in_a, vector <TNum> in_b, vector <TNum> in_c,
	vector <TNum> in_f) {
	vector <TNum> res(size);
	for (size_t i = 1; i < size; i++) {
		in_c[i] -= in_a[i] * in_b[i - 1] / in_c[i - 1];
		in_f[i] -= in_a[i] * in_f[i - 1] / in_c[i - 1];

	}
	res[size - 1] = in_f[size - 1] / in_c[size - 1];
	for (size_t in = size - 1; in > 0; in--) {
		size_t i = in - 1;
		res[i] = (in_f[i] - in_b[i] * res[i + 1]) / in_c[i];
	}
	return res;
}

void CubeSpline(void) {
	TNum x = 2.66666667;
	//TNum x = .1;
	vector <pair <TNum, TNum>> table(0);
	table.push_back(make_pair(1.0, 2.4142));
	table.push_back(make_pair(1.9, 1.0818));
	table.push_back(make_pair(2.8, .50953));
	table.push_back(make_pair(3.7, .11836));
	table.push_back(make_pair(4.6, -.24008));
	/*table.push_back(make_pair(-0.4, 1.9823));
	table.push_back(make_pair(-0.1, 1.6710));
	table.push_back(make_pair(0.2, 1.3694));
	table.push_back(make_pair(0.5, 1.0472));
	table.push_back(make_pair(0.8, 0.6435));*/

	vector <TNum> a(table.size());
	vector <TNum> b(table.size());
	vector <TNum> c(table.size()); 
	vector <TNum> d(table.size());
	vector <TNum> h(table.size() - 1);

	for (size_t i = 0; i < table.size(); i++) {
		a[i] = table[i].second;
	}
	for (size_t i = 0; i < table.size() - 1; i++) {
		h[i] = table[i + 1].first - table[i].first;
	}

	vector <TNum> sub_a(table.size() - 2);
	vector <TNum> sub_b(table.size() - 2); 
	vector <TNum> sub_c(table.size() - 2);
	vector <TNum> sub_f(table.size() - 2);
	for (size_t i = 0; i < table.size() - 3; i++) {
		sub_b[i] = h[i];
		sub_a[i + 1] = h[i];
	}
	for (size_t i = 0; i < table.size() - 2; i++) {
		sub_c[i] = 2 * (h[i + 1] + h[i]);
	}
	for (size_t i = 1; i < table.size() - 1; i++) {
		sub_f[i - 1] = 3 * ((a[i + 1] - a[i]) / h[i] - (a[i] - a[i - 1]) / h[i - 1]);
	}
	c = CubeSplineSub(table.size() - 2, sub_a, sub_b, sub_c, sub_f);
	//c.insert(c.begin(), 0);
	c[0] = 0;
	for (size_t i = 0; i < h.size() - 1; i++) {
		d[i] = (c[i+1] - c[i]) / (3 * h[i]);
		b[i] = (a[i + 1] - a[i]) / h[i] - h[i] * (c[i + 1] + 2 * c[i]) / 3;
	}
	d[h.size()] = -c.back() / (3 * h.back());
	b[h.size()] = (a.back() - a[a.size() - 2]) / h.back() + 2 * h.back() * c.back() / 3;
	size_t i = d.size() - 1;
	while (table[i].first > x)
		i--;
	x -= table[i].first;
	TNum res = a[i] + x * (b[i] + x * (c[i] + x * d[i]));

	cout << res << endl;
}

/*
========================
APPROXIMATION POLYNOMIAL
========================
*/

vector <TNum> GaussMethod(size_t size, vector <vector <TNum>> a, vector <TNum> b) {
	for (size_t i = 0; i < size; i++) {
		if (a[i][i] == 0.) {
			for (size_t j = i + 1; j < size && a[j][i] == 0.; j++) {
				if (j + 1 != size) {
					swap(a[j + 1], a[i]);
					swap(b[j + 1], b[i]);
				}
			}
		}
		for (size_t j = i + 1; j < size; j++) {
			TNum tmp = a[j][i] / a[i][i];
			if (tmp != 0.) {
				for (size_t k = 0; k < size; ++k) {
					a[j][k] -= a[i][k] * tmp;
				}
				b[j] -= b[i] * tmp;
			}
		}
	}
	for (size_t in = size; in > 0; in--) {
		size_t i = in - 1;
		for (size_t jn = i; jn > 0; jn--) {
			size_t j = jn - 1;
			TNum tmp = a[j][i] / a[i][i];
			if (tmp != 0.0) {
				for (size_t k = 0; k < size; k++) {
					a[j][k] -= a[i][k] * tmp;
				}
				b[j] -= b[i] * tmp;
			}
		}
	}
	for (size_t i = 0; i < size; i++) {
		b[i] /= a[i][i];
	}
	return b;
}

vector <TNum> ApproximationSub(size_t size, vector <pair <TNum, TNum>> table) {
	vector <TNum> Sx(2 * size - 1);
	vector <TNum> Sxy(size);
	for (size_t i = 0; i < table.size(); i++) {
		TNum add = 1.;
		for (size_t j = 0; j < 2 * size - 1; j++) {
			Sx[j] += add;
			add *= table[i].first;
		}
	}
	for (size_t i = 0; i < table.size(); i++) {
		TNum add = table[i].second;
		for (size_t j = 0; j < size; j++) {
			Sxy[j] += add;
			add *= table[i].first;
		}
	}
	vector <vector<TNum>> matrix(size, vector<TNum>(size));
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			matrix[i][j] = Sx[i + j];
		}
	}
	return GaussMethod(size, matrix, Sxy);
}
TNum ApproximationError(vector <TNum> coeff, vector <pair <TNum, TNum>> table) {
	TNum res = 0;
	for (size_t i = 0; i < table.size(); i++) {
		TNum yn = coeff[coeff.size() - 1];
		for (size_t jn = coeff.size() - 1; jn > 0; jn--) {
			size_t j = jn - 1;
			yn = coeff[j] + table[i].first * yn;
		}
		res += pow((table[i].second - yn), 2);
	}
	return sqrt(res);
}
void ApproximationMethod(void) {
	vector <pair <TNum, TNum>> table(0);
	table.push_back(make_pair(1.0, 2.4142));
	table.push_back(make_pair(1.9, 1.0818));
	table.push_back(make_pair(2.8, .50953));
	table.push_back(make_pair(3.7, .11836));
	table.push_back(make_pair(4.6, -.24008));

	vector <TNum> c2 = ApproximationSub(2, table);
	vector <TNum> c3 = ApproximationSub(3, table);
	for (size_t i = 0; i < c2.size(); i++) {
		cout << c2[i] << " ";
	}
	cout << endl;
	cout << "ERROR = " << ApproximationError(c2, table) << endl;
	for (size_t i = 0; i < c3.size(); i++) {
		cout << c3[i] << " ";
	}
	cout << endl;
	cout << "ERROR = " << ApproximationError(c3, table) << endl;
}

/*
=============
DIFFERENCIALS
=============
*/

TNum DiffLeft(TNum x, vector <pair <TNum, TNum>> table) {
	size_t i;
	for (i = 0; i + 1 < table.size() && table[i + 1].first < x; i++);
	return (table[i + 1].second - table[i].second) / (table[i + 1].first - table[i].first);
}
TNum DiffRight(TNum x, vector <pair <TNum, TNum>> table) {
	size_t i;
	for (i = 0; i + 1 < table.size() && table[i].first < x; i++);
	return (table[i + 1].second - table[i].second) / (table[i + 1].first - table[i].first);
}
TNum DiffSecondary(TNum x, vector <pair <TNum, TNum>> table) {
	size_t i;
	for (i = 0; i + 1 < table.size() && table[i].first < x; i++);
	return 2 * (DiffRight(x, table) - DiffLeft(x, table)) / (table[i + 1].first - table[i - 1].first);
}

void DiffMethod(void) {
	TNum x = .2;
	vector <pair <TNum, TNum>> table(0);
	table.push_back(make_pair(0., 1.));
	table.push_back(make_pair(.1, 1.1052));
	table.push_back(make_pair(.2, 1.2214));
	table.push_back(make_pair(.3, 1.3499));
	table.push_back(make_pair(.4, 1.4918));

	cout << "Левая производная:" << endl;
	cout << DiffLeft(x, table) << endl;
	cout << "Правая производная:" << endl;
	cout << DiffRight(x, table) << endl;
	cout << "Вторая производная:" << endl;
	cout << DiffSecondary(x, table) << endl;
}

/*
=========
INTEGRATE
=========
*/

TNum IntegrateFunc(TNum x) {
	return (3 * x + 4) / (2 * x + 7);
}

TNum IntegrateRectangleMethod(TNum a, TNum b, TNum h) {
	TNum res = 0;
	for (TNum x = a; x < b; x += h) {
		res += IntegrateFunc(x + h / 2) * h;
	}
	return res;
}
TNum IntegrateTrapezeMethod(TNum a, TNum b, TNum h) {
	TNum res = 0;
	for (TNum x = a; x < b; x += h) {
		res += h * (IntegrateFunc(x) + IntegrateFunc(x + h)) / 2.;
	}
	return res;
}
TNum IntegrateSimpsonMethod(TNum a, TNum b, TNum h) {
	TNum res = 0;
	for (TNum x = a; x < b; x += h) {
		res += (IntegrateFunc(x) + 4. * IntegrateFunc(x + h / 2.) + IntegrateFunc(x + h)) * h / 6.; 
	}
	return res;
}

void IntegrateMethod(void) {
	TNum a = -2.;
	TNum b = 2.;
	TNum h1 = 1.;
	TNum h2 = .5;

	TNum rect[2] = {
		IntegrateRectangleMethod(a, b, h1),
		IntegrateRectangleMethod(a, b, h2)
	};
	TNum trap[2] = {
		IntegrateTrapezeMethod(a, b, h1),
		IntegrateTrapezeMethod(a, b, h2)
	};
	TNum simpson[2] = {
		IntegrateSimpsonMethod(a, b, h1),
		IntegrateSimpsonMethod(a, b, h2)
	};
	cout << "Метод прямоугольников:" << endl;
	cout << rect[0] << "\t" << rect[1] << "\t" << abs(rect[1] - rect[0]) << endl;
	cout << "Метод трапеций:" << endl;
	cout << trap[0] << "\t" << trap[1] << "\t" << abs(trap[1] - trap[0]) / 3. << endl;
	cout << "Метод Симпсона:" << endl;
	cout << simpson[0] << "\t" << simpson[1] << "\t" << abs(simpson[1] - simpson[0]) / 15. << endl;
}


/*
====
MAIN
====
*/

int main(void) {
	cout << "=================" << endl;
	cout << "Выберите задание:" << endl;
	cout << "1 - Построение интерполяционных многочленов" << endl;
	cout << "2 - Построение кубического сплайна" << endl;
	cout << "3 - Нахождение приближающих многочленов" << endl;
	cout << "4 - Вычисления производных" << endl;
	cout << "5 - Вычисление определённого интеграла" << endl;
	cout << "=================" << endl;

	cout << "Ваш выбор: ";
	size_t selection = 0;
	cin >> selection;
	cout << "=================" << endl;

	switch (selection) {
		case 1:
			InterpolationPoly();
			break;
		case 2:
			CubeSpline();
			break;
		case 3:
			ApproximationMethod();
			break;
		case 4:
			DiffMethod();
			break;
		case 5:
			IntegrateMethod();
			break;
		case '\0':
			break;
		default:
			cout << INCORRECT_SELECTION << endl;
			break;
	}
	return 0;
}
