#include "Worker.hpp"

Worker::Worker(InitializerManager& initManager) : initManager(initManager) {
    // 1 numaralı ortam değişkenini tüm komutlara atamak için
    // assignDefaultEnv();
}
using namespace std;
void Worker::processCommands() {
    while (true) {
        CommandInfo cmdInfo = initManager.receiveCommand();
        
        // handleCommand(cmdInfo);

        // Env update mesajlarını işleme al
        auto envUpdateMsg = initManager.receiveEnv();
        for (const auto& [key, value] : envUpdateMsg) {
            commandEnvs[1][key] = value;
        }
        for(auto &ham:this->commandEnvs)
        {
            cout << ham.first << endl;
            for(auto &hamm:ham.second)
            {
                cout << hamm.first <<"  " << hamm.second <<  endl;
            }
        }

    }
}

void Worker::handleCommand(const CommandInfo& cmdInfo) {
    auto& partialCommand = partialCommands[cmdInfo.commandID];

    if (cmdInfo.splitStatus == CommandStatusSplit::SPLIT) {
        if (partialCommand.command.empty()) {
            partialCommand.commandID = cmdInfo.commandID;
            partialCommand.type = cmdInfo.commandType;
            partialCommand.priority = cmdInfo.priority;
        }
        partialCommand.command += cmdInfo.command;
    } else {
        if (partialCommand.command.empty()) {
            partialCommand.commandID = cmdInfo.commandID;
            partialCommand.type = cmdInfo.commandType;
            partialCommand.priority = cmdInfo.priority;
            partialCommand.command = cmdInfo.command;
        } else {
            partialCommand.command += cmdInfo.command;
        }

        // Komut tamamlandı, işleme al
        executeCommand(partialCommand);

        // Tamamlanmış komutu işleme al ve map'ten sil
        partialCommands.erase(cmdInfo.commandID);
    }
}

void Worker::executeCommand(const CommandInfoString& cmdInfoStr) {
    std::map<std::string, std::string> env = commandEnvs[cmdInfoStr.commandID];
    // std::cout << "Executing command ID: " << cmdInfoStr.commandID << std::endl;
    // std::cout << "Command: " << cmdInfoStr.command << std::endl;
    // std::cout << "Environment Variables:" << std::endl;
    // for (const auto& [key, value] : env) {
    //     std::cout << key << "=" << value << std::endl;
    // }
}

void Worker::updateEnv(const EnvUpdateMessage& envUpdateMsg) {
    auto& env = commandEnvs[envUpdateMsg.commandID];
    if (envUpdateMsg.updateType == CREATE || envUpdateMsg.updateType == UPDATE) {
        env[envUpdateMsg.key] = envUpdateMsg.value;
    } else if (envUpdateMsg.updateType == REMOVE) {
        env.erase(envUpdateMsg.key);
    }
}

void Worker::assignDefaultEnv() {
    std::map<std::string, std::string> defaultEnv = initManager.receiveEnv();
    for (auto& [commandID, env] : commandEnvs) {
        env = defaultEnv;
    }
}
