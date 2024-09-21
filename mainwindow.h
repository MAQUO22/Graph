#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "graph.h"
#include "GraphGraphicsScene.h"
#include "graphgraphicsview.h"
#include "propmatrix.h"
#include "incmatrix.h"
#include "adjmatrix.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_actionSave_As_triggered();

    void on_actionOpen_Exist_triggered();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

signals:
    void graphChanged();

private:
    Ui::MainWindow *ui;
    GraphGraphicsScene *_scene;
    GraphType::Graph *_graph;
    AdjacencyMatrixTable *_adjMatrix;
    GraphGraphicsView *_view;
    QTableWidget *_table;
    IncidenceMatrixTable *_incMatrix;
    ElementPropertiesTable *_propTable;

    void resetGraph(GraphType::Graph *graph);
};
#endif // MAINWINDOW_H
