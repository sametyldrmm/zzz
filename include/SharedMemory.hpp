#ifndef SHAREDMEMORY_HPP
#define SHAREDMEMORY_HPP

#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

const size_t SHARED_MEMORY_SIZE = 4096; // 4 KB
const std::string DEFAULT_SHARED_MEMORY_NAME = "/shared_memory_";

enum MessageStatus {
    EMPTY,
    WRITTEN,
    READ,
    WRITING
};

enum Sender {
    NONE,
    EMPLOYER,
    WORKER
};

struct SharedData {
    MessageStatus status;
    Sender sender;
    int commandID;
    int envID;
    char data[256]; // Ortak veri alanı
};

class SharedMemory {
public:
    // Constructor for Employer
    SharedMemory(int commandID, Sender sender);

    // Constructor for Worker
    SharedMemory(int commandID);

    ~SharedMemory();

    bool writeData(const SharedData& data, bool waitForSemaphore = true);
    SharedData readData();

private:
    std::string sharedMemoryName;
    int shm_fd;
    void* ptr;
    sem_t* sem;

    void createSharedMemory();
    void openSharedMemory();
    void createSemaphore();
    void openSemaphore();
    void unlinkResources();
};

#endif // SHAREDMEMORY_HPP
