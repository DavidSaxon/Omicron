#include <iostream>

#include <QtWidgets/QApplication>

#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello world!" << std::endl;

    QApplication app(argc, argv);

    MainWindow* window = new MainWindow();

    window->show();

    return QApplication::exec();
}
