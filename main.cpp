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
		value[i][j] = newValue;
	}
	
	int getValue(int i, int j) {
		return value[i][j];
	}
	
	
	
	
	void print(void) {	// for check
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
	Sudoku sudoku;
	vector<int> candidate[9][9];
	vector<int> exist[9][9];
public:
	SudokuSolver(Sudoku& _sudoku) {
		sudoku = _sudoku;
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

	void getRowNumbers(int& i, int& j, vector<int>& temp) {
		for (int k = 0; k < 9; k++) {
			if ( sudoku.getValue(i, k) ) { 
				temp.erase(remove(temp.begin(), temp.end(), sudoku.getValue(i,k)), temp.end());
			}
		}
	}
	
	void getColNumbers(int& i, int& j, vector<int>& temp) {
		for (int k = 0; k < 9; k++) {
			int value = sudoku.getValue(k, j);
			if ( value ) {
				if ( find(temp.begin(), temp.end(), value) != temp.end() )
				{
					temp.erase(remove(temp.begin(), temp.end(), value), temp.end());
				}
			}
		}
	}
	
	void getBoxNumbers(int& i, int& j, vector<int>& temp) {
		int loc_i = (i / 3) * 3;
		int loc_j = (j / 3) * 3;
		
		for (int p = loc_i; p < loc_i + 3; p++) {
			for (int q = loc_j; q < loc_j + 3; q++) {
				int value = sudoku.getValue(p, q);	
				if ( value ) {						
					if ( find(temp.begin(), temp.end(), value) != temp.end() )	
					{
						temp.erase(remove(temp.begin(), temp.end(), value), temp.end());
					}
				}
			}
		}
	}

	void getPossibleNumber() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if ( sudoku.getValue(i, j) ) {
					continue;
				}
				vector<int> temp = {1, 2, 3, 4, 5, 6, 7, 8, 9};
				getRowNumbers(i, j, temp);
				getColNumbers(i, j, temp);
				getBoxNumbers(i, j, temp);
				candidate[i][j] = temp;
			}
		}
	}





	void checkExist(void) {		// for check
		cout << "=======[checkExist]======" << endl;
		for (int i = 0; i < 9; i++) {
			for (int j = 0 ; j < 9; j++) {
				cout << "[" << i << ", " << j << "] : ";
				for (vector<int>::iterator iter = candidate[i][j].begin(); iter != candidate[i][j].end(); iter++) {
					cout << *iter << ' ';
				}
				cout << endl;
			}
		}
		cout << "=======[checkExist]======" << endl;
	}
	
	bool solitaryRow(int i, int j, int value) {
		
		//cout << "-------------------------" << i << ", " << j << ", " << value << endl;
		
		bool result = true;
		for (int k = 0; k < 9; k++) {
			if ( !sudoku.getValue(i, k) && k != j ) {		// sudoku[i][k] == 0, k!=j
				for (vector<int>::iterator iter = candidate[i][k].begin(); iter != candidate[i][k].end(); iter++) {
					if (*iter == value) {
						//cout << "i, j, value : " << i << ", " << k << ", " << *iter << endl;
						result = false;
					}
					
				}
			}
			if ( sudoku.getValue(i, k) == value ) {
				result = false;
			}
		}
		if (result == true) {
			cout << "[change_in_row] { i, j, value } : {" << i << ", " << j << ", " << value << "} " << endl;
		}
		return result;
	}
	
	bool solitaryCol(int i, int j, int value) {
		bool result = true;
		for (int k = 0; k < 9; k++) {
			if ( !sudoku.getValue(k, j) && k != i ) {		// sudoku[i][k] == 0, k!=j
				for (vector<int>::iterator iter = candidate[k][j].begin(); iter != candidate[k][j].end(); iter++) {
					if (*iter == value) {
						result = false;
					}
				}
			}
			if ( sudoku.getValue(k, j) == value ) {
				result = false;
			}
		}
		if (result == true) {
			cout << "[change_in_col] { i, j, value } : {" << i << ", " << j << ", " << value << "} " << endl;
		}
		return result;
		
	}
	
	bool solitaryBox(int i, int j, int value) {
		bool result = true;
		int loc_i = (i / 3) * 3;
		int loc_j = (j / 3) * 3;
		
		for (int p = 0; p < loc_i + 3; p++) {
			for (int q = 0; q < loc_j + 3; q++) {
				if ( !sudoku.getValue(p, q) && (p != i || q != j) ) {		// sudoku[i][k] == 0, k!=j
					for (vector<int>::iterator iter = candidate[p][q].begin(); iter != candidate[p][q].end(); iter++) {
						if (*iter == value) {
							result = false;
						}
					}
				}
				if ( sudoku.getValue(p, q) == value ) {
					result = false;
				}
			}
		}
		
		if (result == true) {
			cout << "[change_in_box] { i, j, value } : {" << i << ", " << j << ", " << value << "} " << endl;
		}
		return result;	
	}
	
	
	
	
	
	void eraseCandidate() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				vector<int> temp = candidate[i][j];
				getRowNumbers(i, j, temp);
				getColNumbers(i, j, temp);
				getBoxNumbers(i, j, temp);
				candidate[i][j] = temp;
			}
		}		
	}
	
	
	
	
	
	void fillInSUDOKU(void) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if ( !sudoku.getValue(i, j) ) {
					for (vector<int>::iterator iter = candidate[i][j].begin(); iter != candidate[i][j].end(); iter++) {
						int value = *iter;					
						if ( solitaryRow(i, j, value) || solitaryCol(i, j, value) || solitaryBox(i, j, value) ) {
							sudoku.putValue(i, j, value);
						}
					}
				}
			}
		}
		eraseCandidate();
	}
	
	Sudoku& getSudoku(void) {
		return this->sudoku;
	}
	
	
	
};






int main(int argc, char** argv) {
	
	Sudoku sudoku;
	
	
	string filePath = "ex1.txt";
	
	ifstream openFile(filePath.data());
	if (openFile.is_open()) {
		string line;
		for (int i = 0; getline(openFile, line); i++) {
			for (int j = 0; j < 9; j++) {
				sudoku.putValue(i, j, line[j] - 48);
			}
		}
	}	
	sudoku.print();		//check




	SudokuSolver solver(sudoku);	
	solver.getPossibleNumber();
	
	solver.checkExist();		//check
	
	for (int i = 0; i < 5; i++) {		// check, repeating
	solver.fillInSUDOKU();
	solver.getSudoku().print();
	}
	
	return 0;
}
