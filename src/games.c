/*
** EPITECH PROJECT, 2020
** co
** File description:
** navy
*/

#include <unistd.h>
#include "../include/navy.h"

void *global_data = NULL;

void game_loop_second(char *f, char *msg, char **map, char **map2)
{
    my_printf("%s: %s\n\n", f,
    *(msg + 1) - '1' ? "hit" : "missed");
    set(map2, f, *(msg + 1) - '1');
    print_game(map, map2, 0);
}

void game_loop(char **map, char **map2, int pid, char *f)
{
    int boom;
    char *msg = malloc(3);
    if (!f)
        f = malloc(3);
    while (1) {
        usleep(1000);
        if (global_data) {
            my_strncpy(msg, global_data, 2);
            free(global_data);
            global_data = NULL;
            if (*msg < 'J') {
                game_loop_third(pid, msg, map, boom);
                f = attack(f, pid);
                print_game(map, map2, 0);
            } else
                game_loop_second(f, msg, map, map2);
        }
    }
}

int pid_f(void)
{
    int pid;
    if (global_data) {
        pid = *((int *) global_data);
        global_data = NULL;
    }
    return pid;
}

void player_one(char **av)
{
    size_t size = 3;
    char **map = map_init(av[1]);
    char **map2 = map_init(NULL);
    char *f = malloc(size--);
    struct sigaction signal1 = {0};
    struct sigaction signal2 = {0};
    signal1.sa_sigaction = connection_a;
    signal1.sa_flags = SA_SIGINFO;
    my_printf("waiting for enemy connection...\n\n");
    sigaction(SIGUSR2, &signal1, NULL);
    pause();
    int pid = pid_f();
    signal2.sa_sigaction = receive;
    sigaction(SIGUSR1, &signal2, NULL);
    sigaction(SIGUSR2, &signal2, NULL);
    my_printf("enemy connected\n\n");
    print_game(map, map2, 1);
    f = attack(f, pid);
    game_loop(map, map2, pid, f);
}

void player_two(char **av)
{
    struct sigaction signal1 = {0};
    struct sigaction signal2 = {0};
    char **map = map_init(av[2]);
    char **map2 = map_init(NULL);
    signal1.sa_sigaction = connection_b;
    signal1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &signal1, NULL);
    int pid = my_getnbr(av[1]);

    if (kill(pid, SIGUSR2) == -1)
        return;
    pause();
    signal2.sa_sigaction = receive;
    sigaction(SIGUSR1, &signal2, NULL);
    sigaction(SIGUSR2, &signal2, NULL);
    print_game(map, map2, 0);
    my_printf("\nwaiting for enemy's attack...\n");
    game_loop(map, map2, pid, NULL);
}