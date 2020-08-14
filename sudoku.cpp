#include <iostream>
using namespace std;
#include <fstream>
using std::ifstream;
//#include <cstdlib> 


int crow[100][100], ccol[100][100], cEntries[100];
int maxdepth = 0;

int nextEmptyLoc(int sudoku[9][9], int &row, int &col) {
	while (row < 9) {
		row = row + (col == 8);
		col = (col + 1) % 9;
		if (sudoku[row][col] == 0)
			return 1;
	}

	return 0;
}

int validRow(int sudoku[9][9], int row, int num) {
	for (int i=0 ; i<9 ; ++i)
		if (sudoku[row][i] == num)
			return 0;
	return 1;
}

int validCol(int sudoku[9][9], int col, int num) {
	for (int i=0 ; i<9 ; ++i)
		if (sudoku[i][col] == num)
			return 0;
	return 1;
}

int validBox(int sudoku[9][9], int row, int col, int num) {
	for (int i=0 ; i<3 ; ++i)
		for (int j=0 ; j<3 ; ++j)
			if (sudoku[row+i][col+j] == num)
				return 0;
	return 1;
}

int isAllowed(int sudoku[9][9], int row, int col, int num) {
	if (validRow(sudoku, row, num) && validCol(sudoku, col, num) && validBox(sudoku, (row/3)*3, (col/3)*3, num))
		return 1;
	return 0;
}

int isConfirmed(int sudoku[9][9], int row, int col) {
	int safeCount = 0, safeNum;
	for (int num=1 ; num<=9 ; ++num) {
		if (isAllowed(sudoku, row, col, num)) {
			safeCount++;
			safeNum = num;
		}
	}

	if (safeCount == 1) {
		sudoku[row][col] = safeNum;
		return 1;
	}
	return 0;
}

int checkSudoku(int sudoku[9][9]) {
	for (int i=0 ; i<9 ; i++) {
		for(int j=0 ; j<9 ; j++) {
			if (sudoku[i][j]) {
				int n = sudoku[i][j];
				sudoku[i][j] = 0;

				if(isAllowed(sudoku,i,j,n))
					sudoku[i][j] = n;
				else
					return 0;
			}
		}
	}

	return 1;
}

void fillConfirmed(int sudoku[9][9], int depth) {
	int checkagain = 1;
	int ent = 0;
	while (checkagain) {
		checkagain = 0;
		int row = 0, col = -1;
		while (nextEmptyLoc(sudoku, row, col)) {
			if (isConfirmed(sudoku, row, col)) {
				checkagain = 1;
				crow[depth][ent] = row;
				ccol[depth][ent] = col;
				ent++;
			}
		}
	}	
	cEntries[depth] = ent;
}

void removeConfirmed(int sudoku[9][9], int depth) {
	for (int i=0 ; i<cEntries[depth] ; ++i)
		sudoku[crow[depth][i]][ccol[depth][i]] = 0;

	cEntries[depth] = 0;
}

int hitAndTrial(int sudoku[9][9], int depth) {
	int row = 0, col = -1;
	
	maxdepth = max(maxdepth, depth);
	if (!nextEmptyLoc(sudoku, row, col))
		return 1;
	
	for (int num=1 ; num<=9 ; ++num) {
		if (isAllowed(sudoku, row, col, num)) {
			sudoku[row][col] = num;
			fillConfirmed(sudoku, depth);
			if (hitAndTrial(sudoku, depth+1))
				return 1;
			removeConfirmed(sudoku, depth);
			sudoku[row][col] = 0;
		}
	}
	
	return 0;
}

void printSudoku(int sudoku[9][9]) {
	for (int i=0 ; i<9 ; ++i) {
		for (int j=0 ; j<9 ; ++j)
			cout << sudoku[i][j] << ' ';
		cout << endl;
	}
}

int solveSudoku(int sudoku[9][9]) {
	fillConfirmed(sudoku, 0);
	if (hitAndTrial(sudoku, 1))
	{
		//printSudoku(sudoku);
		return 1;
	}
	else
	{
		cout << "No solution exists.\n";
		return 0;
	}	
}

int main() {
	int sudoku[9][9];
	
	ifstream file; // indata is like cin
	file.open("C:/Users/Tanya Gupta/Downloads/input.txt"); // opens the file
   	if(!file) 
   	{ 
    cerr << "Error: file could not be opened" << endl;
    exit(1);
   	}
	// Input sudoku
	for (int i=0 ; i<9 ; ++i)
		for (int j=0 ; j<9 ; ++j)
			file>>sudoku[i][j];
    file.close();

    ofstream file1;
    file1.open("C:/Users/Tanya Gupta/Downloads/output.txt");

    if (checkSudoku(sudoku)) 
	{
		if(solveSudoku(sudoku))
		{
			for (int i=0 ; i<9 ; ++i)
			{
				for (int j=0 ; j<9 ; ++j)
					file1<<sudoku[i][j]<<" ";
				file1<<endl;
			}
		}
		file1.close();
	}
	
	else
		cout << "Invalid Input.\n";

	return 0;
}
