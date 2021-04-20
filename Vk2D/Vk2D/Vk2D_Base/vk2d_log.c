#include "vk2d_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

void vk2d_assert(i32 condition)
{
    if (!condition)
    {
        vk2d_log_fatal("Vk2D", "Assertion failed!");
    }
}

void vk2d_log_info(const char* tag, const char* message)
{
    time_t now;
    time(&now);
    printf(RESET);
    printf("%s [%s] : %s", ctime(&now), tag, message);
}

void vk2d_log_warn(const char* tag, const char* message)
{
    time_t now;
    time(&now);
    printf(RESET);
    printf(BOLDYELLOW);
    printf("%s [%s] : %s", ctime(&now), tag, message);
    printf(RESET);
}

void vk2d_log_error(const char* tag, const char* message)
{
    time_t now;
    time(&now);
    printf(RESET);
    printf(RED);
    printf("%s [%s] : %s", ctime(&now), tag, message);
    printf(RESET);
}

void vk2d_log_fatal(const char* tag, const char* message)
{
    time_t now;
    time(&now);
    printf(RESET);
    printf(RED);
    printf("%s [%s] : %s", ctime(&now), tag, message);
    printf(RESET);
}

void vk2d_log_flush()
{
    fflush(stdout);
}