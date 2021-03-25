#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QDebug"
#include "QString"
#include <QObject>
#include "QPushButton"
#include "QSignalMapper"
#include "QPixmap"
#include "QIcon"
#include <iostream>
#include <ctime>
#include <string>
#include <QTimer>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


using namespace std;

class MyButton : public QPushButton {
private:
  QPixmap m_pixmap;
public:
  explicit MyButton(QWidget* parent = nullptr) : QPushButton(parent) {}
  virtual ~MyButton() {}

  void setPixmap(const QPixmap& pixmap) { m_pixmap = pixmap; }

  /*virtual QSize sizeHint() const override {
            const auto parentHint = QPushButton::sizeHint();
        return QSize(parentHint.width() + m_pixmap.width(),max(parentHint.height(), m_pixmap.height()));
  }*/

protected:
  virtual void paintEvent(QPaintEvent* e) override {
    QPushButton::paintEvent(e);

    if (!m_pixmap.isNull()) {
      const int y = (height() - m_pixmap.height()) / 2;
      QPainter painter(this);
      painter.drawPixmap(2, y, m_pixmap);
    }
  }
};

class Field{
    int value;
    int isRevealted;

public: 
    MyButton* button;
    Field(): value(0),isRevealted(0){}
    void setV(int value){this->value=value;}
    int getV(){return value;}
    int getR(){return isRevealted;}
    void increase(){value++;}
    void setR(int status){isRevealted=status;} //0-not revealted 1-revealted 2-flag
};

class MainWindow : public QMainWindow, public Field
{
    Q_OBJECT
    QString name;
    QSignalMapper *mapper;
    int w,k,b;
    static int qf,qr;
    Field **board;
    QTimer *timer;
    static int minutes,second;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setName(QString name){this->name=name;}
    void setWK(int w,int k){this->w=w;this->k=k;}
    int getF(){return qf;}
    void increaseF(){qf++;}
    void decreaseF(){qf--;}
    void increaseR(){qr++;}
    static void time0(){minutes=0;second=0;}

    friend void allocation(Field **&board, int w, int k);
    friend void delete_Board(Field **&board,int w);
    void fillMines(Field **&board, int x, int y);
    void discover(Field **&board, int x, int y);
    void randomMines(Field **&board);
    void checkWin(Field **board);
    void showValue(Field **board);

private slots:
    void on_start_clicked();
    void slot_button_clicked(const QString &str);
    void on_check_clicked();
    void stoper();

    void on_exit_clicked();

    void on_back_clicked();

    void on_sad_clicked();

    void on_bad_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
