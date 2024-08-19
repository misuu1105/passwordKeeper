#include "UI.h"

// Definitions of the extern constants
const char* BACKGROUND_IMAGE_PATH = "./backgroundImage.bmp";
const char* FONT_PATH = "./buttonsFont.ttf";
const int TEXT_SIZE = 24;
const int WIDTH = 1000;
const int HEIGHT = 550;
const int BUTTONS_COUNTER = 4;
const int BUTTONS_VERTICAL_SPACING = 80;
const SDL_Color BUTTON_COLOR = {255, 255, 255, 255}; 
const SDL_Color TEXT_COLOR = {0, 0, 80, 255}; 
const SDL_Color TEXT_LIGHT_COLOR = {255, 255, 255, 255}; 
const SDL_Rect FIRST_BUTTON_SURFACE = {340, 125, 300, 50};
const SDL_Rect BACK_BUTTON_SURFACE = {20, 20, 100, 50};
TTF_Font* FONT;
Button BACK_BUTTON = {BACK_BUTTON_SURFACE, "BACK"};

UI::UI(SDL_Window*& window, SDL_Renderer*& renderer) : sdlWindow{window}, sdlRenderer{renderer} {}

UI::~UI()
{
    quitApplication();
}

void UI::runApplication()
{
    windowSurface = SDL_GetWindowSurface(sdlWindow);

    if (windowSurface == nullptr)
        std::cerr << SDL_GetError() << '\n';

    FONT = TTF_OpenFont(FONT_PATH, TEXT_SIZE);

    if (FONT == nullptr)
    {
        std::cerr << "The font could not be opened: " << TTF_GetError() << '\n';
        quitApplication();
    }

    SDL_Event event;
    bool appIsRunning = true;
    std::vector<Button> buttons{BUTTONS_COUNTER};

    showMainMenu(buttons);

    while (appIsRunning)
    {
        handleEvents(event, appIsRunning, buttons);
    }

    TTF_CloseFont(FONT);
}

void UI::quitApplication()
{
    SDL_DestroyWindow(sdlWindow);
    SDL_DestroyRenderer(sdlRenderer);
    TTF_CloseFont(FONT);
    TTF_Quit();
    SDL_Quit();
    std::exit(0);
}

void UI::handleEvents(SDL_Event& event, bool& appIsRunning, std::vector<Button>& buttons)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                appIsRunning = false;
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                
                int button = whichButton(buttons, x, y);
                handleButton(button, buttons); 
                break;
        }
    }
}

bool UI::inButtonSurface(const int x, const int y, const SDL_Rect& rectangle)
{
    return x >= rectangle.x && x <= rectangle.x + rectangle.w && y >= rectangle.y && y <= rectangle.y + rectangle.h;
}

int UI::whichButton(const std::vector<Button>& buttons, const int x, const int y)
{
    for (int currentButton = ADD_PASSWORD; currentButton < BUTTONS_COUNTER; ++currentButton)
    {
        if (inButtonSurface(x, y, buttons[currentButton].surface))
            return currentButton;
    }

    return NO_BUTTON_PRESSED;
}

void UI::handleButton(int currentButton, std::vector<Button>& buttons)
{
    switch (currentButton)
    {
        case NO_BUTTON_PRESSED:
            break;

        case ADD_PASSWORD:
            showAddPasswordMenu(buttons);
            std::cout << currentButton + 1 << '\n';
            break;

        case VIEW_PASSWORD:
            std::cout << currentButton + 1 << '\n';
            showViewPasswordMenu();
            break;

        case DELETE_PASSWORD:
            std::cout << currentButton + 1 << '\n';
            showDeletePasswordMenu();
            break;

        case LIST_PASSWORDS:
            std::cout << currentButton + 1 << '\n';
            showViewAllPasswordsMenu();
            break;

        default:
            std::cerr << "Unexpected error in the handleButton function\n";
            break;
    }
}

void UI::setBackgroundImage()
{
    if (!windowSurface)
    {
        std::cerr << "Invalid window surface.\n";
        quitApplication();
    }

    SDL_Surface* backgroundImage = SDL_LoadBMP(BACKGROUND_IMAGE_PATH);
    if (!backgroundImage)
    {
        std::cerr << "Failed to load background image: " << SDL_GetError() << '\n';
        quitApplication();
    }

    SDL_Surface* optimizedBackgroundImage = SDL_ConvertSurfaceFormat(backgroundImage, windowSurface->format->format, 0);
    if (!optimizedBackgroundImage)
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
    SDL_UpdateWindowSurface(sdlWindow);
}

void UI::setButtonName(std::vector<Button>& buttons, int currentButton)
{
    switch (currentButton)
    {
        case ADD_PASSWORD:
            buttons[currentButton].name = "ADD A PASSWORD";
            break;
        
        case VIEW_PASSWORD:
            buttons[currentButton].name = "VIEW A PASSWORD";
            break;

        case DELETE_PASSWORD:
            buttons[currentButton].name = "DELETE A PASSWORD";
            break;

        case LIST_PASSWORDS:
            buttons[currentButton].name = "LIST PASSWORDS";
            break;

        default:
            std::cerr << "You have to add a name for the button in setButtonName function\n";
            buttons[currentButton].name = "NO NAME";
            break;
    }
}

void UI::fillButtonSurface(Button& button)
{
    Uint32 color = SDL_MapRGB(windowSurface->format, BUTTON_COLOR.r, BUTTON_COLOR.g, BUTTON_COLOR.b);
    if (SDL_FillRect(windowSurface, &button.surface, color) < 0)
    {
        std::cerr << "An error encountered in the fillButtonSurface function: " << SDL_GetError() << '\n';
        quitApplication();
    }
}

