/**
 *  @author: Emma Morse
 *  @email: morsee201@potsdam.edu
 *  @course: CIS 310 Operating Systems
 *  @assignment: pfileSystem
 *  @due: 12/18/2020
 *
 */

#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#include <string>
#include <string.h>


/**
 * @brief Prints details about a file
 *
 * @param stats - contains the details of the file
 */
void printFileStats(struct stat stats);

/**
 * @brief Helper for printing file type
 *
 * @param stats - contains the details of the file
 * @return std::string - file type
 */
std::string getFileType(struct stat stats);


/**
 * @brief Helper for printing file permissions
 *
 * @param stats - contains the details of the file
 */
void getPermissions(struct stat stats);

/**
 * @brief Helper for numerical permissions. Converts decimal int to octal int
 *
 * @param modeInDec - int to be converted to octal
 * @return int - octal version of modeInDec
 */
int getNumericalPermissions(int modeInDec);


/**
 * @brief  Prints statistics of a given command line arg
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
    struct stat stats;
    std::cout << "Hello World" << std::endl;
    if (argv[1] != nullptr){
        if  (stat(argv[1], &stats) == 0) {
        stat(argv[1], &stats);
            printf("\nFile: %s", argv[1]);
            printFileStats(stats);
        } else {
            printf("%s", strerror(errno));
        }
    }
}

void printFileStats(struct stat stats) {
    struct tm time; //used for time format conversion

    printf("\nSize: %ld               Blocks: %ld", stats.st_size, stats.st_blocks);
    printf("           IO Block: %ld      ", stats.st_blksize);

    std::string type = getFileType(stats);
    std::cout << type << std::endl;;
    std::cout << "Device: " <<  std::hex << stats.st_dev; //prints device id in hex
    std::cout << "h/" << std::dec << stats.st_dev << "d";  //prints device id in decimal
    printf("    Inode: %lu      Links: %lu \n", stats.st_ino, stats.st_nlink);

    std::cout << "Access (0" << getNumericalPermissions(stats.st_mode)% 1000 <<"/-";
    getPermissions(stats);

    //getpwuid and getgrgid are used to access owner name
    std::cout << ")       Uid:  (" << stats.st_uid << "/  " << getpwuid(stats.st_uid)->pw_name;
    std::cout << ")       Gid: ( " << stats.st_gid << "/  " << getgrgid(stats.st_gid)->gr_name;
    std::cout << ")" << std::endl;


    //Converts times in seconds to date and time format
    //Access
    time = *(gmtime(&stats.st_atime));
    printf("Access: %d-%d-%d %d:%d:%d\n", time.tm_mday, time.tm_mon, time.tm_year + 1900,
                                           time.tm_hour, time.tm_min, time.tm_sec);
    //Modify
    time = *(gmtime(&stats.st_mtime));
    printf("Modify: %d-%d-%d %d:%d:%d\n", time.tm_mday, time.tm_mon, time.tm_year + 1900,
                                           time.tm_hour, time.tm_min, time.tm_sec);

    //Change
    time = *(gmtime(&stats.st_ctime));
    printf("Change: %d-%d-%d %d:%d:%d\n", time.tm_mday, time.tm_mon, time.tm_year + 1900,
                                           time.tm_hour, time.tm_min, time.tm_sec);

    //Linux doesn't support Birth, so it is automatically blank
    printf("Birth: -\n");
}

/**
 * @brief Get the file type (regular file, directory, symbolic link)
 *
 * @param stats - the struct stat object that contains the file stats
 *
 * @return std::string  - the file type in string form for ease of printing
 */
std::string getFileType(struct stat stats) {
    std::string type = "";
    if (S_ISREG(stats.st_mode)) {
        type = "regular file";
    } else if (S_ISDIR(stats.st_mode)) {
        type = "directory";
    } else if (S_ISLNK(stats.st_mode)) {
        type = "symbolic link";
    }
    return type;
}

/**
 * @brief Prints the accurate string representation of the file permissions
 *   Printed instead of returned as a string to eliminate crazy amounts of if statements
 *
 * @param stats the struct stat object that contains the file stats
 */
void getPermissions(struct stat stats) {
    //User
    printf((stats.st_mode & S_IRUSR)? "r":"-");
    printf((stats.st_mode & S_IWUSR)? "w":"-");
    printf((stats.st_mode & S_IXUSR)? "x":"-");

    //Group
    printf((stats.st_mode & S_IRGRP)? "r":"-");
    printf((stats.st_mode & S_IWGRP)? "w":"-");
    printf((stats.st_mode & S_IXGRP)? "x":"-");

    //Owner
    printf((stats.st_mode & S_IROTH)? "r":"-");
    printf((stats.st_mode & S_IWOTH)? "w":"-");
    printf((stats.st_mode & S_IXOTH)? "x":"-");
}

/**
 * @brief Converts decimal int to octal int
 * Just so happens that the last 3 digits of the octal version of st_mode
 * represent the numerical permissions
 *
 * @param modeInDec
 * @return int  - modeInDec in octal
 */
int getNumericalPermissions(int modeInDec) {
    int octalPermissions = 0;
    int i = 1;

    while (modeInDec != 0) {
        octalPermissions += (modeInDec % 8) * i;
        modeInDec /= 8;
        i *= 10;
    }

    return octalPermissions;
}