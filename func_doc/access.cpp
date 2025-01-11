#include <unistd.h>
#include <iostream>
#include <sys/stat.h>

// This function checks if the file is accessible with the flag you have passed as a paramater.
//    (F_OK -> file exist   R_OK -> file has read permission   X_OK -> excute   W_OK -> write)


int main(int argc, char **argv) {
    int find = access(argv[1], F_OK);

    if (find == -1)
        std::cerr << "The file you search about it not fond!" << std::endl;
    else
        std::cout << "The file is exist and access function return " << find << std::endl;

    struct stat file_stat;

    // This function get all the information about the file have passed as a parameter and put them in (struct stat) object.


    int status = stat(argv[1], &file_stat);

    if (status == 0) {
        std::cout << file_stat.st_size << std::endl;
    }
}