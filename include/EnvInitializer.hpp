#ifndef ENVINITIALIZER_HPP
#define ENVINITIALIZER_HPP

#include <iostream>
#include <unordered_map>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <vector>
#include <map>

const int MAX_ENV_SIZE = 256;

enum EnvMessageType {
    EMPTY_EMT,
    ENV_SEND,
    ENV_UPDATE
};

enum EnvUpdateType {
    EMPTY_EUT,
    CREATE,
    UPDATE,
    REMOVE,
};

enum ENVSPLITSTATUS {
    EMPTY_ESS,
    SPLIT_KEY_ESS,
    SPLIT_VALUE_ESS,
    COMPLETE_ESS
};

struct EnvMessage {
    long type;
    int envID;
    char key[MAX_ENV_SIZE];
    char value[MAX_ENV_SIZE];
    ENVSPLITSTATUS status;
};

struct EnvUpdateMessage {
    long type;
    int commandID;
    EnvUpdateType updateType;
    char key[MAX_ENV_SIZE];
    char value[MAX_ENV_SIZE];
    ENVSPLITSTATUS status;
};

class EnvInitializer {
public:
    EnvInitializer(key_t key);
    ~EnvInitializer();

    // void sendEnv(const std::unordered_map<std::string, std::string>& env, EnvMessageType type, const int envNumber);
    std::map<std::string, std::string> receiveEnv();
    void sendEnvUpdate(int commandID, EnvUpdateType updateType, const std::string& key, const std::string& value);
    EnvUpdateMessage receiveEnvUpdate();
    int envId = 1;
    int getEnvID() const;
    void sendEnvPart(int envID, const std::string& keyPart, const std::string& valuePart, ENVSPLITSTATUS status) ;

    // void sendEnvPart(int envID, const std::string& key, const std::string& value);
protected:
    key_t msgKey;
    int msgQueueId;

    void sendEnvLinePart(int envID, const std::string& key, const std::string& value);
};

#endif // ENVINITIALIZER_HPP
