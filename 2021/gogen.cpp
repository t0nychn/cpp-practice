#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which removes unprintable characters like carriage returns and newlines from strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
  char **board = allocate_2D_array(5, 6);
  ifstream input(filename);
  assert(input);
  char buffer[512];
  int lines = 0;
  input.getline(buffer, 512);
  while (input && lines < HEIGHT) {
    filter(buffer);
    if (strlen(buffer) != WIDTH)
      cout << "WARNING bad input = [" << buffer << "]" << endl;
    assert(strlen(buffer) == WIDTH);
    strcpy(board[lines], buffer);
    input.getline(buffer, 512);
    lines++;
  }
  input.close();
  return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
  ofstream out(filename); 
  if (!out)
    return false;
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      out << board[r][c];
    }
    out << endl;
  }
  bool result = out.good();
  out.close();
  return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
  char word[512];
  int count = 0;
  ifstream in(filename);
  while (in >> word)
    count++;
  in.close();
  return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
  int count = count_words(filename);
  ifstream in(filename);
  assert(in);
  int n=0;
  char **buffer = new char *[count+1]; // +1 because we NULL terminate 
  char word[512];
  for (; (in >> word) && n<count; n++) {
    buffer[n] = new char[strlen(word) + 1];
    strcpy(buffer[n], word);
  }
  buffer[n] = NULL;
  in.close();
  return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      cout << "[" << board[r][c] << "]";
      if (c < WIDTH-1)
	cout << "--";
    }
    cout <<endl;
    if (r < HEIGHT-1) {
      cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
      cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
    }
  }
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
  for (int n=0; words[n]; n++) 
    cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) {
  deallocate_2D_array(board, HEIGHT);
}

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
  int count = 0;
  for (; words[count]; count++);
  deallocate_2D_array(words, count);
}

/* add your own function definitions here */
bool get_position(char **board, const char ch, int &row, int &col){
  for (int r=0; r<5; r++){
    for (int c=0; c<5; c++){
      if (board[r][c] == ch){
	row = r;
	col = c;
	return true;
      }
    }
  }
  return false;
}

bool valid_solution(char **board, char **words){
  int r_a, r_b, c_a, c_b;
  for (int i=0; words[i]; i++){
    for (int j=0; j< (int) strlen(words[i])-1; j++){
      if (words[i][j] == 'Z' || words[i][j+1] == 'Z')
	return false;
      
      bool p1 = get_position(board, words[i][j], r_a, c_a);
      bool p2 = get_position(board, words[i][j+1], r_b, c_b);

      if (abs(r_b - r_a) > 1 || abs(c_b - c_a) > 1 || !p1 || !p2){
	return false;
      }
    }
  }
  return true;
}

void update(char **board, const char ch, Mask &mask){
  if (ch > 'Y' || ch < 'A')
    return;
  
  // check if mask passed in is fixed already, i.e. is set board operation
  if (mask.count() == 1){
    int r, c;
    mask.get_position(true, r, c);
    board[r][c] = ch;
    return;
  }

  // else we know it's a set mask operation, rather than set board
  mask.set_all(false);
  bool fixed = false;

  for (int r=0; r<5; r++){
    for (int c=0; c<5; c++){
      if (board[r][c] == ch){
	mask[r][c] = true;
	fixed = true;
      }
    }
  }

  if (fixed)
    return;

  for (int r=0; r<5; r++){
    for (int c=0; c<5; c++){
      if (board[r][c] == '.')
	mask[r][c] = true;
    }
  }

  return;
}

void neighbourhood_intersect(Mask &one, Mask &two){
  one *= two.neighbourhood();
  two *= one.neighbourhood();
  return;
}

/*
bool solve_remaining(char** board, Mask masks[25], char **words){
  for (int i=0; i<25; i++){
    if (masks[i].count() > 1){
      for (int r=0; r<5; r++){
	for (int c=0; c<5; c++){
	  if (board[r][c] == '.'){
	    board[r][c] = 'A' + i;
	    update(board, 'A' + i, masks[i]);
	    if (!solve_remaining(board, masks, words)){
	      board[r][c] = '.';
	      update(board, 'A' + i, masks[i]);
	      return false;
	    }
	  }
	}
      }
    }
  }
  return valid_solution(board, words);
}
*/	  

bool solve_board(char **board, char** words){
  Mask masks[25];
  for (int i=0; i<25; i++){
    masks[i].set_all(false);
    update(board, 'A'+i, masks[i]);
    //masks[i].print();
    //cout << endl;
  }
  for (int i=0; words[i]; i++){
    for (int j=0; j<(int) strlen(words[i]) -1; j++){
      neighbourhood_intersect(masks[words[i][j]-'A'], masks[words[i][j+1]-'A']);
      //masks[words[i][j]-'A'].print();
      //cout << endl;
    }
  }
  for (int i=0; i<25; i++){
    update(board, 'A'+i, masks[i]);
  }
  if (valid_solution(board, words))
    return true;
  else
    return solve_board(board, words);
}

