#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <vector>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}


bool find_marker(const char marker, char **m, int height, int width, int &row, int &col){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      if (m[r][c] == marker){
	row = r;
	col = c;
	return true;
      }
    }
  }
  row = -1;
  col = -1;
  return false;
}

bool valid_solution(const char* sol, char **m, int height, int width){
  int r, c;
  if (!find_marker('>', m, height, width, r, c))
    return false;
  while (*sol){
    if (*sol == 'N'){
      r--;
    }
    else if (*sol == 'S'){
      r++;
    }
    else if (*sol == 'E'){
      c++;
    }
    else if (*sol == 'W'){
      c--;
    }
    else
      return false;
    sol++;

    if (m[r][c] == ' '){
      continue;
    }
    else if (m[r][c] == 'X'){
      return true;
    }
    else
      return false;
  }
  return false;
}

vector<char> valid_moves(char **m, int r, int c, const char end){
  vector<char> directions;
  if (m[r-1][c] == end){
    directions.push_back('N');
    return directions;
  }
  else if (m[r+1][c] == end){
    directions.push_back('S');
    return directions;
  }
  else if (m[r][c+1] == end){
    directions.push_back('E');
    return directions;
  }
  else if (m[r][c-1] == end){
    directions.push_back('W');
    return directions;
  }
  if (m[r-1][c] == ' '){
    directions.push_back('N');
  }
  if (m[r+1][c] == ' '){
    directions.push_back('S');
  }
  if (m[r][c+1] == ' '){
    directions.push_back('E');
  }
  if (m[r][c-1] == ' '){
    directions.push_back('W');
  }
  return directions;
}

string find_path(char **m, int height, int width, const char start, const char end){
  string sol;
  int r, c;
  if (!find_marker(start, m, height, width, r, c))
    return "no solution";
  vector<char> moves = valid_moves(m, r, c, end);
  if (moves.size() == 0){
    return "no solution";
  }
  for (int i = 0; i < moves.size(); i++){
    m[r][c] = '#';
    sol = moves[i];
    int r_n, c_n;
    if (moves[i] == 'N'){
      r_n = r - 1;
      c_n = c;
    }
    else if (moves[i] == 'S'){
      r_n = r + 1;
      c_n = c;
    }
    else if (moves[i] == 'E'){
      r_n = r;
      c_n = c + 1;
    }
    else{
      r_n = r;
      c_n = c - 1;
    }
    if (m[r_n][c_n] == end){
      m[r_n][c_n] = '#';
      return "";
    }
    m[r_n][c_n] = start;
    string path = find_path(m, height, width, start, end);
    if (path != "no solution"){
      sol += path;
      return sol;
    }
    else{
      m[r][c] = start;
      m[r_n][c_n] = ' ';
    }
  }
  return "no solution";
}
