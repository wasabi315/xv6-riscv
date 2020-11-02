// pingpong.c for xv6 (assignment #1 for CSC.T371)
// name: Satoshi Takimoto
// id: 18B08951

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void writeNumber(unsigned char n) {
    int n1 = n / 100;
    int n2 = (n % 100) / 10;
    int n3 = n % 10;

    if (n1 != 0) {
        char c1[1] = {n1 + '0'};
        write(1, c1, 1);
    }
    if (n2 != 0) {
        char c2[1] = {n2 + '0'};
        write(1, c2, 1);
    }
    char c3[1] = {n3 + '0'};
    write(1, c3, 1);
    return;
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
    int pid = fork();
    if (pid == 0) { /* child */
        unsigned char cnt;
        for (int i = 0; i < n; i++) {
            read(p[0], &cnt, 1);

            write(1, "Child read: ", 12);
            writeNumber(cnt);
            write(1, "\n", 1);

            cnt++;
            write(p[1], &cnt, 1);
        }

        close(p[0]);
        close(p[1]);
        exit(0);
    } else { /* parent */
        unsigned char cnt = 0;
        for (int i = 0; i < n; i++) {
            write(p[1], &cnt, 1);
            read(p[0], &cnt, 1);

            write(1, "Parent read: ", 13);
            writeNumber(cnt);
            write(1, "\n", 1);

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
