#include "UI.h"

// definitions of the extern constants
const char* BACKGROUND_IMAGE_PATH = "./lib/backgroundImage.bmp";
const char* FONT_PATH = "./lib/buttonsFont.ttf";
const int TEXT_SIZE = 24;
const int WIDTH = 1000;
const int HEIGHT = 550;
const int BUTTONS_COUNTER = 3;
const int BUTTONS_SPACING = 80;
const int MAX_STRING_SIZE = 30;
const SDL_Color BUTTON_COLOR = {255, 255, 255, 255}; 
const SDL_Color TEXT_COLOR = {0, 0, 80, 255}; 
const SDL_Color TEXT_LIGHT_COLOR = {255, 255, 255, 255}; 
const SDL_Rect FIRST_BUTTON_SURFACE = {340, 125, 300, 50};
const SDL_Rect BACK_BUTTON_SURFACE = {20, 20, 100, 50};
const Button BACK_BUTTON = {BACK_BUTTON_SURFACE, "BACK"};

TTF_Font* FONT;
SDL_Rect MESSAGE_BOX_SURFACE = {0, HEIGHT - 50, WIDTH, 50};
TextBox MESSAGE_BOX = {MESSAGE_BOX_SURFACE, "MESSAGE BOX"};

UI::UI(SDL_Window* window, SDL_Renderer* renderer) : window{window}
{
    windowSurface = SDL_GetWindowSurface(window);

    if (windowSurface == nullptr)
        std::cerr << SDL_GetError() << '\n';
    FONT = TTF_OpenFont(FONT_PATH, TEXT_SIZE);

    if (FONT == nullptr)
    {
        std::cerr << "The font could not be opened: " << TTF_GetError() << '\n';
        quitApplication();
    }
}

UI::~UI()
{
    quitApplication();
}

// PUBLIC METHODS

void UI::runApplication()
{
    SDL_Event event;
    bool appIsRunning = true;
    std::vector<Button> buttons{BUTTONS_COUNTER};

    renderMainMenu(buttons);

    while (appIsRunning)
    {
        handleMainMenuEvents(event, appIsRunning, buttons);
    }
}

void UI::quitApplication()
{
    SDL_DestroyWindow(window);
    TTF_CloseFont(FONT);
    TTF_Quit();
    SDL_Quit();
    std::exit(0);
}

// MAIN MENU RELATED METHODS

void UI::renderMainMenu(std::vector<Button>& buttons)
{
    setBackgroundImage();
    initializeMainMenuButtons(windowSurface, buttons);
}

void UI::initializeMainMenuButtons(SDL_Surface* windowSurface, std::vector<Button>& buttons)
{
    for (int currentButton = ADD_PASSWORD; currentButton < BUTTONS_COUNTER; currentButton++)    
    {
        // setting the correct area for a button
        switch (currentButton)
        {
            case ADD_PASSWORD:
                buttons[currentButton].surface = FIRST_BUTTON_SURFACE;
                break;

            default:
                buttons[currentButton].surface = buttons[currentButton - 1].surface;
                buttons[currentButton].surface.y += BUTTONS_SPACING;
                break;
        }

        fillButtonSurface(buttons[currentButton]);
        setMainMenuButtonName(buttons, currentButton);

        SDL_Surface* textSurface = TTF_RenderText_Solid(FONT, buttons[currentButton].name.c_str(), TEXT_COLOR);
                        
        if (textSurface == nullptr)
        {
            std:: cerr << "Text surface could not be rendered: " << SDL_GetError() << '\n';
            quitApplication();
        }

        SDL_Rect textRect;
        textRect.x = buttons[currentButton].surface.x + (buttons[currentButton].surface.w - textSurface->w) / 2; // Center text horizontally
        textRect.y = buttons[currentButton].surface.y + (buttons[currentButton].surface.h - textSurface->h) / 2; // Center text vertically

        SDL_BlitSurface(textSurface, nullptr, windowSurface, &textRect);
        SDL_FreeSurface(textSurface);
    }

    SDL_UpdateWindowSurface(window);
}

void UI::handleMainMenuEvents(SDL_Event& event, bool& appIsRunning, std::vector<Button>& buttons)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                appIsRunning = false;
                return;
            
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                
                int button = whichMainMenuButtonPressed(buttons, x, y);
                handleMainMenuButtonAction(button, buttons); 
                break;
        }
    }
}

