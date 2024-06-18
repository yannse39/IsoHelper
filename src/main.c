#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "iso9660.c"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <iso file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0)
    {
        perror("fstat");
        close(fd);
        return 1;
    }

    void *iso_data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (iso_data == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    struct iso_prim_voldesc *pvd =
        (struct iso_prim_voldesc *)((uint8_t *)iso_data
                                    + ISO_BLOCK_SIZE * ISO_PRIM_VOLDESC_BLOCK);

    struct iso_dir current_dir = pvd->root_dir;

    char command[256];
    while (1)
    {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "help") == 0)
        {
            help();
        }
        else if (strcmp(command, "info") == 0)
        {
            info(pvd);
        }
        else if (strcmp(command, "ls") == 0)
        {
            list_directory(&current_dir, iso_data);
        }
        else if (strncmp(command, "cat ", 4) == 0)
        {
            cat(&current_dir, iso_data, command + 4);
        }
        else if (strncmp(command, "cd ", 3) == 0)
        {
            change_directory(&current_dir, iso_data, command + 3);
        }
        else if (strcmp(command, "pwd") == 0)
        {
            print_working_directory(&current_dir, iso_data);
        }
        else if (strcmp(command, "quit") == 0)
        {
            quit();
        }
        else if (strncmp(command, "get ", 4) == 0)
        {
            get_file(&current_dir, iso_data, command + 4);
        }
        else
        {
            printf("Unknown command: %s\n", command);
        }
    }

    munmap(iso_data, sb.st_size);
    close(fd);
    return 0;
}
