#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <string>
#include <unordered_map>
#include <memory>

#include "FileHandler.h"

const std::string FILE_NAME = "./assets/passwords.xml";

class PasswordManager
{
    public:
        void addPassword(std::string& appName, const std::string& password);
        void deletePassword(std::string& appName);
        void changePassword(std::string& appName, const std::string& password);

        const std::string& getPassword(std::string& appName);

    private:
        // The key represents the website or the aplication and the value the password itself
        std::unordered_map<std::string, std::string> passwords;
        FileHandler fileHandler{FILE_NAME}; 

        void validateAndNormalizeAppName(std::string& appName);
};

#endif