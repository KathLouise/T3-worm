#ifndef _BRUTEFORCE_H
#define _BRUTEFORCE_H

void iterate(char *str, const char *base[], int idx, int len, FILE *output);
void keyGenerator(unsigned int len);
int bruteforce(unsigned int lenKey);

#endif