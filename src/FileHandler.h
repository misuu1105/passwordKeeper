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

        void saveToFile(const std::string& appName, const std::string& password) noexcept;
        int deleteFromFile(const std::string& appName) noexcept;

        template<typename SmartPointerType>
        SmartPointerType loadFromFile(const std::string& appName) const noexcept;

    private:
        tinyxml2::XMLDocument file;
        std::string fileName;

        void createNewFile(tinyxml2::XMLDocument& file, const char* fileName);
};

template<typename SmartPointerType>
SmartPointerType FileHandler::loadFromFile(const std::string& appName) const noexcept
{
    // making sure that the template provided is the right one
    static_assert
    ( 
        std::is_same_v<SmartPointerType, std::unique_ptr<std::string>> || std::is_same_v<SmartPointerType, std::shared_ptr<std::string>>,
        "SmartPointerType must be either std::unique_ptr<std::string> or std::shared_ptr<std::string>"
    );

    const char* APP_NAME = appName.c_str();
    const char* ATTRIBUTE = "password";

    const tinyxml2::XMLElement* root = file.RootElement();
    const tinyxml2::XMLElement* searchedElement = root ? root->FirstChildElement(APP_NAME) : nullptr;

    // returning the right form of the template
    if (searchedElement)
    {
        const char* password = searchedElement->Attribute(ATTRIBUTE);

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