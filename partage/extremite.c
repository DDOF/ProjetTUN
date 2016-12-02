/* echo / serveur simpliste
   Master Informatique 2012 -- Université Aix-Marseille  
   Emmanuel Godard
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "iftun.h"
#include "extremite.h"
/* taille maximale des lignes */
#define MAXLIGNE 80
#define CIAO "Au revoir ...\n"

int ext_in(char * hote, int tunfd)
{
  char * port; /* port TCP du serveur */   
  char *ip; /* adresse IPv6 en notation pointée */
  struct addrinfo *resol,indic; /* struct pour la résolution de nom */
  int s; /* descripteur de socket */
  memset(&indic, 0, sizeof(indic));
/* set-up hints structure */
  indic.ai_family = PF_UNSPEC;
  indic.ai_socktype = SOCK_STREAM;
  /* Traitement des arguments */



  port="123"; /* port TCP du serveur */   

  /* Résolution de l'hôte */
if ( getaddrinfo(hote,port,&indic, &resol) < 0 ){
  perror("résolution adresse");
  exit(2);
}
/* Switch pour connaitre le type d'ip */
struct sockaddr_storage ss;
switch (ss.ss_family) {
  case AF_INET:
  inet_ntop(ss.ss_family,
    &((struct sockaddr_in *)&ss)->sin_addr, ip, MAXLIGNE);
  break;
  case AF_INET6:
  inet_ntop(ss.ss_family,
    &((struct sockaddr_in6 *)&ss)->sin6_addr, ip, MAXLIGNE);
  break;
}
  /* On extrait l'addresse IP */
//sprintf(ip,"%s",inet_ntop(AF_INET6,&((struct sockaddr_in6*)resol->ai_addr)->sin6_addr,ip,INET6_ADDRSTRLEN));

  /* Création de la socket, de type TCP / IP */
  /* On ne considère que la première adresse renvoyée par getaddrinfo */
if ((s=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0) {
  perror("allocation de socket");
  exit(3);
}
fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* Connexion */
fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
  hote,ip,port);
if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
  perror("connexion");
  exit(4);
}
  freeaddrinfo(resol); /* /!\ Libération mémoire */
  rewrite(tunfd,s);
  /* Session */
  //char tampon[MAXLIGNE + 3]; /* tampons pour les communications */
//ssize_t lu;
//int fini=0;
//rewrite(tunfd,s);
// while( 1 ) { 
//   /* Jusqu'à fermeture de la socket (ou de stdin)     */
//   /* recopier à l'écran ce qui est lu dans la socket  */
//   /* recopier dans la socket ce qui est lu dans stdin */

//   /* réception des données */
//   lu = recv(tunfd,tampon,MAXLIGNE,0); /* bloquant */
//   if (lu == 0 ) {
//     fprintf(stderr,"Connexion terminée par l'hôte distant\n");
//     break; /* On sort de la boucle infinie */
//   }
//   tampon[lu] = '\0';
//   printf("reçu: %s",tampon);
//   if ( fini == 1 )
//     break;   on sort de la boucle infinie

//   /* recopier dans la socket ce qui est entré au clavier */    
//   if ( fgets(tampon,MAXLIGNE - 2,stdin) == NULL ){/* entrée standard fermée */
//     fini=1;
//   fprintf(stderr,"Connexion terminée !!\n");
//   fprintf(stderr,"Hôte distant informé...\n");
//     shutdown(s, SHUT_WR); /* terminaison explicite de la socket 
//          dans le sens client -> serveur */
//     /* On ne sort pas de la boucle tout de suite ... */
//   }else{   /* envoi des données */
//   send(s,tampon,strlen(tampon),0);
// }
// } 
  /* Destruction de la socket */
close(s);

fprintf(stderr,"Fin de la session.\n");
return EXIT_SUCCESS;
}


int ext_out(int tunfd)
{
  int s,n; /* descripteurs de socket */
  int len,on; /* utilitaires divers */
  struct addrinfo * resol; /* résolution */
  struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           AF_INET6,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
  struct sockaddr_in6 client; /* adresse de socket du client */
  char * port; /* Port pour le service */
  int err; /* code d'erreur */
  
  
  port="123";
  fprintf(stderr,"Ecoute sur le port %s\n",port);
  err = getaddrinfo(NULL,port,&indic,&resol); 
  if (err<0){
    fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
    exit(2);
  }

  /* Création de la socket, de type TCP / IP */
  if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }
  fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* On rend le port réutilisable rapidement /!\ */
  on = 1;
  if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
    perror("option socket");
    exit(4);
  }
  if (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY,
                     (char *)&on, sizeof(on)) == -1)
          perror("setsockopt IPV6_V6ONLY");
  fprintf(stderr,"Option(s) OK!\n");

  /* Association de la socket s à l'adresse obtenue par résolution */
  if (bind(s,resol->ai_addr,resol->ai_addrlen)<0) {
    perror("bind");
    exit(5);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */
  fprintf(stderr,"bind!\n");

  /* la socket est prête à recevoir */
  if (listen(s,SOMAXCONN)<0) {
    perror("listen");
    exit(6);
  }
  fprintf(stderr,"listen!\n");
  echo(s,tunfd);
  // while(1) {
  //   /* attendre et gérer indéfiniment les connexions entrantes */
  //   len=sizeof(struct sockaddr);
  //   if( (n=accept(s,(struct sockaddr*)&client,(socklen_t*)&len)) < 0 ) {
  //     perror("accept");
  //     exit(7);
  //   }
  //   /* Nom réseau du client */
  //   char hotec[NI_MAXHOST];  char portc[NI_MAXSERV];
  //   err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
  //   if (err < 0 ){
  //     fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
  //   }else{ 
  //     fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
  //   }
  //   /* traitement */
  //   echo(n,tunfd);
  // }
  return EXIT_SUCCESS;
}

/* echo des messages reçus (le tout via le descripteur f) */
void echo(int f,int tunfd)
{
  int lu; /* nb d'octets reçus */
  char tampon[MAXLIGNE+1]; 

  /* message d'accueil */
  //snprintf(msg,MAXLIGNE,"Bonjour %s! (vous utilisez le port %s)\n",hote,port);
  /* envoi du message d'accueil */
  
  do { /* Faire echo et logguer */

    lu = read(f,tampon,MAXLIGNE);
    if (lu > 0 )
      {
        tampon[lu] = '\0';
        /* log */
        //fprintf(stderr,"[%s:%s](%i): %3i :%s",hote,port,pid,compteur,tampon);
        //snprintf(msg,MAXLIGNE,"%s",tampon);
        /* echo vers le client */
        int s = write(tunfd, tampon, lu);
         if (s == -1)
          {
            perror ("write");
            abort ();
          }
      } else {
        break;
      }
  } while ( 1 );
       
  /* le correspondant a quitté */
  send(f,CIAO,strlen(CIAO),0);
  close(f);
  //fprintf(stderr,"[%s:%s](%i): Terminé.\n",hote,port,pid);
}