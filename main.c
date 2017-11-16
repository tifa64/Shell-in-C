#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void readline(char *line, char **argv)
{
    gets(line);
    *argv = NULL;
    char* del = strtok(line," ");
    while(del != NULL) {
        *argv++ = del;
        del = strtok(NULL, " ");
     }
     *argv = NULL;

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
     char  input[1024];
     char  *argv[64];
     pid_t pid;
     int status;

     while (1) {
          printf("Shell -> ");
          readline(input, argv);
          printf("\n");
          if ((pid = fork()) < 0) {
            printf("Error !! child process failed\n");
            exit(1);
          }
          else if (pid == 0) {
              if (execvp(*argv, argv) < 0) {
                  if(strcmp(argv[0], "cd") == 0) {
                      if (chdir(argv[1]) < 0)
                          perror("");
                  }
                  else {
                      perror("Error !! exec failed\n");
                      exit(1);
                  }
             }
         }
         else {
             if(!inlimit(input)) {
                printf("Error !! characters exceeded 512\n");
                continue;
             }
             int size = 0;
             char **temp = argv;
             while(*temp != NULL) {
                 printf("%s\n", *temp);
                 size++;
                 *temp++;
             }
             if(strcmp(argv, "&") == 0)
                waitpid(pid, &status, WUNTRACED);
             else
                while (wait(&status) != pid);
         }
     }
     return 0;
}
