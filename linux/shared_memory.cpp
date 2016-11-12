#include <sys/mman.h>   // shm_open
#include <sys/wait.h>   // waitpid
#include <fcntl.h>      // open flags(O_CREAT..)
#include <stdio.h>      // perror
#include <unistd.h>     // ftruncate
#include <ctime>
#include <vector>
#include <iostream>
#include <algorithm>
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
    int res = ftruncate(fd, share_size);
    if (res != 0) {
        error_out("ftruncate");
    }

   void *ptr = mmap(0, share_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   if (ptr == MAP_FAILED) {
       error_out("mmap");
   }
   int *data = static_cast<int*>(ptr);

   // Close the file after mmap() doesn't affect the mapping
   close(fd);

   pid_t pid = fork();
   if (pid == 0) {
       // Child process prepares the data to be sorted in parent process.
       // Data format: # of data(int), followed by # integers to sort.
       cout << "[Child] PID: " << getpid() << endl;

       int sort_num = 10;
       *data = sort_num;
       srand(time(NULL));
       for (size_t i = 0; i < sort_num; ++i) {
           data[i + 1] = rand() % sort_num;
       }

       cout << "[Child] Data:\n";
       for_each(data, data + sort_num + 1, [](int i) { cout << i << ", "; });
       cout << endl;

       // Child process terminates
       exit(0);
   }
   else {
       cout << "[Parent] PID: " << getpid() << endl;

       int status;
       waitpid(pid, &status, 0);
       vector<int> v(data, data + data[0] + 1);

       sort(v.begin() + 1, v.end());
       cout << "[Parent] Data:\n";
       for_each(v.cbegin(), v.cend(), [](int i) { cout << i << ", "; });
       cout << endl;
   }

   res = munmap(ptr, share_size);
   if (res != 0) {
       error_out("munmap");
   }

   res = shm_unlink(share_name.c_str());
   if (res != 0) {
       error_out("shm_unlink");
   }

   return 0;
}
