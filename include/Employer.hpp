#ifndef EMPLOYER_HPP
#define EMPLOYER_HPP

#include <iostream>
#include <vector>
#include "InitializerManager.hpp"

class Employer {
public:
    Employer(InitializerManager& initManager);
    void createAndSendCommand(CommandType type, const std::string& command, Priority priority);
    void createAndSendEnv(const std::unordered_map<std::string, std::string>& env,const int envNumber);

protected:
    InitializerManager& initManager;
    void splitAndSendCommand(const CommandInfo& cmdInfo);
    // void splitAndSendEnv(const std::unordered_map<std::string, std::string>& env);
    void splitAndSendEnv(const std::unordered_map<std::string, std::string>& env,const int envNumber);

};

#endif // EMPLOYER_HPP