void UI::setMainMenuButtonName(std::vector<Button>& buttons, const int currentButton)
{
    switch (currentButton)
    {
        case ADD_PASSWORD:
            buttons[currentButton].name = "ADD A PASSWORD";
            break;
        
        case VIEW_PASSWORD:
            buttons[currentButton].name = "SEARCH PASSWORD";
            break;

        case DELETE_PASSWORD:
            buttons[currentButton].name = "DELETE A PASSWORD";
            break;

        default:
            std::cerr << "You have to add a name for the button in setMainMenuButtonName function\n";
            buttons[currentButton].name = "NO NAME";
            break;
    }
}

int UI::whichMainMenuButtonPressed(const std::vector<Button>& buttons, const int x, const int y)
{
    for (int currentButton = ADD_PASSWORD; currentButton < BUTTONS_COUNTER; ++currentButton)
    {
        if (inButtonSurface(x, y, buttons[currentButton]))
        {
            return currentButton;
        }
    }

    return NO_BUTTON_PRESSED;
}

void UI::handleMainMenuButtonAction(int currentButton, std::vector<Button>& buttons)
{
    switch (currentButton)
    {
        case NO_BUTTON_PRESSED:
            break;

        case ADD_PASSWORD:
            renderAddPasswordMenu(buttons);
            break;

        case VIEW_PASSWORD:
            renderSearchPasswordMenu(buttons);
            break;

        case DELETE_PASSWORD:
            renderDeletePasswordMenu(buttons);
            break;

        default:
            std::cerr << "Unexpected error in the handleMainMenuButtonAction function\n";
            break;
    }
}

// ADD PASSWORD MENU RELATED METHODS

void UI::renderAddPasswordMenu(std::vector<Button>& buttons)
{
    SDL_StartTextInput();

    setBackgroundImage();
    renderButton(BACK_BUTTON);
    renderTextBox(MESSAGE_BOX);
    renderInfoText();

    TextBox appNameTextBox = {FIRST_BUTTON_SURFACE, "APP NAME:"};
    renderTextBox(appNameTextBox);

    TextBox passwordTextBox = {appNameTextBox.surface, "PASSWORD:"};
    passwordTextBox.surface.y += BUTTONS_SPACING;
    renderTextBox(passwordTextBox);

    Button clearButton = {BACK_BUTTON_SURFACE, "CLEAR"};
    clearButton.surface.x = passwordTextBox.surface.x;
    clearButton.surface.y = passwordTextBox.surface.y + BUTTONS_SPACING;
    renderButton(clearButton);

    const int sendButtonSpacing = 10;

    Button sendButton = {clearButton.surface, "SEND"};
    sendButton.surface.x = clearButton.surface.x + clearButton.surface.w + sendButtonSpacing;
    renderButton(sendButton);

    std::string appNameInput;
    std::string passwordInput;
    SDL_Event addPasswordEvent;
    
    handleInput(appNameInput, appNameTextBox);

    bool inAddPasswordMenu = true;

    while (inAddPasswordMenu)
    {
        handleAddPasswordMenuEvents
        (
            addPasswordEvent,
            inAddPasswordMenu,
            appNameInput,
            passwordInput,
            appNameTextBox,
            passwordTextBox,
            clearButton,
            sendButton
        );
    }

    renderMainMenu(buttons);
}

void UI::handleAddPasswordMenuEvents(SDL_Event& event, bool& inMenu, std::string& appNameInput, std::string& passwordInput, const TextBox& appNameTextBox, const TextBox& passwordTextBox, const Button& clearButton, const Button& sendButton)
{ 
    int key;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                quitApplication();
                return;

            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleAddPasswordMenuButtons(x, y, appNameInput, passwordInput, inMenu, appNameTextBox, passwordTextBox, clearButton, sendButton);
                break;

            case SDL_KEYDOWN:

                key = event.key.keysym.sym;
    
                if (key == SDLK_ESCAPE)
                {
                    inMenu = false;
                }

                break;

            default:
                break;
        }
    }
}

