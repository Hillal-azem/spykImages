#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <locale.h>
#include <sys/stat.h>


static int find_images(const char *dirname);


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

    int i = 1;
    while (i < argc) {
        if (!find_images(argv[i]))
            exit(EXIT_FAILURE);
        i++;
    }

    if (argc == 1)
    {
        find_images("C:/");
        find_images("D:/");
    }

    return EXIT_SUCCESS;
}

static int ends_with(const char *str, const char *suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    return (str_len >= suffix_len) &&
           (!memcmp(str + str_len - suffix_len, suffix, suffix_len));
}

static int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

static int find_images(const char *dirname)
{
    char buffer[PATH_MAX + 2];
    char *p = buffer;
    char *end = &buffer[PATH_MAX];

    const char *src = dirname;
    while (p < end && *src != '\0') {
        *p++ = *src++;
    }
    *p = '\0';

    DIR *dir = opendir(dirname);
    if (!dir) {
        fprintf(stderr,
                "Cannot open %s (%s)\n", dirname, strerror(errno));
        return 0;
    }

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        char *q = p;
        char c;

        if (buffer < q)
            c = q[-1];
        else
            c = ':';

        if (c != ':' && c != '/' && c != '\\')
            *q++ = '/';

        src = ent->d_name;
        while (q < end && *src != '\0') {
            *q++ = *src++;
        }
        *q = '\0';

        if(isDirectory(buffer)) {
            if (strcmp(ent->d_name, ".") != 0
                && strcmp(ent->d_name, "..") != 0) {
                find_images(buffer);
            }
        }
        else {
            if (ends_with(ent->d_name, ".jpg")
                || ends_with(ent->d_name, ".JPG")
                || ends_with(ent->d_name, ".png")
                || ends_with(ent->d_name, ".PNG")
                || ends_with(ent->d_name, ".jpeg")
                || ends_with(ent->d_name, ".gif")
                || ends_with(ent->d_name, ".svg")
                || ends_with(ent->d_name, ".bmp")
                || ends_with(ent->d_name, ".webp")) {

                /* TRANSFORMER LES IMAGES EN BUFFER POUR LES ENVOYER AU SERVEUR */
                printf("%s\n", buffer);

            }
        }
    }

    closedir(dir);
    return 1;
}