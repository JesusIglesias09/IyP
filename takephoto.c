#include <stdio.h>   // fprintf()
#include <unistd.h>  // fork(), exec()
#include <string.h>  // strerror()
#include <errno.h>   // errno
#include <stdlib.h>  // exit()

void forkAndExecute (const char *path, char *const args[]) {
    int pid = fork();
    if (pid == -1) {
        fprintf ( stderr,
            "fork() failed: %s",
            strerror(errno)
        );
        return;
    }
    if (pid != 0) return;
// If pid == 0, this is the child process.
    if (system("raspistill -n -o Desktop/image.jpg") == -1) {
        fprintf ( stderr,
            "execvp(%s) failed: %s %s",
            path, strerror(errno)
        );
        exit(-1);
    }
} 

int main (int argc, const char *argv[]) {
    char *const args[] = { "raspistill", "-n", "-o", "Desktop/frst_image.jpg" };
    //forkAndExecute("raspistill", args);
    system("sudo raspistill -n -o image.jpg");
    puts("Taking photo");
    sleep(7);
    return 0;
}  


