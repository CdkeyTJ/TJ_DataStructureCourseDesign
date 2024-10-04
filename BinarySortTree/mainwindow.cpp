// mainwindow.cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"

/**
 *
 *  WARNING : LABEL ERROR , RELEASE ERROR ：
 *  无法定位程序输入点?changeEvent@QLabel@@MEAAXPEAVQEvent@@@Z
 *  于动态链接库D:\wx\OneDrivel桌面\BSTBSTBinarySortTree.exe 上.
 *
 *  解决方案：
 *  配置文件时地址是MSVC2019运行环境，使用该环境会导致上述报错出现
 *  更改为MinGW运行环境即可规避上述报错
 *
 *  编译器和标准库不兼容：


 *
**/

// valid input check
#include <QIntValidator>

// pause
#include <QEventLoop>
#include <QTimer>
#include <QDebug>

// visualize
#include <QGraphicsEllipseItem>
// #include <QGraphicsTextItem>
#include <QFont>

// construct
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // use enter to have push button effect
    QObject::connect(ui->lineEditInsert, &QLineEdit::returnPressed,
                     ui->pushButtonInsert, &QPushButton::click);
    QObject::connect(ui->lineEditSearch, &QLineEdit::returnPressed,
                     ui->pushButtonSearch, &QPushButton::click);
    QObject::connect(ui->lineEditRemove, &QLineEdit::returnPressed,
                     ui->pushButtonRemove, &QPushButton::click);
    QObject::connect(ui->lineEditBatchInsert, &QLineEdit::returnPressed,
                     this, &MainWindow::onReturnPressed);

    // set scene into QGraphicsView
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

// deconstruct
MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::labelDisplay(QString prompt){
    ui->label->clear();
    ui->label->setText(prompt);
}

void pause(int milliseconds)
{
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}

/*---------------------------------------------------
 *
 *  D R A W   I N   T R E E W I D G E T
 *
 *-------------------------------------------------*/

template<typename T>
void MainWindow::displayTree(TreeNode<T>* root, QTreeWidgetItem *pa){
    if(!root) return;
    QTreeWidgetItem * cur = new QTreeWidgetItem();
    cur->setText(0, QString::number(root->val));

    if(!pa){
        MainWindow::WTroot = cur;
        ui->treeWidget->addTopLevelItem(cur);
    } else
        pa->addChild(cur);

    displayTree(root->left, cur);
    displayTree(root->right, cur);
    cur->setExpanded(true);
}

// delete TreeItem
void MainWindow::deleteNode(QTreeWidgetItem * cur){
    if(!cur) return;
    while(cur->childCount()){
        deleteNode(cur->child(0));
    }
    delete cur;
}

void MainWindow::clearTree(){
    deleteNode(MainWindow::WTroot);
    WTroot = nullptr;
}

/*---------------------------------------------------
 *
 *  D R A W   I N   G R A P H I C   V I E W
 *
 *-------------------------------------------------*/

// x,y represent the centre coord
void MainWindow::drawNode(int x, int y, QString value, bool special) {
    // draw node according to search mode
    if(special)
        scene->addEllipse(x - ellipseWidth/2, y - ellipseHeight/2,
                          ellipseWidth, ellipseHeight,
                          QPen(Qt::black), QBrush(Qt::yellow));
    else
        scene->addEllipse(x - ellipseWidth/2, y - ellipseHeight/2,
                          ellipseWidth, ellipseHeight,
                          QPen(Qt::black), QBrush(Qt::white));

    // draw node text
    QGraphicsTextItem* text = scene->addText(value);
    QFont font = text->font();
    font.setPointSize(fontSize); // set font size
    text->setFont(font);
    text->setPos(x - text->boundingRect().width() / 2,
                 y - text->boundingRect().height() / 2);
}

template<typename T>
void MainWindow::updateTree(TreeNode<T>* root, int x, int y, int hSpacing) {
    if (!root) return;

    // draw current node
    drawNode(x, y, QString::number(root->val));

    int childy = y + ellipseHeight*2;
    // painter recursively
    if (root->left) {
        // draw left line
        int leftx = x - hSpacing - ellipseWidth/2;

        scene->addLine(x, y + ellipseHeight/2, leftx, childy, QPen(Qt::black));
        updateTree(root->left, leftx, childy, hSpacing / 2);
    }
    if (root->right) {
        // draw right line
        int rightx = x + hSpacing + ellipseWidth/2;
        scene->addLine(x, y + ellipseHeight/2, rightx, childy, QPen(Qt::black));
        updateTree(root->right, rightx, childy, hSpacing / 2);
    }
}

