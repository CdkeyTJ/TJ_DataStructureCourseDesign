// mainwindow.cpp
#include "mainwindow.h"
#include "nation.h"
#include "./ui_mainwindow.h"

#include <QSpinBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nationList = nullptr;
    rankRule = nullptr;
    initClicked = 0;
    nationInfoClicked = 0;
    eventResults={""};
}

MainWindow::~MainWindow(){
    delete ui;
    // warning
    delete[] nationList;
    delete[] sortList;
    delete[] rankRule;
}

void MainWindow::sort(int method){
    for(int i = 1; i < n; i++){
        nation* tmp = sortList[i];
        int j = i - 1;

        // 找到插入点
        while (j >= 0 && nation::compareScore(*tmp, *sortList[j], method)) {
            sortList[j + 1] = sortList[j]; // 向后移动元素
            j--;
        }

        // 插入 tmp
        sortList[j + 1] = tmp;
    }
}

void MainWindow::inputLabelDisplay(QString Prompt, bool good){
    ui->label_PromptInput->clear();
    if(good) ui->label_PromptInput->setStyleSheet("QLabel { color : green; }");
    else ui->label_PromptInput->setStyleSheet("QLabel { color : red; }");
    ui->label_PromptInput->setText(Prompt);
}

void MainWindow::outputLabelDisplay(QString Prompt, bool good){
    ui->label_PromptOutput->clear();
    if(good) ui->label_PromptOutput->setStyleSheet("QLabel { color : green; }");
    else ui->label_PromptOutput->setStyleSheet("QLabel { color : red; }");
    ui->label_PromptOutput->setText(Prompt);
}

/*---------------------------------------------------
 *
 *  I N I T   I N F O
 *
 *-------------------------------------------------*/

void MainWindow::on_pushButtonInit_clicked(){
    if(initClicked){
        inputLabelDisplay("Do not Repeat Confirm \nParameters", 0);
        return;
    }
    initClicked = 1;

    // int n, m, w;
    n = ui->spinBox_n->value();
    m = ui->spinBox_m->value();
    w = ui->spinBox_w->value();

    nationList = new nation[n];
    sortList = new nation*[n];

    for(int i=0;i<n;i++){
        nationList[i].init(i, m ,w);
        sortList[i] = &nationList[i];
    }

    // Init rank rule
    QString listMen = ui->lineEditMenRankRule->text();
    QString listWomen = ui->lineEditWomenRankRule->text();
    rankRule = new bool[m+w];

    bool wrong=0;
    if(m+w != listMen.size()+listWomen.size()){
        initClicked = 0;
        inputLabelDisplay("Rank Rule \nLength Error", 0);
        return;
    }

    // record
    for (int i=0;i<m+w;i++) {
        if(i<m){
            if('1' != listMen[i] && '0' != listMen[i]){
                wrong = 1;
                break;
            }
            if('1' == listMen[i])
                rankRule[i] = 1;
            else
                rankRule[i] = 0;
        } else{
            if('1' != listWomen[i-m] && '0' != listWomen[i-m]){
                wrong = 1;
                break;
            }
            if('1' == listWomen[i-m])
                rankRule[i] = 1;
            else
                rankRule[i] = 0;
        }
    }

    if(wrong){
        initClicked = 0;
        inputLabelDisplay("Rank Rule \nInput Error", 0);
    }
    else inputLabelDisplay("Init Success", 1);
}

