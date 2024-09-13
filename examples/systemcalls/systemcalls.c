#include "systemcalls.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    /* Check, first, if cmd is not NULL */
    if (cmd == NULL) {
        printf("ERROR: cmd is NULL\n");
        return false;
    }

    printf("Start do_system(\"%s\")...\n", cmd);

    int ret_code = system(cmd);
    if (ret_code == -1) {
        printf("system() command failed!\n");
        return false;
    }

    /* If we reached this point, everything is fine :-) */
    return true;
}


/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    // command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

    va_end(args);

    /* Fork new child */
    pid_t pid = fork();
    if (pid == -1) {
        return false;
    }


    if (pid == 0) {
        /* We are in child process. Let's execute the command */
        execv(command[0], command);

        /* Since execv is blocker ... we should not reach this point*/
        /* MANDATORY: exit here ... otherwise the waitpid will return WIFEXITED() true */
 
        exit(EXIT_FAILURE);
        return false;
    }
    else {

        /* We are in parent process and pid is the ID of the child process. Waiting for child to complete. */
        int waiting_status = 0;
        pid_t waiting_pid = waitpid(pid, &waiting_status, 0);
        if (waiting_pid == -1) {
            return false;
        }


        /* If existed, check exit status */
        if (WIFEXITED(waiting_status)) {
            int child_exit_status = WEXITSTATUS(waiting_status);

            if (child_exit_status != EXIT_SUCCESS) {
                return false;
            }
        }
        else {
            return false;
        }

    }

    return true;

}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    // command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    va_end(args);

    /* Create outputfile */
    int fd = open(outputfile, O_RDWR | O_TRUNC | O_CREAT, 0644);
    if (fd == -1) {
        printf("*** Cannot create or open %s. Returning false ...\n", outputfile);
        return false;
    }

    fflush(stdout);


    /* Fork new child */
    pid_t pid = fork();
    if (pid == -1) {
        return false;
    }

    if (pid == 0) {
        /* We are in child process. */
        /* Redirect stdout to fd */
        int redirect_status = dup2(fd, 1);
        close(fd);
        if (redirect_status < 0) {
            return false;
        }
        execv(command[0], command);

        /* Since execv is blocker ... we should not reach this point*/
        /* MANDATORY: exit here ... otherwise the waitpid will return WIFEXITED() true */

        exit(EXIT_FAILURE);
        return false;
    }
    else {
        /* We are in parent process and pid is the ID of the child process. Waiting for child to complete. */
        int waiting_status = 0;
        pid_t waiting_pid = waitpid(pid, &waiting_status, 0);
        if (waiting_pid == -1) {
            return false;
        }

        /* If existed, check exit status */
        if (WIFEXITED(waiting_status)) {
            int child_exit_status = WEXITSTATUS(waiting_status);

            if (child_exit_status != EXIT_SUCCESS) {
                return false;
            }
        }

    }

    return true;

}