void UI::handleAddPasswordMenuButtons(int x, const int y, std::string& appNameInput, std::string& passwordInput, bool& inMenu, const TextBox& appNameTextBox, const TextBox& passwordTextBox, const Button& clearButton, const Button& sendButton)
{
    if (inButtonSurface(x, y, BACK_BUTTON))
    {
       inMenu = false;
    }
    else if (inButtonSurface(x, y, appNameTextBox))
    {
        handleInput(appNameInput, appNameTextBox);
    }
    else if (inButtonSurface(x, y, passwordTextBox))
    {
        handleInput(passwordInput, passwordTextBox);
    }
    else if (inButtonSurface(x, y, sendButton))
    {
        try
        {
            passwordManager.addPassword(appNameInput, passwordInput);
   
            // clearing the text input after it has been send
            appNameInput.clear();
            passwordInput.clear();
            renderTextBox(appNameTextBox);
            renderTextBox(passwordTextBox);
            renderMessageForUser("Password saved successfully", MESSAGE_BOX);
        }
        catch (const std::exception& exception)
        {
            renderMessageForUser(exception.what(), MESSAGE_BOX);
        }
    }
    else if (inButtonSurface(x, y, clearButton))
    {
        appNameInput.clear();
        passwordInput.clear();
        renderTextBox(appNameTextBox);
        renderTextBox(passwordTextBox);
    }
}

// SEARCH PASSWORD RELATED METHODS

void UI::renderSearchPasswordMenu(std::vector<Button>& buttons)
{
    setBackgroundImage();
    renderButton(BACK_BUTTON);
    renderTextBox(MESSAGE_BOX);
    renderInfoText();

    TextBox appNameTextBox = {FIRST_BUTTON_SURFACE, "ENTER THE APP NAME:"};
    renderTextBox(appNameTextBox);

    TextBox getPasswordTextBox = {appNameTextBox.surface, "PASSWORD:"};
    getPasswordTextBox.surface.y += 2 * BUTTONS_SPACING;
    renderTextBox(getPasswordTextBox);

    Button searchButton = {BACK_BUTTON_SURFACE, "SEARCH"};
    searchButton.surface.x = (appNameTextBox.surface.x + appNameTextBox.surface.w) / 2 + 21;
    searchButton.surface.y = appNameTextBox.surface.y + appNameTextBox.surface.h + 20;
    renderButton(searchButton);

    std::string appNameInput;
    bool inSearchPasswordMenu = true;
    SDL_Event searchPasswordEvent;

    handleInput(appNameInput, appNameTextBox);

    while (inSearchPasswordMenu)
    {
        handleSearchPasswordMenuEvents
        (
            searchPasswordEvent,
            inSearchPasswordMenu,
            appNameInput, 
            appNameTextBox,
            getPasswordTextBox,
            searchButton
        );
    }

    renderMainMenu(buttons);
}

void UI::handleSearchPasswordMenuEvents(SDL_Event& event, bool& inMenu, std::string& appNameInput, const TextBox& appNameTextBox, const TextBox& getPasswordTextBox, const Button& searchButton)
{
    int key;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                quitApplication();
                return;

            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleSearchPasswordMenuButtons(x, y, inMenu, appNameInput, appNameTextBox, getPasswordTextBox, searchButton);
                break;

            case SDL_KEYDOWN:
                key = event.key.keysym.sym;

                if (key == SDLK_ESCAPE)
                {
                    inMenu = false;
                }
            
            default:
                break;
        }
    }
}

void UI::handleSearchPasswordMenuButtons(const int x, const int y, bool& inMenu, std::string& appNameInput, const TextBox& appNameTextBox, const TextBox& getPasswordTextBox, const Button& searchButton)
{
    if (inButtonSurface(x, y, BACK_BUTTON))
    {
        inMenu = false;
    }
    else if (inButtonSurface(x, y, appNameTextBox))
    {
        handleInput(appNameInput, appNameTextBox);
    }
    else if (inButtonSurface(x, y, searchButton))
    {
        try
        {
            std::string password = passwordManager.getPassword(appNameInput);
            renderText (getPasswordTextBox, password);
            SDL_SetClipboardText(password.c_str());
            renderMessageForUser("Password saved to clipboard", MESSAGE_BOX);
        }
        catch (const std::exception& exception)
        {
            renderMessageForUser(exception.what(), MESSAGE_BOX);
            renderTextBox(getPasswordTextBox);
        }

        renderTextBox(appNameTextBox);
        appNameInput.clear();
    }
}

