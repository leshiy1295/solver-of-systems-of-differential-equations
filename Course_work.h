#ifndef COURSE_WORK_H
#define COURSE_WORK_H
#include <QtGui>
#include <QFile>
#include <QString>
#include <QPainter>
#include "Solver.h"
class HelpWindow;
class MainWindow;
class PeremWindow;

class MyWindow:public QWidget
{
	Q_OBJECT
public:
	MyWindow(QWidget *parent=0);
	HelpWindow *help;
	MainWindow *main;
	~MyWindow();
private slots:
	void myActivate_newWindow();
	void myActivate_helpWindow();
};
class Area:public QFrame
{ 
public:
	Area(QWidget *parent,Solver **source,MainWindow *win);
private:
	Solver **solver;
	MainWindow *main_window;
protected:
	virtual void paintEvent(QPaintEvent *event);
};

class MainWindow:public QWidget
{
	Q_OBJECT
	void makeForm();
	int temp_n;
	char temp_a[150], temp_b[150], temp_x0[150];
	double *temp_Koshi;
	Solver *solver;
	PeremWindow *perems;
	Area *graph;
public:
	bool opened;
	int metod;
	char temp_x[150];
	int temp_razr;
	MyWindow *back;
	bool cancel;
	MainWindow(QWidget *parent);
private slots:
	void closeWin();
	void go_to_step4();
	void go_to_step5();
	void perems_win();
	void solve();
	void save_results();
};

class PeremWindow:public QWidget
{
	Q_OBJECT
	QSignalMapper *signalMapperChange,*signalMapperDelete,*signalMapperEdit,*signalMapperEditCancel;
	void getResults();
	void deleteAllPerems();
public:
	~PeremWindow();
	PeremWindow(QWidget *parent);
	MainWindow *back;
private slots:
	void closeWin();
	void addPerem();
	void changePerem(int);
	void deletePerem(int);
	void saveEditPerem(int);
	void cancelEditPerem(int);
};
class HelpWindow:public QWidget
{
	Q_OBJECT
public:
	MyWindow *back;
	HelpWindow(QWidget *parent);
private slots:
	void closeWin();
};
#endif