#ifndef WORKER_HPP
#define WORKER_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include "InitializerManager.hpp"
#include <map>
struct CommandInfoString {
    int commandID; // Komut ID'si
    CommandType type;
    std::string command;
    Priority priority;
};

class Worker {
public:
    Worker(InitializerManager& initManager);
    void processCommands();

private:
    InitializerManager& initManager;
    std::unordered_map<int, CommandInfoString> partialCommands; // Parça komutları tutmak için
    std::map<int, std::map<std::string, std::string>> commandEnvs; // Komutların env bilgileri

    void handleCommand(const CommandInfo& cmdInfo);
    void executeCommand(const CommandInfoString& cmdInfoStr);
    void updateEnv(const EnvUpdateMessage& envUpdateMsg);
    void assignDefaultEnv();
};

#endif // WORKER_HPP
