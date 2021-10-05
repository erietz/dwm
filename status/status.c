#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* print_storage()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = popen("df -Ht ext4", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    /* The output looks likes this and we are parsing the second line */

    /* Filesystem      Size  Used Avail Use% Mounted on */
    /* /dev/dm-0       237G   56G  170G  25% / */

    while ((read = getline(&line, &len, fp)) != -1) {
        /* printf("%s", line); */

        if (strstr(line, "/dev/")) {
            strtok(line, " ");
            char *total = strtok(NULL, " ");
            /* strtok(NULL, " "); // for getting the 4th column rather than 3rd */
            char *avail = strtok(NULL, " ");

            char storage[20];
            sprintf(storage, "%s/%s", avail, total);

            fclose(fp);
            if (line)
                free(line);
            return strdup(storage);
        }
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

}

int main(void)
{
    char *storage = print_storage();

    char cmd[100];
    while (1) {
        sprintf(cmd, "xsetroot -name \"%s\"", storage);
        /* system(cmd); */
        puts(cmd);
        sleep(1);
    }

    return 0;
}
