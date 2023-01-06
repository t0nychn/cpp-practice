#include <iostream>
#define i_loop(x) for (int i=0; i<x; i++)

using namespace std;

int main(){
  i_loop(10){
    cout << i << endl;
  }
  return 0;
}
