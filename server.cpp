#include "server.h"

Server::Server(QWidget *parent) : QDialog(parent)
{
    trayMenu = new QMenu;
    trayMenu->addAction(QIcon(":/off"), "Exit", qApp, SLOT(quit()));

    tray = new QSystemTrayIcon(QIcon(":/icon"), this);
    tray->setContextMenu(trayMenu);
    tray->show();

    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(accept()));

    if(server->listen(QHostAddress::AnyIPv4, 7777))
    {
        QMessageBox::information(this, "Success", "Listening on port 7777");
    }
    else
    {
        QMessageBox::critical(this, "Error", "Unable to listen on port 7777");
        exit(EXIT_FAILURE);
    }
}

Server::~Server()
{
}

void Server::accept()
{
    client = server->nextPendingConnection();
    client->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect(client, SIGNAL(readyRead()), this, SLOT(read()));
}

void Server::read()
{
#ifdef Q_OS_LINUX
    QString request = QString(client->readAll()).remove('|');
    exec(request);
#endif

#ifdef Q_OS_WIN32
    
    /** \bug winsock causes abnormal buffer read, so using a delimiter
     * to separate the requests and execute them one by one
     */
    
    QStringList requests = QString(client->readAll()).split('|');
    requests.pop_back(); // remove the last '|' character
    foreach (QString cmd, requests)
        exec(cmd);
#endif
}

void Server::exec(QString request)
{
    QPoint pos = QCursor::pos();

    if(request == "connect")
    {
        tray->setIcon(QIcon(":/on"));
    }
    else if(request == "disconnect")
    {
        tray->setIcon(QIcon(":/off"));
    }
    else if(request == "click")
    {
#ifdef Q_OS_WIN32
        mouse_event(MOUSEEVENTF_LEFTDOWN, pos.x(), pos.y(), 0,0);
        mouse_event(MOUSEEVENTF_LEFTUP, pos.x(), pos.y(), 0,0);
#endif
        
#ifdef Q_OS_LINUX
        /** \todo add x11 based left click event **/
#endif
    }
    else if(request == "menu")
    {
#ifdef Q_OS_WIN32
        mouse_event(MOUSEEVENTF_RIGHTDOWN, pos.x(), pos.y(), 0,0);
        mouse_event(MOUSEEVENTF_RIGHTUP, pos.x(), pos.y(), 0,0);
#endif
        
#ifdef Q_OS_LINUX
        /** \todo add x11 based right click event **/
#endif
    }
    else
    {
        float x = request.split(",")[0].toFloat();
        float y = request.split(",")[1].toFloat();
        QCursor::setPos(pos.x() + x, pos.y() + y);
    }
}
