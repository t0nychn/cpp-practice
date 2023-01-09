#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>


using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */
bool valid_step(const char* current, const char* next){
  if (strlen(current) != strlen(next))
    return false;

  if (!dictionary_search(current) || !dictionary_search(next))
    return false;
  
  int counter = 0;
  for (int i=0; current[i]; i++){
    if (current[i] != next[i])
      counter++;
    if (counter > 1)
      return false;
  }
  return true;
}

bool display_chain(const char* chain[], ostream &out){
  if (!out)
    return false;
  for (int i=0; chain[i]; i++){
    if (i == 0 || chain[i+1] == NULL)
      out << chain[i];
    else{
      for (int j=0; chain[i][j]; j++)
	out.put(tolower(chain[i][j]));
    }
    out << endl;
  }
  if (!out)
    return false;
  return true;
}

bool valid_chain(const char* chain[]){
  if (chain[1] == NULL)
    return false;
  for (int i=0; chain[i+1]!=NULL; i++){
    if (!valid_step(chain[i], chain[i+1]))
      return false;
    for (int j=0; chain[j]; j++){
      if (!strcmp(chain[i], chain[j]) && i != j)
	return false;
    }
  }
  return true;
}

int char_diff(const char* start, const char* target){
  if (strlen(start) != strlen(target))
    return 0;

  int counter = 0;
  for (int i=0; start[i]; i++){
    if (start[i] != target[i])
      counter++;
  }
  return counter;
}

const char* insert(string word, int i, char ch){
  word[i] = ch;
  if (dictionary_search(word.c_str())){
    return word.c_str();
  }
  else
    return "";
}

void add_to_chain(const char* answer_chain[], int i, string word){
  char *ptr = new char[100];
  strcpy(ptr, word.c_str());
  answer_chain[i] = ptr;
  return;
}

bool in_chain(const char* answer_chain[], const char* word){
  for (int i=0; answer_chain[i]; i++){
    if (!strcmp(answer_chain[i], word))
      return true;
  }
  return false;
}

bool find_chain_recur(const char* start, const char* target, const char* answer_chain[], int max_steps, int ind){
  int diff = char_diff(start, target);

  if (diff == 0 || diff > max_steps)
    return false;
  else if (diff == 1 && valid_step(start, target)){
    answer_chain[ind+1] = NULL;
    add_to_chain(answer_chain, ind, target);
    return true;
  }

  for (int i=0; start[i]; i++){
    if (start[i] != target[i]){
      for (int a=0; a<25; a++){
	const char* try_start = insert(start, i, 'A'+a);
	if (find_chain_recur(try_start, target, answer_chain, max_steps-1, ind+1)){
	  if (!in_chain(answer_chain, try_start))
	    add_to_chain(answer_chain, ind, try_start);
	  return true;
	}
      }
    }
  }
  return false;
}

bool find_chain(const char* start, const char* target, const char* answer_chain[], int max_steps){
  flush_chain(answer_chain);
  bool success = find_chain_recur(start, target, answer_chain, max_steps, 1);
  if (success)
    add_to_chain(answer_chain, 0, start);
  return success;
}

