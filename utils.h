#ifndef utils_h
#define utils_h

long long int generate_timestamp(char date_string[]);
int is_date_string_valid(char date_string[]);
unsigned int HashSDBM(char* str, unsigned int length);
double get_time();
#endif
