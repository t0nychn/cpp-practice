#ifndef PLAYFAIR_H
#define PLAYFAIR_H
#include <string>
using std::string;


void prepare(string input, char* output);
void grid(string codeword, char square[6][6]);
void bigram(char square[6][6], char inchar1, char inchar2, char &outchar1, char &outchar2);
void encode(char square[6][6], const char *raw, char* encoded);
void decode(char square[6][6], char* encoded, char* decoded);

#endif
