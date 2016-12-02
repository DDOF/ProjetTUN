#ifndef IFTUN_H
#define IFTUN_H

int tun_alloc(char *dev);
void rewrite(int src,int dest);

#endif