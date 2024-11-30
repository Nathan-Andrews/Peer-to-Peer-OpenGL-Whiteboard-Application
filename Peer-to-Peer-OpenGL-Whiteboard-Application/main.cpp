//  Include the header file for the ClientInterface class, main GUI for application
#include "Source.h"

// // Include QApplication to handle application-wide settings and resources.

// #include "Source.h"

int main(int argc, char *argv[])
{
    // Create a QApplication object and It is needed for managing GUI resources and control flow.
    QApplication a(argc, argv);

    // Create an instance of the ClientInterface class (the main window).
    ClientInterface w;

    // Display the main window by calling the 'show()' method.
    w.show();

    // The call to 'a.exec()' starts the Qt application event loop.
    return a.exec(); // The return value is used to exit the program cleanly

    // std::cout << "hello world\n";
    // return 0;
}
