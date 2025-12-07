#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define ITERATIONS 10

static volatile sig_atomic_t signal_received = 0;

void signal_handler(int sig) {
    (void)sig;
    signal_received = 1;
}

void setup_signal_handler(void) {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

void wait_for_signal(void) {
    while (!signal_received) {
        pause();
    }
    signal_received = 0;
}

void run_parent(pid_t child_pid) {
    // Small delay to allow child process to start
    sleep(1);
    
    for (int i = 0; i < ITERATIONS; i++) {
        printf("Parent: PING %d\n", i + 1);
        fflush(stdout);
        
        sleep(1);
        kill(child_pid, SIGUSR1);
        wait_for_signal();
    }
    
    int status;
    waitpid(child_pid, &status, 0);
}

void run_child(pid_t parent_pid) {
    for (int i = 0; i < ITERATIONS; i++) {
        wait_for_signal();
        
        printf("Child:  PONG %d\n", i + 1);
        fflush(stdout);
        
        sleep(1);
        kill(parent_pid, SIGUSR1);
    }
}

int main(void) {
    setup_signal_handler();
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    
    if (pid == 0) {
        run_child(getppid());
    } else {
        run_parent(pid);
    }
    
    return EXIT_SUCCESS;
}

