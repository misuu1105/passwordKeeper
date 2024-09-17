#include "PasswordManager.h"

void PasswordManager::addPassword(std::string& appName, const std::string& password)
{
    validateAndNormalizeAppName(appName);

    // checking too see if there is a password saved for the provided app name
    auto oldPassword = fileHandler.loadFromFile<std::unique_ptr<std::string>>(appName);

    if (oldPassword)
    {
        throw std::invalid_argument("There is a saved password for " + appName + ". You can change it in the dedicated menu");
    }

    if (password.empty())
    {
        throw std::out_of_range("The password can not be empty");
    }

    passwords[appName] = password; // temporary stored to the map for easy acces
    fileHandler.saveToFile(appName, password);
}

void PasswordManager::deletePassword(std::string& appName)
{
    validateAndNormalizeAppName(appName);
    passwords[appName] = getPassword(appName); // save the password until the app is closed in case the user deleted it by mistake

    if (fileHandler.deleteFromFile(appName) < 0)
    {
        throw std::invalid_argument("There is no password saved for " + appName);
    }
}

void PasswordManager::changePassword(std::string& appName, const std::string& password)
{
    deletePassword(appName);
    addPassword(appName, password);
}

const std::string& PasswordManager::getPassword(std::string& appName) 
{
    validateAndNormalizeAppName(appName);

    // std::unorderd_map<>::iterator
    auto password = passwords.find(appName);
    
    if (password != passwords.end())
    {
        return password->second;  
    }
    else
    {
        auto password = fileHandler.loadFromFile<std::unique_ptr<std::string>>(appName);

        if (password == nullptr)
        {
            throw std::invalid_argument("There is no password saved for " + appName);
        }

        passwords[appName] = *password;
        return *password; 
    } 
}

void PasswordManager::validateAndNormalizeAppName(std::string& appName) 
{
    if (appName.size() > 30 || appName.empty())
    {
        throw std::out_of_range("App name is invalid");
    }

    for (char& currentChar : appName)
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