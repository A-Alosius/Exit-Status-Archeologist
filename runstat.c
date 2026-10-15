#include "runstat.h"

// WCOREDUMP is not defined in some linux distros. 
// This line remedies that
#define _DEFAULT_SOURCE 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void runstat(char* command, char* argv[]){
    int fd = fork();

    if (fd == -1){
        perror("Fork failed: ");
        _exit(-1);
    }

    // we are in child
    if (fd == 0){
        errno = 0;
        execvp(command, argv);  
    
        // check error number and adjust exit status based on specific issue
        _exit(errno); // command not found code
    }
    else{
        int status;
        // use while loop and check errno for Interrupt in case waitpid failed
        // because a signal interrupted or OS needed to pause it to use resources
        // etc.
        while (waitpid(fd, &status, 0) == -1){
            if (errno == EINTR){
                continue;
            }
            perror("Could wait for child ");
            return;
        }

        if (WIFEXITED(status)){
            if (WEXITSTATUS(status) == ENOENT){
                printf("command not found: status=%d\n", WEXITSTATUS(status));
                return;
            }
            if (WEXITSTATUS(status) == EACCES){
                printf("Permission denied: status=%d\n", WEXITSTATUS(status));
                return;
            }
            if (WEXITSTATUS(status) == EISDIR){
                printf("Expected file not directory: status=%d\n", WEXITSTATUS(status));
                return;
            }

            printf("normal exit: status=%d\n", WEXITSTATUS(status));
        }

        else if (WIFSIGNALED(status)){
            printf("signal termination: signal=%d, core=%d\n", WTERMSIG(status), WCOREDUMP(status));
        }
    }
}

int main (int argc, char** argv){
    if (argc < 2){
        printf("Too few inputs\n");
        exit(1);
    }

    runstat(argv[1], &argv[1]);
    return 0;
}