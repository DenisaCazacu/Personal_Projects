#include <QApplication>
#include <iostream>
#include <future>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include "Frontend/mainwindow.h"
using namespace std;

constexpr int BUFFER_SIZE = 1024;
constexpr const char* RED = "\033[31m";
constexpr const char* MAGENTA = "\033[0;35m";
constexpr const char* CYAN = "\033[0;36m";
constexpr const char* BLUE = "\033[0;34m";
constexpr const char* GREEN = "\033[0;32m";
constexpr const char* RESET_COLOR = "\033[0m";

inline void handle_error(const string& error_name) {
    cerr << RED << "[Client] Error at " << error_name << ".\n" << RESET_COLOR;
    exit(EXIT_FAILURE);
}

int port;
void wait(int socket_descriptor, MainWindow* mainWin);
void play(int socket_descriptor,  MainWindow* mainWin);
string get_answer_question();
string socket_read(int descriptor);

void client_network(const std::string& server_address, int port, MainWindow* mainWindow) {
    int socket_descriptor;
    sockaddr_in server;

    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        handle_error("socket");

    // Server socket structure initialization
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_address.c_str());
    server.sin_port = htons(port);

    // Connect to the server
    if (connect(socket_descriptor, reinterpret_cast<struct sockaddr*>(&server), sizeof(server)) == -1)
        handle_error("connect");
    
    mainWindow->setSocketDescriptor(socket_descriptor);

    wait(socket_descriptor, mainWindow);
    play(socket_descriptor,mainWindow);
   
    close(socket_descriptor);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (argc != 3) {
        std::cerr << RED << "Syntax: " << argv[0] << " <server_address> <port>\n" << RESET_COLOR;
        return -1;
    }

    std::string server_address = argv[1];
    int port = std::stoi(argv[2]);

    MainWindow mainWindow;
    mainWindow.show();
    QObject::connect(&app, &QApplication::lastWindowClosed, &app, &QApplication::quit);
    auto thread = std::async(std::launch::async, client_network, server_address, port, &mainWindow);
    QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
        exit(0);
    });
    return app.exec();
    exit(0);
}

string socket_read(int descriptor) {
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read = read(descriptor, buffer, BUFFER_SIZE - 1);
    if (bytes_read <= 0)
        handle_error("read");

    return string(buffer);
}

void wait(int socket_descriptor, MainWindow* mainWin) {

    while (true) {
        string buffer = socket_read(socket_descriptor);
        cout << CYAN << "[Server] " << buffer << "\n" << RESET_COLOR;

        if (buffer == "Ready?") {
            sleep(2);
             QMetaObject::invokeMethod(mainWin, "showReadyPage");
        }
        else {
            QMetaObject::invokeMethod(mainWin, "showStartPage", Q_ARG(QString, QString::fromStdString(buffer)));
        }
        if (buffer == "START!") {
            break;
        }
    }
}

void play(int socket_descriptor,  MainWindow* mainWin) {
    while (true) {
        string buffer = socket_read(socket_descriptor);
        if (buffer.find("GAME OVER!") != string::npos) {
            cout << GREEN << "[Server] " << buffer << "\n" << RESET_COLOR;
            string buffer = socket_read(socket_descriptor);
            QMetaObject::invokeMethod(mainWin, "showGameOverPage", Q_ARG(QString, QString::fromStdString(buffer)));
            cout << BLUE << "[Server] " << buffer << "\n" << RESET_COLOR;
            break;
        }

        cout << MAGENTA << "[Server] " << buffer << "\n" << RESET_COLOR;

        QMetaObject::invokeMethod(mainWin, "showPlayPage", Q_ARG(QString, QString::fromStdString(buffer)));
    }

}