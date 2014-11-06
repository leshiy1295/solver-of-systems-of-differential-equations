#include "Course_work.h"
QTextCodec *codec=QTextCodec::codecForName("Windows-1251");
MyWindow::MyWindow(QWidget *parent):QWidget(parent)
{
	main=NULL;
	help=NULL;
	log_file=fopen("log.txt","w+");
	this->setFixedSize(this->minimumSize());
	this->setWindowTitle(codec->toUnicode("Курсовая работа"));
	QVBoxLayout *vlayout=new QVBoxLayout(this);
	QLabel *title=new QLabel(codec->toUnicode("ПРОГРАММА РЕШЕНИЯ СИСТЕМ ДИФФЕРЕНЦИАЛЬНЫХ УРАВНЕНИЙ В НОРМАЛЬНОЙ ФОРМЕ"));
	QPushButton *exit_button,*new_button,*open_button,*help_button;
	exit_button=new QPushButton(codec->toUnicode("Выйти из программы"));
	new_button=new QPushButton(codec->toUnicode("Новый анализ"));
	help_button=new QPushButton(codec->toUnicode("Открыть справку"));
	vlayout->addWidget(title);
	vlayout->addWidget(new_button);
	vlayout->addWidget(help_button);
	vlayout->addWidget(exit_button);
	connect(new_button,SIGNAL(clicked()),this,SLOT(myActivate_newWindow()));
	connect(help_button,SIGNAL(clicked()),this,SLOT(myActivate_helpWindow()));
	connect(exit_button,SIGNAL(clicked()),this,SLOT(close()));
}
MyWindow::~MyWindow()
{
	fclose(log_file);
}
void MyWindow::myActivate_newWindow()
{
	this->hide();
	main=new MainWindow(0);
	if (!(main->cancel))
	{
		main->back=this;
		main->show();
		main->setFocus();
	}
	else
	{
		delete main;
		this->show();
		this->setFocus();
	}
}
void MyWindow::myActivate_helpWindow()
{
	this->hide();
	help=new HelpWindow(0);
	help->back=this;
	help->show();
	help->setFocus();
}
MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
	// Ввод параметров 
	opened=false;
	bool bOk;
	char s[150],*ukaz=NULL,buffer[150];
	temp_Koshi=NULL;
	cancel=false;
	solver=NULL;
	temp_a[0]='\0'; 
	temp_b[0]='\0';
	temp_x0[0]='\0';
	temp_x[0]='\0';
	temp_razr=0;
	temp_n=0;
	int i;
	QString str;
	str=QInputDialog::getText(0,codec->toUnicode("Шаг 1."),codec->toUnicode("Введите разрядность системы"),QLineEdit::Normal,codec->toUnicode(""),&bOk);
	if (!bOk)
		cancel=true;
	else
	{
		s[0]='\0';
		strcat(s,str.toStdString().c_str());
		i=atoi(s);
		bOk=VYRAZ::provDopInt(s,&ukaz);
		fflush(log_file);
		buffer[0]='\0';
		strcat(buffer,ukaz);
		while ((!cancel) && ((!bOk) || (i<=0)))
		{
			if (!bOk)
				QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buffer));	
			else
				QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode("Некорректные данные!!! В системе не может быть меньше одного уравнения!!"));
			str=QInputDialog::getText(0,codec->toUnicode("Шаг 1."),codec->toUnicode("Введите разрядность системы"),QLineEdit::Normal,codec->toUnicode(""),&bOk);
			if (!bOk)
				cancel=true;
			else
			{
				s[0]='\0';
				strcat(s,str.toStdString().c_str());
				i=atoi(s);
				bOk=VYRAZ::provDopInt(s,&ukaz);
				fflush(log_file);
				buffer[0]='\0';
				strcat(buffer,ukaz);
			}
		}
		if (!cancel)
		{
			temp_razr=i;
			QStringList items;
			items<<codec->toUnicode("Явный метод Эйлера")<<codec->toUnicode("Метод Рунге-Кутта II порядка")<<codec->toUnicode("Метод Рунге-Кутта IV порядка");
			str=QInputDialog::getItem(0,codec->toUnicode("Шаг 2."),codec->toUnicode("Выберите метод решения системы"),items,0,false,&bOk);
			if (!bOk)
				cancel=true;
			else
			{
				if (str==items[0])
				{
					metod=1;
					do 
					{
						fflush(log_file);
						str=QInputDialog::getText(0,codec->toUnicode("Шаг 3."),codec->toUnicode("Введите имя переменной, по которой ведётся интегрирование"),QLineEdit::Normal,codec->toUnicode(""),&bOk);	
						if (!bOk)
							cancel=true;
						else
						{
							temp_x[0]='\0';
							strcat(temp_x,str.toStdString().c_str());
							bOk=(VYRAZ::PeremTestName(temp_x,&ukaz)) && (Eyler::provX(temp_x,temp_razr,&ukaz));
							if (!bOk)
								QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(ukaz));
						}
					}while ((!bOk) && (!cancel));
					// После этого необходимо вывести окно для ввода уравнений системы
					makeForm();
				}
				else
					if (str==items[1])
					{
						metod=2;
						do 
						{
							fflush(log_file);
							str=QInputDialog::getText(0,codec->toUnicode("Шаг 3."),codec->toUnicode("Введите имя переменной, по которой ведётся интегрирование"),QLineEdit::Normal,codec->toUnicode(""),&bOk);	
							if (!bOk)
								cancel=true;
							else
							{
								temp_x[0]='\0';
								strcat(temp_x,str.toStdString().c_str());
								bOk=(VYRAZ::PeremTestName(temp_x,&ukaz)) && (RKII::provX(temp_x,temp_razr,&ukaz));
								if (!bOk)
									QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(ukaz));
							}
						}while ((!bOk) && (!cancel));
						// После этого необходимо вывести окно для ввода уравнений системы
						makeForm();
					}
					else
					{
						metod=3;
						do 
						{
							fflush(log_file);
							str=QInputDialog::getText(0,codec->toUnicode("Шаг 3."),codec->toUnicode("Введите имя переменной, по которой ведётся интегрирование"),QLineEdit::Normal,codec->toUnicode(""),&bOk);	
							if (!bOk)
								cancel=true;
							else
							{
								temp_x[0]='\0';
								strcat(temp_x,str.toStdString().c_str());
								bOk=(VYRAZ::PeremTestName(temp_x,&ukaz)) && (RKIV::provX(temp_x,temp_razr,&ukaz));
								if (!bOk)
									QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(ukaz));
							}
						}while ((!bOk) && (!cancel));
						// После этого необходимо вывести окно для ввода уравнений системы
						makeForm();
					}
				delete [] ukaz;
			}
		}
	}
}
void MainWindow::makeForm()
{
	this->setWindowTitle(codec->toUnicode("Курсовая работа"));
	perems=NULL;
	QHBoxLayout *mainlayout=new QHBoxLayout(this);
	QVBoxLayout *vhod=new QVBoxLayout();
	mainlayout->addLayout(vhod);
	QHBoxLayout *shag=new QHBoxLayout();
	vhod->addLayout(shag);
	QLabel *nomer_shaga=new QLabel(codec->toUnicode("Шаг 4. Введите уравнения системы и данные об отрезке интегрирования"));
	nomer_shaga->setObjectName("nomer_shaga");
	shag->addWidget(nomer_shaga);
	shag->addStretch();
	QPushButton *perems=new QPushButton(codec->toUnicode("Дополнительные переменные..."));
	shag->addWidget(perems);
	shag->addStretch();
	QPushButton *back_step=new QPushButton(codec->toUnicode("Вернуться на предыдущий шаг"));
	back_step->setVisible(false);
	back_step->setEnabled(false);
	back_step->setObjectName("back_step");
	shag->addWidget(back_step);
	QPushButton *dalee=new QPushButton(codec->toUnicode("Перейти к шагу 5"));
	dalee->setObjectName("dalee_button");
	shag->addWidget(dalee);
	QPushButton *build=new QPushButton(codec->toUnicode("Решить и построить графики!"));
	build->setObjectName("build_button");
	build->setVisible(false);
	build->setEnabled(false);
	shag->addWidget(build);
	QHBoxLayout *data=new QHBoxLayout();
	vhod->addLayout(data);
	QScrollArea *scroll=new QScrollArea(this);
	scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	data->addWidget(scroll);
	scroll->setWidgetResizable(true);
	QFrame *frame=new QFrame();
	frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
	QVBoxLayout *urav_field=new QVBoxLayout(frame);
	QHBoxLayout *uravnenia=new QHBoxLayout();
	uravnenia->setObjectName("uravnenia");
	urav_field->addLayout(uravnenia);
	urav_field->addStretch();
	scroll->setWidget(frame);
	QVBoxLayout *urav_1=new QVBoxLayout();
	QVBoxLayout *urav_2=new QVBoxLayout();
	QVBoxLayout *urav_3=new QVBoxLayout();
	uravnenia->addLayout(urav_1);
	uravnenia->addLayout(urav_2);
	uravnenia->addLayout(urav_3);
	QLabel *urav_lev;
	QLabel *ravno;
	QLineEdit *urav_prav;
	char buf[150],buf2[150];
	for (int i=0;i<temp_razr;i++)
	{
		itoa(i+1,buf2,10);
		buf[0]='\0';
		strcat(buf,"df");
		strcat(buf,buf2);
		strcat(buf,"/d");
		strcat(buf,temp_x);
		urav_lev=new QLabel(codec->toUnicode(buf));
		urav_1->addWidget(urav_lev);
		ravno=new QLabel(codec->toUnicode("="));
		urav_2->addWidget(ravno);
		urav_prav=new QLineEdit();
		urav_3->addWidget(urav_prav);
		urav_prav->setObjectName("dfi_vyraz");
		urav_prav->setMaxLength(150);
	}
	scroll->setMinimumSize(2.1*urav_prav->sizeHint());
	QVBoxLayout *params=new QVBoxLayout();
	vhod->addLayout(params);
	QHBoxLayout *otrez=new QHBoxLayout();
	params->addLayout(otrez);
	QLabel *ablab=new QLabel(codec->toUnicode("Введите границы отрезка интегрирования a и b: "));
	QLabel *alab=new QLabel(codec->toUnicode("a = "));
	QLineEdit *a=new QLineEdit();
	a->setObjectName("a");
	QLabel *blab=new QLabel(codec->toUnicode("b = "));
	QLineEdit *b=new QLineEdit();
	b->setObjectName("b");
	otrez->addWidget(ablab);
	otrez->addWidget(alab);
	otrez->addWidget(a);
	otrez->addWidget(blab);
	otrez->addWidget(b);
	QHBoxLayout *x0lay=new QHBoxLayout();
	params->addLayout(x0lay);
	buf[0]='\0';
	strcat(buf,"Введите начальное значение аргумента, используемое в задаче Коши: ");
	strcat(buf,temp_x);
	strcat(buf,"0 = ");
	QLabel *x0lab=new QLabel(codec->toUnicode(buf));
	QLineEdit *x0=new QLineEdit();
	x0->setObjectName("x0");
	x0lay->addWidget(x0lab);
	x0lay->addWidget(x0);
	QHBoxLayout *nlay=new QHBoxLayout();
	params->addLayout(nlay);
	QLabel *nlab=new QLabel(codec->toUnicode("Введите количество точек на отрезке: n = "));
	QLineEdit *n=new QLineEdit();
	n->setObjectName("n");
	nlay->addWidget(nlab);
	nlay->addWidget(n);
	QVBoxLayout *vihod=new QVBoxLayout();
	mainlayout->addLayout(vihod);
	graph=new Area(this,&solver,this);
	graph->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	graph->setFrameStyle(QFrame::Panel | QFrame::Raised);
	vihod->addWidget(graph);
	QHBoxLayout *sluz=new QHBoxLayout();
	vihod->addLayout(sluz);
	sluz->addStretch();
	QPushButton *save_button=new QPushButton(codec->toUnicode("Сохранить результаты"));
	save_button->setVisible(false);
	save_button->setEnabled(false);
	save_button->setObjectName("save_button");
	sluz->addWidget(save_button);
	QPushButton *back_button=new QPushButton(codec->toUnicode("Назад"));
	sluz->addWidget(back_button);
	connect(dalee,SIGNAL(clicked()),this,SLOT(go_to_step5()));
	connect(back_button,SIGNAL(clicked()),this,SLOT(closeWin()));
	connect(perems,SIGNAL(clicked()),this,SLOT(perems_win()));
	connect(build,SIGNAL(clicked()),this,SLOT(solve()));
	connect(back_step,SIGNAL(clicked()),this,SLOT(go_to_step4()));
	connect(save_button,SIGNAL(clicked()),this,SLOT(save_results()));
}
void MainWindow::save_results()
{
	QFile file;
	file.setFileName("temp");
	while (!(file.copy(QFileDialog::getSaveFileName(this, codec->toUnicode("Сохранить результаты"),
                            "",codec->toUnicode("Текстовый документ (*.txt)")))))
		QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode("При сохранении файла возникла ошибка"));
	file.close();
} 
Area::Area(QWidget *parent,Solver **my_source,MainWindow *win):QFrame(parent)
{
	solver=my_source;
	main_window=win;
}
void Area::paintEvent(QPaintEvent *event)
{
	if (*solver)
	{
		QPainter painter(this);
		double mx,my,Fmin=(*solver)->min[0],Fmax=(*solver)->max[0],xn=(*solver)->a,xk=(*solver)->b,dx,dy,xl,yl;
		int x,y,xm,ym,xs,ys,i,razr=(*solver)->razr,n=(*solver)->n,x1=0,y1=0,x2=width(),y2=height();
		char buf[150];
		if (xn>xk)
		{
			dx=xn;
			xn=xk;
			xk=dx;
		}
		for (int i=1;i<razr;i++)
		{
			if ((*solver)->min[i]<Fmin)
				Fmin=(*solver)->min[i];
			if ((*solver)->max[i]>Fmax)
				Fmax=(*solver)->max[i];
		}
		QPainterPath *m_Path=new QPainterPath[razr+2];
		Solver::point_type *results=(*solver)->results;
		bool flag;
		QList<QCheckBox *> check_list=main_window->findChildren<QCheckBox *>("check_box");
		// составление m_Path[i]. До конца, если ERROR - перестать, вывести. Если после этого встречается не ERROR, то начать составлять новый m_Path.
		xm=x2-x1+1;
		ym=y2-y1+1;
		xs=60;
		ys=ym-xs;
		if ((Fmax-Fmin)<1e-307)
		{
			_gcvt(Fmin-2,10,buf);
			if (!strcmp(buf,"-1.#INF"))
				Fmax+=2;
			else
			{
				_gcvt(Fmax+2,10,buf);
				if (!strcmp(buf,"1.#INF"))
					Fmin-=2;
				else
				{
					Fmin-=1;
					Fmax+=1;
				}
			}
		}
		if ((xk-xn)<1e-307)
		{
			_gcvt(xn-2,10,buf);
			if (!strcmp(buf,"-1.#INF"))
				xk+=2;
			else
			{
				_gcvt(xk+2,10,buf);
				if (!strcmp(buf,"1.#INF"))
					xn-=2;
				else
				{
					xn-=1;
					xk+=1;
				}
			}
		}
		mx=(xm-xs*2)/(xk-xn);
		my=(ym-xs*2)/(Fmax-Fmin);
		setBackgroundRole(QPalette::Light);
		for (int i=0;i<razr;i++)
		{
			if (check_list.at(i)->isChecked())
			{
				flag=false;
				switch (i%5)
				{
				case 0:painter.setPen(QPen(QColor(0,0,0),3,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));break;
				case 1:painter.setPen(QPen(QColor(0,0,0),3,Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));break;
				case 2:painter.setPen(QPen(QColor(0,0,0),3,Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));break;
				case 3:painter.setPen(QPen(QColor(0,0,0),3,Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));break;
				case 4:painter.setPen(QPen(QColor(0,0,0),3,Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin));break;
				}
				for (int j=0;j<n;j++)
				{
					if (strcmp(results[j+i*n].y,"ERROR"))
					{
						if (!flag)
						{
							flag=true;
							m_Path[i].moveTo(x1+xs+(atof(results[j+i*n].x)-xn)*mx,((Fmin-atof(results[j+i*n].y))*my)+y1+ys);
							painter.drawPoint(x1+xs+(atof(results[j+i*n].x)-xn)*mx,((Fmin-atof(results[j+i*n].y))*my)+y1+ys);
						}
						else
							m_Path[i].lineTo(x1+xs+(atof(results[j+i*n].x)-xn)*mx,((Fmin-atof(results[j+i*n].y))*my)+y1+ys);
					}
					else
						flag=false;
				}
			}
			painter.drawPath(m_Path[i]);
		}
		painter.setPen(QPen(QColor(0,0,0),1,Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
		xl=xn;
		dx=(xk-xn)/15;
		x=0;
		do
		{
			painter.drawText((xl-xn)*mx+x1+xs-10,ys+y1+15,QString().setNum(xl,'g',3));
			m_Path[razr].moveTo((xl-xn)*mx+x1+xs,ym+y1-xs);
			m_Path[razr].lineTo((xl-xn)*mx+x1+xs,y1+xs);
			xl=xl+dx;
			x++;
		}
		while (x<=15);
		y=0;
		yl=Fmin;
		dy=(Fmax-Fmin)/20;
		do
		{
			painter.drawText(x1+15,-(yl-Fmin)*my+y1+ys+5,QString().setNum(yl,'g',3));
			m_Path[razr].moveTo(x1+xs,-(yl-Fmin)*my+y1+ys);
			m_Path[razr].lineTo(xm+x1-xs,-(yl-Fmin)*my+y1+ys);
			yl=yl+dy;
			y++;
		}
		while (y<=20);
		painter.drawPath(m_Path[razr]);
		delete [] m_Path;
	}
}
void MainWindow::go_to_step4()
{
	/* Необходимо изменить номер шага, удалить кнопку "Построить", добавить кнопку "Следующий шаг", заблокировать поля для задачи Коши, открыть поля для 4-го шага*/
	QPushButton *temp_btn=findChild<QPushButton *>("back_step");
	temp_btn->setVisible(false);
	temp_btn->setEnabled(false);
	temp_btn=findChild<QPushButton *>("dalee_button");
	temp_btn->setVisible(true);
	temp_btn->setEnabled(true);
	temp_btn=findChild<QPushButton *>("build_button");
	temp_btn->setVisible(false);
	temp_btn->setEnabled(false);
	QList<QLineEdit *> vyraz_list=findChildren<QLineEdit *>("dfi_vyraz");
	int size=vyraz_list.size();
	for (int j=0;j<size;j++)
	{
		vyraz_list.at(j)->setEnabled(true);
		vyraz_list.at(j)->setReadOnly(false);
	}
	QLineEdit *param=findChild<QLineEdit *>("a");
	param->setReadOnly(false);
	param->setEnabled(true);
	param=findChild<QLineEdit *>("b");
	param->setReadOnly(false);
	param->setEnabled(true);
	param=findChild<QLineEdit *>("x0");
	param->setReadOnly(false);
	param->setEnabled(true);
	param=findChild<QLineEdit *>("n");
	param->setReadOnly(false);
	param->setEnabled(true);
	QLabel *label=findChild<QLabel *>("nomer_shaga");
	label->setText(codec->toUnicode("Шаг 4. Введите уравнения системы и данные об отрезке интегрирования"));
	label->setVisible(true);
	vyraz_list=findChildren<QLineEdit *>("func_koshi_znach");
	size=vyraz_list.size();
	for (int j=0;j<size;j++)
	{
		vyraz_list.at(j)->setEnabled(false);
		vyraz_list.at(j)->setReadOnly(true);
	}
}
void MainWindow::solve()
{
	QList<QLineEdit *> list=findChildren<QLineEdit *>("func_koshi_znach");
	bool flag=true;
	QString sss;
	int i=0,size=list.size();
	char *buf=NULL,buffer[150],buffer2[150];
	while (flag && (i<size))
	{
		buffer[0]='\0';
		strcat(buffer,(list.at(i)->text()).toStdString().c_str());
		flag=VYRAZ::provDopVesh(buffer,&buf);
		fflush(log_file);
		if (flag)
			i++;
		else
		{
			sss="Ошибка в значении f";
			sss+=itoa(i,buffer,10);
			sss+="(";
			sss+=temp_x;
			sss+="0)";
			QMessageBox::information(0,codec->toUnicode(sss.toStdString().c_str()),codec->toUnicode(buf));
			delete [] buf;
		}
	}
	if (flag)
	{
		perem_list *cur,*cur2;
		cur=first;
		while (cur)
		{
			if (!(cur->perem->getResult()))
				cur->perem->Clear();
			cur=cur->next;
		}
		temp_Koshi=new double[temp_razr+1];
		temp_Koshi[0]=atof(temp_x0);
		for (i=0;i<temp_razr;i++)
			temp_Koshi[i+1]=atof((list.at(i)->text()).toStdString().c_str());
		list=findChildren<QLineEdit *>("dfi_vyraz");
		if (!first)
		{
			first=new perem_list;
			first->next=NULL;
			buffer[0]='\0';
			strcat(buffer,(list.at(0)->text()).toStdString().c_str());
			first->perem=new VYRAZ("df1",buffer);
			perem_count_list++;
			cur2=first;
		}
		else
		{
			cur=new perem_list;
			cur->next=first;
			first=cur;
			buffer[0]='\0';
			strcat(buffer,(list.at(0)->text()).toStdString().c_str());
			first->perem=new VYRAZ("df1",buffer);
			perem_count_list++;
			cur2=first;
		}
		for (i=1;i<temp_razr;i++)
		{
			cur=new perem_list;
			cur->next=cur2->next;
			cur2->next=cur;
			cur2=cur;
			buffer[0]='\0';
			strcat(buffer,"df");
			itoa(i+1,buffer2,10);
			strcat(buffer,buffer2);
			buffer2[0]='\0';
			strcat(buffer2,(list.at(i)->text()).toStdString().c_str());
			cur->perem=new VYRAZ(buffer,buffer2);
			perem_count_list++;
		}
		QPushButton *temp_btn=findChild<QPushButton *>("back_step");
		temp_btn->setText(codec->toUnicode("Изменить входные данные"));
		temp_btn=findChild<QPushButton *>("build_button");
		temp_btn->setVisible(false);
		temp_btn->setEnabled(false);
		QList<QLineEdit *> vyraz_list=findChildren<QLineEdit *>("dfi_vyraz");
		int size=vyraz_list.size();
		for (int j=0;j<size;j++)
		{
			vyraz_list.at(j)->setEnabled(false);
			vyraz_list.at(j)->setReadOnly(true);
		}
		QLineEdit *param=findChild<QLineEdit *>("a");
		param->setReadOnly(true);
		param->setEnabled(false);
		param=findChild<QLineEdit *>("b");
		param->setReadOnly(true);
		param->setEnabled(false);
		param=findChild<QLineEdit *>("x0");
		param->setReadOnly(true);
		param->setEnabled(false);
		param=findChild<QLineEdit *>("n");
		param->setReadOnly(true);
		param->setEnabled(false);
		QLabel *label=findChild<QLabel *>("nomer_shaga");
		label->setText("Подождите. Идёт вычисление значений функций...");
		vyraz_list=findChildren<QLineEdit *>("func_koshi_znach");
		if (opened)
			perems->close();
		size=vyraz_list.size();
		for (int j=0;j<size;j++)
		{
			vyraz_list.at(j)->setEnabled(false);
			vyraz_list.at(j)->setReadOnly(true);
		}
		if (solver)
			delete solver;
		switch (metod)
		{
			case 1:	solver=new Eyler(temp_a,temp_b,temp_n,temp_razr,temp_Koshi,temp_x);break;
			case 2:	solver=new RKII(temp_a,temp_b,temp_n,temp_razr,temp_Koshi,temp_x);break;
			case 3:	solver=new RKIV(temp_a,temp_b,temp_n,temp_razr,temp_Koshi,temp_x);break;
		}
		delete [] temp_Koshi;
		delete [] buf;
		solver->solve();
		solver->Print_results();
		solver->delete_Temp_Variables();
		label->setText("Подождите. Идёт построение графиков функций");
		label->setVisible(false);
		temp_btn=findChild<QPushButton *>("save_button");
		temp_btn->setVisible(true);
		temp_btn->setEnabled(true);
		cur=first;
		for (i=0;i<temp_razr;i++)
		{
			cur=cur->next;
			delete first->perem;
			delete first;
			first=cur;
		}
		while (cur)
		{
			cur->perem->setFlag_iter_leks(true);
			cur->perem->Leks_anal(); //для обновления списков переменных
			cur->perem->setFlag_iter(false);
			cur=cur->next;
		}
	}
}
void MainWindow::go_to_step5()
{
	/* Необходимо проверить, чтобы все уравнения системы были введены (необязательное требование, поскольку парсер всё равно бы отсёк их)
	Также необходимо считать n,a,b и собственно уравнения. После этого необходимо создать дополнительные поля для ввода задачи Коши, при этом эти поля необходимо заморозить.*/
	bool flag;
	char *buf=NULL,s[150];
	s[0]='\0';
	QString sss=findChild<QLineEdit *>("a")->text();
	strcat(s,sss.toStdString().c_str());
	flag=VYRAZ::provDopVesh(s,&buf);
	fflush(log_file);
	if (flag)
	{
		temp_a[0]='\0';
		strcat(temp_a,s);
		sss=findChild<QLineEdit *>("b")->text();
		s[0]='\0';
		strcat(s,sss.toStdString().c_str());
		flag=VYRAZ::provDopVesh(s,&buf);
		fflush(log_file);
		if (flag)
		{
			temp_b[0]='\0';
			strcat(temp_b,s);
			sss=findChild<QLineEdit *>("x0")->text();
			temp_x0[0]='\0';
			strcat(temp_x0,sss.toStdString().c_str());
			flag=VYRAZ::provDopVesh(temp_x0,&buf);
			fflush(log_file);
			if (flag)
			{
				if (!((abs(atof(temp_a)-atof(temp_b))>=1e-307) && (((atof(temp_x0)-atof(temp_a))>=0) && ((atof(temp_b)-atof(temp_x0))>=0) || (((atof(temp_x0)-atof(temp_b))>=0) && ((atof(temp_a)-atof(temp_x0))>=0))) || ((abs(atof(temp_a)-atof(temp_b))<1e-307) && (abs(atof(temp_a)-atof(temp_x0))<1e-307) && (abs(atof(temp_x0)-atof(temp_b))<1e-307))))
				{
					if ((atof(temp_a)-atof(temp_b))>=1e-307)
					{
						sss="Значение ";
						sss+=temp_x;
						sss+="0 должно входить в отрезок интегрирования [";
						sss+=temp_b;
						sss+=",";
						sss+=temp_a;
						sss+="]";
						s[0]='\0';
						strcat(s,"Ошибка в значении ");
						strcat(s,temp_x);
						strcat(s,"0");
						QMessageBox::information(0,codec->toUnicode(s),codec->toUnicode(sss.toStdString().c_str()));
					}
					else
					{
						sss="Значение ";
						sss+=temp_x;
						sss+="0 должно входить в отрезок интегрирования [";
						sss+=temp_a;
						sss+=",";
						sss+=temp_b;
						sss+="]";
						s[0]='\0';
						strcat(s,"Ошибка в значении ");
						strcat(s,temp_x);
						strcat(s,"0");
						QMessageBox::information(0,codec->toUnicode(s),codec->toUnicode(sss.toStdString().c_str()));
					}
				}
				else
				{
					sss=findChild<QLineEdit *>("n")->text();
					s[0]='\0';
					strcat(s,sss.toStdString().c_str());
					flag=VYRAZ::provDopInt(s,&buf);
					fflush(log_file);
					int i=atoi(s);
					if (flag)
					{
						if ((abs(atof(temp_x0)-atof(temp_a))<1e-307) || (abs(atof(temp_x0)-atof(temp_b))<1e-307))
						{
							if (((i<2) && (abs(atof(temp_a)-atof(temp_b))>=1e-307)) || ((i<1) && (abs(atof(temp_a)-atof(temp_b))<1e-307)))
							{
								flag=false;
								QMessageBox::information(0,codec->toUnicode("Ошибка в значении n"),codec->toUnicode("Некорректные данные!!! Всего в отрезке должно быть не меньше точек, чем граничных!!"));
							}
						}
						else
							if (((i<3) && (abs(atof(temp_a)-atof(temp_b))>=1e-307)) || ((i<1) && (abs(atof(temp_a)-atof(temp_b))<1e-307)))
							{
								flag=false;
								QMessageBox::information(0,codec->toUnicode("Ошибка в значении n"),codec->toUnicode("Некорректные данные!!! Всего в отрезке должно быть не меньше точек, чем граничных, а также точки отрезка, определяющей задачу Коши!!"));
							}
						if (flag)
						{
							temp_n=i;
							// необходимо отобразить кнопку "К предыдущему шагу", заблокировать данные поля для ввода! и отобразить поля для задачи Коши
							QPushButton *temp_btn=findChild<QPushButton *>("back_step");
							temp_btn->setVisible(true);
							temp_btn->setEnabled(true);
							temp_btn->setText(codec->toUnicode("Вернуться на предыдущий шаг"));
							temp_btn=findChild<QPushButton *>("dalee_button");
							temp_btn->setVisible(false);
							temp_btn->setEnabled(false);
							temp_btn=findChild<QPushButton *>("build_button");
							temp_btn->setVisible(true);
							temp_btn->setEnabled(true);
							QList<QLineEdit *> vyraz_list=findChildren<QLineEdit *>("dfi_vyraz");
							int size=vyraz_list.size();
							for (int j=0;j<size;j++)
							{
								vyraz_list.at(j)->setEnabled(false);
								vyraz_list.at(j)->setReadOnly(true);
							}
							QLineEdit *param=findChild<QLineEdit *>("a");
							param->setReadOnly(true);
							param->setEnabled(false);
							param=findChild<QLineEdit *>("b");
							param->setReadOnly(true);
							param->setEnabled(false);
							param=findChild<QLineEdit *>("x0");
							param->setReadOnly(true);
							param->setEnabled(false);
							param=findChild<QLineEdit *>("n");
							param->setReadOnly(true);
							param->setEnabled(false);
							QLabel *label=findChild<QLabel *>("nomer_shaga");
							label->setText(codec->toUnicode("Шаг 5. Введите условия задачи Коши"));
							vyraz_list=findChildren<QLineEdit *>("func_koshi_znach");
							if (vyraz_list.size())
							{
								size=vyraz_list.size();
								for (int j=0;j<size;j++)
								{
									vyraz_list.at(j)->setEnabled(true);
									vyraz_list.at(j)->setReadOnly(false);
								}
							}
							else
							{
								QHBoxLayout *lay=findChild<QHBoxLayout *>("uravnenia");
								QVBoxLayout *func_koshi_num=new QVBoxLayout();
								QVBoxLayout *func_koshi_ravno=new QVBoxLayout();
								QVBoxLayout *func_koshi_znach=new QVBoxLayout();
								QVBoxLayout *check_label=new QVBoxLayout();
								QVBoxLayout *check_boxes=new QVBoxLayout();
								QCheckBox *check_box;
								lay->addLayout(func_koshi_num);
								lay->addLayout(func_koshi_ravno);
								lay->addLayout(func_koshi_znach);
								lay->addLayout(check_label);
								lay->addLayout(check_boxes);
								for (int j=0;j<temp_razr;j++)
								{
									itoa(j+1,s,10);
									buf[0]='\0';
									strcat(buf,"f");
									strcat(buf,s);
									strcat(buf,"(");
									strcat(buf,temp_x);
									strcat(buf,"0)");
									label=new QLabel(codec->toUnicode(buf));
									func_koshi_num->addWidget(label);
									label=new QLabel(codec->toUnicode("="));
									func_koshi_ravno->addWidget(label);
									param=new QLineEdit();
									func_koshi_znach->addWidget(param);
									param->setObjectName("func_koshi_znach");
									label=new QLabel(codec->toUnicode("Построить график - "));
									check_label->addWidget(label);
									check_box=new QCheckBox();
									check_box->setCheckState(Qt::Checked);
									check_box->setObjectName("check_box");
									check_boxes->addWidget(check_box);
									check_boxes->setSpacing(10);
								}
							}
						}
					}
					else
						QMessageBox::information(0,codec->toUnicode("Ошибка в значении n"),codec->toUnicode(buf));
				}
			}
			else
			{
				sss="Ошибка в значении ";
				sss+=temp_x;
				sss+="0";
				QMessageBox::information(0,codec->toUnicode(sss.toStdString().c_str()),codec->toUnicode(buf));
			}
		}
		else
			QMessageBox::information(0,codec->toUnicode("Ошибка в значении b"),codec->toUnicode(buf));
	}
	else
		QMessageBox::information(0,codec->toUnicode("Ошибка в значении a"),codec->toUnicode(buf));
}
void MainWindow::closeWin()
{
	if (solver)
		delete solver;
	back->show();
	back->setFocus();
	if (perems)
		delete perems;
	delete back->main;
}
void MainWindow::perems_win()
{
	//окно дополнительных переменных
	if (!perems)
	{
		perems=new PeremWindow(0);
		perems->back=this;
	}
	perems->show();
	opened=true;
	perems->setFocus();
}
PeremWindow::PeremWindow(QWidget *parent):QWidget(parent)
{
	this->setWindowTitle(codec->toUnicode("Курсовая работа"));
	QVBoxLayout *mainlayout=new QVBoxLayout(this);
	QLabel *title=new QLabel(codec->toUnicode("Дополнительные переменные"));
	mainlayout->addWidget(title);
	QVBoxLayout *all_perems=new QVBoxLayout();
	mainlayout->addLayout(all_perems);	
	QScrollArea *scroll=new QScrollArea(this);
	scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	all_perems->addWidget(scroll);
	scroll->setWidgetResizable(true);
	QFrame *frame=new QFrame();
	frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
	QVBoxLayout *perem_field=new QVBoxLayout(frame);
	perem_field->setObjectName("perem_field");
	scroll->setWidget(frame);	
	QHBoxLayout *perem;
	QLineEdit *edit;
	QPushButton *change_button;
	QPushButton *del_button;
	int i=1;
	char buf[150];
	perem=new QHBoxLayout();
	perem->setObjectName("add_perem");
	perem_field->addLayout(perem);
	itoa(i,buf,10);
	QLabel *lab=new QLabel(codec->toUnicode(buf));
	lab->setObjectName("add_lab");
	perem->addWidget(lab);
	edit=new QLineEdit();
	edit->setObjectName("add_name");
	edit->setMaxLength(20);
	edit->setFixedWidth(140);
	edit->setText(codec->toUnicode("Введите имя"));
	perem->addWidget(edit);
	lab=new QLabel(codec->toUnicode("Текущее имя: "));
	lab->setObjectName("add_label_name");
	lab->setVisible(false);
	perem->addWidget(lab);
	edit=new QLineEdit();
	edit->setObjectName("add_edit_name");
	edit->setMaxLength(20);
	edit->setFixedWidth(140);
	edit->setText(codec->toUnicode("Введите имя"));
	perem->addWidget(edit);
	edit->setVisible(false);
	edit->setReadOnly(true);
	edit=new QLineEdit();
	edit->setObjectName("add_vyraz");
	edit->setMaxLength(150);
	edit->setText(codec->toUnicode("Введите выражение"));
	perem->addWidget(edit);
	lab=new QLabel(codec->toUnicode("Текущее выражение: "));
	lab->setObjectName("add_label_vyraz");
	lab->setVisible(false);
	perem->addWidget(lab);
	edit=new QLineEdit();
	edit->setMaxLength(150);
	edit->setObjectName("add_edit_vyraz");
	edit->setText(codec->toUnicode("Введите выражение"));
	perem->addWidget(edit);
	edit->setVisible(false);
	edit->setReadOnly(true);
	QPushButton *add_button=new QPushButton(codec->toUnicode("Добавить"));
	add_button->setObjectName("add_button");
	perem->addWidget(add_button);
	QHBoxLayout *exit=new QHBoxLayout();
	mainlayout->addLayout(exit);
	exit->addStretch();
	QPushButton *back_button=new QPushButton(codec->toUnicode("Закрыть окно"));
	exit->addWidget(back_button);
	connect(back_button,SIGNAL(clicked()),this,SLOT(closeWin()));
	connect(add_button,SIGNAL(clicked()),this,SLOT(addPerem()));
	signalMapperChange=new QSignalMapper(this);
	signalMapperDelete=new QSignalMapper(this);
	signalMapperEdit=new QSignalMapper(this);
	signalMapperEditCancel=new QSignalMapper(this);
	connect(signalMapperChange,SIGNAL(mapped(int)),this,SLOT(changePerem(int)));
	connect(signalMapperDelete,SIGNAL(mapped(int)),this,SLOT(deletePerem(int)));
	connect(signalMapperEdit,SIGNAL(mapped(int)),this,SLOT(saveEditPerem(int)));
	connect(signalMapperEditCancel,SIGNAL(mapped(int)),this,SLOT(cancelEditPerem(int)));
}
PeremWindow::~PeremWindow()
{
	deleteAllPerems();
}
void PeremWindow::changePerem(int num)
{
	QList<QLineEdit *> list_name=findChildren<QLineEdit *>("name");
	QList<QLabel *> list_edit_label_name=findChildren<QLabel *>("label_name");
	QList<QLineEdit *> list_edit_name=findChildren<QLineEdit *>("edit_name");
	QList<QLineEdit *> list_vyraz=findChildren<QLineEdit *>("vyraz");
	QList<QLabel *> list_edit_label_vyraz=findChildren<QLabel *>("label_vyraz");
	QList<QLineEdit *> list_edit_vyraz=findChildren<QLineEdit *>("edit_vyraz");
	QList<QPushButton *> list_change=findChildren<QPushButton *>("change_button");
	QList<QPushButton *> list_edit=findChildren<QPushButton *>("edit_button");
	QList<QPushButton *> list_edit_cancel=findChildren<QPushButton *>("edit_cancel_button");
	list_name.at(num-1)->setReadOnly(false);
	list_vyraz.at(num-1)->setReadOnly(false);
	list_edit_name.at(num-1)->setVisible(true);
	list_edit_label_name.at(num-1)->setVisible(true);
	list_edit_name.at(num-1)->setText(list_name.at(num-1)->text());
	list_edit_vyraz.at(num-1)->setVisible(true);
	list_edit_label_vyraz.at(num-1)->setVisible(true);
	list_edit_vyraz.at(num-1)->setText(list_vyraz.at(num-1)->text());
	list_change.at(num-1)->setVisible(false);
	list_change.at(num-1)->setEnabled(false);
	list_edit.at(num-1)->setVisible(true);
	list_edit.at(num-1)->setEnabled(true);
	list_edit_cancel.at(num-1)->setVisible(true);
	list_edit_cancel.at(num-1)->setEnabled(true);
}
void PeremWindow::deletePerem(int num)
{
	QVBoxLayout *perem_field=findChild<QVBoxLayout *>("perem_field");
	QList<QLabel *> list_lab=findChildren<QLabel *>("lab");
	QList<QLineEdit *> list_name=findChildren<QLineEdit *>("name");
	QList<QLabel *> list_edit_label_name=findChildren<QLabel *>("label_name");
	QList<QLineEdit *> list_edit_name=findChildren<QLineEdit *>("edit_name");
	QList<QLineEdit *> list_vyraz=findChildren<QLineEdit *>("vyraz");
	QList<QLabel *> list_edit_label_vyraz=findChildren<QLabel *>("label_vyraz");
	QList<QLineEdit *> list_edit_vyraz=findChildren<QLineEdit *>("edit_vyraz");
	QList<QLineEdit *> list_result=findChildren<QLineEdit *>("result");
	QList<QPushButton *> list_change=findChildren<QPushButton *>("change_button");
	QList<QPushButton *> list_edit=findChildren<QPushButton *>("edit_button");
	QList<QPushButton *> list_edit_cancel=findChildren<QPushButton *>("edit_cancel_button");
	QList<QPushButton *> list_del=findChildren<QPushButton *>("del_button");
	delete perem_field->takeAt(num-1);
	delete list_lab.at(num-1);		
	delete list_name.at(num-1);
	delete list_edit_label_name.at(num-1);
	delete list_edit_name.at(num-1);
	delete list_vyraz.at(num-1);
	delete list_edit_label_vyraz.at(num-1);
	delete list_edit_vyraz.at(num-1);
	delete list_result.at(num-1);
	delete list_change.at(num-1);
	delete list_edit.at(num-1);
	delete list_edit_cancel.at(num-1);
	delete list_del.at(num-1);
	perem_list *cur=first,*prev;
	int i=0;
	while (i++<num-1)
	{
		prev=cur;
		cur=cur->next;
	}
	VYRAZ::provChange(cur->perem->getName(),first);
	if (cur==first)
	{
		first=first->next;
		delete cur->perem;
		delete cur;
	}
	else
	{
		prev->next=cur->next;
		delete cur->perem;
		delete cur;
	}
	perem_count_list--;
	fputs("Переменная успешно удалена",log_file);
	fprintf(log_file,"\n");
	perem_count_list--;
	char buf[150];
	int size=list_lab.size();
	i=num;
	while (i<size)
	{
		list_lab.at(i)->setText(itoa(i,buf,10));
		signalMapperChange->removeMappings(list_change.at(i));
		signalMapperDelete->removeMappings(list_del.at(i));
		signalMapperEdit->removeMappings(list_edit.at(i));
		signalMapperEditCancel->removeMappings(list_edit_cancel.at(i));
		signalMapperChange->setMapping(list_change.at(i),i);
		signalMapperDelete->setMapping(list_del.at(i),i);
		signalMapperEdit->setMapping(list_edit.at(i),i);
		signalMapperEditCancel->setMapping(list_edit_cancel.at(i),i);
		i++;
	}
	findChild<QLabel *>("add_lab")->setText(itoa(size,buf,10));
	getResults();
}

void PeremWindow::saveEditPerem(int num)
{
	QList<QLineEdit *> list_name=findChildren<QLineEdit *>("name");
	QList<QLabel *> list_edit_label_name=findChildren<QLabel *>("label_name");
	QList<QLineEdit *> list_edit_name=findChildren<QLineEdit *>("edit_name");
	QList<QLineEdit *> list_vyraz=findChildren<QLineEdit *>("vyraz");
	QList<QLabel *> list_edit_label_vyraz=findChildren<QLabel *>("label_vyraz");
	QList<QLineEdit *> list_edit_vyraz=findChildren<QLineEdit *>("edit_vyraz");
	QList<QPushButton *> list_change=findChildren<QPushButton *>("change_button");
	QList<QPushButton *> list_edit=findChildren<QPushButton *>("edit_button");
	QList<QPushButton *> list_edit_cancel=findChildren<QPushButton *>("edit_cancel_button");
	bool flag;
	int i;
	char *buf=NULL,buffer[350];
	char s_name[20];
	s_name[0]='\0';
	strcat(s_name,list_name.at(num-1)->text().toStdString().c_str());
	char s[150];
	s[0]='\0';
	strcat(s,list_vyraz.at(num-1)->text().toStdString().c_str());
	flag=VYRAZ::PeremTestName(s_name,&buf);
	if (!flag)
	{
		fputs("Введено некорректное имя переменной.\nИмя должно состоять из латинских букв или цифр,\n причём первым символом должна быть буква\n",log_file);
		fprintf(log_file,"\n");
		buffer[0]='\0';
		strcat(buffer,buf);
		strcat(buffer,"\nВведено некорректное имя переменной.\nИмя должно состоять из латинских букв или цифр,\n причём первым символом должна быть буква");
		QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buffer));	
	}
	else
	{
		switch (back->metod)
		{
			case 1:	flag=Eyler::provX(s_name,back->temp_razr,&buf);break;
			case 2:	flag=RKII::provX(s_name,back->temp_razr,&buf);break;
			case 3:	flag=RKIV::provX(s_name,back->temp_razr,&buf);break;
		}
		if (!flag)
			QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buf));
		else
		{
			if (!strcmp(back->temp_x,s_name))
			{
				fprintf(log_file,"ОШИБКА!!! Переменная %s является аргументом функций системы.\n",s_name);
				buffer[0]='\0';
				strcat(buffer,"Переменная ");
				strcat(buffer,s_name);
				strcat(buffer," является аргументом функций системы");
				QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buffer));		
			}
			else
			{
				perem_list *cur=first,*item;
				item=first;
				i=0;
				while (i++<num-1)
					item=item->next;
				cur=first;
				while (flag && cur)
				{
					if ((!strcmp(s_name,cur->perem->getName())) && (cur!=item))
						flag=false;
					cur=cur->next;
				}
				if (!flag)
				{
					fprintf(log_file,"ОШИБКА!!!\nПеременная с таким именем %s уже существует.\n",s_name);
					buffer[0]='\0';
					strcat(buffer,"Переменная с таким именем ");
					strcat(buffer,s_name);
					strcat(buffer," уже существует");
					QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buffer));			
				}
				else
				{
					list_name.at(num-1)->setReadOnly(true);
					list_vyraz.at(num-1)->setReadOnly(true);
					list_edit_name.at(num-1)->setVisible(false);
					list_edit_label_name.at(num-1)->setVisible(false);
					list_name.at(num-1)->setText(s_name);
					list_edit_vyraz.at(num-1)->setVisible(false);
					list_edit_label_vyraz.at(num-1)->setVisible(false);
					list_vyraz.at(num-1)->setText(s);
					list_change.at(num-1)->setVisible(true);
					list_change.at(num-1)->setEnabled(true);
					list_edit.at(num-1)->setVisible(false);
					list_edit.at(num-1)->setEnabled(false);
					list_edit_cancel.at(num-1)->setVisible(false);
					list_edit_cancel.at(num-1)->setEnabled(false);
					VYRAZ::provChange(item->perem->getName(),first);
					item->perem->setName(s_name);
					item->perem->setFormula(s);
					VYRAZ::provChange(s_name,first);
					getResults();
				}
			}
		}
	}
	delete [] buf;
}
void PeremWindow::cancelEditPerem(int num)
{
	QList<QLineEdit *> list_name=findChildren<QLineEdit *>("name");
	QList<QLabel *> list_edit_label_name=findChildren<QLabel *>("label_name");
	QList<QLineEdit *> list_edit_name=findChildren<QLineEdit *>("edit_name");
	QList<QLineEdit *> list_vyraz=findChildren<QLineEdit *>("vyraz");
	QList<QLabel *> list_edit_label_vyraz=findChildren<QLabel *>("label_vyraz");
	QList<QLineEdit *> list_edit_vyraz=findChildren<QLineEdit *>("edit_vyraz");
	QList<QPushButton *> list_change=findChildren<QPushButton *>("change_button");
	QList<QPushButton *> list_edit=findChildren<QPushButton *>("edit_button");
	QList<QPushButton *> list_edit_cancel=findChildren<QPushButton *>("edit_cancel_button");
	list_name.at(num-1)->setReadOnly(true);
	list_vyraz.at(num-1)->setReadOnly(true);
	list_edit_name.at(num-1)->setVisible(false);
	list_edit_label_name.at(num-1)->setVisible(false);
	list_name.at(num-1)->setText(list_edit_name.at(num-1)->text());
	list_edit_vyraz.at(num-1)->setVisible(false);
	list_edit_label_vyraz.at(num-1)->setVisible(false);
	list_vyraz.at(num-1)->setText(list_edit_vyraz.at(num-1)->text());
	list_change.at(num-1)->setVisible(true);
	list_change.at(num-1)->setEnabled(true);
	list_edit.at(num-1)->setVisible(false);
	list_edit.at(num-1)->setEnabled(false);
	list_edit_cancel.at(num-1)->setVisible(false);
	list_edit_cancel.at(num-1)->setEnabled(false);
}
void PeremWindow::deleteAllPerems()
{
	perem_list *cur=first;
	int i=0;
	QVBoxLayout *perem_field=findChild<QVBoxLayout *>("perem_field");
	QList<QLabel *> list_lab=findChildren<QLabel *>("lab");
	QList<QLineEdit *> list_name=findChildren<QLineEdit *>("name");
	QList<QLabel *> list_edit_label_name=findChildren<QLabel *>("label_name");
	QList<QLineEdit *> list_edit_name=findChildren<QLineEdit *>("edit_name");
	QList<QLineEdit *> list_vyraz=findChildren<QLineEdit *>("vyraz");
	QList<QLabel *> list_edit_label_vyraz=findChildren<QLabel *>("label_vyraz");
	QList<QLineEdit *> list_edit_vyraz=findChildren<QLineEdit *>("edit_vyraz");
	QList<QLineEdit *> list_result=findChildren<QLineEdit *>("result");
	QList<QPushButton *> list_change=findChildren<QPushButton *>("change_button");
	QList<QPushButton *> list_edit=findChildren<QPushButton *>("edit_button");
	QList<QPushButton *> list_edit_cancel=findChildren<QPushButton *>("edit_cancel_button");
	QList<QPushButton *> list_del=findChildren<QPushButton *>("del_button");
	while (cur)
	{
		delete perem_field->takeAt(0);
		delete list_lab.at(i);		
		delete list_name.at(i);
		delete list_edit_label_name.at(i);
		delete list_edit_name.at(i);
		delete list_vyraz.at(i);
		delete list_edit_label_vyraz.at(i);
		delete list_edit_vyraz.at(i);
		delete list_result.at(i);
		delete list_change.at(i);
		delete list_edit.at(i);
		delete list_edit_cancel.at(i);
		delete list_del.at(i++);
		first=cur->next;
		delete cur->perem;
		delete cur;
		cur=first;
		perem_count_list--;
	}
	findChild<QLabel *>("add_lab")->setText("1");
}

