#include "UI.h"

// Definitions of the extern constants
const char* BACKGROUND_IMAGE_PATH = "./backgroundImage.bmp";
const char* FONT_PATH = "./buttonsFont.ttf";
const int TEXT_SIZE = 24;
const int WIDTH = 1000;
const int HEIGHT = 550;
const int BUTTONS_COUNTER = 4;
const int BUTTONS_VERTICAL_SPACING = 80;
const SDL_Color BUTTON_COLOR = {180, 180, 180, 255}; // Added alpha for SDL_Color
const SDL_Color TEXT_COLOR = {0, 0, 80, 255}; // Added alpha for SDL_Color
const SDL_Rect FIRST_BUTTON_SURFACE = {340, 125, 300, 50};

UI::UI(SDL_Window*& window, SDL_Renderer*& renderer) : sdlWindow{window}, sdlRenderer{renderer} {}

UI::~UI()
{
    quitApplication();
}

void UI::runApplication()
{
    SDL_Surface* windowSurface = SDL_GetWindowSurface(sdlWindow);

    if (windowSurface == nullptr)
        std::cerr << SDL_GetError() << '\n';

    TTF_Font* font = TTF_OpenFont(FONT_PATH, TEXT_SIZE);

    if (font == nullptr)
    {
        std::cerr << "The font could not be opened: " << TTF_GetError() << '\n';
        quitApplication();
    }

    SDL_Event event;
    bool appIsRunning = true;
    std::vector<Button> buttons{BUTTONS_COUNTER};

    showMainMenu(windowSurface, buttons, font);

    while (appIsRunning)
    {
        handleEvents(event, appIsRunning, buttons);
    }

    TTF_CloseFont(font);
}

void UI::quitApplication()
{
    SDL_DestroyWindow(sdlWindow);
    SDL_DestroyRenderer(sdlRenderer);
    TTF_Quit();
    SDL_Quit();
    std::exit(0);
}

void UI::handleEvents(SDL_Event& event, bool& appIsRunning, const std::vector<Button>& buttons)
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
                handleButton(button); 
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

void UI::handleButton(int currentButton)
{
    switch (currentButton)
    {
        case NO_BUTTON_PRESSED:
            break;

        case ADD_PASSWORD:
            showAddPasswordMenu();
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

void UI::setBackgroundImage(SDL_Surface*& windowSurface)
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

void UI::fillButton(SDL_Rect& button, SDL_Surface* windowSurface)
{
    Uint32 color = SDL_MapRGB(windowSurface->format, BUTTON_COLOR.r, BUTTON_COLOR.g, BUTTON_COLOR.b);
    if (SDL_FillRect(windowSurface, &button, color) < 0)
    {
        std::cerr << "An error encountered in the fillButton function: " << SDL_GetError() << '\n';
        quitApplication();
    }
}

void UI::initializeButtons(SDL_Surface*& windowSurface, std::vector<Button>& buttons, TTF_Font*& font)
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

        fillButton(buttons[currentButton].surface, windowSurface);
        setButtonName(buttons, currentButton);

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, buttons[currentButton].name.c_str(), TEXT_COLOR);
                        
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

void UI::showMainMenu(SDL_Surface*& windowSurface, std::vector<Button>& buttons, TTF_Font*& font)
{
    setBackgroundImage(windowSurface);
    initializeButtons(windowSurface, buttons, font);
}

void UI::showAddPasswordMenu(SDL_Surface*& windowSurface, std::vector<Button>& buttons, TTF_Font*& font)
{
    SDL_StartTextInput();

    setBackgroundImage(windowSurface);
    renderButton(windowSurface, BACK_BUTTON);

    Button appNameTextBox = {FIRST_BUTTON_SURFACE, "APP NAME:"};
    renderTextBox(windowSurface, appNameTextBox);

    Button passwordTextBox = {FIRST_BUTTON_SURFACE, "PASSWORD:"};
    passwordTextBox.surface.y += BUTTONS_VERTICAL_SPACING;
    renderTextBox(windowSurface, passwordTextBox);

    Button clearButton = {BACK_BUTTON_SURFACE, "CLEAR"};
    clearButton.surface.x = passwordTextBox.surface.x;
    clearButton.surface.y = passwordTextBox.surface.y + BUTTONS_VERTICAL_SPACING;
    renderButton(windowSurface, clearButton);

    Button sendButton = {clearButton.surface, "SEND"};
    sendButton.surface.x = clearButton.surface.x + clearButton.surface.w + 10;
    renderButton(windowSurface, sendButton);

    std::string appNameInput;
    std::string passwordInput;

    SDL_Event addPasswordEvent;
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

    showMainMenu(windowSurface, buttons, font);
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