#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

int main(){
  cout << "Establishing ofstream for 'test.txt'" << endl;
  
  ofstream out("test.txt");
  if (!out)
    cout << "No file found!" << endl;
  else
    cout << "File found" << endl;

  cout << "Outputting string: 'Hello'..." << endl;
  string o = "Hello hello";
  out << o << endl;
  cout << "Outputting char array: 'there'..." << endl;
  char p[100];
  strcpy(p, "there there");
  out << p << endl;

  cout << "Establishing ifstream for 'test.txt'" << endl;
  ifstream in("test.txt");
  if (!in)
    cout << "No file found!" << endl;
  else
    cout << "File found" << endl;

  cout << "Inputting first word into char array..." << endl;
  char a[100];
  in.getline(a, 3);
  cout << a << endl;
  cout << "Inputting second word into string..." << endl;
  string b;
  getline(in, b);
  cout << b << endl;

  if (in && out){
    cout << "Closing fstreams" << endl;
    in.close();
    out.close();
  }
  else
    cout << "Error when reading in/out file!" << endl;
  return 0;
}
