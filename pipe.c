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
        exit(errno);
    }
    if (argc == 2) {
        if(execlp(argv[1], argv[1], NULL) < 0)
            exit(errno);
    } else {
        int fds[2];
        
        for (int i = 1; i < argc; i++) {
            if(pipe(fds) == -1){
                errno = EBADF;
                exit(errno); //EBADF
            }
            pid_t ret = fork();
            if(ret == -1)
                exit(errno);
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
                if(execlp(argv[i], argv[i], NULL) < 0){
                    exit(errno);
                    
                }
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
