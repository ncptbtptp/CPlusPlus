#include <sys/mman.h>   // shm_open
#include <fcntl.h>      // open flags(O_CREAT..)
#include <stdio.h>      // perror
#include <unistd.h>     // ftruncate
#include <iostream>
using namespace std;

void error_out(const string &msg)
{
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

int main()
{
    string share_name("sample");
    size_t share_size = 100;

    // Create a shared memory object
    // File mode: (read,write,execute). A leading 0 means octal number
    // So 0666 means: r/w for user,group,others
    int fd = shm_open(share_name.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666);
    if (fd == -1) {
        error_out("shm_open");
    }

    // Resize the size of an open file
    int r = ftruncate(fd, share_size);
    if (r != 0) {
        error_out("ftruncate");
    }

   void *ptr = mmap(0, share_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 

    return 0;

}
