#include "main.h"


long get_epoch_from_datetime(const char* date, const char* time)
{
    time_t epoch;
    struct tm tms = {0};

    if (sscanf(date, "%d.%d.%d", &tms.tm_mday, &tms.tm_mon, &tms.tm_year) == 3) {
        if (sscanf(time, "%d:%d", &tms.tm_hour, &tms.tm_min) == 2) {
            tms.tm_year -= 1900;
            tms.tm_mon -= 1;

            epoch = mktime(&tms);

            return epoch;
        }
    }

    return -1;
}

int main(int argc, char** argv, char** env)
{
    if (argc != 4) {
        printf("Insufficient arguments supplied!");
        printf("Usage:\non_time <absolute_file_path> <date> <time>\n• date format: dd.MM.yyyy\n• time format: HH:MM\n \
               Example: on_time ./my_file.txt 01.01.1999 05:00");

        return -1;
    }

    struct stat file_stats;

    if (stat(argv[1], &file_stats) == 0) {
        long epoch = get_epoch_from_datetime(argv[2], argv[3]);

        if (epoch != -1) {
            struct utimbuf new_time;
            new_time.actime = epoch;
            new_time.modtime = epoch;

            utime(argv[1], &new_time);

            printf("Successfully altered timestamps!\n");

            return 0;
        }
        else {
            printf("Failed to parse date & time, aborting...\n");
        }
    }
    else {
        printf("File not found, aborting...\n");
    }

    return -1;
}
