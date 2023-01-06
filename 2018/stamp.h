enum MessageStatus {
  INVALID_HEADER=-1,
  WRONG_RECIPIENT=-2,
  INVALID_MESSAGE_DIGEST=-3,
  INVALID_HEADER_DIGEST=-4,
  VALID_EMAIL=1
};

/* pre-supplied helper function */
/* generates the SHA1 hash of input string text into output parameter digest */
void text_to_SHA1_digest(const char *text, char *digest);

using std::string;
int leading_zeros(const char* digest);
bool file_to_SHA1_digest(const char *file, char* digest);
bool make_header(string r, const char* file, char *header);
MessageStatus check_header(string address, string header, const char *file);
