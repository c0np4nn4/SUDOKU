#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;


class Sudoku {
private:
	int value[9][9];
public:
	Sudoku(void) {
		for (int i = 0; i <9; i++) {
			for (int j = 0; j < 9; j++) {
				value[i][j] = 0;
			}
		}
	}
	
	
	void putValue(int i, int j, int newValue) {
		value[i][j] = newValue - 48;
	}
	
	int getValue(int i, int j) {
		return value[i][j];
	}
	
	void print(void) {
		cout << "===========[Sudoku]===========" << endl;
		for (int i = 0; i < 9; i++) {
			if (i % 3 == 0 && i != 0) {
				for (int j = 0; j < 3; j++) { cout << "-----   "; }
				cout << endl;
			}
			
			for (int j = 0; j < 9; j++) {
				if (j % 3 == 0 && j != 0) { cout << '|' << ' '; }
				cout << value[i][j] << ' ';
			}
			cout << endl;
		}
		cout << "===========[ end  ]===========" << endl;
	}
};



class SudokuSolver {
private:
	vector<int> candidate[9][9];
	vector<int> exist[9][9];
public:
	SudokuSolver(void) {
		init();
	}
	
	void init(void) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				candidate[i][j].clear();
				exist[i][j].clear();
			}
		}
	}
	
	void getRowNumbers(int& i, int& j, Sudoku& sudoku, vector<int>& exist) {
		for (int k = 0; k < 9; k++) {
			if ( sudoku.getValue(i, k) ) {
				exist.push_back(sudoku.getValue(i, k));
			}
		}
	}
	
	void getColNumbers(int& i, int& j, Sudoku& sudoku, vector<int>& exist) {
		for (int k = 0; k < 9; k++) {
			int value = sudoku.getValue(k, j);
			if ( value ) {
				if ( find(exist.begin(), exist.end(), value) == exist.end() )
				{
					exist.push_back(sudoku.getValue(k, j));
				}
			}
		}
	}
	
	void getBoxNumbers(int& i, int& j, Sudoku& sudoku, vector<int>& exist) {
		int loc_i = (i / 3) * 3;
		int loc_j = (j / 3) * 3;
//		cout << loc_i << ' ' << loc_j << endl;
		
		for (int p = loc_i; p < loc_i + 3; p++) {
			for (int q = loc_j; q < loc_j + 3; q++) {
				int value = sudoku.getValue(p, q);	// locate value
				if ( value ) {						// if value is not zero.
					if ( find(exist.begin(), exist.end(), value) == exist.end() )	
					{
						exist.push_back(sudoku.getValue(p, q));
					}
				}
			}
		}
	}
	
	void getPossibleNumber(Sudoku& sudoku) {
				
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				getRowNumbers(i, j, sudoku, exist[i][j]);
				getColNumbers(i, j, sudoku, exist[i][j]);
				getBoxNumbers(i, j, sudoku, exist[i][j]);
			}
		}
		
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				sort(exist[i][j].begin(), exist[i][j].end());
			}
		}

	}
	
	
	
	void checkExist(void) {
		cout << "=======[checkExist]======" << endl;
		for (int i = 0; i < 9; i++) {
			for (int j = 0 ; j < 9; j++) {
				cout << "[" << i << ", " << j << "] : ";
				for (vector<int>::iterator iter = exist[i][j].begin(); iter != exist[i][j].end(); iter++) {
					cout << *iter << ' ';
				}
				cout << endl;
			}
		}
		cout << "=======[checkExist]======" << endl;
	}
};






int main(int argc, char** argv) {
	
	Sudoku sudoku;
	SudokuSolver solver;
	
	string filePath = "ex1.txt";
	
	ifstream openFile(filePath.data());
	if (openFile.is_open()) {
		string line;
		for (int i = 0; getline(openFile, line); i++) {
			for (int j = 0; j < 9; j++) {
				sudoku.putValue(i, j, line[j]);
			}
		}
	}
	
	
	//check
	sudoku.print();

	
	
	solver.getPossibleNumber(sudoku);
	//check
	solver.checkExist();
	
	return 0;
}
