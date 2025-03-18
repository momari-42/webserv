
#include <dirent.h>  // For opendir(), readdir(), closedir(), DIR, struct dirent
#include <stdio.h>   // For printf(), perror()
#include <stdlib.h>  // For malloc(), free() if needed
#include <errno.h>   // For errno, if needed for error handling


int main() {
    struct dirent *entry;
    DIR *dir = opendir("/Users/momari/cursus/webserv");

    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            printf("Found: %s\n", entry->d_name);  // Print each entry's name
        }
        closedir(dir);  // Close the directory stream
    }
}