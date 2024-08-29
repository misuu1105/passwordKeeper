#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

// STD library
#include <iostream>
#include <string>
#include <memory>
#include <filesystem>

// external headers
#include <tinyxml2.h>

class FileHandler
{
    public:
        FileHandler(const std::string& fileName);

        void saveToFile(const std::string& appName, const std::string& password);
        int deleteFromFile(const std::string& appName);

        template<typename SmartPointerType>
        SmartPointerType loadFromFile(const std::string& appName) const;

    private:
        tinyxml2::XMLDocument file;
        std::string fileName;

        void createNewFile(tinyxml2::XMLDocument& file, const char* fileName);
};

template<typename SmartPointerType>
SmartPointerType FileHandler::loadFromFile(const std::string& appName) const
{
    // making sure that the template provided is the right one
    static_assert
    ( 
        std::is_same_v<SmartPointerType, std::unique_ptr<std::string>> || std::is_same_v<SmartPointerType, std::shared_ptr<std::string>>,
        "SmartPointerType must be either std::unique_ptr<std::string> or std::shared_ptr<std::string>"
    );

    const char* APPNAME = appName.c_str();
    const char* ATRIBUTE = "password";

    const tinyxml2::XMLElement* root = file.RootElement();
    const tinyxml2::XMLElement* searchedElement = root ? root->FirstChildElement(APPNAME) : nullptr;

    // returning the right form of the template
    if (searchedElement)
    {
        const char* password = searchedElement->Attribute(ATRIBUTE);

        if constexpr (std::is_same_v<SmartPointerType, std::unique_ptr<std::string>>)
        {
            return std::make_unique<std::string>(password);
        }
        else if constexpr (std::is_same_v<SmartPointerType, std::shared_ptr<std::string>>)
        {
            return std::make_shared<std::string>(password);
        }
    }

    return nullptr;
}

#endif // FILE_HANDLER_H 
