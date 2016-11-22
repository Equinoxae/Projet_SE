/* Includes */
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

/* Constantes */

/* Définition des fonctions */
void help();
int lancer (const char *cmd, const char *fichier, int maxProc, int nProc);
int parcourir (const char *cmd, const char *racine, int maxproc, int nproc);
/*Fonctions */

/*Affiche l'aide à l'utilisation du programme*/
void help()
{
  printf("aide");
}

int parcourir (const char *cmd, const char *racine, int maxProc, int nProc)
{
  return 0;
}

int lancer (const char *cmd, const char *fichier, int maxproc, int nproc)
{
  return 0;
}

int main(int argc, char const *argv[])
{
  int i;
  char* cmd=NULL;
  char* path= NULL;
  char* p;
  int maxProc=1;
  errno = 0;

  int c, errflg = 0 ;
  extern char *optarg ;
  extern int optind,optopt ;
  long conv;

  while ((c = getopt(argc, argv, "hn:e:"))!= -1)
   switch (c)
     {
     case 'h':
       help();
       exit(0);
       break;
     case 'n':
        /*try to convert the next parameter to a long*/
       conv = strtol(optarg, &p, 10);
       /* If the parameter is a number and is under INT_MAX*/
       if (errno != 0 || *p != '\0' || conv > INT_MAX || conv <= 0)
       {
         printf("%s is not a valid number\n Exiting ...",optarg);
         return -1;
       }
       else
       {
           maxProc = conv;
       }
       break;
     case 'e':
       cmd = optarg;
       break;
     case '/':
      path=optarg;
      break;
     case '?':
       if (optopt == 'c')
         fprintf (stderr, "Option -%c requires an argument.\n", optopt);
       else if (isprint (optopt))
         fprintf (stderr, "Unknown option `-%c'.\n", optopt);
       else
         fprintf (stderr,
                  "Unknown option character `\\x%x'.\n",
                  optopt);
       return -1;
     default:
       abort();
  }

  printf("maxProc : %d Cmd : %s Path :%s",maxProc,cmd,argv[optind]);
  if(path==NULL)
  {
    printf("No path specified");
    return -1;
  }
  else if(cmd==NULL)
  {
    printf("No command specified");
    return -1;
  }

  return 0;
}
