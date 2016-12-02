#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include "iftun.h"




int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
  	perror("création tun");
  	exit(1);
  }
     
  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN; 
  if( *dev )
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
     close(fd);
     return err;
  }
  strcpy(dev, ifr.ifr_name);
  return fd;
}
void rewrite(int src,int dest){
 while(1){
  char c[2048];
  read(src,c,2048);
  write(dest,c,2048);
}
}

int main (int argc, char** argv){
  int fd = 0;
  if (argc != 2) {
    printf("usage : tun_name %s\n",argv[0]);
    exit(1);
  }
  char cmd[128];

  fd = tun_alloc(argv[1]);

  sprintf(cmd,"./configure-tun.sh %s",argv[1]);
  system(cmd);

  rewrite(fd,1);
  return fd;
}