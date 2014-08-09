#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "ledhandler.h"

const char *result[2] = {"FAILED", "OK"};
const char *status[2] = {"off", "on"};
const char *colors[3] = {"red", "green", "blue"};

// Обработчики команд
static string getLedState(LedHandler& handler, const string&, const string&);
static string setLedState(LedHandler& handler, const string&, const string &arg);
static string getLedColor(LedHandler& handler, const string&, const string&);
static string setLedColor(LedHandler& handler, const string&, const string &arg);
static string getLedRate (LedHandler& handler, const string&, const string&);
static string setLedRate (LedHandler& handler, const string&, const string &arg);

LedHandler::LedHandler() :
    status_(-1), color_(""), rate_(-1)
{
    commands_.insert(make_pair("get-led-state", &getLedState));
    commands_.insert(make_pair("set-led-state", &setLedState));
    commands_.insert(make_pair("get-led-color", &getLedColor));
    commands_.insert(make_pair("set-led-color", &setLedColor));
    commands_.insert(make_pair("get-led-rate" , &getLedRate ));
    commands_.insert(make_pair("set-led-rate" , &setLedRate ));
}

string LedHandler::handle(const string &command)
{
    // Разбираем комнаду на части
    string str = command;
    string delimiter = " ";
    size_t pos;
    vector<string> tokens;
    while ((pos = str.find(delimiter)) != string::npos) {
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    tokens.push_back(str);
    string cmd = tokens.at(0); // Сама команда
    string arg = tokens.size() > 1 ? tokens.at(1) : ""; // Аргумент команды
    // Кол-во аргументов не проверяем!
    // Проверяем есть ли такая команда
    handler_map::iterator it = commands_.find(cmd);
    if (it == commands_.end()) {
        // Такой команды нет
        return result[0];
    }
    // Выполянем команду
    return it->second(*this, cmd, arg);
}

string getLedState(LedHandler &handler, const string &, const string &)
{
    if (handler.status() == -1) {
        return result[0];
    }
    string resp = result[1];
    return resp.append(" ").append(status[handler.status()]);
}

string setLedState(LedHandler &handler, const string&, const string &arg)
{
    if (arg.compare("on") == 0) {
        handler.setStatus(1);
    } else if (arg.compare("off") == 0) {
        handler.setStatus(0);
    } else {
        return result[0];
    }
    return result[1];
}

string getLedColor(LedHandler& handler, const string&, const string&)
{
    if (handler.color().empty()) {
        return result[0];
    }
    string resp = result[1];
    return resp.append(" ").append(handler.color());
}

string setLedColor(LedHandler &handler, const string&, const string &arg)
{
    vector<string> cols;
    cols.assign(colors, colors + sizeof(colors) / sizeof(char*));
    vector<string>::iterator it = find(cols.begin(), cols.end(), arg);
    if (it == cols.end()) {
        return result[0];
    }
    handler.setColor(*it);
    return result[1];
}

string getLedRate(LedHandler &handler, const string&, const string&)
{
    if (handler.rate() == -1) {
        return result[0];
    }
    // Обрабатываем топорно (знаем, что число только из 1ой цифры)
    string resp = result[1];
    char rate_str[2];
    sprintf(rate_str, "%u", handler.rate());
    rate_str[1] = '\0';
    return resp.append(" ").append(rate_str);
}

string setLedRate(LedHandler &handler, const string&, const string &arg)
{
    int rate = atoi(arg.c_str());
    if (rate < 0 || rate > 5) {
        return result[0];
    }
    handler.setRate(static_cast<char>(rate));
    return result[1];
}