void UI::initializeButtons(SDL_Surface*& windowSurface, std::vector<Button>& buttons)
{
    for (int currentButton = ADD_PASSWORD; currentButton < BUTTONS_COUNTER; currentButton++)    
    {
        // setting the correct area of a button
        switch (currentButton)
        {
            case ADD_PASSWORD:
                buttons[currentButton].surface = FIRST_BUTTON_SURFACE;
                break;

            default:
                buttons[currentButton].surface = buttons[currentButton - 1].surface;
                buttons[currentButton].surface.y += BUTTONS_VERTICAL_SPACING;
                break;
        }

        fillButtonSurface(buttons[currentButton]);
        setButtonName(buttons, currentButton);

        SDL_Surface* textSurface = TTF_RenderText_Solid(FONT, buttons[currentButton].name.c_str(), TEXT_COLOR);
                        
        if (textSurface == nullptr)
        {
            std:: cerr << "Text surface could not be rendered " << SDL_GetError() << '\n';
            quitApplication();
        }

        SDL_Rect textRect;
        textRect.x = buttons[currentButton].surface.x + (buttons[currentButton].surface.w - textSurface->w) / 2; // Center text horizontally
        textRect.y = buttons[currentButton].surface.y + (buttons[currentButton].surface.h - textSurface->h) / 2; // Center text vertically

        SDL_BlitSurface(textSurface, nullptr, windowSurface, &textRect);
        SDL_FreeSurface(textSurface);
    }

    SDL_UpdateWindowSurface(sdlWindow);
}

void UI::showMainMenu(std::vector<Button>& buttons)
{
    setBackgroundImage();
    initializeButtons(windowSurface, buttons);
}

void UI::showAddPasswordMenu(std::vector<Button>& buttons)
{
    SDL_StartTextInput();

    setBackgroundImage();
    renderButton(BACK_BUTTON);

    Button appNameTextBox = {FIRST_BUTTON_SURFACE, "APP NAME:"};
    renderTextBox(appNameTextBox);

    Button passwordTextBox = {FIRST_BUTTON_SURFACE, "PASSWORD:"};
    passwordTextBox.surface.y += BUTTONS_VERTICAL_SPACING;
    renderTextBox(passwordTextBox);

    Button clearButton = {BACK_BUTTON_SURFACE, "CLEAR"};
    clearButton.surface.x = passwordTextBox.surface.x;
    clearButton.surface.y = passwordTextBox.surface.y + BUTTONS_VERTICAL_SPACING;
    renderButton(clearButton);

    Button sendButton = {clearButton.surface, "SEND"};
    sendButton.surface.x = clearButton.surface.x + clearButton.surface.w + 10;
    renderButton(sendButton);

    std::string appNameInput;
    std::string passwordInput;

    SDL_Event addPasswordEvent;
    bool inAddPasswordMenu = true;

    while (inAddPasswordMenu)
    {
        // handleAddPasswordMenuEvents
        // (
        //     addPasswordEvent,
        //     inAddPasswordMenu,
        //     appNameInput,
        //     passwordInput,
        //     appNameTextBox,
        //     passwordTextBox,
        //     clearButton,
        //     sendButton
        // );
    }

    showMainMenu(buttons);
}

void UI::handleAddPasswordMenuButtons(const int x, const int y, std::string& appNameInput, std::string& passwordInput, bool& inMenu, const Button& appNameTextBox, const Button& passwordTextBox, const Button& clearButton, const Button& sendButton)
{
    if (inButtonSurface(x, y, BACK_BUTTON))
    {
       inMenu = false;
    }
    else if (inButtonSurface(x, y, appNameTextBox))
    {
        inMenu = true;
    }
    else if (inButtonSurface(x, y, clearButton))
    {
        appNameInput.clear();
        passwordInput.clear();
    }
    else if (inButtonSurface(x, y, sendButton))
    {
        // sendAppNameAndPassword(appNameInput, passwordInput);
    }
}

bool UI::inButtonSurface(const int x, const int y, const Button& button)
{
    return x >= button.surface.x && x <= button.surface.x + button.surface.w && y >= button.surface.y && y <= button.surface.y + button.surface.h;
}

void UI::renderButton(Button& button)
{
    fillButtonSurface(button);

    SDL_Surface* textSurface = TTF_RenderText_Solid(FONT, button.name.c_str(), TEXT_COLOR);
    if (!textSurface) 
    {
        std::cerr << "Failed to render text surface in renderButton method.\n";
        quitApplication();
    }

    SDL_Rect textPosition;
    textPosition.x = button.surface.x + (button.surface.w - textSurface->w) / 2;
    textPosition.y = button.surface.y + (button.surface.h - textSurface->h) / 2;

    SDL_BlitSurface(textSurface, nullptr, windowSurface, &textPosition);
    SDL_FreeSurface(textSurface);
}

void UI::renderTextBox(Button& button)
{
    fillButtonSurface(button);

    SDL_Surface* textSurface = TTF_RenderText_Solid(FONT, button.name.c_str(), TEXT_LIGHT_COLOR);

    if (textSurface == nullptr)
    {
        std::cerr << "Unable to render text surface in renderTextBox method\n";
    }

    SDL_Rect textPosition;
    textPosition.x = button.surface.x - textSurface->w - 10; // left aligning the text
    textPosition.y = button.surface.y + (button.surface.h - textSurface->h) / 2;
    textPosition.w = button.surface.w;
    textPosition.h = button.surface.h;

    SDL_BlitSurface(textSurface, nullptr, windowSurface, &textPosition);
    SDL_FreeSurface(textSurface);
    SDL_UpdateWindowSurface(sdlWindow);
}

void UI::showViewPasswordMenu()
{
}

void UI::showDeletePasswordMenu()
{
}

void UI::showViewAllPasswordsMenu()
{
}