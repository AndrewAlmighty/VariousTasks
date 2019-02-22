#include "mainwindow.h"
#include <list>
#include <utility>
#include <vector>
#include <QPainter>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_tree = "";
    resize(600, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::acceptSerializedTree(const std::string &tree)
{
    if (m_tree == tree || tree.empty())
        return;

    m_tree = tree;
    paintEvent(nullptr);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::green);
    int my_x = 300;
    int my_y = 50;
    unsigned nodes_count = 0;
    std::list<std::pair<std::string, std::string>> connections; //parent <- child

    {
        std::string tmp, nodeName;
        bool getNodeName = false;
        bool getParentName = false;

        for (unsigned i = 0; i < m_tree.size(); i++)
        {
            tmp += m_tree[i];
           // std::cerr << tmp << std::endl;

            if (!getNodeName && tmp == "N:")
            {
                tmp = "";
                getNodeName = true;
            }

            else if (((getNodeName && m_tree[i] == ')') ||
                     (getNodeName && tmp == "NULL")) && nodeName.empty())
            {
                getNodeName = false;
                nodeName = tmp;
                tmp = "";
                std::cerr << "Mamy " << nodeName << std::endl;
            }

            if (!getParentName && tmp == "P:")
            {
                tmp = "";
                getParentName = true;
            }

            else if (((getParentName && m_tree[i] == ')') ||
                      (getParentName && tmp == "NULL")))
            {
                connections.push_back(std::pair<std::string, std::string>(tmp, nodeName));
                tmp = "";
                getParentName = false;
            }

            if (m_tree[i] == ';')
            {
                tmp = "";
                nodes_count++;
                nodeName = "";
            }
        }
    }

    for (const auto& pair : connections)
        std::cerr << pair.first << "<-" << pair.second << std::endl;


    /*
    std::vector<int> node_pos (nodes_count * 2, 0); //every two int are x and y of next node.

    for (unsigned i = 0; i < nodes_count; i++)
    {
        QRectF rectangle(my_x, my_y, 40.0, 40.0);
        node_pos[i * 2] = my_x;
        node_pos[i * 2 + 1] = my_y;
        painter.drawEllipse(rectangle);
    }
    */
}


