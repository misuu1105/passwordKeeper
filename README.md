# passwordKeeper

PasswordKeeper is a simple password manager application built using C++, SDL2, and TinyXML2.

This is my first C++ project, created primarily for learning purposes. The project focuses on practicing OOP principles, which is why all the application logic is implemented using only classes and objects. I chose SDL2 for creating the GUI, mainly to move away from building console applications and try something new.

Why the Simple GUI?

The GUI isn’t particularly sophisticated because the primary goal was to implement it in the simplest way possible. By focusing on simplicity, I could concentrate on understanding and applying fundamental concepts of GUI development and OOP in C++. This approach allowed me to:

  - Avoid the complexities of more advanced GUI frameworks.
	- Learn how to manage UI elements and events using a minimalistic library like SDL2.
	- Keep the codebase clean and focused, making it easier to debug and extend.

While the GUI might not have the polished look of more advanced frameworks, it serves its purpose for this learning project and keeps things straightforward and manageable.

Features:

  - Password Storage: Stores passwords in an XML file format using TinyXML2.
	- Simple User Interface: A basic interface built using SDL2.
	- Cross-Platform: Runs on major operating systems, including Windows, macOS, and Linux, thanks to SDL2’s cross-platform capabilities.

Limitations:

  - Basic UI: As mentioned, the GUI is intentionally kept simple. It may lack certain features found in more sophisticated password managers, such as autofill or advanced encryption.
	- No Security: The focus of this project is on learning rather than providing a password management solution. Thus, it does not use security measures.
 
Libraries Used:

  - SDL2: Simple DirectMedia Layer is a cross-platform development library designed to provide low-level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D.
	- TinyXML2: A simple and lightweight XML parser used for managing password data storage.

Feel free to fork this repository and contribute by submitting pull requests!
