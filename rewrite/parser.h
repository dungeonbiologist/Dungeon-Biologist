#ifndef PARSE
#define PARSE
creature* tokenize(string letters, creature* victim);
int changeProp(int properties, bool NOT, string symbol);
void parse(string file);
#endif