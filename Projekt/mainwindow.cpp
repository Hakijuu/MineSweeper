#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    mapper=new QSignalMapper(this);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(stoper()));
    ui->exit->setVisible(false);
    ui->back->setVisible(false);
    ui->kom1->setVisible(false);
    ui->kom2->setVisible(false);
    ui->sad->setVisible(false);
    ui->bad->setVisible(false);
}

int MainWindow::qf=0;
int MainWindow::qr=0;
int MainWindow::minutes=0;
int MainWindow::second=0;

void allocation(Field **&board, int w, int k){
    board=new Field*[w];
    for(int i=0;i<w;i++)
        board[i]= new Field[k];
}

void delete_Board(Field **&board, int w, int k){
    MainWindow::time0();
    for(int i=0;i<w;i++)
        for(int j=0;j<k;j++)
            board[i][j].button->deleteLater();

    for(int i=0;i<w;i++)
    delete board[i];

}

void MainWindow::checkWin(Field **board){
    int temp=0;

    if(qr==(w*k)-b){
        for(int i=0;i<w;i++)
            for(int j=0;j<k;j++){
                if((board[i][j].getR()==2 || board[i][j].getR()==0 )&& board[i][j].getV()==9)
                    temp++;
            }
       if(temp==b) {
           ui->sad->setVisible(true);
           ui->kom2->setVisible(true);
           ui->check->setVisible(false);
           ui->sad->raise();
           for (int i=0; i<w; i++)
           for (int j=0;j<k;j++){
               board[i][j].button->setEnabled(false);
               board[i][j].button->setDefault(false);
              }
           timer->stop();
           QString time;
           if(minutes<10) time="0"+QString::number(minutes)+":";
               else time=QString::number(minutes)+":";
           if(second<10) time=time+"0"+QString::number(second);
               else time=time+QString::number(second);
           ui->lineScore->setText(time);
                }
        else {
           showValue(board);}

    } else return;
}

void MainWindow::showValue(Field **board)
{   QPixmap pixmapB(":/bombaZm.png");
    ui->bad->setVisible(true);
    ui->bad->raise();
    for (int i=0; i<w; i++)
    for (int j=0;j<k;j++){
        board[i][j].button->setEnabled(false);
        board[i][j].button->setDefault(false);
        if(board[i][j].getV()==9)
            board[i][j].button->setIcon(pixmapB);
            else if(board[i][j].getV()==0) board[i][j].button->setIcon(QIcon());
                else board[i][j].button->setText(QString::number(board[i][j].getV()));
       board[i][j].button->show();
       }
    ui->check->setVisible(false);
    ui->qmines->setVisible(false);
    ui->lineTimer->setVisible(false);
    ui->exit->setVisible(true);
    ui->back->setVisible(true);
    ui->kom1->setVisible(true);
}

void MainWindow::fillMines(Field **&board, int x, int y){
    if(board[x][y].getV()!=9){
        board[x][y].setV(9);

        for (int i = -1; i<2; i++)
            for (int j = -1; j<2; j++){
                if ((x+j)<0 || (y+i)<0 ) continue;
                if ((x+j)>w-1 || (y+i)>k-1 ) continue;

                if (board[x+j][y+i].getV()==9) continue;
                board[x+j][y+i].increase();
            }
    }
}

void MainWindow::randomMines(Field **&board){
    int temp=b;
    int x,y;
    while(temp>0){
        x=rand()%w;
        y=rand()%k;
        if (board[x][y].getV()!=9){
            fillMines(board,x,y);
            temp--;
      }
    }
}

void MainWindow::discover(Field **&board,int x, int y){
    if (x<0 || x>w-1) return;
    if (y<0 || y>k-1) return;
    if (board[x][y].getR()==1) return;

    if(board[x][y].getV()!=9 && board[x][y].getR()==0){
            board[x][y].setR(1);
            if(board[x][y].getV()==0) {
                board[x][y].button->setDefault(true);

            }
            else {
                board[x][y].button->setText(QString::number(board[x][y].getV()));
                board[x][y].button->setDefault(true);
            }
            increaseR();
    }
    if(board[x][y].getV()!=0) return;
    discover(board,x-1,y-1);
    discover(board,x-1,y);
    discover(board,x-1,y+1);
    discover(board,x+1,y-1);
    discover(board,x+1,y);
    discover(board,x+1,y+1);
    discover(board,x,y-1);
    discover(board,x,y);
    discover(board,x,y+1);

}

