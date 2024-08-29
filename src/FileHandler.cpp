#include "FileHandler.h"

const char* PASSWORD = "password";

void createNewFile(tinyxml2::XMLDocument& file, const char* fileName);

FileHandler::FileHandler(const std::string& fileName)
{
    const char* FILE_NAME = fileName.c_str();
    auto fileStatus = file.LoadFile(FILE_NAME);

    if (fileStatus != tinyxml2::XML_SUCCESS)
        switch (fileStatus)
        {
            case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
                createNewFile(file, FILE_NAME);
                break;

            default:
                std::cerr << "Unknown error encountered while trying to acces the file\n";
                break;
        }

    this->fileName = fileName;
}

void FileHandler::saveToFile(const std::string& appName, const std::string& password)
{
    // cast to cstring for matching XML functions parameters
    const char* APP_NAME = appName.c_str(); 
    const char* NEW_PASSWORD = password.c_str();
    const char* FILE_NAME = fileName.c_str();

    tinyxml2::XMLElement* root = file.RootElement();

    if (root == nullptr)
    {
        std::cerr << "No root element found in " << appName;
    }

    tinyxml2::XMLElement* newPassword = file.NewElement(APP_NAME);
    newPassword->SetAttribute(PASSWORD, NEW_PASSWORD);

    root->InsertEndChild(newPassword);

    if (file.SaveFile(FILE_NAME) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Failed to save file";
    }
}

int FileHandler::deleteFromFile(const std::string& appName)
{
    const char* APP_NAME = appName.c_str();
    const char* FILE_NAME = fileName.c_str();

    tinyxml2::XMLElement* root = file.RootElement();
    tinyxml2::XMLElement* searchedElement = root->FirstChildElement(APP_NAME);

    if (searchedElement)
    {
        root->DeleteChild(searchedElement);

        auto fileStatus = file.SaveFile(FILE_NAME);

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

void FileHandler::createNewFile(tinyxml2::XMLDocument& file, const char* fileName)
{
    file.Clear();   
    tinyxml2::XMLElement* root = file.NewElement("passwords");
    file.InsertFirstChild(root);

    auto fileStaus = file.SaveFile(fileName);

    if (fileStaus != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Unknown error encountered while trying to create the file\n";
    }
}