template<typename T>
bool MainWindow::displayNode(TreeNode<T> *root, int x, int y, int hSpacing, T& val){
    if (!root) return 0;

    // abs(root->val-val)<1e-9
    if(root->val == val){
        drawNode(x, y, QString::number(root->val), 1);
        return 1;
    }

    int childy = y + ellipseHeight*2;
    if (val < root->val) {
        int leftx = x - hSpacing - ellipseWidth/2;
        return displayNode(root->left, leftx, childy, hSpacing / 2, val);
    }
    if (val > root->val) {
        int rightx = x + hSpacing + ellipseWidth/2;
        return displayNode(root->right, rightx, childy, hSpacing / 2, val);
    }
    return 0;
}

/*---------------------------------------------------
 *
 *  A C T I O N S
 *
 *-------------------------------------------------*/

void MainWindow::on_pushButtonInsert_clicked()
{
    QString InputText = ui->lineEditInsert->text();

    bool ok;
    // try to converse
    double number = InputText.toDouble(&ok);

    if (ok) {
        if(Tree.search(number)){
            labelDisplay("二叉排序树已存在该数字");
            return;
        }
        Tree.insert(number);

        // clear graphic
        scene->clear();
        // repaint tree
        updateTree(Tree.getRoot(), width() / 2, 50, initialSpacing);

        //
        clearTree();
        displayTree(Tree.getRoot(), nullptr);

        labelDisplay("插入成功！");

    } else
        labelDisplay("请输入有效的数字！");

    ui->lineEditInsert->clear();
}

void MainWindow::on_pushButtonRemove_clicked()
{
    QString InputText = ui->lineEditRemove->text();

    bool ok;
    double number = InputText.toDouble(&ok);

    if (ok) {
        if(!Tree.search(number)){
            labelDisplay("二叉排序树中无该数字");
            return;
        }
        Tree.remove(number);

        // clear graphic
        scene->clear();
        // repaint tree
        updateTree(Tree.getRoot(), width() / 2, 50, initialSpacing);

        clearTree();
        displayTree(Tree.getRoot(), nullptr);
        labelDisplay("删除成功！");
    } else
        labelDisplay("请输入有效的数字！");

    ui->lineEditRemove->clear();
}

void MainWindow::on_pushButtonSearch_clicked()
{
    QString InputText = ui->lineEditSearch->text();

    bool ok;
    double number = InputText.toDouble(&ok);

    if (ok) {
        if(!displayNode(Tree.getRoot(),  width() / 2, 50, initialSpacing, number))
            labelDisplay("二叉排序树中无该数字");
        else{
            labelDisplay("存在数字"+InputText);
            pause(1000);
            // repaint tree to recover
            updateTree(Tree.getRoot(), width() / 2, 50, initialSpacing);
        }

    }
    else
        labelDisplay("请输入有效的数字！");

    ui->lineEditSearch->clear();
}

void MainWindow::onReturnPressed(){
    QString inputText = ui->lineEditBatchInsert->text();
    // seperate text by space
    QStringList dataList = inputText.split(" ", Qt::SkipEmptyParts);

    for (const QString &data : dataList) {
        bool OK;
        double number = data.toDouble(&OK);
        if(!OK){
            labelDisplay("输入非法");
            on_pushButtonClear_clicked();
            return;
        }
        if(Tree.search(number)){
            labelDisplay("二叉排序树已存在该数字");
            return;
        }
        Tree.insert(number);
    }

    clearTree();
    displayTree(Tree.getRoot(), nullptr);

    // clear graphic
    scene->clear();
    // repaint tree
    updateTree(Tree.getRoot(), width() / 2, 50, initialSpacing);

    labelDisplay("插入成功！");
    ui->lineEditBatchInsert->clear();
}

void MainWindow::on_pushButtonClear_clicked()
{
    Tree.deleteTree();
    clearTree();
    scene->clear();
}
