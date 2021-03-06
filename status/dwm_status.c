/*
 * Author      : Ethan Rietz
 * Date        : 2021-10-04
 * Description : A status line that is more efficient than a shell script
 */


/* TODO: CPU usage can be calculated like so: */
/* grep 'cpu ' /proc/stat | awk '{usage=100-($5*100)/($2+$3+$4+$5+$6+$7+$8)} END {print usage}' */
/* https://unix.stackexchange.com/questions/393566/what-does-proc-loadavg-s-cpu-and-io-utilization-mean */

/* headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* macros */
#define STATUS_LENGTH 400
#define COMPONENT_LENGTH 30
#define PARSE_ERROR "bad parsing..."

/* types */
/* function declarations */
static void get_storage(char *storage);
static void get_memory(char *memory);
static void get_date(char *date);
static void get_battery(char *battery);
static void get_temperature(char *temperature);

/* global variables */
/* function definitions */

void
get_storage(char *storage)
{
    FILE *fp;
    char *line = NULL;  // getline() will allocate enough memory for &line
    size_t len = 0;
    ssize_t read;

    fp = popen("df -Ht ext4", "r");
    if (fp == NULL) {
        strncpy(storage, "not ext4 drive", COMPONENT_LENGTH);
        return;
    }

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

            snprintf(storage, COMPONENT_LENGTH, "%s/%s", avail, total);

            fclose(fp);
            if (line) free(line);
            return;
        }
    }

    fclose(fp);
    if (line) free(line);

    strncpy(storage, PARSE_ERROR, COMPONENT_LENGTH);
    return;
}

void
get_memory(char *memory)
{
    FILE *fp;
    char *line = NULL;  // getline() will allocate memory for &line
    size_t len = 0;
    ssize_t read;

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        strncpy(memory, "no meminfo", COMPONENT_LENGTH);
        return;
    }

    while ((read = getline(&line, &len, fp)) != -1) {

        if (strstr(line, "MemAvailable")) {
            strtok(line, " ");
            char *availChar = strtok(NULL, " ");
            float avail = atof(availChar);

            snprintf(memory, COMPONENT_LENGTH, "%.2f GiB", avail / 1024 / 1024);

            fclose(fp);
            if (line) free(line);
            return;
        }
    }

    fclose(fp);
    if (line) free(line);
    strncpy(memory, PARSE_ERROR, COMPONENT_LENGTH);
    return;

}

void
get_date(char *date) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(date, COMPONENT_LENGTH, "%x %X", tm);
    return;
}

void
get_battery(char *battery) {
    FILE *fp;

    fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (fp == NULL) {
        strncpy(battery, "no bat", COMPONENT_LENGTH);
        return;
    }

    fgets(battery, COMPONENT_LENGTH, fp);

    // Remove the newline character from parsed line and replace it with null
    // terminator
    size_t len = strlen(battery);
    if (len > 0 && battery[len-1] == '\n') {
        battery[len-1] = '\0';
    }

    fclose(fp);

    strncat(battery, "% charge", COMPONENT_LENGTH);
    return;
}

void
get_temperature(char *temperature) {
    FILE *fp;

    fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (fp == NULL){
        strncpy(temperature, "no temp", COMPONENT_LENGTH);
    }

    temperature[0] = fgetc(fp);
    temperature[1] = fgetc(fp);
    temperature[2] = ' ';
    temperature[3] = 'C';
    temperature[4] = '\0';

    fclose(fp);
    return;

}

int
main(void)
{
    char cmd[STATUS_LENGTH];
    while (1) {
        char storage[COMPONENT_LENGTH];
        char memory[COMPONENT_LENGTH];
        char date[COMPONENT_LENGTH];
        char battery[COMPONENT_LENGTH];
        char temperature[COMPONENT_LENGTH];

        get_storage(storage);
        get_memory(memory);
        get_date(date);
        get_battery(battery);
        get_temperature(temperature);

        snprintf(cmd, STATUS_LENGTH,
            "xsetroot -name \"%s | %s | %s | %s | %s\"",
            storage, 
            memory,
            temperature,
            battery,
            date
        );

        system(cmd);
        /* puts(cmd); */
        sleep(1);
    }

    return 0;
}
