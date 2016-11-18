#ifndef _BRUTEFORCE_H
#define _BRUTEFORCE_H

void iterate(char *str, const char *base[], int idx, int len, FILE *output);
void keyGenerator(unsigned int len);
int match(char *user, char *pass, char *ip);
int bruteforce(unsigned int lenKey, char *ip, char *username, char *password);

#endif
