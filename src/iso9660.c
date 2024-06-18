#include "iso9660.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void help()
{
    printf("help: display command help\n");
    printf("info: display volume info\n");
    printf("ls: display the content of a directory\n");
    printf("cd: change current directory\n");
    printf("tree: display the tree of a directory\n");
    printf("get: copy file to local directory\n");
    printf("cat: display file content\n");
    printf("pwd: print current path\n");
    printf("quit: exit program\n");
}

void info(struct iso_prim_voldesc *pvd)
{
    printf("System Identifier: %.32s\n", pvd->syidf);
    printf("Volume Identifier: %.32s\n", pvd->vol_idf);
    printf("Block count: %u\n", pvd->vol_blk_count.le);
    printf("Block size: %u\n", pvd->vol_blk_size.le);
    printf("Creation date: %.17s\n", pvd->date_creat);
    printf("Application Identifier: %.128s\n", pvd->app_idf);
}

//  Notre fonction ls :)
void list_directory(struct iso_dir *dir, void *iso_data)
{
    uint8_t *ptr = (uint8_t *)iso_data + dir->data_blk.le * ISO_BLOCK_SIZE;
    while (ptr < (uint8_t *)iso_data + dir->data_blk.le * ISO_BLOCK_SIZE
               + dir->file_size.le)
    {
        struct iso_dir *entry = (struct iso_dir *)ptr;
        if (entry->dir_size == 0)
            break;

        printf("%c%c %9u %.*s\n", (entry->type & ISO_FILE_ISDIR) ? 'd' : '-',
               (entry->type & ISO_FILE_HIDDEN) ? 'h' : '-', entry->file_size.le,
               entry->idf_len, (char *)(entry + 1));

        ptr += entry->dir_size;
    }
}
// Notre fonction cat :)
void cat(struct iso_dir *dir, void *iso_data, const char *filename)
{
    uint8_t *ptr = (uint8_t *)iso_data + dir->data_blk.le * ISO_BLOCK_SIZE;
    while (ptr < (uint8_t *)iso_data + dir->data_blk.le * ISO_BLOCK_SIZE
               + dir->file_size.le)
    {
        struct iso_dir *entry = (struct iso_dir *)ptr;
        if (entry->dir_size == 0)
            break;

        if (strncmp((char *)(entry + 1), filename, entry->idf_len) == 0)
        {
            uint8_t *file_ptr =
                (uint8_t *)iso_data + entry->data_blk.le * ISO_BLOCK_SIZE;
            write(STDOUT_FILENO, file_ptr, entry->file_size.le);
            return;
        }

        ptr += entry->dir_size;
    }
    printf("Fichier non trouvé: %s\n", filename);
}

// Notre fonction qui nous permet de changer de directory cd :)
void change_directory(struct iso_dir *current_dir, void *iso_data,
                      const char *path)
{
    uint8_t *ptr =
        (uint8_t *)iso_data + current_dir->data_blk.le * ISO_BLOCK_SIZE;
    while (ptr < (uint8_t *)iso_data + current_dir->data_blk.le * ISO_BLOCK_SIZE
               + current_dir->file_size.le)
    {
        struct iso_dir *entry = (struct iso_dir *)ptr;
        if (entry->dir_size == 0)
            break;

        if ((entry->type & ISO_FILE_ISDIR)
            && strncmp((char *)(entry + 1), path, entry->idf_len) == 0)
        {
            *current_dir = *entry;
            return;
        }

        ptr += entry->dir_size;
    }
    printf("Dossier introuvable: %s\n", path);
}

void print_working_directory(struct iso_dir *current_dir, void *iso_data)
{
    return;
}

// Cette fonction nous permet de quitter notre programme :)
void quit()
{
    exit(0);
}

// Notre fonction qui peremet d'obternir le fichier de l'iso :)
void get_file(struct iso_dir *dir, void *iso_data, const char *filename)
{
    uint8_t *ptr = (uint8_t *)iso_data + dir->data_blk.le * ISO_BLOCK_SIZE;
    while (ptr < (uint8_t *)iso_data + dir->data_blk.le * ISO_BLOCK_SIZE
               + dir->file_size.le)
    {
        struct iso_dir *entry = (struct iso_dir *)ptr;
        if (entry->dir_size == 0)
            break;

        if (strncmp((char *)(entry + 1), filename, entry->idf_len) == 0)
        {
            uint8_t *file_ptr =
                (uint8_t *)iso_data + entry->data_blk.le * ISO_BLOCK_SIZE;
            FILE *fp = fopen(filename, "wb");
            fwrite(file_ptr, 1, entry->file_size.le, fp);
            fclose(fp);
            return;
        }

        ptr += entry->dir_size;
    }
    printf("Fichier introuvable: %s\n", filename);
}
