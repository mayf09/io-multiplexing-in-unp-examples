#include <stdio.h>
#include <time.h>
#include <sys/select.h>
#include <sys/poll.h>

#define OPEN_MAX 16

void print_fd(char *name, int line, int fd);
void print_select_fds(char *name, int line, fd_set *fds);
void print_poll_macro();
void print_poll_fds(char *name, int line, struct pollfd *fds);

/* 打印文件描述符（整数） */
void print_fd(char *name, int line, int fd)
{
    time_t t;
    printf("%ld (%s) L%d: %d\n\n", time(&t), name, line, fd);
}

/* 使用十六进制输出 fd_set 数组第一个元素，用于观察 fd_set 变化
 * select 支持 1024 个文件描述符
 * 在 64 位机器上，使用 1024/64 = 16 个元素的数组表示（具体查看 fd_set 结构定义）
 * 本示例中，我们只观察不超过 64 个文件描述符，所以只打印 fd_set 数组第一个元素 */
void print_select_fds(char *name, int line, fd_set *fds)
{
    time_t t;
    printf("%ld (%s) L%d: %0*lx\n\n", time(&t), name, line, __NFDBITS / 4, __FDS_BITS(fds)[0]);
}

void print_poll_macro()
{
    printf("POLLIN   %#05x\n", POLLIN);
    printf("POLLOUT  %#05x\n", POLLOUT);
    printf("POLLERR  %#05x\n", POLLERR);
    printf("POLLHUP  %#05x\n", POLLHUP);
    printf("POLLNVAL %#05x\n", POLLNVAL);
    printf("\n");
}

/* 打印 pollfd 内容 */
void print_poll_fds(char *name, int line, struct pollfd *fds)
{
    int i;
    time_t t;
    printf("%ld (%s) L%d:\n", time(&t), name, line);
    for (i = 0; i < OPEN_MAX; i++)
    {
        printf("(%2d %04x %04x) ", fds[i].fd, fds[i].events, fds[i].revents);
        if (!((i+1) % 4)) {
            printf("\n");
        }
    }
    printf("\n");
}