void PeremWindow::getResults()
{
	perem_list *cur=first;
	QList<QLineEdit *> list=findChildren<QLineEdit *>("result");
	char *buf_1=NULL;
	if (!cur)
	{
		fputs("Список пуст\n",log_file);
		fprintf(log_file,"\n");
	}
	else
	{
		int i=1;
		fputs("ПОЛНЫЙ СПИСОК РЕЗУЛЬТАТОВ\n",log_file);
		fprintf(log_file,"\n");
		while (cur)
		{
			fprintf(log_file,"%d:\n",i);
			cur->perem->getResult();
			cur->perem->Print_result(&buf_1);
			list.at(i-1)->setText(codec->toUnicode(buf_1));
			fprintf(log_file,"\n\n");
			cur=cur->next;
			i++;
			fflush(log_file);
		}
	}
	if (buf_1)
		delete [] buf_1;
	fflush(log_file);
}

void PeremWindow::addPerem()
{	
	char *buf=NULL,buffer[350];
	bool flag;
	QLineEdit *name,*vyraz;
	name=findChild<QLineEdit *>("add_name");
	vyraz=findChild<QLineEdit *>("add_vyraz");
	if (perem_count_list==2147483647)
	{
		fputs("Достигнут предел количества хранимых переменных - 2147483647",log_file);
		fprintf(log_file,"\n");
		QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode("Достигнут предел количества хранимых переменных - 2147483647"));	
	}
	else
	{
		char s_name[20],s_vyraz[150];
		s_name[0]='\0';
		strcat(s_name,name->text().toStdString().c_str());
		s_vyraz[0]='\0';
		strcat(s_vyraz,vyraz->text().toStdString().c_str());
		flag=VYRAZ::PeremTestName(s_name,&buf);
		if (!flag)
		{
			fputs("Введено некорректное имя переменной.\nИмя должно состоять из латинских букв или цифр,\n причём первым символом должна быть буква\n",log_file);
			fprintf(log_file,"\n");
			buffer[0]='\0';
			strcat(buffer,buf);
			strcat(buffer,"\nВведено некорректное имя переменной.\nИмя должно состоять из латинских букв или цифр,\n причём первым символом должна быть буква");
			QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buffer));	
		}
		else
		{
			switch (back->metod)
			{
				case 1:	flag=Eyler::provX(s_name,back->temp_razr,&buf);break;
				case 2:	flag=RKII::provX(s_name,back->temp_razr,&buf);break;
				case 3:	flag=RKIV::provX(s_name,back->temp_razr,&buf);break;
			}
			if (!flag)
				QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buf));
			else
			{
				if (!strcmp(back->temp_x,s_name))
				{
					fprintf(log_file,"ОШИБКА!!! Переменная %s является аргументом функций системы.\n",s_name);
					buffer[0]='\0';
					strcat(buffer,"Переменная ");
					strcat(buffer,s_name);
					strcat(buffer," является аргументом функций системы");
					QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buffer));		
				}
				else
				{
					perem_list *cur=first;
					while (flag && cur)
					{
						if (!strcmp(s_name,cur->perem->getName()))
							flag=false;
						cur=cur->next;
					}
					if (!flag)
					{
						fprintf(log_file,"ОШИБКА!!!\nПеременная с таким именем %s уже существует.\n",s_name);
						buffer[0]='\0';
						strcat(buffer,"Переменная с таким именем ");
						strcat(buffer,s_name);
						strcat(buffer," уже существует");
						QMessageBox::information(0,codec->toUnicode("Ошибка"),codec->toUnicode(buffer));			
					}
					else
					{
						if (!first)		
						{	
							first=new perem_list;
							first->next=NULL;
							first->perem=new VYRAZ(s_name,s_vyraz);
							cur=first;
							perem_count_list++;
						}
						else
						{
							cur=first;
							while (cur->next)
								cur=cur->next;
							cur->next=new perem_list;
							cur=cur->next;
							cur->perem=new VYRAZ(s_name,s_vyraz);
							perem_count_list++;
							cur->next=NULL;
							VYRAZ::provChange(s_name,first);
						}
						name->setReadOnly(true);
						name->setObjectName("name");
						vyraz->setReadOnly(true);
						vyraz->setObjectName("vyraz");
						findChild<QLabel *>("add_lab")->setObjectName("lab");
						findChild<QLineEdit *>("add_edit_name")->setObjectName("edit_name");
						findChild<QLineEdit *>("add_edit_vyraz")->setObjectName("edit_vyraz");
						findChild<QLabel *>("add_label_name")->setObjectName("label_name");
						findChild<QLabel *>("add_label_vyraz")->setObjectName("label_vyraz");
						int i=(findChildren<QHBoxLayout *>("perem")).size();
						QHBoxLayout *perem=findChild<QHBoxLayout *>("add_perem");
						perem->setObjectName("perem");
						QPushButton *btn;
						btn=findChild<QPushButton *>("add_button");
						disconnect(btn,SIGNAL(clicked()),this,SLOT(addPerem()));
						delete btn;
						QLineEdit *edit=new QLineEdit();
						edit->setObjectName("result");
						cur->perem->Print_result(&buf);
						edit->setText(codec->toUnicode(buf));
						edit->setReadOnly(true);
						perem->addWidget(edit);
						QPushButton *change_button=new QPushButton(codec->toUnicode("Изменить"));
						perem->addWidget(change_button);
						change_button->setObjectName("change_button");
						QPushButton *edit_button=new QPushButton(codec->toUnicode("Сохранить"));
						perem->addWidget(edit_button);
						edit_button->setObjectName("edit_button");
						edit_button->setVisible(false);
						edit_button->setEnabled(false);
						QPushButton *edit_cancel_button=new QPushButton(codec->toUnicode("Отмена"));
						perem->addWidget(edit_cancel_button);
						edit_cancel_button->setObjectName("edit_cancel_button");
						edit_cancel_button->setVisible(false);
						edit_cancel_button->setEnabled(false);
						QPushButton *del_button=new QPushButton(codec->toUnicode("Удалить"));
						perem->addWidget(del_button);
						del_button->setObjectName("del_button");
						connect(change_button,SIGNAL(clicked()),signalMapperChange,SLOT(map()));
						signalMapperChange->setMapping(change_button,i+1);
						connect(edit_button,SIGNAL(clicked()),signalMapperEdit,SLOT(map()));
						signalMapperEdit->setMapping(edit_button,i+1);
						connect(edit_cancel_button,SIGNAL(clicked()),signalMapperEditCancel,SLOT(map()));
						signalMapperEditCancel->setMapping(edit_cancel_button,i+1);
						connect(del_button,SIGNAL(clicked()),signalMapperDelete,SLOT(map()));
						signalMapperDelete->setMapping(del_button,i+1);
						QVBoxLayout *perem_field=findChild<QVBoxLayout *>("perem_field");
						perem=new QHBoxLayout();
						perem->setObjectName("add_perem");
						perem_field->addLayout(perem);
						itoa(i+2,buffer,10);
						QLabel *lab=new QLabel(codec->toUnicode(buffer));
						lab->setObjectName("add_lab");
						perem->addWidget(lab);
						edit=new QLineEdit();
						edit->setObjectName("add_name");
						edit->setMaxLength(20);
						edit->setFixedWidth(140);
						edit->setText(codec->toUnicode("Введите имя"));
						perem->addWidget(edit);
						lab=new QLabel(codec->toUnicode("Текущее имя: "));
						lab->setObjectName("add_label_name");
						lab->setVisible(false);
						perem->addWidget(lab);
						edit=new QLineEdit();
						edit->setObjectName("add_edit_name");
						edit->setMaxLength(20);
						edit->setFixedWidth(140);
						edit->setText(codec->toUnicode("Введите имя"));
						perem->addWidget(edit);
						edit->setVisible(false);
						edit->setReadOnly(true);
						edit=new QLineEdit();
						edit->setObjectName("add_vyraz");
						edit->setMaxLength(150);
						edit->setText(codec->toUnicode("Введите выражение"));
						perem->addWidget(edit);
						lab=new QLabel(codec->toUnicode("Текущее выражение: "));
						lab->setObjectName("add_label_vyraz");
						lab->setVisible(false);
						perem->addWidget(lab);
						edit=new QLineEdit();
						edit->setObjectName("add_edit_vyraz");
						edit->setMaxLength(150);
						edit->setText(codec->toUnicode("Введите выражение"));
						perem->addWidget(edit);
						edit->setVisible(false);
						edit->setReadOnly(true);
						QPushButton *add_button=new QPushButton(codec->toUnicode("Добавить"));
						perem->addWidget(add_button);
						add_button->setObjectName("add_button");
						connect(add_button,SIGNAL(clicked()),this,SLOT(addPerem()));
						getResults();
					}
				}
			}
		}
		delete [] buf;
	}
	fflush(log_file);
}
void PeremWindow::closeWin()
{
	this->close();
	back->setFocus();
	back->opened=false;
}

HelpWindow::HelpWindow(QWidget *parent):QWidget(parent)
{
	this->setWindowTitle(codec->toUnicode("Курсовая работа"));
	QTextEdit *help_edit;
	help_edit=new QTextEdit();
	QFile help_file("help.txt");
	help_file.open(QIODevice::ReadOnly);
	QTextStream in(&help_file);
	help_edit->clear();
	help_edit->append(in.readAll());
	help_file.close();
	help_edit->setReadOnly(true);
	QLabel *title=new QLabel(codec->toUnicode("Справка по использованию"));
	QVBoxLayout *vlayout=new QVBoxLayout(this);
	vlayout->addWidget(title);
	vlayout->addWidget(help_edit);
	QPushButton *back_button=new QPushButton(codec->toUnicode("Назад"));
	vlayout->addWidget(back_button);
	connect(back_button,SIGNAL(clicked()),this,SLOT(closeWin()));
}
void HelpWindow::closeWin()
{
	back->show();
	back->setFocus();
	delete back->help;
}