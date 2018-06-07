#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include "types.h"

using namespace std;

const string INCORRECT_SELECTION = "Неверная опция";

/*
=========
SECTION 1
=========
*/

/*
=========
SECTION 2
=========
*/

/*
=========
SECTION 3
=========
*/

/*
=========
SECTION 4
=========
*/

/*
=========
SECTION 5
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
			//
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
		case '\0':
			break;
		default:
			cout << INCORRECT_SELECTION << endl;
			break;
	}
	return 0;
}
