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
	TNum x = .1;
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

/*
========================
APPROXIMATION POLYNOMIAL
========================
*/

/*
=============
DIFFERENCIALS
=============
*/

/*
=========
INTEGRATE
=========
*/




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
			//
			break;
		case 3:
			//
			break;
		case 4:
			//
			break;
		case 5:
			//
			break;
		case '\0':
			break;
		default:
			cout << INCORRECT_SELECTION << endl;
			break;
	}
	return 0;
}
