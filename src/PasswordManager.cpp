#include "PasswordManager.h"

void PasswordManager::addPassword(std::string& appName, const std::string& password)
{
    appNameNormalization(appName);

    if (password.empty())
    {
        throw std::out_of_range("The password can not be empty");
    }

    passwords[appName] = password; // temporary stored to the map for easy acces
    handler.saveToFile(appName, password);
}

void PasswordManager::deletePassword(std::string& appName)
{
    appNameNormalization(appName);
    passwords[appName] = this->getPassword(appName); // save the password until the app is closed in case the user deleted it by mistake

    if (handler.deleteFromFile(appName) < 0)
    {
        throw std::invalid_argument("There is no password saved for " + appName);
    }
}

const std::string& PasswordManager::getPassword(std::string& appName) 
{
    appNameNormalization(appName);

    // std::unorderd_map<>::iterator
    auto password = passwords.find(appName);
    
    if (password != passwords.end())
    {
        return password->second;  
    }
    else
    {
        std::unique_ptr<std::string> password = handler.loadFromFile<std::unique_ptr<std::string>>(appName);

        if (password == nullptr)
        {
            throw std::invalid_argument("There is no password saved for " + appName);
        }

        passwords[appName] = *password;
        return *password; 
    } 
}

void PasswordManager::appNameNormalization(std::string& appName) 
{
    if (appName.size() > 30 || appName.empty())
    {
        throw std::out_of_range("App name is invalid");
    }

    for (char currentChar : appName)
    {
        if (isalpha(currentChar) == false)
        {
            throw std::invalid_argument("The app name should only contain letters");
        }

        if (islower(currentChar))
        {
            currentChar = toupper(currentChar);
        }
    }
}