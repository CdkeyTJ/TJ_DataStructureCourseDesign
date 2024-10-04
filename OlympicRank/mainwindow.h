// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "nation.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

const QColor Black=Qt::gray;
const QColor Purple(128, 0, 128);
const QColor Golden(255, 215, 0);
const QColor Blue=Qt::blue;
const QColor Copper(184, 115, 51);
const QColor Iron(60, 60, 60);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void sort(int method);
    void inputLabelDisplay(QString Prompt, bool good);
    void outputLabelDisplay(QString Prompt, bool good);
    void setRankSpecialEffect(QTableWidgetItem*& idItem, int projectID, int rank);

private slots:
    void on_pushButtonInit_clicked();
    void on_pushButtonNationInfo_clicked();
    void on_pushButtonDisplayNation_clicked();
    void on_pushButtonDisplayProject_clicked();
    void on_pushButtonDisplayRank_clicked();

private:
    Ui::MainWindow *ui;

    //                           0  1  2  3  4  5
    const int RANKRULE[2][6]  = {0, 5, 3, 2, 0, 0,
                                 0, 7, 5, 3, 2, 1};


    //                  0      1       2       3       4       5
    QColor medal[2][6]={Black, Golden, Blue,   Copper, Black,  Black,
                        Black, Purple, Golden, Blue,   Copper, Iron};

    // i th project's rule in score
    bool* rankRule;
    int n;
    int m;
    int w;

    nation* nationList;
    nation** sortList;
    bool initClicked;
    bool nationInfoClicked;

    QStringList eventResults;
};
#endif // MAINWINDOW_H
