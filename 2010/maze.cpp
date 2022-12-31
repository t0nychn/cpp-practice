#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

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
    cout << "(" << r << ", " << c << ") gives: '" << m[r][c] << "'" << endl;
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
  if (m[r-1][c] == ' ' || m[r-1][c] == end){
    directions.push_back('N');
  }
  if (m[r+1][c] == ' ' || m[r+1][c] == end){
    directions.push_back('S');
  }
  if (m[r][c+1] == ' ' || m[r][c+1] == end){
    directions.push_back('E');
  }
  if (m[r][c-1] == ' ' || m[r][c-1] == end){
    directions.push_back('W');
  }
  return directions;
}

char* find_path(char **m, int height, int width, const char start, const char end){
  char* sol;
  int r, c;
  if (!find_marker(start, m, height, width, r, c))
    return "no solution";
  m[r][c] = '#';
  vector<char> moves = valid_moves(m, r, c, end);
  for (int i = 0; i < moves.size(); i++){
    while (moves[i] == 'N'){
      vector<char> next = valid_moves(m, r-1, c, end);
      if (next.size() == 1 && next[0] == 'N'){
	r--;
      }
      else{
	m[r][c] = start;
	char* res = find_path(m, height, width, start, end);
	if (!strcmp(res, "no solution")){
	  break;
	}
	else{
	  sol = strcat(sol, res);
	  while (m[r][c] == ' '){
	    m[r][c] = '#';
	    r--;
	  }
	}
      }
    }
    // others
  }
  return sol;
}
    

int main(){
  int height, width, row, column;
  char **maze = load_maze("test.txt", height, width);
  bool success = find_marker('X', maze, height, width, row, column);
  if (success)
    cout << row << column << endl;

  cout << "A path through the maze from ’>’ to ’X’ is: ";
  cout << find_path(maze, height, width, '>', 'X') << endl;
  print_maze(maze, height, width);
  return 0;
}
