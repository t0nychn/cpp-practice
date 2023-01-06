#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */


// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */
int leading_zeros(const char* digest){
  int counter = 0;
  while (*digest){
    if (*digest == '0')
      counter++;
    else
      return counter;
    digest++;
  }
  return counter;
}

bool file_to_SHA1_digest(const char *file, char* digest){
  ifstream in(file);

  if (!in)
    return false;

  string msg, buff;

  while (!in.eof()){
    in >> buff;
    if (msg.size() > 0)
      msg += " ";
    msg += buff;
  }

  text_to_SHA1_digest(msg.c_str(), digest);
    
  in.close();
  return true;
}

bool make_header(string r, const char* file, char *header){
  char digest[41];
  if (!file_to_SHA1_digest(file, digest))
    return false;

  int counter = 0;

  string d_str = digest;

  string unencrypted = r + ":" + d_str + ":" + to_string(counter);

  text_to_SHA1_digest(unencrypted.c_str(), digest);

  while (leading_zeros(digest) != 5 || counter > 1e7){
    counter++;
    unencrypted = r + ":" + d_str + ":" + to_string(counter);
    text_to_SHA1_digest(unencrypted.c_str(), digest);
  }
  
  strcpy(header, unencrypted.c_str());
  return true;
}

MessageStatus check_header(string address, string header, const char *file){
  char digest[41];
  text_to_SHA1_digest(header.c_str(), digest);
  if (leading_zeros(digest) != 5)
    return INVALID_HEADER_DIGEST;

  string email, msg, counter;
  int colons = 1;
  for (int i=0; header[i]; i++){
    if (header[i] == ':')
      colons++;
    else{
      if (colons == 1)
	email += header[i];
      else if (colons == 2)
	msg += header[i];
      else if (colons == 3){
	if (isxdigit(header[i]))
	  counter += header[i];
	else
	  return INVALID_HEADER;
      }
    }
  }

  if (colons != 3)
    return INVALID_HEADER;

  if (email != address)
    return WRONG_RECIPIENT;

  char msg_comp[41];
  file_to_SHA1_digest(file, msg_comp);
  if (strcmp(msg_comp, msg.c_str()))
    return INVALID_MESSAGE_DIGEST;

  return VALID_EMAIL;
}
