#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

#define EVENT_DEVICE    "/dev/input/event4"
#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_X    ABS_X
#define EVENT_CODE_Y    ABS_Y

/* TODO: Close fd on SIGINT (Ctrl-C), if it's open */

int main(int argc, char *argv[])
{
    struct input_event ev;
    int fd;
    char name[256] = "Unknown";

    if ((getuid()) != 0) {
        fprintf(stderr, "You are not root! This may not work...\n");
        return EXIT_SUCCESS;
    }

    /* Open Device */
    fd = open(EVENT_DEVICE, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "%s is not a vaild device\n", EVENT_DEVICE);
        return EXIT_FAILURE;
    }

    /* Print Device Name */
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
    printf("Reading from:\n");
    printf("device file = %s\n", EVENT_DEVICE);
    printf("device name = %s\n", name);

    for (;;) {
        const size_t ev_size = sizeof(struct input_event);
        ssize_t size;

        /* TODO: use select() */
        // if (kbhit())
        // {
        //     printf("exiting ...");
        //     return EXIT_SUCCESS;
        // }

        size = read(fd, &ev, ev_size);
        if (size < ev_size) {
            fprintf(stderr, "Error size when reading\n");
            goto err;
        }

        // if (ev.type == EVENT_TYPE && (ev.code == EVENT_CODE_X
        //               || ev.code == EVENT_CODE_Y)) {
        //     /* TODO: convert value to pixels */
        //     printf("%s = %d\n", ev.code == EVENT_CODE_X ? "X" : "Y",
        //             ev.value);
        // }

        if (ev.type == EVENT_TYPE)
        {
            if (ev.code == EVENT_CODE_X)
            {
                int x = ev.value * 3840 / 32767;

                if (x >= 0 && x <= 3840)
                {
                    printf("X = %d\n", x);
                }
            }

            if (ev.code == EVENT_CODE_Y)
            {
                int y = ev.value * 2160 / 32767;

                if (y >= 0 && y <= 2160)
                {
                    printf("Y = %d\n", y);
                }
            }
        }
    }

    return EXIT_SUCCESS;

err:
    close(fd);
    return EXIT_FAILURE;
}
