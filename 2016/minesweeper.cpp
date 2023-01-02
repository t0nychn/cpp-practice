#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++) 
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++) 
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */ 
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}

/* add your functions here */
bool is_complete(char mines[9][9], char revealed[9][9]){
  for (int i=0; i<9; i++){
    for (int j=0; j<9; j++){
      if (mines[i][j] == '*' && revealed[i][j] != '*'){
	return false;
      }
    }
  }
  return true;
}

int count_mines(const char position[2], char mines[9][9]){
  int r = position[0] - 'A';
  int c = position[1] - '1';

  if (r < 0 || c < 0 || r > 8 || c > 8)
    return INVALID_MOVE;

  int seed_r = (r == 0) ? r : r-1;
  int seed_c = (c == 0) ? c : c-1;
  int end_r = (r == 8) ? r : r+1;
  int end_c = (c == 8) ? c : c+1;

  int counter = 0;
  
  for (int i=seed_r; i<=end_r; i++){
    for (int j=seed_c; j<=end_c; j++){
      if (mines[i][j] == '*'){
	counter++;
      }
    }
  }
  return counter;
}

MoveResult make_move(const char position[3], char mines[9][9], char revealed[9][9]){
  bool flag = false;
  if (strlen(position) == 3){
    if (position[2] != '*')
      return INVALID_MOVE;
    flag = true;
  }
  else if (strlen(position) > 3){
    return INVALID_MOVE;
  }

  int r = position[0] - 'A';
  int c = position[1] - '1';

  if (r < 0 || c < 0 || r > 8 || c > 8)
    return INVALID_MOVE;

  if (flag){
    revealed[r][c] = '*';
    return VALID_MOVE;
  }

  if (mines[r][c] == '*')
    return BLOWN_UP;

  if (revealed[r][c] != '?' || revealed[r][c] == '*')
    return REDUNDANT_MOVE;

  // cout << "Counting mines at: " << position << endl;
  int number = count_mines(position, mines);
  if (number > 0){
    revealed[r][c] = '0' + number;
  }
  // if number == 0
  else if (number == 0){
    revealed[r][c] = ' ';
    int seed_r = (r == 0) ? r : r-1;
    int seed_c = (c == 0) ? c : c-1;
    int end_r = (r == 8) ? r : r+1;
    int end_c = (c == 8) ? c : c+1;

    for (int i=seed_r; i<=end_r; i++){
      for (int j=seed_c; j<=end_c; j++){
	const char move[3] = {i + 'A', j + '1', '\0'};
        make_move(move, mines, revealed);
      }
    }
  }

  if (is_complete(mines, revealed))
    return SOLVED_BOARD;
  else
    return VALID_MOVE;
}

bool find_safe_move(char revealed[9][9], char* move){return false;}

/*
int main(){
  char mines[9][9], revealed[9][9];

  load_board("mines.dat", mines);
  initialise_board(revealed);
  MoveResult result1 = make_move("B6", mines, revealed);
  display_board(revealed);
  MoveResult result2 = make_move("E5", mines, revealed);
  display_board(revealed);
  MoveResult result3 = make_move("H3*", mines, revealed);
  display_board(revealed);
  display_board(mines);
  return 0;
}
*/
