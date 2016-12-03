#ifndef EXTREMITE_H
#define EXTREMITE_H

void echo(int f,int tunfd);
int ext_out(int tunfd, char * port);
int ext_in(char * hote,char * port, int tunfd);

#endif

