// pingpong.c for xv6 (assignment #1 for CSC.T371)
// name: Satoshi Takimoto
// id: 18B08951

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int fork_or_exit(void) {
    int pid = fork();
    if (pid == -1) {
        fprintf(1, "error: fork\n");
        exit(1);
    }
    return pid;
}

void read_or_exit(int fd, void *buf, int n) {
    if (read(fd, buf, n) == -1) {
        fprintf(1, "error: read\n");
        exit(1);
    }
}

void write_or_exit(int fd, void *buf, int n) {
    if (write(fd, buf, n) == -1) {
        fprintf(1, "error: write\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(1, "usage: %s N\n", argv[0]);
        exit(1);
    }

    // # of rounds
    int n = atoi(argv[1]);

    // tick value before starting rounds
    int start_tick = uptime();

    int p[2];
    pipe(p);
    int pid = fork_or_exit();
    if (pid == 0) { /* child */
        unsigned char cnt;
        for (int i = 0; i < n; i++) {
            read_or_exit(p[0], &cnt, 1);
            cnt++;
            write_or_exit(p[1], &cnt, 1);
        }

        close(p[0]);
        close(p[1]);
        exit(0);
    } else { /* parent */
        unsigned char cnt = 0;
        for (int i = 0; i < n; i++) {
            write_or_exit(p[1], &cnt, 1);
            read_or_exit(p[0], &cnt, 1);
            cnt++;
        }

        close(p[0]);
        close(p[1]);
        wait(0);
    }

    // print # of ticks in nrounds
    printf("# of ticks in %d rounds: %d\n", n, uptime() - start_tick);
    exit(0);
}
