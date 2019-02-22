#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void acceptSerializedTree(const std::string &tree);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    std::string m_tree;
};

#endif // MAINWINDOW_H
