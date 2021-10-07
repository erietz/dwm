/*
 * Author      : Ethan Rietz
 * Date        : 2021-10-04
 * Description : A status line that is more efficient than a shell script
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define STATUS_LENGTH 256
#define COMPONENT_LENGTH 20

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

            char storage[COMPONENT_LENGTH];
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

char* print_memory()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        /* printf("%s", line); */

        if (strstr(line, "MemAvailable")) {
            strtok(line, " ");
            char *availChar = strtok(NULL, " ");
            float avail = atof(availChar);

            char memory[COMPONENT_LENGTH];
            sprintf(memory, "%.2f GiB", avail / 1024 / 1024);

            fclose(fp);
            if (line)
                free(line);
            return strdup(memory);
        }
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

}

char* print_date() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);
    char date[COMPONENT_LENGTH];
    sprintf(date, "%s\n", s);
    return strdup(date);
}

char* print_battery() {
    // TODO this is dumb because it will print 10% when at 100%
    FILE *fp;
    char line[2];
    size_t len = 0;
    ssize_t read;

    fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (fp == NULL)
        return "no bat";

    line[0] = fgetc(fp);
    line[1] = fgetc(fp);

    fclose(fp);
    return strcat(strdup(line), "% charge");

}

char* print_temperature() {
    FILE *fp;
    char line[2];
    size_t len = 0;
    ssize_t read;

    fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (fp == NULL)
        return "no bat";

    line[0] = fgetc(fp);
    line[1] = fgetc(fp);

    fclose(fp);
    return strcat(strdup(line), " C");

}

int main(void)
{
    char *storage = print_storage();
    char *memory = print_memory();
    char *date = print_date();
    char *battery = print_battery();
    char *temperature = print_temperature();

    char cmd[STATUS_LENGTH];
    while (1) {
        sprintf(cmd, "xsetroot -name \"%s | %s | %s | %s | %s\"", storage, memory, temperature, battery, date);
        system(cmd);
        /* puts(cmd); */
        sleep(1);
    }

    return 0;
}
