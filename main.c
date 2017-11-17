#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void readline(char *line, char **comand)
{
    gets(line);
    *comand = NULL;
    char* del = strtok(line," ");
    while(del != NULL) {
        *comand++ = del;
        del = strtok(NULL, " ");
     }
     *comand = NULL;

}
int inlimit(char *line) {
    int size = 0;
    while(*line != NULL) {
        *line++;
        size++;
    }
    return size <= 512;
}

int main()
{
     char  input[1000];
     char  *comand[1000];
     pid_t pid;
     int status;
     int c, d;
     FILE *file, *file2;
     file = fopen("Minion.txt", "r");
     if (file) {
         while ((c = getc(file)) != EOF)
             putchar(c);
             fclose(file);
     }
     file2 = fopen("Welcome.txt", "r");
     if (file) {
         while ((d = getc(file)) != EOF)
             putchar(d);
             fclose(file);
     }

     while (1) {
          printf("Shell -> ");
          readline(input, comand);
          printf("\n");
          int size = 0, isBG = 0;
          char **temp = comand;
	  char **temp2 = comand;
          while(*temp != NULL) {
                 size++;
                 *temp++;
          }
          if ((pid = fork()) < 0) {
            printf("Error !! child process failed\n");
            exit(1);
          }
          else if (pid == 0) {
              if(!inlimit(input)) {
                  printf("Error !! characters exceeded 512\n");
              }
              else {
                  if(strcmp(comand[size-1], "&") == 0) {
                      isBG = 1;
                      comand[size-1] = NULL;
                  }
                  if(strcmp(comand[0], "cd") == 0) {
                      if (chdir(comand[1]) < 0)
                          perror("");
                  }
                  else {
                      int val = execvp(*comand, comand);
                      if (val < 0) {
                          printf("Error !! exec failed\n");
                      }
                 }
              }
         }
         else {
            if(strcmp(temp2[size-1], "&") != 0)
                waitpid(pid, &status, WUNTRACED);
        }
     }
     return 0;
}
