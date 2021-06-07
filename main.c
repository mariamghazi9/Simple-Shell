#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define MAX_SIZE 1000


void removeEndOfLine(char line[])
{
    int i=0;
    while(line[i] != '\n')
        i++;
    line[i]='\0';
}
void readLine(char line[])
{
    char* read=fgets(line,MAX_SIZE,stdin);
    removeEndOfLine(line);
    if(strcmp(line,"exit")==0 || read==NULL)
        exit(0);
}
int parseLine(char *args[],char line[])
{
    int i=0;
    args[i]=strtok(line," ");
    if (args[i]==NULL)
        return 1;
    while(args[i] != NULL)
    {
        i++;
        args[i]=strtok(NULL," ");
    }
    return 1;
}


int waitCheck(char *args[])
{
    int flag=0;
    int size =0;
    while(args[size] != NULL)
    {
        size++;
    }
    if(strcmp(args[size-1],"&")==0)
    {
        flag=1;
    }
    else flag=0;

    return flag;
}
int readParseLine(char *args[],char line[])
{
    readLine(line);
    parseLine(args,line);
    return 1;
}
void logFile()
{
    FILE *fp;
    fp = fopen("log.txt", "a");
    fputs("Child process was terminated\n", fp);
    fclose(fp);
}
void processesExit()
{
    int wstat;
    pid_t	pid;
    logFile();

    while (1)
    {
        pid = wait3 (&wstat, WNOHANG, (struct rusage *)NULL );
        if (pid == 0)
            return;
        else if (pid == -1)
            return;
    }
}


int main()
{
    char * arguments[MAX_SIZE];
    char input[MAX_SIZE];

    signal(SIGCHLD, processesExit);
    while (readParseLine(arguments,input))
    {
        pid_t child_pid = fork();
        if(child_pid == 0)
        {
            if(execvp(arguments[0],arguments) < 0)
            {
                perror("fork error");
                exit(EXIT_FAILURE);

            }
        }
        else if(child_pid > 0)
        {
            if(!waitCheck(arguments))
            {
                waitpid(child_pid, NULL, 0);
            }
        }
    }

    return 0;
}