void UI::renderDeletePasswordMenu(std::vector<Button>& buttons)
{
    setBackgroundImage();
    renderButton(BACK_BUTTON);
    renderTextBox(MESSAGE_BOX);
    renderInfoText();

    TextBox appNameTextBox{FIRST_BUTTON_SURFACE, "APP NAME: "};
    renderTextBox(appNameTextBox);

    const int spacing = 20;

    Button deleteButton = {BACK_BUTTON_SURFACE, "DELETE"};
    deleteButton.surface.x = appNameTextBox.surface.x + (appNameTextBox.surface.w - deleteButton.surface.w) / 2;
    deleteButton.surface.y = appNameTextBox.surface.y + appNameTextBox.surface.h + spacing;

    renderButton(deleteButton);

    std::string appNameInput;

    handleInput(appNameInput, appNameTextBox);

    SDL_Event renderDeletePasswordEvent;
    bool inDeletePasswordMenu = true;

    while (inDeletePasswordMenu)
    {
        handleDeletePasswordMenuEvents
        (
            renderDeletePasswordEvent,
            inDeletePasswordMenu,
            appNameInput,
            appNameTextBox,
            deleteButton
        );
    }

    renderMainMenu(buttons);
}

void UI::handleDeletePasswordMenuEvents(SDL_Event& event, bool& inMenu, std::string& appNameInput, const TextBox& appNameTextBox, const Button& deleteButton)
{
    int key;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                quitApplication();
                return;

            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleDeletePasswordMenuButtons(x, y, inMenu, appNameInput, appNameTextBox, deleteButton);

            case SDL_KEYDOWN:
                key = event.key.keysym.sym;
    
                if (key == SDLK_ESCAPE)
                {
                    inMenu = false;
                }
                break;

            default:
                break;
        }
    }
}

void UI::handleDeletePasswordMenuButtons(const int x, const int y, bool& inMenu, std::string& appNameInput, const TextBox& appNameTextBox, const Button& deleteButton)
{
    if (inButtonSurface(x, y, BACK_BUTTON))
    {
        inMenu = false;
    }
    else if (inButtonSurface(x, y, appNameTextBox))
    {
        handleInput(appNameInput, appNameTextBox);
    }
    else if (inButtonSurface(x, y, deleteButton))
    {
        try
        {
            passwordManager.deletePassword(appNameInput);
            renderMessageForUser("Password deleted! You can still access it until you close the app", MESSAGE_BOX);
        }
        catch(const std::exception& e)
        {
            renderMessageForUser(e.what(), MESSAGE_BOX);
        }
        
        appNameInput.clear();
        renderTextBox(appNameTextBox);
    }
}

// UNIVERSAL METHODS

