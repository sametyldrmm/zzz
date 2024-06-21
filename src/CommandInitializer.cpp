#include "CommandInitializer.hpp"
using namespace std;

CommandInitializer::CommandInitializer(key_t key) : msgKey(key) {
    msgQueueId = msgget(key, 0666 | IPC_CREAT);
    if (msgQueueId == -1) {
        perror("msgget failed");
        exit(1);
    }
}

CommandInitializer::~CommandInitializer() {
    if (msgctl(msgQueueId, IPC_RMID, nullptr) == -1) {
        perror("msgctl failed");
    }
}

void CommandInitializer::sendCommand(const CommandInfo& cmdInfo) {
    //    std::cout << "Sending command of size: " << sizeof(CommandInfo) - sizeof(long) << std::endl;
    // std::cout << msgQueueId << endl;

    if (msgsnd(msgQueueId, &cmdInfo, sizeof(CommandInfo) - sizeof(long), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }
}

CommandInfo CommandInitializer::receiveCommand() {
    CommandInfo cmdInfo;
    // std::cout << "Receiving command of size: " << sizeof(CommandInfo) - sizeof(long) << std::endl;
    // std::cout << msgQueueId << endl;
    if (msgrcv(msgQueueId, &cmdInfo, sizeof(CommandInfo) - sizeof(long), 0, 0) == -1) {
        perror("msgrcv receiveCommand failed");
        exit(1);
    }
    // std::cout << cmdInfo.command << "\n"; 
    return cmdInfo;
}

int CommandInitializer::getCommandID() {
    return this->commandID;
}
    
void CommandInitializer::IncreaseByOneCommandID() {
    this->commandID++;
}
