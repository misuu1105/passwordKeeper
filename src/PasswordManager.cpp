#include "PasswordManager.h"

void PasswordManager::addPassword(std::string& appName, const std::string& password)
{
    correctedAppName(appName);
    passwords[appName] = password; // temporary stored to the map for easy acces
    handler.saveToFile(appName, password);
}

void PasswordManager::deletePassword(std::string& appName)
{
    correctedAppName(appName);
    passwords[appName] = this->getPassword(appName); // save the password until the app is closed in case the user deleted it by mistake
    handler.deleteFromFile(appName);
}

const std::string& PasswordManager::getPassword(std::string& appName)
{
    correctedAppName(appName);

    // std::unorderd_map<>::iterator
    auto password = passwords.find(appName);
    
    if (password != passwords.end())
    {
        return password->second;  
    }
    else
    {
        std::unique_ptr<std::string> password = handler.loadFromFile<std::unique_ptr<std::string>>(appName);
        passwords[appName] = *password;
        return *password; 
    } // error handling included in the load function 
}

void PasswordManager::correctedAppName(std::string& appName)
{
    for (char& currentChar: appName)
        if (currentChar >= 'a' && currentChar <= 'z')
            currentChar += 'A' - 'a';
}