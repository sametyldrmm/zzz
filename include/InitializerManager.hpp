#ifndef INITIALIZERMANAGER_HPP
#define INITIALIZERMANAGER_HPP

#include "CommandInitializer.hpp"
#include "EnvInitializer.hpp"

class InitializerManager : public CommandInitializer, public EnvInitializer {

public:
    InitializerManager(key_t commandKey, key_t envKey);
    ~InitializerManager() ;
    // int getEnvID();
    // void sendEnvPart(int envID, const std::string& key, const std::string& value) ;

};

#endif // INITIALIZERMANAGER_HPP
