// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QGraphicsScene>
#include "binarysorttree.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void labelDisplay(QString prompt);

    // TreeWidget
    template<typename T>
    void displayTree(TreeNode<T>* root, QTreeWidgetItem *pa);
    void clearTree();
    void deleteNode(QTreeWidgetItem* cur);

    // Graphic
    template<typename T>
    void updateTree(TreeNode<T> *root, int x, int y, int hSpacing);
    void drawNode(int x, int y, QString value, bool special = 0);
    template<typename T>
    bool displayNode(TreeNode<T> *root, int x, int y, int hSpacing, T& val);


private slots:
    // actions
    void on_pushButtonInsert_clicked();
    void on_pushButtonRemove_clicked();
    void on_pushButtonSearch_clicked();
    void onReturnPressed();
    void on_pushButtonClear_clicked();

private:
    Ui::MainWindow *ui;

    // TreeWidget root
    QTreeWidgetItem *WTroot=nullptr;

    // Graphic
    QGraphicsScene* scene;

    // Root of tree, asserted as double
    BinarySortTree<double> Tree;

    // Graphic configurations
    int startX = 50;
    int startY = 0;
    const int ellipseWidth=30;
    const int ellipseHeight=15;
    const int initialSpacing = 90;
    const int fontSize = 10;
};

#endif // MAINWINDOW_H