// input nation info
void MainWindow::on_pushButtonNationInfo_clicked(){
    // block input info request
    if(nationInfoClicked){
        inputLabelDisplay("Do not Repeat Confirm \nnation info", 0);
        return;
    } else if(!initClicked){
        inputLabelDisplay("Should Init Game First", 0);
        return;
    }

    nationInfoClicked = 1;

    eventResults = ui->textEditEventResults->toPlainText().split("\n", Qt::SkipEmptyParts);

    // Iterate over each event
    for (int i = 0; i < m+w; ++i) {
        QStringList eachLine = eventResults[i].split(" ", Qt::SkipEmptyParts);


        for (int j = 0; j < eachLine.size(); ++j) {
            if((rankRule[i] && eachLine.size() != 5) || (!rankRule[i] && eachLine.size() != 3)){
                inputLabelDisplay("Incorrespond to Rank Rule Input", 0);
                nationInfoClicked = 0;
                return;
            }
            bool OK;
            int ID = eachLine[j].toInt(&OK) - 1;
            if(!OK){
                inputLabelDisplay("Invalid Letter Exist \nOnly Numbers Allowed", 0);
                nationInfoClicked = 0;
                return;
            }
            // the nation ID have rank j+1 in i th project
            nationList[ID].setRank(i, j + 1);
        }
    }

    for(int i=0;i<n;i++) nationList[i].calcScore(rankRule);


    inputLabelDisplay("Competition Info Success \nYou can Turn to Output Window", 1);
}

/*---------------------------------------------------
 *
 *  D I S P L A Y   C O U N T R Y   I N F O
 *
 *-------------------------------------------------*/

void MainWindow::setRankSpecialEffect(QTableWidgetItem*& idItem, int projectID, int rank){
    QColor medalColor = medal[rankRule[projectID]][rank];
    idItem->setForeground(medalColor);
    // idItem->setBackground(medalColor);
    if(Black == medalColor) return;

    QFont font = idItem->font();
    font.setBold(true);
    idItem->setFont(font);
}

void MainWindow::on_pushButtonDisplayNation_clicked(){
    // Clear the table
    ui->tableWidget->clear();

    // Illegal action forbidden
    if(!initClicked || !nationInfoClicked){
        outputLabelDisplay("No Input Init \ncannot Functioning", 0);
        return;
    }

    int N = ui->spinBox_nSelect->value();
    // Illegal input
    if(N > n){
        outputLabelDisplay("Wrong Nation ID \nPlease Check Your Input", 0);
        return;
    }

    // Init table
    ui->tableWidget->setRowCount(1);

    // Header display
    ui->tableWidget->setColumnCount(4+m+w);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Total Score", "Men Score", "Women Score"});

    for(int i=1;i<=m+w;i++){
        QString headerText;
        if(i<=m) headerText = QString("Men's %1").arg(i);
        else     headerText = QString("Women's %1").arg(i-m);
        ui->tableWidget->setHorizontalHeaderItem(i+3, new QTableWidgetItem(headerText));
    }

    // Item display
    QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(nationList[N-1].getID()));
    QTableWidgetItem *totalScoreItem = new QTableWidgetItem(QString::number(nationList[N-1].getTotalScore()));
    QTableWidgetItem *menScoreItem = new QTableWidgetItem(QString::number(nationList[N-1].getMenScore()));
    QTableWidgetItem *womenScoreItem = new QTableWidgetItem(QString::number(nationList[N-1].getWomenScore()));
    ui->tableWidget->setItem(0, 0, idItem);
    ui->tableWidget->setItem(0, 1, totalScoreItem);
    ui->tableWidget->setItem(0, 2, menScoreItem);
    ui->tableWidget->setItem(0, 3, womenScoreItem);

    // Output Rank
    for(int i=0;i<m+w;i++){
        int rank = nationList[N-1].getRank(i);
        QTableWidgetItem *projectItem = new QTableWidgetItem(QString::number(rank) + " +("+QString::number(RANKRULE[rankRule[i]][rank]) +")");
        if(medal[rankRule[i]][rank] != Qt::black)qDebug()<<rankRule[i]<<rank;

        setRankSpecialEffect(projectItem, i, rank);
        ui->tableWidget->setItem(0, i+4, projectItem);
    }

    outputLabelDisplay("Display Nation Success", 1);

    // TODO: delete table window
    // 通常在 QTableWidget 中，直接使用 new 是安全的。
}

