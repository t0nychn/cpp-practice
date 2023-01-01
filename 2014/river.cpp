#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
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

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */
char** make_river_scene(string left, string boat){
  char** scene = create_scene();
  if (left.size() > 7 || boat.size() > 2 )
    return scene;
  // <-- init -->
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene, 19, 19, "river.txt");

  int mr, mc, cr, cc, br, bc, m, c;
  mr = 2;
  mc = 1;
  cr = 11;
  cc = 1;
  br = 17; 
  bc = 36; // right bank coord, set to left if appear in left string
  m = 0; // missionary counter
  c = 0; // cannibal counter

  // <-- left bank -->
  for (int i=0; i<left.size(); i++){
    if (left[i] == 'M'){
      if (mc > 19)
	continue;
      add_to_scene(scene, mr, mc, "missionary.txt");
      mc += 6;
      m++;
    }
    else if (left[i] == 'C'){
      if (cr > 19)
	continue;
      add_to_scene(scene, cr, cc, "cannibal.txt");
      cc += 6;
      c++;
    }
    else if (left[i] == 'B'){
      bc = 19;
    }
  }

  // <-- boat -->
  add_to_scene(scene, br, bc, "boat.txt");
  int cbc = bc + 3;
  for (int i=0; i<boat.size(); i++){
    string file;
    if (boat[i] == 'M'){
      m++;
      file = "missionary.txt";
    }
    else if (boat[i] == 'C'){
      c++;
      file = "cannibal.txt";
    }
    add_to_scene(scene, 11, cbc, file.c_str());
    cbc += 6;
  }

  // <-- right bank -->
  mc = 54;
  cc = 54;
  while (m < 3){
    add_to_scene(scene, mr, mc, "missionary.txt");
    m++;
    mc += 6;
  }
  while (c < 3){
    add_to_scene(scene, cr, cc, "cannibal.txt");
    c++;
    cc += 6;
  }
  return scene;
}

int perform_crossing(char *left, string targets){
  if (targets.size() > 2 || targets.size() == 0)
    return ERROR_INVALID_MOVE;
  char new_left[10];
  int m, c, k;
  m = c = k = 0;
  bool boat_left;
  
  for (int i = 0; i < targets.size(); i++){
    if (targets[i] == 'M')
      m++;
    else if (targets[i] == 'C')
      c++;
  }
  for (int i = 0; left[i]; i++){
    if (left[i] == 'M' || left[i] == 'B'){
      if (left[i] == 'M' && m > 0){
	m--;
      }
      else if (left[i] == 'B'){
	boat_left = true;
      }
      else
	new_left[k++] = 'M';
    }
    else if (left[i] == 'C'){
      if (c > 0){
	c--;
      }
      else
	new_left[k++] = 'C';
    }
  }
	
  if (boat_left)
    new_left[k] = 'B';
  else
    new_left[k] = '\0';
  char** scene = make_river_scene(new_left, targets);
  cout << "Loading the boat.." << endl;
  print_scene(scene);

  if (boat_left)
    new_left[k] = '\0';
  strcpy(left, new_left);
  
  cout << "Crossing the river..." << endl;
  scene = make_river_scene(left, targets);
  print_scene(scene);
  
  cout << "Unloading the boat..." << endl;
  scene = make_river_scene(left, "");
  print_scene(scene);

  for (int i=0; left[i]; i++){
    if (left[i] == 'M')
      m++;
    else if (left[i] == 'C')
      c++;
  }
  if (c > m || c == m)
    return VALID_NONGOAL_STATE;
  if (c < m)
    return ERROR_MISSIONARIES_EATEN;
  if (c == 0 && m == 0)
    return VALID_GOAL_STATE;
}

void play_game(){
  int code = VALID_NONGOAL_STATE;
  cout << "<-- GAME BEGINS! -->" << endl;
  print_scene(make_river_scene("MMMCCCB", ""));
  char left[10] = "MMMCCC";
  while (code != ERROR_MISSIONARIES_EATEN && code != VALID_GOAL_STATE){
    char boat[3];
    cout << "Enter boat load:" << endl;
    cin >> boat;
    code = perform_crossing(left, boat);
  }
  cout << status_description(code) << endl;
  return;
}

int main(){
  play_game();
  return 0;
}
