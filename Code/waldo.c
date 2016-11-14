/* Includes */
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Constantes */

/* Définition des fonctions */
void help();
int lancer (const char *cmd, const char *fichier, int maxProc, int nProc);
int parcourir (const char *cmd, const char *racine, int maxproc, int nproc);
/*Fonctions */
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

  if(argc<4)
  {
    printf("Il faut spécifier au moins 3 paramètres (%d/3 fournis)\n Plus d'information avec l'option -h",argc-1 );
  }

  for(i=1;i<argc;i++)
  {
    /*If -h parameter is specified, print instructions and exit */
    if(argv[i]=="-h")
    {
      help();
      return 0;
    }
    /*If -e parameter is specified, check if the next parameter exist and then
    assign it to the cmd var */
    else if(argv[i]=="-e" && i+1<argc)
    {
      cmd=(char*)argv[++i];
    }
    /*If -n parameter is specified and the next parameter exist*/
    else if(argv[i]=="-n" && i+1<argc)
    {
      /*try to convert the next parameter to a long*/
      long conv = strtol(argv[++i], &p, 10);
      /* If the parameter is a number and is under INT_MAX*/
      if (errno != 0 || *p != '\0' || conv > INT_MAX || conv <= 0)
      {
        printf("%s is not a valid number\n Exiting ...",argv[i]);
        return -1;
      }
      else
      {
          maxProc = conv;
      }
    }
    /* if it's not a parameter it must be a path */
    else
    {
      path=(char*)argv[i];
    }
  }

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

  parcourir(cmd, path, maxProc,0);

  return 0;
}
