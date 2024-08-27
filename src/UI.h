#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "PasswordManager.h"

extern const char* BACKGROUND_IMAGE_PATH;
extern const char* FONT_PATH;
extern const int TEXT_SIZE;
extern const int WIDTH;
extern const int HEIGHT;
extern const int BUTTONS_COUNTER;
extern const int BUTTONS_SPACING;
extern const SDL_Color BUTTON_COLOR;
extern const SDL_Color TEXT_COLOR;
extern const SDL_Rect FIRST_BUTTON_SURFACE;

enum
{
    ADD_PASSWORD, // the name of the first button
    VIEW_PASSWORD, // the name of the second button
    DELETE_PASSWORD, // ...
    LIST_PASSWORDS,
    NO_BUTTON_PRESSED = -1
};

enum class Highlight
{
    TURN_ON,
    TURN_OFF
};

struct Button
{
    SDL_Rect surface;
    std::string name;
};

typedef Button TextBox;

class UI
{
    public:
        UI(SDL_Window* window, SDL_Renderer* renderer);
        ~UI();

        void runApplication();
        void quitApplication();

    private:
        SDL_Window* appWindow;
        SDL_Surface* windowSurface;
        PasswordManager passwordManager;

        // MAIN MENU METHODS
        void handleMainMenuEvents(SDL_Event& event, bool& appIsRunning, std::vector<Button>& buttons);
        void handleMainMenuButtonAction(int currentButton, std::vector<Button>& buttons);
        void setMainMenuButtonName(std::vector<Button>& buttons, const int currentButton);
        void initializeMainMenuButtons(SDL_Surface* windowSurface, std::vector<Button>& buttons);
        int whichMainMenuButtonPressed(const std::vector<Button>& buttons, const int x, const int y);

        // ADD PASSWORD MENU METHODS
        void renderMainMenu(std::vector<Button>& buttons);
        void renderAddPasswordMenu(std::vector<Button>& buttons);
        void handleAddPasswordMenuEvents(SDL_Event& event, bool& inMenu,std::string& appNameInput, std::string& passwordInput, const TextBox& appNameTextBox, const TextBox& passwordTextBox, const Button& clearButton, const Button& sendButton);
        void handleAddPasswordMenuButtons(const int x, const int y, std::string& appNameInput, std::string& passwordInput, bool& inMenu, const TextBox& appNameTextBox, const TextBox& passwordTextBox, const Button& clearButton, const Button& sendButton);

        void renderViewPasswordMenu();
        void renderDeletePasswordMenu();

        // UNIVERSAL METHODS
        void renderText(const TextBox& textBox, std::string& text);
        void renderUserMessage(const std::string& text, TextBox& textBox);
        void handleInput(std::string& inputString, const TextBox& textBox);
        void handleKeyDown(const SDL_Keycode& key, const SDL_Keymod& modifierKey, std::string& inputString, const TextBox& textBox, bool& inTextBox);
        void renderButton(const 
        Button& button);
        void renderTextBox(const Button& button);
        void toggleHighlight(const TextBox& textBox, const Highlight& highlightState);
        void fillButtonSurface(const Button& button);
        bool inButtonSurface(const int x, const int y, const Button& button);
        void setBackgroundImage();
};

#endif
