#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
//last prg should not have a pipe
int main(int argc, char *argv[]) {
    if(argc == 1){
        errno = EINVAL;
        exit(1);
    }
    if (argc == 2) {
        execlp(argv[1], argv[1], NULL);
    } else {
        int fds[2];
        
        for (int i = 1; i < argc; i++) {
            pipe(fds);
            // TODO: ERROR CHECK IF pipe() RETURNS -1
            pid_t ret = fork();
            // TODO: ERROR CHECK IF fork() RETURNS -1
            if (ret == 0) { // child
                if (i != argc - 1) {
                    close(fds[0]); // Close the read end of the pipe
                    dup2(fds[1], STDOUT_FILENO);// Redirect stdout to the write end of the pipe
                    close(fds[1]);
                } else {
                    close(fds[1]); // Close the write end of the pipe
                    //dup2(fds[0], STDIN_FILENO);// Redirect stdin to the read end of the pipe
                    close(fds[0]);
                }
                execlp(argv[i], argv[i], NULL);
                // TODO: ERROR CHECK IF execlp() RETURNS -1
            } else { // parent
                if (i != argc - 1) {
                    close(fds[1]);// Close the write end of the pipe
                    dup2(fds[0], STDIN_FILENO);
                    close(fds[0]);
                }
                else{
                    close(fds[0]);
                    dup2(fds[1], STDOUT_FILENO);
                    close(fds[1]);
                }
                //close(fds[0]); // Close the read end of the pipe
                waitpid(ret, NULL, 0);
            }
        }
    }

    return 0;
}
