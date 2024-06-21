#include "Employer.hpp"
#include <cstring>
#include <algorithm>

using namespace std;
Employer::Employer(InitializerManager& initManager) : initManager(initManager) {
}

void Employer::createAndSendCommand(CommandType type, const std::string& command, Priority priority) {
    int commandLength = command.length();
    int chunks = commandLength / MAX_COMMAND_SIZE + (commandLength % MAX_COMMAND_SIZE != 0 ? 1 : 0);
    for (int i = 0; i < chunks; ++i) {
        CommandInfo cmdInfo;
        cmdInfo.type = 1; // Set a default message type
        cmdInfo.commandID = initManager.getCommandID();
        cmdInfo.commandType = type;
        cmdInfo.priority = priority;
        cmdInfo.splitStatus =  CommandStatusSplit::COMPLETE;

        int start = i * MAX_COMMAND_SIZE;
        int end = std::min(start + MAX_COMMAND_SIZE, commandLength);
        std::string commandChunk = command.substr(start, end - start);
        strncpy(cmdInfo.command, commandChunk.c_str(), MAX_COMMAND_SIZE);
        cmdInfo.command[MAX_COMMAND_SIZE - 1] = '\0'; // Null-terminate for safety
        cmdInfo.command[MAX_COMMAND_SIZE - 2] = '\0'; // Null-terminate for safety

        initManager.IncreaseByOneCommandID();
        splitAndSendCommand(cmdInfo);
    }
}

void Employer::createAndSendEnv(const std::unordered_map<std::string, std::string>& env,const int envNumber) {
    splitAndSendEnv(env,envNumber);
}

void Employer::splitAndSendCommand(const CommandInfo& cmdInfo) {
    initManager.sendCommand(cmdInfo);
}

void Employer::splitAndSendEnv(const std::unordered_map<std::string, std::string>& env,const int envNumber) {
    for (const auto& [key, value] : env) {
        size_t keyLength = key.length();
        size_t valueLength = value.length();

        size_t keyParts = (keyLength + MAX_ENV_SIZE - 1) / MAX_ENV_SIZE;
        size_t valueParts = (valueLength + MAX_ENV_SIZE - 1) / MAX_ENV_SIZE;

        for (size_t i = 0; i < keyParts; ++i) {
            std::string keyPart = key.substr(i * MAX_ENV_SIZE, MAX_ENV_SIZE);
            std::string valuePart = (i == keyParts - 1) ? value.substr(0, std::min(MAX_ENV_SIZE, (int)valueLength)) : "";

            initManager.sendEnvPart(1, keyPart, valuePart, (i == keyParts - 1 && valueParts == 0) ? COMPLETE_ESS : SPLIT_KEY_ESS);
        }

        for (size_t i = 0; i < valueParts; ++i) {
            std::string valuePart = value.substr(i * MAX_ENV_SIZE, MAX_ENV_SIZE);
            std::string keyPart = (i == 0) ? key.substr(0, std::min(MAX_ENV_SIZE, (int)keyLength)) : "";

            initManager.sendEnvPart(envNumber, keyPart, valuePart, (i == valueParts - 1) ? COMPLETE_ESS : SPLIT_VALUE_ESS);
        }
    }
}
