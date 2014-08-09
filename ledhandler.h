#ifndef LEDHANDLER_H
#define LEDHANDLER_H

#include <map>
#include <string>

using namespace std;

class LedHandler
{
public:
    LedHandler();

    string handle(const string &command);

    typedef string (*handler_proc)(LedHandler& /* handler */, const string& /* command */, const string& /* arg */);
    typedef map<string, handler_proc> handler_map;

    char status() const { return status_; }
    void setStatus(char value) { status_ = value; }
    string color() const { return color_; }
    void setColor(const string &value) { color_ = value; }
    char rate() const { return rate_; }
    void setRate(char value) { rate_ = value; }

private:
    handler_map commands_; // Список команд и их обработчиков
    char status_;  // Статус (0 - off, 1 - on, -1 - FAILED)
    string color_; // Цвет ("" - FAILED)
    char rate_;    // Частота (0..5 - OK, -1 - FAILED)
};

#endif // LEDHANDLER_H
