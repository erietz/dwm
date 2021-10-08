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

#define STATUS_LENGTH 1000
#define COMPONENT_LENGTH 30
#define PARSE_ERROR "bad parsing..."

char* print_storage()
{
    FILE *fp;
    char *line = NULL;  // getline() will allocate enough memory for &line
    size_t len = 0;
    ssize_t read;

    fp = popen("df -Ht ext4", "r");
    if (fp == NULL) return "not ext4 drive";

    /*
     * The output looks likes this and we are parsing the second line
     *
     * Filesystem      Size  Used Avail Use% Mounted on
     * /dev/dm-0       237G   56G  170G  25% /
     *
     */

    while ((read = getline(&line, &len, fp)) != -1) {
        if (strstr(line, "/dev/")) {
            strtok(line, " ");                  // field 1
            char *total = strtok(NULL, " ");    // field 2
            char *avail = strtok(NULL, " ");    // field 3

            char storage[COMPONENT_LENGTH];
            snprintf(storage, COMPONENT_LENGTH, "%s/%s", avail, total);

            fclose(fp);
            if (line) free(line);
            return strdup(storage);
        }
    }

    fclose(fp);
    if (line) free(line);
    return PARSE_ERROR;
}

char* print_memory()
{
    FILE *fp;
    char *line = NULL;  // getline() will allocate memory for &line
    size_t len = 0;
    ssize_t read;

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) return "no meminfo";

    while ((read = getline(&line, &len, fp)) != -1) {

        if (strstr(line, "MemAvailable")) {
            strtok(line, " ");
            char *availChar = strtok(NULL, " ");
            float avail = atof(availChar);

            char memory[COMPONENT_LENGTH];
            snprintf(memory, COMPONENT_LENGTH, "%.2f GiB", avail / 1024 / 1024);

            fclose(fp);
            if (line) free(line);
            return strdup(memory);
        }
    }

    fclose(fp);
    if (line) free(line);
    return PARSE_ERROR;

}

char* print_date() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[COMPONENT_LENGTH];
    strftime(s, COMPONENT_LENGTH, "%x %X", tm);
    return strdup(s);
}

char* print_battery() {
    FILE *fp;
    char line[COMPONENT_LENGTH];

    fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (fp == NULL) return "no bat";

    fgets(line, sizeof(line), fp);
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
        line[len-1] = '\0';
    }

    fclose(fp);

    strcat(line, "% charge");
    char tmp[COMPONENT_LENGTH];
    snprintf(tmp, COMPONENT_LENGTH, "%s", line);

    return strdup(tmp);
}

char* print_temperature() {
    FILE *fp;
    char line[COMPONENT_LENGTH];

    fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (fp == NULL) return "no temp";

    line[0] = fgetc(fp);
    line[1] = fgetc(fp);
    line[2] = '\0';

    strcat(line, " C");

    fclose(fp);
    return strdup(line);

}

int main(void)
{
    char cmd[STATUS_LENGTH];
    while (1) {
        char *storage = print_storage();
        char *memory = print_memory();
        char *date = print_date();
        char *battery = print_battery();
        char *temperature = print_temperature();

        snprintf(cmd, STATUS_LENGTH, "xsetroot -name \"%s | %s | %s | %s | %s\"",
            storage, 
            memory,
            temperature,
            battery,
            date
        );

        system(cmd);
        /* puts(cmd); */

        if (storage) free(storage);
        if (memory) free(memory);
        if (date) free(date);
        if (battery) free(battery);
        if (temperature) free(temperature);
        sleep(1);
    }

    return 0;
}
