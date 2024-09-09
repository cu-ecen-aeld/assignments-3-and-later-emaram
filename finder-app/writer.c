/*
    writer.c - Assignment 2
        Requires 2 arguments:
            - name of the file where the message will be written
            - message (string) to be written
*/

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>


int main(int argc, char** argv)
{
    /* Initializing syslog */
    openlog(NULL, 0, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "writer.c - Invalid number of parameters");
        syslog(LOG_INFO, "Usage: writer <path_to_file> <string_to_be_written_to_file>");
        return 1;
    }

    const char* command = argv[0];
    const char* filename = argv[1];
    const char* text = argv[2];

    /* Opening file for writing */
    FILE* fd = fopen(filename, "w+");
    if (fd == NULL) {
        syslog(LOG_ERR, "writer.c - error opening file %s", filename);
        return 1;
    }

    /* File successfully open. Writing the message */
    syslog(LOG_DEBUG, "Writing '%s' to %s", text, filename);
    fprintf(fd, "%s", text);
    fclose(fd);

    closelog();

    return 0;
}