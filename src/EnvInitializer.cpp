#include "EnvInitializer.hpp"
#include <cstring>
#include <algorithm>

EnvInitializer::EnvInitializer(key_t key) : msgKey(key) {
    msgQueueId = msgget(key, 0666 | IPC_CREAT);
    if (msgQueueId == -1) {
        perror("msgget failed");
        exit(1);
    }
}

EnvInitializer::~EnvInitializer() {
    if (msgctl(msgQueueId, IPC_RMID, nullptr) == -1) {
        perror("msgctl failed");
    }
}

// void EnvInitializer::sendEnv(const std::unordered_map<std::string, std::string>& env, EnvMessageType type,const int envNumber) {
//     for (const auto& [key, value] : env) {
//         sendEnvPart(envId++, key, value);
//     }
// }

void EnvInitializer::sendEnvPart(int envID, const std::string& keyPart, const std::string& valuePart, ENVSPLITSTATUS status) {
    EnvMessage msg;
    msg.type = ENV_SEND;
    msg.envID = envID;

    strncpy(msg.key, keyPart.c_str(), MAX_ENV_SIZE);
    strncpy(msg.value, valuePart.c_str(), MAX_ENV_SIZE);

    msg.status = status;

    if (msgsnd(msgQueueId, &msg, sizeof(EnvMessage) - sizeof(long), 0) == -1) {
        perror("msgsnd failed");
    }
}

std::map<std::string, std::string> EnvInitializer::receiveEnv() {
    std::map<std::string, std::string> env;
    EnvMessage msg;
    std::string key;
    std::string value;

    while (true) {
        if (msgrcv(msgQueueId, &msg, sizeof(EnvMessage) - sizeof(long), ENV_SEND, 0) == -1) {
            perror("msgrcv failed");
            break;
        }

        if (msg.status == SPLIT_KEY_ESS) {
            key += msg.key;
        } else if (msg.status == SPLIT_VALUE_ESS) {
            value += msg.value;
        } else if (msg.status == COMPLETE_ESS) {
            key += msg.key;
            value += msg.value;
            env[key] = value;
            key.clear();
            value.clear();
        }

        if (msg.status == COMPLETE_ESS) {
            break;
        }
    }

    return env;
}

void EnvInitializer::sendEnvUpdate(int commandID, EnvUpdateType updateType, const std::string& key, const std::string& value) {
    EnvUpdateMessage msg;
    msg.type = ENV_UPDATE;
    msg.commandID = commandID;
    msg.updateType = updateType;
    strncpy(msg.key, key.c_str(), MAX_ENV_SIZE);
    strncpy(msg.value, value.c_str(), MAX_ENV_SIZE);
    msg.status = COMPLETE_ESS;
    if (msgsnd(msgQueueId, &msg, sizeof(EnvUpdateMessage) - sizeof(long), 0) == -1) {
        perror("msgsnd failed");
    }
}

EnvUpdateMessage EnvInitializer::receiveEnvUpdate() {
    EnvUpdateMessage msg;
    if (msgrcv(msgQueueId, &msg, sizeof(EnvUpdateMessage) - sizeof(long), ENV_UPDATE, 0) == -1) {
        perror("msgrcv failed");
    }
    return msg;
}

int EnvInitializer::getEnvID() const {return this->envId;}
