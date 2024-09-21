#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include<iostream>
#include "graphgraphicsview.h"
#include "algorithms.h"
#include <QPainter>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _graph = new GraphType::Graph(0,0);
    this->_scene = new GraphGraphicsScene(_graph);
    this->_view = new GraphGraphicsView();
    this->_adjMatrix = new AdjacencyMatrixTable(_graph);
    this->_incMatrix = new IncidenceMatrixTable(_graph);

    connect(_adjMatrix, SIGNAL(graphChanged()), _scene, SLOT(reload()));
    connect(_adjMatrix, SIGNAL(graphChanged()), _incMatrix, SLOT(reload()));
    connect(_scene, SIGNAL(graphChanged()), _adjMatrix, SLOT(reload()));
    connect(_scene, SIGNAL(graphChanged()), _incMatrix, SLOT(reload()));


    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setScene(_scene);

    ui->adjLayout->addWidget(this->_adjMatrix, 0, Qt::AlignCenter);
    ui->incLayout->addWidget(this->_incMatrix, 0, Qt::AlignCenter);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    _graph->addNode("");
    _scene->graphChanged();
    _scene->setGraph(_graph);
}


void MainWindow::on_pushButton_2_clicked()
{
    QString name = QInputDialog::getText(0,"Node","Enter node name:");
    if(_graph->node(name.toStdString())){
        std::string node_name = name.toStdString();
        _graph->removeNode(_graph->node(node_name));
        _scene->graphChanged();
        _scene->setGraph(_graph);
    } 
}


void MainWindow::on_pushButton_3_clicked()
{
    QString name = QInputDialog::getText(0,"Edge","Enter edges name (ex:a0-b0):");
    std::string f,s;
    f = name.toStdString().substr(0,2);
    s = name.toStdString().substr(3,name.toStdString().size());
    if(name.size() == 5){
        _graph->removeEdge(f,s);
        _scene->graphChanged();
        _scene->setGraph(_graph);
    }
}


void MainWindow::on_pushButton_5_clicked()
{
    _graph->clear();
    _scene->setGraph(_graph);
}

void MainWindow::resetGraph(Graph *graph) {
    delete this->_graph;
    this->_graph = graph;
    this->_scene->setGraph(_graph);
    this->_adjMatrix->setGraph(_graph);
    this->_incMatrix->setGraph(_graph);
}

void MainWindow::on_actionOpen_Exist_triggered()
{
    resetGraph(new Graph(Graph::readFromFile("graph.txt")));
    _scene->setGraph(_graph);
}


void MainWindow::on_actionSave_As_triggered()
{
    Graph::writeToFile("graph.txt",*_graph);
    QMessageBox::information(0,"Save","Graph successfully saved");
}


void MainWindow::on_pushButton_4_clicked()
{
    QString name = QInputDialog::getText(0,"Edge","Enter edges name (ex:a0-b0):");

    if(name.size() == 5){
        std::string startNode, goalNode;
        startNode = name.toStdString().substr(0,2);
        goalNode = name.toStdString().substr(3,name.toStdString().size());
        Algorithms::Dijkstra(_graph,startNode,goalNode);
    }
}


void MainWindow::on_pushButton_6_clicked()
{
    bool isDirected, isWeighted;
    isDirected = ui->checkBox_2->isChecked();
    isWeighted = ui->checkBox->isChecked();
    QMessageBox::information(0,"Graph","Graph is initialized.");
    resetGraph(new Graph(isDirected,isWeighted));
}


void MainWindow::on_pushButton_7_clicked()
{
    QColor col(255,0,0);
    std::list<std::pair<std::string, std::string>>::iterator it;
    auto result = Algorithms::displayColoring(_graph);
    it = result.begin();
    while(it != result.end()){
        int iColor = std::stoi(it->second) + 2;
        auto nodeItem = _scene->nodeItem(it->first);
        nodeItem->setDefaultColor(NodeGraphicsItem::colorTable()[iColor]);
        it++;
    }
    _scene->update();
}


void MainWindow::on_pushButton_9_clicked()
{

    Algorithms::randomizeGraph();
    resetGraph(new Graph(Graph::readFromFile("random_graph.txt")));
    _adjMatrix->setGraph(_graph);
    _incMatrix->setGraph(_graph);
    _scene->setGraph(_graph);

}

