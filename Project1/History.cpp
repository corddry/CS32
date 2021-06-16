#include "History.h"
#include "globals.h"
#include <iostream>

using namespace::std;

History::History(int nRows, int nCols){
	m_nRows = nRows;
	m_nCols = nCols;
	for (int r = 0; r < m_nRows; r++) {
		for (int c = 0; c < m_nCols; c++) {
			arr[r][c] = 0;
		}
	}
}

bool History::record(int r, int c) {
	if (r > (m_nRows) || r > (m_nCols)) {
		return false;
	}
	arr[r-1][c-1] ++;
	return true;
}

void History::display() const {
	clearScreen();
	for (int r = 0; r < m_nRows; r++) {
		for (int c = 0; c < m_nCols; c++) {
			if (arr[r][c] == 0) {
				cout << '.';
			}
			else if (arr[r][c] < 26){
				char az = 'A' + (arr[r][c] - 1);
				cout << az;
			}
			else {
				cout << 'Z';
			}
		}
		cout << endl;
	}
	cout << endl;

}