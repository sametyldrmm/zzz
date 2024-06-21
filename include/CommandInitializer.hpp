#ifndef COMMANDINITIALIZER_HPP
#define COMMANDINITIALIZER_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/ipc.h>
#include <sys/msg.h>

const int MAX_COMMAND_SIZE = 256;

enum Priority {
    EMPTY,
    LOW,
    MEDIUM,
    HIGH
};

enum CommandType {
    EMPTY_CT,
    CMD_EXECUTE,
    CMD_HTTP_REQUEST,
    CMD_CUSTOM // Yeni komut türleri eklenebilir
};

enum CommandStatusSplit {
    EMPTY_CSS,
    COMPLETE,
    SPLIT
};

struct CommandInfo {
    long type;
    int commandID;
    CommandType commandType;
    char command[MAX_COMMAND_SIZE];
    Priority priority;
    CommandStatusSplit splitStatus;
};

class CommandInitializer {
public:
    CommandInitializer(key_t key);
    ~CommandInitializer();

    void sendCommand(const CommandInfo& cmdInfo);
    CommandInfo receiveCommand();
    int getCommandID();
    key_t msgKey;
    int msgQueueId;
    int commandID = 1;
    void IncreaseByOneCommandID();

protected:
};

#endif // COMMANDINITIALIZER_HPP
