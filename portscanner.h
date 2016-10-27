#ifndef PORTSCANNER_H
#define PORTSCANNER_H

int validateIPAddr(char *ip);
char* itoa(int val, int base);
void createToken(char *range[],  char string[], const char delimiter[], int *cont);
void constructIPRange(char range_ipSeq[], char *rangeTotal[], int *numIP);
void constructPortRange(char range_portSeq[], unsigned int range[], int *numPort);
int connectIP(unsigned int port_number, char *ip);
void initPortScanner(char range_ipSeq[], char range_portSeq[]);

#endif
