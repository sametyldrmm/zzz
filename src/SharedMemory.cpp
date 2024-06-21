#include "SharedMemory.hpp"
#include <iostream>
#include <cstring>

// Constructor for Employer
SharedMemory::SharedMemory(int commandID, Sender sender) {
    sharedMemoryName = DEFAULT_SHARED_MEMORY_NAME + std::to_string(commandID);
    createSharedMemory();
    createSemaphore();

    // Initialize the shared memory segment
    SharedData* sharedData = static_cast<SharedData*>(ptr);
    sharedData->status = EMPTY;
    sharedData->sender = sender;
    sharedData->commandID = commandID;
    sharedData->envID = 0;
    memset(sharedData->data, 0, sizeof(sharedData->data));
}

// Constructor for Worker
SharedMemory::SharedMemory(int commandID) {
    sharedMemoryName = DEFAULT_SHARED_MEMORY_NAME + std::to_string(commandID);
    openSharedMemory();
    openSemaphore();
}

SharedMemory::~SharedMemory() {
    unlinkResources();
}

void SharedMemory::createSharedMemory() {
    shm_fd = shm_open(sharedMemoryName.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    if (ftruncate(shm_fd, SHARED_MEMORY_SIZE) == -1) {
        perror("ftruncate failed");
        exit(1);
    }

    ptr = mmap(0, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
}

void SharedMemory::openSharedMemory() {
    shm_fd = shm_open(sharedMemoryName.c_str(), O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    ptr = mmap(0, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
}

void SharedMemory::createSemaphore() {
    sem = sem_open((sharedMemoryName + "_sem").c_str(), O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }
}

void SharedMemory::openSemaphore() {
    sem = sem_open((sharedMemoryName + "_sem").c_str(), 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }
}

void SharedMemory::unlinkResources() {
    if (munmap(ptr, SHARED_MEMORY_SIZE) == -1) {
        perror("munmap failed");
    }
    if (shm_unlink(sharedMemoryName.c_str()) == -1) {
        perror("shm_unlink failed");
    }
    if (sem_close(sem) == -1) {
        perror("sem_close failed");
    }
    if (sem_unlink((sharedMemoryName + "_sem").c_str()) == -1) {
        perror("sem_unlink failed");
    }
}

bool SharedMemory::writeData(const SharedData& data, bool waitForSemaphore) {
    if (waitForSemaphore) {
        if (sem_wait(sem) == -1) {
            perror("sem_wait failed");
            return false;
        }
    } else {
        if (sem_trywait(sem) == -1) {
            return false; // Semaphore is not available, skip writing
        }
    }

    SharedData* sharedData = static_cast<SharedData*>(ptr);
    memcpy(sharedData, &data, sizeof(SharedData));
    if (sem_post(sem) == -1) {
        perror("sem_post failed");
        return false;
    }

    return true;
}

SharedData SharedMemory::readData() {
    if (sem_wait(sem) == -1) {
        perror("sem_wait failed");
        exit(1);
    }

    SharedData* sharedData = static_cast<SharedData*>(ptr);
    SharedData data;
    memcpy(&data, sharedData, sizeof(SharedData));

    if (sem_post(sem) == -1) {
        perror("sem_post failed");
        exit(1);
    }

    return data;
}
