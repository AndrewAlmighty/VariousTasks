#include "mainwindow.h"
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
    nodes_count = 0;
    connections.clear();
    prepareListOfConnections();
    connections.sort();
    fillNodePosVector();
    paintEvent(nullptr);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (nodes_count == 0)
        return;

    QPainter painter(this);
    painter.setBrush(Qt::green);

    for (unsigned i = 0; i < node_pos.size(); i++)
    {
        QRectF rectangle(node_pos[i].m_x, node_pos[i].m_y, 30.0, 30.0);
        painter.drawEllipse(rectangle);
        painter.drawText(node_pos[i].m_x, node_pos[i].m_y, QString::fromStdString(node_pos[i].m_name));
        if (node_pos[i].point_to_x >= 0)
            painter.drawLine(node_pos[i].m_x + 15, node_pos[i].m_y + 15, node_pos[i].point_to_x + 15, node_pos[i].point_to_y + 15);
    }
}

void MainWindow::prepareListOfConnections()
{
    std::string tmp, nodeName;
    bool getNodeName = false;
    bool getParentName = false;

    for (unsigned i = 0; i < m_tree.size(); i++)
    {
        if (m_tree[i] == ';')
        {
            tmp = "";
            nodes_count++;
            nodeName = "";
            continue;
        }

        tmp += m_tree[i];

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
        }

        else if (!getParentName && tmp == "P:")
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
    }
}

void MainWindow::fillNodePosVector()
{
    if (nodes_count == 0)
        return;

    int actual_x = 300;
    int actual_y = 50;
    const int max_x = 600;

    node_pos = std::vector<node>(nodes_count, node(0,0,0,0,""));
    std::list<std::string> names_on_level;
    std::list<std::string> parents_on_upper_level;

    for (const auto& pair : connections)
    {
        if (pair.first == "NULL")
        {
          node_pos[0].m_x = actual_x;
          node_pos[0].m_y = actual_y;
          node_pos[0].m_name = pair.second;
          node_pos[0].point_to_x = -1;
          node_pos[0].point_to_y = -1;
          names_on_level.push_back(pair.second);
          break;
        }
    }

    int factor;
    while (node_pos[nodes_count - 1].m_name.empty())
    {
        parents_on_upper_level = names_on_level;
        names_on_level.clear();
        actual_y += 50;

        for(const auto& pair : connections)
        {
            auto findIter = std::find(parents_on_upper_level.begin(), parents_on_upper_level.end(), pair.first);
            if (findIter != parents_on_upper_level.end())
                names_on_level.push_back(pair.second);
        }

        actual_x = max_x / (names_on_level.size() + 1);
        factor = 1;

        for(const auto& name : names_on_level)
        {
            for (int j = 0; j < node_pos.size(); j++)
            {
                if (!node_pos[j].m_name.empty())
                    continue;

                node_pos[j].m_x = actual_x * factor;
                node_pos[j].m_y = actual_y;
                node_pos[j].m_name = name;
                break;
            }

            factor++;
        }
    }

    for (auto& node : node_pos)
    {
        if (node.point_to_x == -1)
            continue;

        for (const auto& pair : connections)
        {
            if (node.m_name == pair.second)
            {
                for (int i = 0; i < node_pos.size(); i++)
                {
                    if (node_pos[i].m_name == pair.first)
                    {
                        node.point_to_x = node_pos[i].m_x;
                        node.point_to_y = node_pos[i].m_y;
                        break;
                    }
                }
                break;
            }
        }
    }
}


