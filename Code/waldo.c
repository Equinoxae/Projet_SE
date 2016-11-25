/* Includes */
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
/* Constantes */
#define MY_EOF -1

/* Définition des fonctions */
void help();
int lancer (const char *cmd, const char *fichier, int maxProc, int nProc);
int parcourir (const char *cmd, const char *racine, int maxproc, int nproc);
void attendre();
/*Fonctions */

/*Affiche l'aide à l'utilisation du programme*/
void help()
{
  printf("Utilisation de findexec\nParamètres optionnels:\n -e [commande à éxecuter, ls par défaut]\n");
  printf(" -h affichage de l'aide\n -n [0<int] (nombre de processus simultanées max)\n");
  printf("Appel : \n findexec [parameters] path(s)\n");
}

void attendre()
{
  int rWait; //variable du retour de wait()
  wait(&rWait);
  if(WIFEXITED(rWait))
    printf("exit(%d)\n",WEXITSTATUS(rWait));
  else if(WIFSIGNALED(rWait))
    printf("signal %d\n",WTERMSIG(rWait));
  else
    printf("autre\n" );
}

int parcourir (const char *cmd, const char *racine, int maxProc, int nProc)
{
  struct dirent *d;
  DIR *dp;
  struct stat statD;


  if ((dp = opendir(racine)) == NULL)
    return MY_EOF;

  chdir(racine);

  while(( d = readdir(dp)) != NULL )
  {
    if((strcmp(d->d_name,".") != 0) && (strcmp(d->d_name,"..") != 0))
    {
      stat(d->d_name,&statD);

      if(S_ISDIR(statD.st_mode))
      {
        parcourir (cmd,d->d_name,maxProc,nProc);
      }
      else if(S_ISREG(statD.st_mode)&&(statD.st_mode & S_IRUSR))
      {
        nProc=lancer(cmd,d->d_name,maxProc,nProc);
      }
      else
      {
        fprintf(stderr, "%s\n",strerror(errno));
      }
    }
  }

  chdir("..");

  if ( closedir(dp) == -1)
  {
    return MY_EOF;
  }

  return nProc;
}

int lancer (const char *cmd, const char *fichier, int maxProc, int nProc)
{
  int fd;
  pid_t f;
  f=fork();

  switch (f)
  {
    case 0: /* fils */
      if((fd= open(fichier,O_RDONLY))!=-1)
      {
        dup2(fd,0);
        close(0);
        execlp(cmd,cmd,fichier,NULL);
      }
      break;
    case -1: /* erreur */
      printf("Error\n");
      return MY_EOF;
    default: /*pere*/
      nProc++;
      if(nProc>=maxProc)
      {
        attendre();
        nProc--;
      }

      break;
  }
  return nProc;
}

int main(int argc, char const *argv[])
{
  int i;
  char* cmd="ls";
  char* p;
  int maxProc=1;
  int nProc=0;
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
         return MY_EOF;
       }
       else
       {
           maxProc = conv;
       }
       break;
     case 'e':
       cmd = optarg;
       break;
     case '?':
       if (optopt == 'n' || optopt == 'e')
         fprintf (stderr, "Option -%c requires an argument.\n", optopt);
       else if (isprint (optopt))
         fprintf (stderr, "Unknown option `-%c'.\n", optopt);
       else
         fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
       return -1;
     default:
       abort();
  }
  for(i=optind;i<argc;i++)
  {
    nProc=parcourir(cmd,argv[i],maxProc,nProc);
  }

  while(nProc>0)
  {
    attendre();
    nProc--;
  }

  return 0;
}
