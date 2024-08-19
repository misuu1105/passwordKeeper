#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern const char* BACKGROUND_IMAGE_PATH;
extern const char* FONT_PATH;
extern const int TEXT_SIZE;
extern const int WIDTH;
extern const int HEIGHT;
extern const int BUTTONS_COUNTER;
extern const int BUTTONS_VERTICAL_SPACING;
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

enum class AppState
{
    MAIN_MENU, 
    ADD_PASSWORD, 
    VIEW_PASSWORD,
    DELETE_PASSWORD,
    LIST_PASSWORDS,
};

struct Button
{
    SDL_Rect surface;
    std::string name;
};

class UI
{
    public:
        UI(SDL_Window*& window, SDL_Renderer*& renderer);
        ~UI();

        void runApplication();
        void quitApplication();

        // void setSdlWindow(SDL_Window* window);
        // void setSdlRenderer(SDL_Renderer* renderer);
        
    private:
        SDL_Renderer* sdlRenderer;
        SDL_Window* sdlWindow;
        SDL_Surface* windowSurface;

        void handleEvents(SDL_Event& event, bool& appIsRunning, std::vector<Button>& buttons);
        void handleButton(int currentButton, std::vector<Button>& buttons);
        void setBackgroundImage();
        void setButtonName(std::vector<Button>& buttons, int currentButton);
        void initializeButtons(SDL_Surface*& windowSurface, std::vector<Button>& buttons);
        void fillButtonSurface(Button& button);
        bool inButtonSurface(const int x, const int y, const SDL_Rect& rectangle);
        int whichButton(const std::vector<Button>& buttons, const int x, const int y);

        void showMainMenu(std::vector<Button>& buttons);
        void showAddPasswordMenu(std::vector<Button>& buttons);
        void handleAddPasswordMenuButtons(const int x, const int y, std::string& appNameInput, std::string& passwordInput, bool& inMenu, const Button& appNameTextBox, const Button& passwordTextBox, const Button& clearButton, const Button& sendButton);


        void renderButton(Button& button);
        bool inButtonSurface(const int x, const int y, const Button& button);
        void renderTextBox(Button& button);

        void showViewPasswordMenu();
        void showDeletePasswordMenu();
        void showViewAllPasswordsMenu();
};

#endif
