#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

using namespace std;

typedef struct Cell {
  int val;
  bool locked = true;
} Cell;

typedef struct Record {
  string cell;
  int prev_val;
} Record;

class Sudoku {
 private:
  int row = 9;
  int col = 9;
  Cell board[9][9] = {};
  bool generated = false;
  stack<Record> history;

 public:
  tuple<int, int> decodeLocation(string cell) {
    return make_tuple(cell[0] - 'A', cell[1] - '1');
  }

  bool violateRules(int row, int col) {
    return checkRowDuplication(row) || checkColDuplication(col) ||
           checkSqDuplication(((int)row / 3) * 3 + col / 3);
  }

  bool checkRowDuplication(int row) {
    int checker[9] = {};
    for (int i = 0; i < col; i++) {
      int val = board[row][i].val;
      if (val == 0) continue;
      if (checker[val - 1] != 0) return true;
      checker[val - 1] = 1;
    }
    return false;
  }

  bool checkColDuplication(int col) {
    int checker[9] = {};
    for (int i = 0; i < row; i++) {
      int val = board[i][col].val;
      if (val == 0) continue;
      if (checker[val - 1] != 0) return true;
      checker[val - 1] = 1;
    }
    return false;
  }

  bool checkSqDuplication(int sq) {
    int col = sq % 3 * 3;
    int row = ((int)sq / 3) * 3;
    int checker[9] = {};
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        int val = board[row + i][col + j].val;
        if (val == 0) continue;
        if (checker[val - 1] != 0) return true;
        checker[val - 1] = 1;
      }
    }
    return false;
  }

  void boardGen() { boardGen(0, 0); }

  void boardGen(int x, int y) {
    if (x == row && y == 0) generated = true;

    // for (int i = 1; i <= 9 && !generated; i++) {
    srand(time(nullptr));
    int triedNum = 0, i;
    int checker[9] = {};

    while (triedNum < 9 && !generated) {
      i = rand() % 9 + 1;

      if (checker[i - 1] == 1) continue;
      triedNum++;
      checker[i - 1] = 1;

      board[x][y].val = i;
      if (violateRules(x, y)) {
        board[x][y].val = 0;
        continue;
      }
      int newx = x, newy = y + 1;
      if (newy >= 9) {
        newx++;
        newy = 0;
      }
      boardGen(newx, newy);
    }
  }

  void questionGen(int num) {
    int slots = 0;
    srand(time(nullptr));
    while (slots < num) {
      int x = rand() % 9, y = rand() % 9;
      if (board[x][y].val != 0) {
        board[x][y].val = 0;
        board[x][y].locked = false;
        slots++;
      }
    }
  }

  void fillCell(string cell, int val) {
    int row, col;
    tie(row, col) = decodeLocation(cell);
    if (row >= this->row || col >= this->col || val > 9 || 0 > val) return;

    if (board[row][col].locked) return;

    int prev_val = board[row][col].val;
    board[row][col].val = val;
    if (violateRules(row, col)) board[row][col].val = prev_val;

    history.push(Record{cell, prev_val});
  }

  void undo() {
    if (history.empty()) return;

    Record r = history.top();
    history.pop();
    fillCell(r.cell, r.prev_val);
  }

  bool isCompleted() {
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++)
        if (board[i][j].val == 0) return false;
    }
    return true;
  }

  void printBoard() {
    cout << "   ";
    for (int i = 1; i <= 9; i++) {
      cout << i << " ";
      if (i % 3 == 0) cout << "  ";
    }

    cout << endl << "  ------------------------" << endl;
    for (int i = 0; i < row; i++) {
      cout << (char)('A' + i) << "| ";
      for (int j = 0; j < col; j++) {
        (board[i][j].val != 0) ? cout << board[i][j].val << " " : cout << "  ";
        if ((j + 1) % 3 == 0) cout << "| ";
      }
      cout << endl;
      if ((i + 1) % 3 == 0) cout << "  ------------------------" << endl;
    }
    cout << endl;
  }
};

int main() {
  Sudoku game;
  game.boardGen();
  //game.printBoard();

  int num;
  cout << "Enter the number missing cells: ";
  cin >> num;
  game.questionGen(num);
  game.printBoard();

  while (!game.isCompleted()) {
    string input;
    int val;
    cout << "Please enter the cell and the number (e.g. A1 4) [type \"undo\"- to undo]: ";
    cin >> input;
    if (input == "undo")
      game.undo();
    else {
      cin >> val;
      game.fillCell(input, val);
    }
    game.printBoard();
  }
  cout << "\tCompleted ;)" << endl;

  return 0;
}