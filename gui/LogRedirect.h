#ifndef LOGREDIRECT_H
#define LOGREDIRECT_H

#include <iostream>
#include <sstream>
#include <streambuf>
#include <QObject>
#include <QString>

// Redirects std::cout to a Qt signal so the GUI can display C++ device logs
class QtLogRedirect : public QObject, public std::streambuf {
    Q_OBJECT

public:
    explicit QtLogRedirect(QObject* parent = nullptr)
        : QObject(parent), oldBuf(std::cout.rdbuf(this)) {}

    ~QtLogRedirect() override {
        std::cout.rdbuf(oldBuf);
    }

signals:
    void newMessage(const QString& msg);

protected:
    int overflow(int c) override {
        if (c == EOF) return EOF;
        if (c == '\n') {
            emit newMessage(QString::fromStdString(buffer));
            buffer.clear();
        } else {
            buffer += static_cast<char>(c);
        }
        return c;
    }

private:
    std::streambuf* oldBuf;
    std::string buffer;
};

#endif