void MainWindow::on_pushButtonDisplayProject_clicked(){
    ui->tableWidget->clear();
    // Illegal action forbidden
    if(!initClicked || !nationInfoClicked){
        outputLabelDisplay("No Input Init \ncannot Functioning", 0);
        return;
    }

    int P = ui->spinBox_pSelect->value();
    // Illegal input value
    if(P > m+w){
        outputLabelDisplay("Wrong Project ID \nPlease Check Your Input", 0);
        return;
    }

    QStringList eachLine = eventResults[P-1].split(" ", Qt::SkipEmptyParts);

    // Header display
    ui->tableWidget->setRowCount(1);
    if(rankRule[P-1]){
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setHorizontalHeaderLabels({"Project ID", "First +7", "Second +5", "Third +3", "Fourth +2", "Fifth +1"});
    }
    else {
        ui->tableWidget->setColumnCount(4);
        ui->tableWidget->setHorizontalHeaderLabels({"Project ID", "First +5", "Second +3", "Third +2"});
    }

    // Item display
    QTableWidgetItem *ProjectID;
    if(P<=m) ProjectID = new QTableWidgetItem("Men's "+QString::number(P));
    else ProjectID = new QTableWidgetItem("Women's "+QString::number(P-m));
    ui->tableWidget->setItem(0,0,ProjectID);
    for (int i = 0; i < eachLine.size(); ++i) {
        QTableWidgetItem *nationID = new QTableWidgetItem(eachLine[i]);

        setRankSpecialEffect(nationID, P-1, i+1);
        ui->tableWidget->setItem(0,i+1, nationID);
    }

    outputLabelDisplay("Display Project Success", 1);
}

void MainWindow::on_pushButtonDisplayRank_clicked(){
    ui->tableWidget->clear();
    // Illegal action forbidden
    if(!initClicked || !nationInfoClicked){
        outputLabelDisplay("No Input Init \ncannot Functioning", 0);
        return;
    }

    int Mode = ui->spinBox_modeSelect->value();
    sort(Mode);

    // Init table
    ui->tableWidget->setRowCount(n);

    // Header display
    ui->tableWidget->setColumnCount(4+m+w);
    ui->tableWidget->setHorizontalHeaderLabels({"Nation ID", "Total Score", "Men Score", "Women Score"});

    for(int i=1;i<=m+w;i++){
        QString headerText;
        if(i<=m) headerText = QString("Men's %1").arg(i);
        else     headerText = QString("Women's %1").arg(i-m);
        ui->tableWidget->setHorizontalHeaderItem(i+3, new QTableWidgetItem(headerText));
    }

    for(int N = 0;N<n;N++){
        // Item display
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(sortList[N]->getID()));
        QTableWidgetItem *totalScoreItem = new QTableWidgetItem(QString::number(sortList[N]->getTotalScore()));
        QTableWidgetItem *menScoreItem = new QTableWidgetItem(QString::number(sortList[N]->getMenScore()));
        QTableWidgetItem *womenScoreItem = new QTableWidgetItem(QString::number(sortList[N]->getWomenScore()));
        ui->tableWidget->setItem(N, 0, idItem);
        ui->tableWidget->setItem(N, 1, totalScoreItem);
        ui->tableWidget->setItem(N, 2, menScoreItem);
        ui->tableWidget->setItem(N, 3, womenScoreItem);

        // Output Rank
        for(int i=0;i<m+w;i++){
            int rank = sortList[N]->getRank(i);
            QTableWidgetItem *projectItem = new QTableWidgetItem(QString::number(rank) + " +("+QString::number(RANKRULE[rankRule[i]][rank]) +")");

            setRankSpecialEffect(projectItem, i, rank);
            ui->tableWidget->setItem(N, i+4, projectItem);
        }
    }

    outputLabelDisplay("Display Rank Success", 1);
}
