#include "FileHandler.h"

FileHandler::FileHandler(const std::string& fileName)
{
    const char* FILE_NAME = fileName.c_str();
    auto fileStatus = file.LoadFile(FILE_NAME);

    if (fileStatus != tinyxml2::XML_SUCCESS)
        switch (fileStatus)
        {
            case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
                createNewFile(FILE_NAME);
                break;

            default:
                std::cerr << "Unknown error encountered while trying to acces the file\n";
                break;
        }

    this->fileName = fileName;
}

void FileHandler::saveToFile(const std::string& appName, const std::string& password) noexcept
{
    tinyxml2::XMLElement* root = file.RootElement();

    if (root == nullptr)
    {
        std::cerr << "No root element found in " << appName;
    }

    tinyxml2::XMLElement* newPassword = file.NewElement(appName.c_str());
    newPassword->SetAttribute("password", password.c_str());

    root->InsertEndChild(newPassword);

    if (file.SaveFile(fileName.c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Failed to save file";
    }
}

int FileHandler::deleteFromFile(const std::string& appName)  noexcept
{
    tinyxml2::XMLNode* root = file.RootElement();
    tinyxml2::XMLNode* searchedApp = root->FirstChildElement(appName.c_str());

    if (searchedApp)
    {
        file.RootElement()->DeleteChild(searchedApp);

        auto fileStatus = file.SaveFile(fileName.c_str());

        if (fileStatus != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Can't save the file" << appName << " after the deletion of the password";
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

void FileHandler::createNewFile(const std::string& fileName) 
{
    file.Clear();   
    tinyxml2::XMLElement* root = file.NewElement("passwords");
    file.InsertFirstChild(root);

    auto fileStaus = file.SaveFile(fileName.c_str());

    if (fileStaus != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Unknown error encountered while trying to create the file\n";
    }
}