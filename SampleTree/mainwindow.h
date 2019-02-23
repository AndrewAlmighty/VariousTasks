#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
#include <string>
#include <utility>

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
    struct node
    {
        node(int x, int y, int p_x, int p_y, std::string name)
        {
            m_x = x;
            m_y = y;
            point_to_x = p_x;
            point_to_y = p_y;
            m_name = name;
        }

        int m_x;
        int m_y;
        int point_to_x;
        int point_to_y;
        std::string m_name;
    };

    void prepareListOfConnections();
    void fillNodePosVector();

    std::list<std::pair<std::string, std::string>> connections; //parent <- child
    std::vector<node> node_pos; //every two int are x and y of next node.
    int nodes_count;
    std::string m_tree;
};

#endif // MAINWINDOW_H
