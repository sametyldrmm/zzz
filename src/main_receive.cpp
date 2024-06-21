#include <iostream>
#include "InitializerManager.hpp"
#include "Employer.hpp"
#include "Worker.hpp"
#include <thread>

int main() {
    key_t commandKey = ftok("/home/samet/Desktop/sharedProcces/bir/build/comamndFile", 65);
    key_t envKey = ftok("/home/samet/Desktop/sharedProcces/bir/build/envfile", 75);
    InitializerManager initManager(commandKey, envKey);
    Employer employer(initManager);
    Worker worker(initManager);


    // worker thread readding
    std::thread workerThread(&Worker::processCommands, &worker);

    std::unordered_map<std::string, std::string> env = {
        {"PATH", "/usr/bin"},
        {"KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK", "/usr/bin"},
        {"PATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATHPATH", "/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/bin/usr/binv"},
        {"HOME", "/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/user/home/userv"}
    };
    employer.createAndSendEnv(env,1);

    // employer
    {
        std::string command = "echo -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -alecho -al";
        employer.createAndSendCommand(CMD_EXECUTE, command, HIGH);
        command = "ls -al";

        employer.createAndSendCommand(CMD_EXECUTE, command, HIGH);
        command = "pwd";

        employer.createAndSendCommand(CMD_EXECUTE, command, HIGH);
    }

    workerThread.join();

    return 0;
}
