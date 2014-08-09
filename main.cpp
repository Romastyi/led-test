#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

#include "ledhandler.h"

// Классы для чтения/записи в файл.
// В целом не нужны совсем, но пускай будут)))

// Чтение.
class fiforeader
{
public:
    fiforeader(const char *name) : fifoname(name) {}

    // Читает только std::string. Для нас достаточно.
    bool operator>>(std::string &line)
    {
        bool ret = false;
        // Открываем FIFO
        std::ifstream fifo(fifoname.c_str());
        if (fifo.good()) {
            // Читаем команду
            ret = getline(fifo, line);
        } else {
            // Ошибка
            perror("read error: ");
        }
        // Закрываем FIFO
        fifo.close();
        return ret;
    }

private:
    std::string fifoname;
};

// Запись
class fifowriter
{
public:
    fifowriter(const char *name) : fifoname(name) {}

    // Записывает только std::string. Для нас достаточно.
    bool operator<<(std::string &line)
    {
        bool ret = false;
        // Открываем FIFO
        std::ofstream fifo(fifoname.c_str());
        if (fifo.good()) {
            // Записываем ответ
            ret = (fifo << line);
        } else {
            // Ошибка
            perror("write error: ");
        }
        // Закрываем FIFO
        fifo.close();
        return ret;
    }

private:
    std::string fifoname;
};

#define NAMEDPIPE_IN    "/tmp/led_pipe_in"
#define NAMEDPIPE_OUT   "/tmp/led_pipe_out"

int main()
{
    pid_t pid;

    // Удаляем старые файлы, на всякий случай
    unlink(NAMEDPIPE_IN);
    unlink(NAMEDPIPE_OUT);

    // Создаем входящий и выходящий FIFO
    if (mkfifo(NAMEDPIPE_IN, ACCESSPERMS) || mkfifo(NAMEDPIPE_OUT, ACCESSPERMS)) {
        perror("mkfifo error: ");
        return 1;
    }

    // Создаем дочерний процесс, который будет выполнять всю работу
    pid = fork();
    if (pid == -1) {
        perror("fork error: ");
        return 1;
    } else if (pid > 0) {
        // Выходим из родителя
        return 0;
    }

    // Остальное в потомке
    int ret = 0;
    fiforeader reader(NAMEDPIPE_IN);
    fifowriter writer(NAMEDPIPE_OUT);

    LedHandler handler;

    // Основной цикл
    do {
        std::string command;
        // Забираем запрос
        if (reader >> command) {
            // Заданием не предусмотрено, но оставляем для остановки сервера.
            if (command.compare("exit") == 0) {
                writer << command.append("\n");
                break;
            }
            // Обрабатываем полученную команду
            string responce = handler.handle(command);
            if (responce.compare(responce.size() - 1, 1, "\n")) {
                responce.append("\n");
            }
            // Записываем ответ
            writer << responce;
        }
    } while (1);

    // Удаляем связанные файлы
    unlink(NAMEDPIPE_IN);
    unlink(NAMEDPIPE_OUT);

    return ret;
}