void MainWindow::on_start_clicked()
{   minutes=0, second=0;
    qf=0,qr=0;
    timer->start(1000);
    ui->wi->setCurrentIndex(1);
    int index=ui->comboLevel->currentIndex();
    name=ui->name1->text();
    ui->lineName->setText(name);
    const int size =31;
        switch(index){
        case 0:
                w=8;
                k=8;
                b=10;
                allocation(board,w,k);
                for (int i=0; i<w; i++)
                for (int j=0;j<k;j++){
                   board[i][j].button = new MyButton(ui->page_2);
                   board[i][j].setV(0);
                   board[i][j].setR(0);

                   board[i][j].button->move(30*j+380,27*i+300);
                   board[i][j].button->setMaximumSize(size,size);
                   board[i][j].button->show();
                   QString str=QString::number(i)+" "+QString::number(j);
                   connect(board[i][j].button, SIGNAL(clicked(bool)),mapper,SLOT(map()));
                   mapper->setMapping(board[i][j].button,str);
                   connect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
                   }
            break;
        case 1:
                w=16;
                k=16;
                b=40;
                allocation(board,w,k);
                for (int i=0; i<w; i++)
                for (int j=0;j<k;j++){
                   board[i][j].button = new MyButton(ui->page_2);
                   board[i][j].setV(0);
                   board[i][j].setR(0);

                   board[i][j].button->move(30*j+270,27*i+200);
                   board[i][j].button->setMaximumSize(size,size);
                   board[i][j].button->show();
                   QString str=QString::number(i)+" "+QString::number(j);
                   connect(board[i][j].button, SIGNAL(clicked(bool)),mapper,SLOT(map()));
                   mapper->setMapping(board[i][j].button,str);
                   connect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
                   }
            break;
        case 2:
            w=16;
            k=30;
            b=99;
            allocation(board,w,k);
            for (int i=0; i<w; i++)
            for (int j=0;j<k;j++){
               board[i][j].button = new MyButton(ui->page_2);
               board[i][j].setV(0);
               board[i][j].setR(0);

               board[i][j].button->move(30*j+42,27*i+200);
               board[i][j].button->setMaximumSize(size,size);
               board[i][j].button->show();
               QString str=QString::number(i)+" "+QString::number(j);
               connect(board[i][j].button, SIGNAL(clicked(bool)),mapper,SLOT(map()));
               mapper->setMapping(board[i][j].button,str);
               connect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
               }
        break;
        }
        randomMines(board);
}

void MainWindow::slot_button_clicked(const QString &str){

    QStringList lst = str.split(" ");
    int i = lst[0].toInt();
    int j = lst[1].toInt();

   QPixmap pixmap(":/flag1.png");
   QString r=QString::number(b-getF());
   if(ui->check->isChecked()) {
       if(board[i][j].getR()==0 && qf<b){
           board[i][j].button->setPixmap(pixmap);
           board[i][j].button->sizeHint();
           board[i][j].setR(2);
           increaseF();

           disconnect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
           connect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
       } else if(board[i][j].getR()==2){
           board[i][j].button->setPixmap(QPixmap());
           board[i][j].setR(0);
           decreaseF();
           disconnect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
           connect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
       }
       checkWin(board);
   }
    else if(ui->check->isChecked()==false && board[i][j].getR()==0){
       if(board[i][j].getV()==9) {
           showValue(board);
           ui->lineName->setText(name);
           disconnect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
           connect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
       }
         else {
            discover(board,i,j);
            checkWin(board);
            disconnect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
            connect(mapper,SIGNAL(mapped(QString)),this,SLOT(slot_button_clicked(QString)));
            }
    }
}

void MainWindow::on_check_clicked()
{
    if(ui->check->isChecked()){
        QPixmap pixmap(":/flag.png");
        ui->status->setPixmap(pixmap);
    }
    else ui->status->setPixmap(QPixmap());
}

void MainWindow::stoper()
{   QString time;
    QString r=QString::number(b-getF());
    ui->qmines->setText(r);
    if(second==59){
        minutes++;
        second=0;
    }
    if(minutes<10) time="0"+QString::number(minutes)+":";
        else time=QString::number(minutes)+":";
    if(second<10) time=time+"0"+QString::number(second);
        else time=time+QString::number(second);
    ui->lineTimer->setText(time);
    second++;
}


MainWindow::~MainWindow()
{
    delete_Board(board,w,k);
    delete ui;

}


void MainWindow::on_exit_clicked()
{
    close();
}

void MainWindow::on_back_clicked()
{   delete_Board(board,w,k);
    ui->wi->widgetRemoved(1);
    ui->exit->setVisible(false);
    ui->back->setVisible(false);
    ui->kom1->setVisible(false);
    ui->sad->setVisible(false);
    ui->bad->setVisible(false);
    ui->check->setVisible(true);
    ui->qmines->setVisible(true);
    ui->lineTimer->setVisible(true);
    ui->wi->setCurrentIndex(0);
}

void MainWindow::on_sad_clicked()
{
    ui->wi->setCurrentIndex(2);
}

void MainWindow::on_bad_clicked()
{
    close();
}
