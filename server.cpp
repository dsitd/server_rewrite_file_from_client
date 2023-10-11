#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

const char* FILE_PATH = "/Users/dsitd/Documents/C++/server/server/example.txt";
const int PORT = 1234;

int main() {
    // Создаем сокет
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }

    // Устанавливаем адрес сервера и порт
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // Привязываем сокет к адресу и порту
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to bind server socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Ожидаем подключения клиента
    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Failed to listen for connections" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Принимаем подключение от клиента
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection" << std::endl;
        close(serverSocket);
        return 1;
    }
    // Читаем данные от клиента
    char buffer[256];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        std::cerr << "Failed to receive data from client" << std::endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }

    // Записываем данные в файл
//    std::ofstream file(FILE_PATH, std::ios::binary);
    std::ofstream file(FILE_PATH, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }
    std::cout << buffer << std::endl;
    file << buffer;
    file.close();
    std::cout << "Data has been written to the file" << std::endl;

    // Закрываем сокеты и завершаем процессы
    close(clientSocket);
    close(serverSocket);
    return 0;
}