void UI::renderText(const TextBox& textBox, std::string& text)
{
    fillButtonSurface(textBox);

    if (text.size() == 0) return; // leaving the text box empty if there is no text
    
    const int maximumTextLength = textBox.surface.w - 10;
    const int minimumTextSize = 16;

    int textSize = TEXT_SIZE - 2; 
    TTF_Font* dynamicFont = TTF_OpenFont(FONT_PATH, textSize);
    SDL_Surface* textSurface = TTF_RenderText_Solid(dynamicFont, text.c_str(), TEXT_COLOR);

    while (textSurface->w > maximumTextLength && textSize > minimumTextSize)
    {
        TTF_CloseFont(dynamicFont);
        
        textSize--;
        dynamicFont = TTF_OpenFont(FONT_PATH, textSize);
        SDL_FreeSurface(textSurface);
        textSurface = TTF_RenderText_Solid(dynamicFont, text.c_str(), TEXT_COLOR);
    }

    while (textSurface->w > maximumTextLength && text.empty() == false)
    {
        text.pop_back();
        SDL_FreeSurface(textSurface);  // Free previous surface
        textSurface = TTF_RenderText_Solid(dynamicFont, text.c_str(), TEXT_COLOR);
    }

    if (textSurface == nullptr)
    {
        std::cerr << "Text could not be loaded\nThe text: " << text;
    }

    SDL_Rect textRect;
    textRect.x = textBox.surface.x + 5;  // slight offset from the left edge
    textRect.y = textBox.surface.y + (textBox.surface.h - textSurface->h) / 2; // center text vertically


    SDL_BlitSurface(textSurface, nullptr, windowSurface, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_UpdateWindowSurface(window);
}

void UI::renderMessageForUser(const std::string& text, const TextBox& textBox)
{
    fillButtonSurface(MESSAGE_BOX);

    TTF_Font* font = TTF_OpenFont(FONT_PATH, TEXT_SIZE - 5);

    if (font == nullptr)
    {
        std::cerr << "Failed to render the font in the renderMessageForUser method:\n" << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color color = {255, 100, 100};

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    
    if (textSurface == nullptr)
    {
        std::cerr << "Failed to render temporary text:\n" << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textPosition;
    textPosition.x = textBox.surface.x + (textBox.surface.w - textSurface->w) / 2; 
    textPosition.y = textBox.surface.y + (textBox.surface.h - textSurface->h) / 2;

    SDL_BlitSurface(textSurface, nullptr, windowSurface, &textPosition);
    SDL_UpdateWindowSurface(window);

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

void UI::handleInput(std::string& inputString, const TextBox& textBox)
{
    SDL_Event textBoxEvent;
    SDL_Keycode key;
    SDL_Keymod modifierKey;
    bool inTextBox = true;

    toggleHighlight(textBox, Highlight::TURN_ON);

    while (inTextBox)
    {
        while (SDL_PollEvent(&textBoxEvent))
        {
            switch (textBoxEvent.type)
            {
                case SDL_QUIT:
                    quitApplication();
                    return;

                case SDL_MOUSEBUTTONDOWN:
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (inButtonSurface(x, y, textBox) == false)
                    {
                        toggleHighlight(textBox, Highlight::TURN_OFF);
                        renderText(textBox, inputString);
                        inTextBox = false;
                    }
                    break;
                
                case SDL_KEYDOWN:
                    key = textBoxEvent.key.keysym.sym;
                    modifierKey = SDL_GetModState();
                    handleInputKeyDown(key, modifierKey, inputString, textBox, inTextBox);
                    break;

                case SDL_TEXTINPUT:
                    inputString += textBoxEvent.text.text;
                    renderText(textBox, inputString);
                    break;
            
                default:
                    break;
            }
        }
    }
}

void UI::handleInputKeyDown(const SDL_Keycode& key, const SDL_Keymod& modifierKey, std::string& inputString, const TextBox& textBox, bool& inTextBox)
{
    if (key == SDLK_ESCAPE)
    {
        toggleHighlight(textBox, Highlight::TURN_OFF);
        inTextBox = false;
    }
    else if (key == SDLK_BACKSPACE && inputString.empty() == false)
    {
        // checking if LGUI or RGUI is pressed to implement the standard behavior of cmd+q on macOS
        if ((modifierKey & KMOD_LGUI) || (modifierKey & KMOD_LGUI) || (modifierKey & KMOD_CTRL))
        {
            inputString.clear();
        }
        else
        {
            inputString.pop_back();
        }
    }
    else if (((modifierKey & KMOD_LGUI) || (modifierKey & KMOD_LGUI) || (modifierKey & KMOD_CTRL)) && key == SDLK_c) // copy
    {
        if (inputString.empty() == false)
        {
            SDL_SetClipboardText(inputString.c_str());
        }
    }
    else if (((modifierKey & KMOD_LGUI) || (modifierKey & KMOD_LGUI) || (modifierKey & KMOD_CTRL)) && key == SDLK_v) // paste
    {
        if (SDL_HasClipboardText())
        {
            char* clipboardText = SDL_GetClipboardText();

            if (clipboardText)
            {
                inputString += clipboardText;
                SDL_free(clipboardText);  // Free the clipboard text memory
            }
        }
    }

    renderText(textBox, inputString);
}

bool UI::inButtonSurface(const int x, const int y, const Button& button)
{
    return x >= button.surface.x && x <= button.surface.x + button.surface.w && y >= button.surface.y && y <= button.surface.y + button.surface.h;
}

void UI::fillButtonSurface(const Button& button)
{
    Uint32 color = SDL_MapRGB(windowSurface->format, BUTTON_COLOR.r, BUTTON_COLOR.g, BUTTON_COLOR.b);

    if (SDL_FillRect(windowSurface, &button.surface, color) < 0)
    {
        std::cerr << "An error encountered in the fillButtonSurface function: " << SDL_GetError() << '\n';
        quitApplication();
    }

    SDL_UpdateWindowSurface(window);
}

void UI::renderButton(const Button& button)
{
    fillButtonSurface(button);

    SDL_Surface* textSurface = TTF_RenderText_Solid(FONT, button.name.c_str(), TEXT_COLOR);
    
    if (textSurface == nullptr) 
    {
        std::cerr << "Failed to render text surface in renderButton method.\n";
        quitApplication();
    }

    SDL_Rect textPosition;
    textPosition.x = button.surface.x + (button.surface.w - textSurface->w) / 2;
    textPosition.y = button.surface.y + (button.surface.h - textSurface->h) / 2;

    SDL_BlitSurface(textSurface, nullptr, windowSurface, &textPosition);
    SDL_FreeSurface(textSurface);
    SDL_UpdateWindowSurface(window);
}

void UI::renderTextBox(const Button& button)
{
    fillButtonSurface(button);

    SDL_Surface* textSurface = TTF_RenderText_Solid(FONT, button.name.c_str(), TEXT_LIGHT_COLOR);

    if (textSurface == nullptr)
    {
        std::cerr << "Unable to render text surface in renderTextBox method\n";
    }

    SDL_Rect textPosition;
    textPosition.x = button.surface.x - textSurface->w - 10; // left alignament
    textPosition.y = button.surface.y + (button.surface.h - textSurface->h) / 2;
    textPosition.w = button.surface.w;
    textPosition.h = button.surface.h;

    SDL_BlitSurface(textSurface, nullptr, windowSurface, &textPosition);
    SDL_FreeSurface(textSurface);
    SDL_UpdateWindowSurface(window);
}

void UI::toggleHighlight(const TextBox& textBox, const Highlight& highlightSwitch)
{
    const SDL_Color highlightColor = {51, 204, 204};
    Uint32 color;

    if (highlightSwitch == Highlight::TURN_ON)
    {
        color = SDL_MapRGB(windowSurface->format, highlightColor.r, highlightColor.g, highlightColor.b);
    }
    else
    {
        color = SDL_MapRGB(windowSurface->format, BUTTON_COLOR.r, BUTTON_COLOR.g, BUTTON_COLOR.b);
    }

    const int lineThickness = 5;

    SDL_Rect highlightRect;
    highlightRect.x = textBox.surface.x; 
    highlightRect.y = textBox.surface.y + textBox.surface.h;
    highlightRect.w = textBox.surface.w; 
    highlightRect.h = lineThickness;

    if (SDL_FillRect(windowSurface, &highlightRect, color) < 0)
    {
        std::cerr << "Could not toggle highligthing\n" << SDL_GetError() << '\n';
    }

    SDL_UpdateWindowSurface(window);
}

void UI::setBackgroundImage()
{
    if (windowSurface == nullptr)
    {
        std::cerr << "Invalid window surface.\n";
        quitApplication();
    }

    SDL_Surface* backgroundImage = SDL_LoadBMP(BACKGROUND_IMAGE_PATH);
    
    if (backgroundImage == nullptr)
    {
        std::cerr << "Failed to load background image: " << SDL_GetError() << '\n';
        quitApplication();
    }

    SDL_Surface* optimizedBackgroundImage = SDL_ConvertSurfaceFormat(backgroundImage, windowSurface->format->format, 0);

    if (optimizedBackgroundImage == nullptr)
    {
        std::cerr << "Failed to convert background image format: " << SDL_GetError() << '\n';
        SDL_FreeSurface(backgroundImage);
        quitApplication();
    }

    SDL_FreeSurface(backgroundImage);

    if (SDL_BlitSurface(optimizedBackgroundImage, nullptr, windowSurface, nullptr) < 0)
    {
        std::cerr << "Failed to blit background image: " << SDL_GetError() << '\n';
        SDL_FreeSurface(optimizedBackgroundImage);
        quitApplication();
    }

    SDL_FreeSurface(optimizedBackgroundImage);
    SDL_UpdateWindowSurface(window);
}

void UI::renderInfoText()
{
    SDL_Surface* infoText = TTF_RenderText_Solid(FONT, "*DOUBLE CLICK THE BUTTONS", TEXT_LIGHT_COLOR);

    if (infoText == nullptr)
    {
        std::cerr << "Cannot initialize infoText variable inside the renderAddPasswordMenu method\n" << SDL_GetError();
    }

    SDL_Rect infoTextPosition = BACK_BUTTON_SURFACE;
    infoTextPosition.x += 8 * BUTTONS_SPACING - 50;
    SDL_BlitSurface(infoText, nullptr, windowSurface, &infoTextPosition);
    SDL_UpdateWindowSurface(window);
}