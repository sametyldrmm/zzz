#include "InitializerManager.hpp"

InitializerManager::InitializerManager(key_t commandKey, key_t envKey)
    : CommandInitializer(commandKey), EnvInitializer(envKey) {

    }

InitializerManager::~InitializerManager() {}
