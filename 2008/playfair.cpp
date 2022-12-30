#include "playfair.h"
using namespace std;

void prepare(string input, char *output){
  int ind = 0;
  for (int i = 0; i < input.size()+1; i++){
    if (isupper(input[i]) || isdigit(input[i])){
      output[ind] = input[i];
      ind++;
    }
    else if (islower(input[i])){
      output[ind] = toupper(input[i]);
      ind++;
    }
    else if (!input[i]){
      if (ind % 2 == 1){
	output[ind] = 'X';
	output[ind+1] = '\0';
      }
      else
	output[ind] = input[i];
    }
    else
      continue;
  }
  return;
}

bool used(char square[6][6], char target){
  for (int i = 0; i < 6; i++){
    for (int j = 0; j < 6; j++){
      if (square[i][j] == target)
	return true;
    }
  }
  return false;
}

void flush(char square[6][6]){
  for (int i = 0; i < 6; i++){
    for (int j = 0; j < 6; j++){
      square[i][j] = '.';
    }
  }
  return;
}

char next(char square[6][6], char &current){
  while (used(square, current)){
    current++;
  }
  return current;
}

void grid(string codeword, char square[6][6]){
  int x = 0;
  int y = 0;
  flush(square);
  
  for (int i = 0; i < codeword.size(); i++){
    if (!used(square, codeword[i])){
      square[x][y] = codeword[i];
      ((y + 1) > 5) ? x++ : x;
      y = ++y % 6;
    }
  }
  char alpha = 'A';
  char numeric = '0';
  for (int i = 0; i < 6; i++){
    for (int j = 0; j < 6; j++){
      if (square[i][j] == '.'){
        if (alpha != 'Z'){
	  square[i][j] = next(square, alpha);
	}
	else{
	  square[i][j] = next(square, numeric);
	}
      }
    }
  }
  return;
}

void find(char square[6][6], char target, int coord[2]){
  for (int i = 0; i < 6; i++){
    for (int j = 0; j < 6; j++){
      if (square[i][j] == target){
	coord[0] = i;
	coord[1] = j;
	return;
      }
    }
  }
  coord[0] = -1;
  coord[1] = -1;
  return;
 }

void bigram(char square[6][6], char inchar1, char inchar2, char &outchar1, char &outchar2){
  int in1[2], in2[2];
  find(square, inchar1, in1);
  find(square, inchar2, in2);
  outchar1 = square[in1[0]][in2[1]];
  outchar2 = square[in2[0]][in1[1]];
  return;
}

void encode(char square[6][6], const char *raw, char* encoded){
  if (!*raw){
    *encoded = '\0';
    return;
  }
  bigram(square, *raw++, *raw++, *encoded++, *encoded++);
  return encode(square, raw, encoded);
}

void decode(char square[6][6], char* encoded, char* decoded){
  encode(square, encoded, decoded);
  return;
}
