#ifndef SERVER_H
#define SERVER_H

#include <QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTcpServer>
#include <QTcpSocket>

#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <X11/Xcursor/Xcursor.h>
#endif

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

public slots:
    void accept();
    void read();
    void exec(QString request);

private:
    QMenu *trayMenu;
    QSystemTrayIcon *tray;
    QTcpServer *server;
    QTcpSocket *client;
};

#endif // SERVER_H
