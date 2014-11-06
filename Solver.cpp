#include "Solver.h"
Solver::Solver(char *ma,char *mb,int mn, int mrazr, double *mish_Koshi, char *mx)
{
	perem_list *cur;
	a=atof(ma);
	b=atof(mb);
	n=mn;
	cur=new perem_list;
	cur->next=first;
	if (abs(b-a)<1e-307)
	{
		n_iter=mn-1;
		cur->perem=new VYRAZ("h",_gcvt((a-mish_Koshi[0])/n_iter,10,buf));	
	}
	else
		if (a>b)
		{
			n_iter=int((mish_Koshi[0]-b)/(a-b)*(n-1));
			if ((mish_Koshi[0]-b>=1e-307) && (n_iter==0))
				n_iter++;
			cur->perem=new VYRAZ("h",_gcvt((b-mish_Koshi[0])/n_iter,10,buf));	
		}
		else
		{
			n_iter=int((mish_Koshi[0]-a)/(b-a)*(n-1));
			if ((mish_Koshi[0]-a>=1e-307) && (n_iter==0))
				n_iter++;
			cur->perem=new VYRAZ("h",_gcvt((a-mish_Koshi[0])/n_iter,10,buf));
		}
	first=cur;
	h=first->perem;
	razr=mrazr;
	cur=first;
	for (int i=0;i<razr;i++)
	{
		buf[0]='\0';
		strcat(buf,"df");
		strcat(buf,itoa(i+1,buf3,10));
		while (strcmp(cur->perem->getName(),buf))
			cur=cur->next;
		cur->perem->Clear();
		cur=cur->next;
	}
	x_name=new char[strlen(mx)+1];
	x_name[0]='\0';
	strcat(x_name,mx);
	results=new point_type[mrazr*mn];
	ish_Koshi=new double[razr+1];
	ish_Koshi[0]=mish_Koshi[0];
	min=new double[razr];
	max=new double[razr];
	for (int i=1;i<razr+1;i++)
	{
		results[(i-1)*n+n_iter].x[0]='\0';
		strcat(results[(i-1)*n+n_iter].x,_gcvt(ish_Koshi[0],10,buf));
		results[(i-1)*n+n_iter].y[0]='\0';
		strcat(results[(i-1)*n+n_iter].y,_gcvt(mish_Koshi[i],10,buf));
		ish_Koshi[i]=mish_Koshi[i];
		min[i-1]=ish_Koshi[i];
		max[i-1]=ish_Koshi[i];
	}
	cur=new perem_list;
	cur->next=first;
	cur->perem=new VYRAZ(x_name,_gcvt(ish_Koshi[0],10,buf));
	first=cur;
	xi=first->perem;
	yi=new VYRAZ*[razr];
	yibuf=new char*[razr];
	flag_stop=new bool[razr];
	for (int i=0;i<razr;i++)
	{
		flag_stop[i]=false;
		yibuf[i]=NULL;
		cur=new perem_list;
		cur->next=first;
		buf[0]='\0';
		strcat(buf,"f");
		strcat(buf,itoa(i+1,buf2,10));
		cur->perem=new VYRAZ(buf,_gcvt(ish_Koshi[i+1],10,buf2));
		first=cur;
		yi[i]=first->perem;
	}	
	temp_file=fopen("temp","w+");
}

void Solver::Print_results()
{
	double temp_x,temp_y;
	fputs("Результаты\n",log_file);
	fputs("Результаты\n",temp_file);
	for (int i=0;i<n;i++)
	{
		if (strcmp(results[i].x,"ERROR"))
		{
			temp_x=atof(results[i].x);
			fprintf(log_file,"%s=%14.8g    ",x_name,temp_x);
			fprintf(temp_file,"%s=%14.8g    ",x_name,temp_x);
		}
		else
		{
			fprintf(log_file,"%s=ERROR           ",x_name);
			fprintf(temp_file,"%s=ERROR           ",x_name);
		}
		for (int j=0;j<razr;j++)
		{
			if (strcmp(results[j*n+i].y,"ERROR"))
			{
				temp_y=atof(results[j*n+i].y);
				fprintf(log_file,"f%d=%14.8g    ",j+1,temp_y);
				fprintf(temp_file,"f%d=%14.8g    ",j+1,temp_y);
			}
			else
			{
				fprintf(log_file,"f%d=ERROR           ",j+1);
				fprintf(temp_file,"f%d=ERROR           ",j+1);
			}
		}
		fprintf(log_file,"\n");
		fprintf(temp_file,"\n");
	}
	if (a>b)
	{
		for (int j=0;j<razr;j++)
		{
			fprintf(log_file,"Минимальное значение функции f%d на отрезке [%g,%g] - %g\n",j+1,b,a,min[j]);
			fprintf(log_file,"Максимальное значение функции f%d на отрезке [%g,%g] - %g\n",j+1,b,a,max[j]);
			fprintf(temp_file,"Минимальное значение функции f%d на отрезке [%g,%g] - %g\n",j+1,b,a,min[j]);
			fprintf(temp_file,"Максимальное значение функции f%d на отрезке [%g,%g] - %g\n",j+1,b,a,max[j]);
		}
	}
	else
	{
		for (int j=0;j<razr;j++)
		{
			fprintf(log_file,"Минимальное значение функции f%d на отрезке [%g,%g] - %g\n",j+1,a,b,min[j]);
			fprintf(log_file,"Максимальное значение функции f%d на отрезке [%g,%g] - %g\n",j+1,a,b,max[j]);
			fprintf(temp_file,"Минимальное значение функции f%d на отрезке [%g,%g] - %g\n",j+1,a,b,min[j]);
			fprintf(temp_file,"Максимальное значение функции f%d на отрезке [%g,%g] - %g\n",j+1,a,b,max[j]);
		}
	}
	fflush(log_file);
	fflush(temp_file);
}
void Solver::solve()
{
	bool flag_stop_local;
	perem_list *cur;
	int len_cycle;
	//Первый полуинтервал
	if ((strncmp(h->getFormula(),"-1.#IND",7)) && (strncmp(h->getFormula(),"-1.#INF",7)))
	{
		xi->setFormula(_gcvt(ish_Koshi[0],10,buf));
		for (int i=0;i<razr;i++)
		{
			yi[i]->setFormula(_gcvt(ish_Koshi[i+1],10,buf));
			_gcvt(ish_Koshi[i+1],10,buf);
		}
		double buf_doubl;
		for (int i=0;i<n_iter;i++)
		{
			flag_stop_local=false;
			for (int j=0;j<razr;j++)
			{
				if (!flag_stop[j])
				{
					if (!zakon[j]->getResult())
						flag_stop[j]=true;
					if (!flag_stop[j])
					{
						buf[0]='\0';
						strcat(buf,zakon[j]->getResult());
						if (i==0)		
						{
							zakon[j]->setFlag_iter(true);
							zakon[j]->setFlag_iter_leks(false);
							dop_solve(j);
							h->setFlag_iter_leks(false);
							cur=first;
							buf2[0]='\0';
							strcat(buf2,"df");
							strcat(buf2,itoa(j+1,buf3,10));
							while (strcmp(cur->perem->getName(),buf2))
								cur=cur->next;
							cur->perem->setFlag_iter_leks(false);
						}
						if (yibuf[j])
							delete [] yibuf[j];
						yibuf[j]=new char[strlen(buf)+1];
						yibuf[j][0]='\0';
						strcat(yibuf[j],buf);
						results[j*n+n_iter-1-i].y[0]='\0';
						strcat(results[j*n+n_iter-1-i].y,buf);
						if (atof(buf)>max[j])
							max[j]=atof(buf);
						if (atof(buf)<min[j])
							min[j]=atof(buf);
					}
					else
					{
						results[j*n+n_iter-1-i].y[0]='\0';
						strcat(results[j*n+n_iter-1-i].y,"ERROR");
						if (yibuf[j])
							delete [] yibuf[j];
						yibuf[j]=new char[strlen("ERROR")+1];
						yibuf[j][0]='\0';
						strcat(yibuf[j],"ERROR");
					}
				}
				else
				{
					results[j*n+n_iter-1-i].y[0]='\0';
					strcat(results[j*n+n_iter-1-i].y,"ERROR");
					if (yibuf[j])
						delete [] yibuf[j];
					yibuf[j]=new char[strlen("ERROR")+1];
					yibuf[j][0]='\0';
					strcat(yibuf[j],"ERROR");
				}
			}
			if (i!=n_iter-1)
			{
				if (!xi->getResult())
					flag_stop_local=true;
				if (!flag_stop_local)
				{
					buf[0]='\0';
					strcat(buf,xi->getResult());
					if (i==1)
						xi->setFlag_iter(true);
					buf_doubl=atof(buf);
					if (!h->getResult())
						flag_stop_local=true;
					if (!flag_stop_local)
					{
						buf[0]='\0';
						strcat(buf,h->getResult());
						if (i==1)
							h->setFlag_iter(true);
						buf_doubl+=atof(buf);
						xi->setFormula(_gcvt(buf_doubl,10,buf));
					}
				}
			}
			else
			{
				if (!xi->getResult())
					flag_stop_local=true;
				if (b>a)
					xi->setFormula(_gcvt(a,10,buf));
				else
					xi->setFormula(_gcvt(b,10,buf));
			}
			VYRAZ::provChange(x_name,first);
			if (!flag_stop_local)
			{			
				for (int j=0;j<razr;j++)
				{
					buf[0]='\0';
					strcat(buf,yibuf[j]);
					yi[j]->setFormula(buf);
					buf[0]='\0';
					strcat(buf,"f");
					strcat(buf,itoa(j+1,buf2,10));
					VYRAZ::provChange(buf,first);
					results[j*n+n_iter-1-i].x[0]='\0';
					strcat(results[j*n+n_iter-1-i].x,xi->getFormula());
				}
			}
			else
			{
				for (int j=0;j<razr;j++)
				{
					results[j*n+n_iter-1-i].x[0]='\0';
					strcat(results[j*n+n_iter-1-i].x,"ERROR");
				}
			}
		}
	}
	//Второй полуинтервал
	n_iter=n-1-n_iter;
	if (a>b)
		h->setFormula(_gcvt((a-ish_Koshi[0])/n_iter,10,buf));
	else
		h->setFormula(_gcvt((b-ish_Koshi[0])/n_iter,10,buf));
	VYRAZ::provChange("h",first);
	h->setFlag_iter_leks(true);
	flag_stop_local=false;
	for (int i=0;i<razr;i++)
		flag_stop[i]=false;
	if ((strncmp(h->getFormula(),"-1.#IND",7)) && (strncmp(h->getFormula(),"1.#INF",6)))
	{
		xi->setFormula(_gcvt(ish_Koshi[0],10,buf));
		for (int i=0;i<razr;i++)
		{
			yi[i]->setFormula(_gcvt(ish_Koshi[i+1],10,buf));
			_gcvt(ish_Koshi[i+1],10,buf);
		}
		double buf_doubl;
		for (int i=0;i<n_iter;i++)
		{
			flag_stop_local=false;
			for (int j=0;j<razr;j++)
			{
				if (!flag_stop[j])
				{
					if (!zakon[j]->getResult())
						flag_stop[j]=true;
					if (!flag_stop[j])
					{
						buf[0]='\0';
						strcat(buf,zakon[j]->getResult());
						if (i==0)		
						{
							zakon[j]->setFlag_iter(true);
							zakon[j]->setFlag_iter_leks(false);
							dop_solve(j);
							h->setFlag_iter_leks(false);
							cur=first;
							buf2[0]='\0';
							strcat(buf2,"df");
							strcat(buf2,itoa(j+1,buf3,10));
							while (strcmp(cur->perem->getName(),buf2))
								cur=cur->next;
							cur->perem->setFlag_iter_leks(false);
						}
						if (yibuf[j])
							delete [] yibuf[j];
						yibuf[j]=new char[strlen(buf)+1];
						yibuf[j][0]='\0';
						strcat(yibuf[j],buf);
						results[j*n+n-n_iter+i].y[0]='\0';
						strcat(results[j*n+n-n_iter+i].y,buf);
						if (atof(buf)>max[j])
							max[j]=atof(buf);
						if (atof(buf)<min[j])
							min[j]=atof(buf);
					}
					else
					{
						results[j*n+n-n_iter+i].y[0]='\0';
						strcat(results[j*n+n-n_iter+i].y,"ERROR");
						if (yibuf[j])
							delete [] yibuf[j];
						yibuf[j]=new char[strlen("ERROR")+1];
						yibuf[j][0]='\0';
						strcat(yibuf[j],"ERROR");
					}
				}
				else
				{
					results[j*n+n-n_iter+i].y[0]='\0';
					strcat(results[j*n+n-n_iter+i].y,"ERROR");
					if (yibuf[j])
						delete [] yibuf[j];
					yibuf[j]=new char[strlen("ERROR")+1];
					yibuf[j][0]='\0';
					strcat(yibuf[j],"ERROR");
				}
			}
			if (i!=n_iter-1)
			{
				if (!xi->getResult())
					flag_stop_local=true;
				if (!flag_stop_local)
				{
					buf[0]='\0';
					strcat(buf,xi->getResult());
					if (i==1)
						xi->setFlag_iter(true);
					buf_doubl=atof(buf);
					if (!h->getResult())
						flag_stop_local=true;
					if (!flag_stop_local)
					{
						buf[0]='\0';
						strcat(buf,h->getResult());
						if (i==1)
							h->setFlag_iter(true);
						buf_doubl+=atof(buf);
						xi->setFormula(_gcvt(buf_doubl,10,buf));
					}
				}
			}
			else
			{
				if (!xi->getResult())
					flag_stop_local=true;
				if (b>a)
					xi->setFormula(_gcvt(b,10,buf));
				else
					xi->setFormula(_gcvt(a,10,buf));
			}
			VYRAZ::provChange(x_name,first);
			if (!flag_stop_local)
			{			
				for (int j=0;j<razr;j++)
				{
					buf[0]='\0';
					strcat(buf,yibuf[j]);
					yi[j]->setFormula(buf);
					buf[0]='\0';
					strcat(buf,"f");
					strcat(buf,itoa(j+1,buf2,10));
					VYRAZ::provChange(buf,first);
					results[j*n+n-n_iter+i].x[0]='\0';
					strcat(results[j*n+n-n_iter+i].x,xi->getFormula());
				}
			}
			else
			{
				for (int j=0;j<razr;j++)
				{
					results[j*n+n-n_iter+i].x[0]='\0';
					strcat(results[j*n+n-n_iter+i].x,"ERROR");
				}
			}
		}
	}
}
bool Eyler::provX(char *x,int razr, char **res)
{
	/*В данной программе есть одно допущение - имена переменных для решения с.д.у. 
	Так, уравнения начинаются с df1..., законы с zakon1..., а текущие значения с f1...
	Поэтому необходимо, чтобы имя переменной аргумента не совпадало ни с одним из этих*/
	char buf[150],buf2[150];
	bool flag=true;
	int i=1;
	while ((i<=razr) && flag)
	{
		buf[0]='\0';
		strcat(buf,"df");
		strcat(buf,itoa(i,buf2,10));
		if (!strcmp(x,buf))
			flag=false;
		else
			i++;
	}
	if (!flag)
	{
		fprintf(log_file,"%s является зарезервированным именем программы. Используйте другое имя.\n",buf);
		buf2[0]='\0';
		strcat(buf2,buf);
		strcat(buf2," является зарезервированным именем программы. Используйте другое имя.");
	}
	else
	{
		buf[0]='\0';
		strcat(buf,"h");
		if (!strcmp(x,buf))
			flag=false;
		i=1;
		while ((i<=razr) && flag)
		{
			buf[0]='\0';
			strcat(buf,"zakon");
			strcat(buf,itoa(i,buf2,10));
			if (!strcmp(x,buf))
				flag=false;
			else
			{
				buf[0]='\0';
				strcat(buf,"f");
				strcat(buf,itoa(i,buf2,10));
				if (!strcmp(x,buf))
					flag=false;
				else
					i++;
			}
		}
		if (!flag)
		{
			fprintf(log_file,"%s является зарезервированным именем программы. Используйте другое имя.\n",buf);
			buf2[0]='\0';
			strcat(buf2,buf);
			strcat(buf2," является зарезервированным именем программы. Используйте другое имя.");
		}
	}
	if (flag)
	{
		buf2[0]='\0';
		strcat(buf2,"OK");
	}
	if (*res)
		delete [] *res;
	*res=new char[strlen(buf2)+1];
	*res[0]='\0';
	strcat(*res,buf2);
	return flag;
}
Eyler::Eyler(char *ma,char *mb,int mn, int mrazr, double *mish_Koshi, char *mx):Solver(ma,mb,mn, mrazr, mish_Koshi, mx)
{
	perem_list *cur;
	zakon=new VYRAZ*[razr];
	for (int i=0;i<razr;i++)
	{
		cur=new perem_list;
		cur->next=first;
		buf2[0]='\0';
		strcat(buf2,"zakon");
		strcat(buf2,itoa(i+1,buf,10));
		buf[0]='\0';
		strcat(buf,"f");
		strcat(buf,itoa(i+1,buf3,10)); 
		strcat(buf,"+h*df");
		strcat(buf,itoa(i+1,buf3,10));
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		zakon[i]=first->perem;
	}
	fprintf(temp_file,"Входные данные:\n");
	fprintf(temp_file,"    Разрядность системы: %d\n",razr);
	fprintf(temp_file,"    Метод: Эйлера\n");
	fprintf(temp_file,"    Имя переменной-аргумента: %s\n",x_name);
	fprintf(temp_file,"Система дифференциальных уравнений:\n");
	fprintf(log_file,"Входные данные:\n");
	fprintf(log_file,"    Разрядность системы: %d\n",razr);
	fprintf(log_file,"    Метод: Эйлера\n");
	fprintf(log_file,"    Имя переменной-аргумента: %s\n",x_name);
	fprintf(log_file,"Система дифференциальных уравнений:\n");
	perem_list *cur_f;
	cur_f=first;
	while (strcmp(cur_f->perem->getName(),"df1"))
		cur_f=cur_f->next;
	cur=cur_f;
	// Вывод системы д.у. в файл
	for (int i=0;i<razr;i++)
	{
		fprintf(temp_file,"    df%d/d%s = %s\n",i+1,x_name,cur->perem->getFormula());
		fprintf(log_file,"    df%d/d%s = %s\n",i+1,x_name,cur->perem->getFormula());
		cur=cur->next;
	}
	fprintf(temp_file,"Отрезок интегрирования: [%g;%g]\n",a,b);
	fprintf(temp_file,"Количество точек на отрезке: %d\n",n);
	fprintf(temp_file,"Значение аргумента в задаче Коши: %s0=%g\n",x_name,ish_Koshi[0]);
	fprintf(temp_file,"Условия задачи Коши:\n");
	fprintf(log_file,"Отрезок интегрирования: [%g;%g]\n",a,b);
	fprintf(log_file,"Количество точек на отрезке: %d\n",n);
	fprintf(log_file,"Значение аргумента в задаче Коши: %s0=%g\n",x_name,ish_Koshi[0]);
	fprintf(log_file,"Условия задачи Коши:\n");
	for (int i=0;i<razr;i++)
	{
		fprintf(temp_file,"f%d(%s0) = %g\n",i+1,x_name,ish_Koshi[i+1]);
		fprintf(log_file,"f%d(%s0) = %g\n",i+1,x_name,ish_Koshi[i+1]);
	}
	if (cur)
	{
		char *buf=NULL;
		int j=0;
		fprintf(temp_file,"Дополнительные переменные\n");
		fprintf(log_file,"Дополнительные переменные\n");
		while (cur)
		{
			fprintf(temp_file,"%d\n",j+1);
			fprintf(log_file,"%d\n",j+1);
			cur->perem->Print_name();
			cur->perem->Print_formula();
			cur->perem->Print_result(&buf);
			fprintf(temp_file,"%s\n",cur->perem->getName());
			fprintf(temp_file,"%s\n",cur->perem->getFormula());
			fprintf(temp_file,"%s\n\n",buf);
			cur=cur->next;
		}
	}
	fflush(temp_file);
	fflush(log_file);
}
void Eyler::dop_solve(int j)
{
	perem_list *cur;
	cur=first;
	int len_cycle=3*razr+2;
	for (int i=0;i<len_cycle;i++)
		cur=cur->next;
	while (cur)
	{
		cur->perem->setFlag_iter_leks(false);
		cur=cur->next;
	}
}
bool RKII::provX(char *x,int razr,char **res)
{
	/*В данной программе есть одно допущение - имена переменных для решения с.д.у. 
	Так, уравнения начинаются с df1..., законы с zakon1..., а текущие значения с f1...
	Поэтому необходимо, чтобы имя переменной аргумента не совпадало ни с одним из этих*/
	char buf[150],buf2[150];
	bool flag=true;
	int i=1,j;
	while ((i<=razr) && flag)
	{
		buf[0]='\0';
		strcat(buf,"df");
		strcat(buf,itoa(i,buf2,10));
		if (!strcmp(x,buf))
			flag=false;
		else
			i++;
	}
	if (!flag)
	{
		fprintf(log_file,"%s является зарезервированным именем программы. Используйте другое имя.\n",buf);
		buf2[0]='\0';
		strcat(buf2,buf);
		strcat(buf2," является зарезервированным именем программы. Используйте другое имя.");
	}
	else
	{
		i=1;
		buf[0]='\0';
		strcat(buf,"h");
		if (!strcmp(x,buf))
			flag=false;
		while ((i<=razr) && flag)
		{
			buf[0]='\0';
			strcat(buf,"zakon");
			strcat(buf,itoa(i,buf2,10));
			if (!strcmp(x,buf))
				flag=false;
			else
			{
				buf[0]='\0';
				strcat(buf,"f");
				strcat(buf,itoa(i,buf2,10));
				if (!strcmp(x,buf))
					flag=false;
				else
				{
					j=1;
					while (flag && (j<=2))
					{
						buf[0]='\0';
						strcat(buf,"k");
						strcat(buf,itoa(j,buf2,10));
						strcat(buf,itoa(i,buf2,10));
						if (!strcmp(x,buf))
							flag=false;
						else
							j++;
					}
					if (flag)
						i++;
				}
			}
		}
		if (!flag)
		{
			fprintf(log_file,"%s является зарезервированным именем программы. Используйте другое имя.\n",buf);
			buf2[0]='\0';
			strcat(buf2,buf);
			strcat(buf2," является зарезервированным именем программы. Используйте другое имя.");
		}
	}
	if (flag)
	{
		buf2[0]='\0';
		strcat(buf2,"OK");
	}
	if (*res)
		delete [] *res;
	*res=new char[strlen(buf2)+1];
	*res[0]='\0';
	strcat(*res,buf2);
	return flag;
}
RKII::RKII(char *ma,char *mb,int mn, int mrazr, double *mish_Koshi, char *mx):Solver(ma,mb,mn, mrazr, mish_Koshi, mx)
{
	int j,len,len1;
	bool flag_ch1,flag_ch2;
	char buf4[150];
	perem_list *cur;
	zakon=new VYRAZ*[razr];
	k1=new VYRAZ*[razr];
	k2=new VYRAZ*[razr];
	for (int i=0;i<razr;i++)
	{
		buf2[0]='\0';
		strcat(buf2,"k1");
		strcat(buf2,itoa(i+1,buf,10));
		buf[0]='\0';
		strcat(buf,"h*df");
		strcat(buf,itoa(i+1,buf3,10));
		cur=new perem_list;
		cur->next=first;
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		k1[i]=first->perem;
		buf[0]='\0';
		strcat(buf,"df");
		strcat(buf,itoa(i+1,buf3,10));
		cur=first;
		while (strcmp(cur->perem->getName(),buf))
			cur=cur->next;
		j=0;
		len=strlen(cur->perem->getFormula());
		buf3[0]='\0';
		strcat(buf3,cur->perem->getFormula());
		buf[0]='\0';
		strcat(buf,"h*(");
		while (j<len)
		{
			flag_ch1=false;
			flag_ch2=false;
			if ((buf3[j]==x_name[0]) && ((j==0) || (!VYRAZ::cyfr(buf3[j-1]) && !VYRAZ::bukv(buf3[j-1])))) // поиск аргумента - первый параметр
			{
				if (!strncmp(buf3+j,x_name,strlen(x_name)))
				{
					len1=strlen(x_name);
					if ((j+len1==len) || (!VYRAZ::cyfr(buf3[j+len1]) && !VYRAZ::bukv(buf3[j+len1])))
					{
						strcat(buf,"(");
						strcat(buf,x_name);
						strcat(buf,"+h)");
						j+=strlen(x_name);
						flag_ch1=true;
					}
				}
			}
			if (!flag_ch1)
				if ((buf3[j]=='f') && ((j==0) || (!VYRAZ::cyfr(buf3[j-1]) && !VYRAZ::bukv(buf3[j-1]))))
				{
					buf2[0]='\0';
					strcat(buf2,"f");
					strcat(buf2,itoa(i+1,buf4,10));
					if (!strncmp(buf3+j,buf2,strlen(buf2)))
					{
						len1=strlen(buf2);
						if ((j+len1==len) || (!VYRAZ::cyfr(buf3[j+len1]) && !VYRAZ::bukv(buf3[j+len1])))
						{
							strcat(buf,"(");
							strcat(buf,buf2);
							j+=strlen(buf2);
							buf2[0]='\0';
							strcat(buf2,"k1");
							strcat(buf2,itoa(i+1,buf4,10));
							strcat(buf,"+");
							strcat(buf,buf2);
							strcat(buf,")");
							flag_ch2=true;
						}
					}
				}
			if ((!flag_ch1) && (!flag_ch2))
			{
				buf[strlen(buf)+1]='\0';
				buf[strlen(buf)]=buf3[j];
				j++;
			}
		}
		strcat(buf,")");
		buf2[0]='\0';
		strcat(buf2,"k2");
		strcat(buf2,itoa(i+1,buf4,10));
		cur=new perem_list;
		cur->next=first;
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		k2[i]=first->perem;
		cur=new perem_list;
		cur->next=first;
		buf2[0]='\0';
		strcat(buf2,"zakon");
		strcat(buf2,itoa(i+1,buf,10));
		buf[0]='\0';
		strcat(buf,"f");
		strcat(buf,itoa(i+1,buf3,10)); 
		strcat(buf,"+0.5*(k1");
		strcat(buf,itoa(i+1,buf3,10));
		strcat(buf,"+k2");
		strcat(buf,itoa(i+1,buf3,10));
		strcat(buf,")");
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		zakon[i]=first->perem;
	}
	fprintf(temp_file,"Входные данные:\n");
	fprintf(temp_file,"    Разрядность системы: %d\n",razr);
	fprintf(temp_file,"    Метод: Рунге-Кутта II порядка\n");
	fprintf(temp_file,"    Имя переменной-аргумента: %s\n",x_name);
	fprintf(temp_file,"Система дифференциальных уравнений:\n");
	fprintf(log_file,"Входные данные:\n");
	fprintf(log_file,"    Разрядность системы: %d\n",razr);
	fprintf(log_file,"    Метод: Рунге-Кутта II порядка\n");
	fprintf(log_file,"    Имя переменной-аргумента: %s\n",x_name);
	fprintf(log_file,"Система дифференциальных уравнений:\n");
	perem_list *cur_f;
	cur_f=first;
	while (strcmp(cur_f->perem->getName(),"df1"))
		cur_f=cur_f->next;
	cur=cur_f;
	// Вывод системы д.у. в файл
	for (int i=0;i<razr;i++)
	{
		fprintf(temp_file,"    df%d/d%s = %s\n",i+1,x_name,cur->perem->getFormula());
		fprintf(log_file,"    df%d/d%s = %s\n",i+1,x_name,cur->perem->getFormula());
		cur=cur->next;
	}
	fprintf(temp_file,"Отрезок интегрирования: [%g;%g]\n",a,b);
	fprintf(temp_file,"Количество точек на отрезке: %d\n",n);
	fprintf(temp_file,"Значение аргумента в задаче Коши: %s0=%g\n",x_name,ish_Koshi[0]);
	fprintf(temp_file,"Условия задачи Коши:\n");
	fprintf(log_file,"Отрезок интегрирования: [%g;%g]\n",a,b);
	fprintf(log_file,"Количество точек на отрезке: %d\n",n);
	fprintf(log_file,"Значение аргумента в задаче Коши: %s0=%g\n",x_name,ish_Koshi[0]);
	fprintf(log_file,"Условия задачи Коши:\n");
	for (int i=0;i<razr;i++)
	{
		fprintf(temp_file,"f%d(%s0) = %g\n",i+1,x_name,ish_Koshi[i+1]);
		fprintf(log_file,"f%d(%s0) = %g\n",i+1,x_name,ish_Koshi[i+1]);
	}
	if (cur)
	{
		char *buf=NULL;
		int j=0;
		fprintf(temp_file,"Дополнительные переменные\n");
		fprintf(log_file,"Дополнительные переменные\n");
		while (cur)
		{
			fprintf(temp_file,"%d\n",j+1);
			fprintf(log_file,"%d\n",j+1);
			cur->perem->Print_name();
			cur->perem->Print_formula();
			cur->perem->Print_result(&buf);
			fprintf(temp_file,"%s\n",cur->perem->getName());
			fprintf(temp_file,"%s\n",cur->perem->getFormula());
			fprintf(temp_file,"%s\n\n",buf);
			cur=cur->next;
		}
	}
	fflush(temp_file);
	fflush(log_file);
}
void RKII::dop_solve(int j)
{
	k1[j]->setFlag_iter_leks(false);
	k2[j]->setFlag_iter_leks(false);
	perem_list *cur;
	cur=first;
	int len_cycle=5*razr+2;
	for (int i=0;i<len_cycle;i++)
		cur=cur->next;
	while (cur)
	{
		cur->perem->setFlag_iter_leks(false);
		cur=cur->next;
	}	
}
bool RKIV::provX(char *x,int razr,char **res)
{
	/*В данной программе есть одно допущение - имена переменных для решения с.д.у. 
	Так, уравнения начинаются с df1..., законы с zakon1..., а текущие значения с f1...
	Поэтому необходимо, чтобы имя переменной аргумента не совпадало ни с одним из этих*/
	char buf[150],buf2[150];
	bool flag=true;
	int i=1,j;
	while ((i<=razr) && flag)
	{
		buf[0]='\0';
		strcat(buf,"df");
		strcat(buf,itoa(i,buf2,10));
		if (!strcmp(x,buf))
			flag=false;
		else
			i++;
	}
	if (!flag)
	{
		fprintf(log_file,"%s является зарезервированным именем программы. Используйте другое имя.\n",buf);
		buf2[0]='\0';
		strcat(buf2,buf);
		strcat(buf2," является зарезервированным именем программы. Используйте другое имя.");
	}
	else
	{
		i=1;
		buf[0]='\0';
		strcat(buf,"h");
		if (!strcmp(x,buf))
			flag=false;
		while ((i<=razr) && flag)
		{
			buf[0]='\0';
			strcat(buf,"zakon");
			strcat(buf,itoa(i,buf2,10));
			if (!strcmp(x,buf))
				flag=false;
			else
			{
				buf[0]='\0';
				strcat(buf,"f");
				strcat(buf,itoa(i,buf2,10));
				if (!strcmp(x,buf))
					flag=false;
				else
				{
					j=1;
					while (flag && (j<=4))
					{
						buf[0]='\0';
						strcat(buf,"k");
						strcat(buf,itoa(j,buf2,10));
						strcat(buf,itoa(i,buf2,10));
						if (!strcmp(x,buf))
							flag=false;
						else
							j++;
					}
					if (flag)
						i++;
				}
			}
		}
		if (!flag)
		{
			fprintf(log_file,"%s является зарезервированным именем программы. Используйте другое имя.\n",buf);
			buf2[0]='\0';
			strcat(buf2,buf);
			strcat(buf2," является зарезервированным именем программы. Используйте другое имя.");
		}
	}
	if (flag)
	{
		buf2[0]='\0';
		strcat(buf2,"OK");
	}
	if (*res)
		delete [] *res;
	*res=new char[strlen(buf2)+1];
	*res[0]='\0';
	strcat(*res,buf2);
	return flag;
}
RKIV::RKIV(char *ma,char *mb,int mn, int mrazr, double *mish_Koshi, char *mx):Solver(ma,mb,mn, mrazr, mish_Koshi, mx)
{
	int j,len,len1;
	bool flag_ch1,flag_ch2;
	char buf4[150];
	perem_list *cur;
	zakon=new VYRAZ*[razr];
	k1=new VYRAZ*[razr];
	k2=new VYRAZ*[razr];
	k3=new VYRAZ*[razr];
	k4=new VYRAZ*[razr];
	for (int i=0;i<razr;i++)
	{
		buf2[0]='\0';
		strcat(buf2,"k1");
		strcat(buf2,itoa(i+1,buf,10));
		buf[0]='\0';
		strcat(buf,"h*df");
		strcat(buf,itoa(i+1,buf3,10));
		cur=new perem_list;
		cur->next=first;
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		k1[i]=first->perem;
		buf[0]='\0';
		strcat(buf,"df");
		strcat(buf,itoa(i+1,buf3,10));
		cur=first;
		while (strcmp(cur->perem->getName(),buf))
			cur=cur->next;
		j=0;
		len=strlen(cur->perem->getFormula());
		buf3[0]='\0';
		strcat(buf3,cur->perem->getFormula());
		buf[0]='\0';
		strcat(buf,"h*(");
		while (j<len)
		{
			flag_ch1=false;
			flag_ch2=false;
			if ((buf3[j]==x_name[0]) && ((j==0) || (!VYRAZ::cyfr(buf3[j-1]) && !VYRAZ::bukv(buf3[j-1])))) // поиск аргумента - первый параметр
			{
				if (!strncmp(buf3+j,x_name,strlen(x_name)))
				{
					len1=strlen(x_name);
					if ((j+len1==len) || (!VYRAZ::cyfr(buf3[j+len1]) && !VYRAZ::bukv(buf3[j+len1])))
					{
						strcat(buf,"(");
						strcat(buf,x_name);
						strcat(buf,"+h/2)");
						j+=strlen(x_name);
						flag_ch1=true;
					}
				}
			}
			if (!flag_ch1)
				if ((buf3[j]=='f') && ((j==0) || (!VYRAZ::cyfr(buf3[j-1]) && !VYRAZ::bukv(buf3[j-1]))))
				{
					buf2[0]='\0';
					strcat(buf2,"f");
					strcat(buf2,itoa(i+1,buf4,10));
					if (!strncmp(buf3+j,buf2,strlen(buf2)))
					{
						len1=strlen(buf2);
						if ((j+len1==len) || (!VYRAZ::cyfr(buf3[j+len1]) && !VYRAZ::bukv(buf3[j+len1])))
						{
							strcat(buf,"(");
							strcat(buf,buf2);
							j+=strlen(buf2);
							buf2[0]='\0';
							strcat(buf2,"k1");
							strcat(buf2,itoa(i+1,buf4,10));
							strcat(buf,"+");
							strcat(buf,buf2);
							strcat(buf,"/2)");
							flag_ch2=true;
						}
					}
				}
			if ((!flag_ch1) && (!flag_ch2))
			{
				buf[strlen(buf)+1]='\0';
				buf[strlen(buf)]=buf3[j];
				j++;
			}
		}
		strcat(buf,")");
		buf2[0]='\0';
		strcat(buf2,"k2");
		strcat(buf2,itoa(i+1,buf4,10));
		cur=new perem_list;
		cur->next=first;
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		k2[i]=first->perem;
		buf[0]='\0';
		strcat(buf,"h*(");
		j=0;
		while (j<len)
		{
			flag_ch1=false;
			flag_ch2=false;
			if ((buf3[j]==x_name[0]) && ((j==0) || (!VYRAZ::cyfr(buf3[j-1]) && !VYRAZ::bukv(buf3[j-1])))) // поиск аргумента - первый параметр
			{
				if (!strncmp(buf3+j,x_name,strlen(x_name)))
				{
					len1=strlen(x_name);
					if ((j+len1==len) || (!VYRAZ::cyfr(buf3[j+len1]) && !VYRAZ::bukv(buf3[j+len1])))
					{
						strcat(buf,"(");
						strcat(buf,x_name);
						strcat(buf,"+h/2)");
						j+=strlen(x_name);
						flag_ch1=true;
					}
				}
			}
			if (!flag_ch1)
				if ((buf3[j]=='f') && ((j==0) || (!VYRAZ::cyfr(buf3[j-1]) && !VYRAZ::bukv(buf3[j-1]))))
				{
					buf2[0]='\0';
					strcat(buf2,"f");
					strcat(buf2,itoa(i+1,buf4,10));
					if (!strncmp(buf3+j,buf2,strlen(buf2)))
					{
						len1=strlen(buf2);
						if ((j+len1==len) || (!VYRAZ::cyfr(buf3[j+len1]) && !VYRAZ::bukv(buf3[j+len1])))
						{
							strcat(buf,"(");
							strcat(buf,buf2);
							j+=strlen(buf2);
							buf2[0]='\0';
							strcat(buf2,"k2");
							strcat(buf2,itoa(i+1,buf4,10));
							strcat(buf,"+");
							strcat(buf,buf2);
							strcat(buf,"/2)");
							flag_ch2=true;
						}
					}
				}
			if ((!flag_ch1) && (!flag_ch2))
			{
				buf[strlen(buf)+1]='\0';
				buf[strlen(buf)]=buf3[j];
				j++;
			}
		}
		strcat(buf,")");
		buf2[0]='\0';
		strcat(buf2,"k3");
		strcat(buf2,itoa(i+1,buf4,10));
		cur=new perem_list;
		cur->next=first;
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		k3[i]=first->perem;
		buf[0]='\0';
		strcat(buf,"h*(");
		j=0;
		while (j<len)
		{
			flag_ch1=false;
			flag_ch2=false;
			if ((buf3[j]==x_name[0]) && ((j==0) || (!VYRAZ::cyfr(buf3[j-1]) && !VYRAZ::bukv(buf3[j-1])))) // поиск аргумента - первый параметр
			{
				if (!strncmp(buf3+j,x_name,strlen(x_name)))
				{
					len1=strlen(x_name);
					if ((j+len1==len) || (!VYRAZ::cyfr(buf3[j+len1]) && !VYRAZ::bukv(buf3[j+len1])))
					{
						strcat(buf,"(");
						strcat(buf,x_name);
						strcat(buf,"+h)");
						j+=strlen(x_name);
						flag_ch1=true;
					}
				}
			}
			if (!flag_ch1)
				if ((buf3[j]=='f') && ((j==0) || (!VYRAZ::cyfr(buf3[j-1]) && !VYRAZ::bukv(buf3[j-1]))))
				{
					buf2[0]='\0';
					strcat(buf2,"f");
					strcat(buf2,itoa(i+1,buf4,10));
					if (!strncmp(buf3+j,buf2,strlen(buf2)))
					{
						len1=strlen(buf2);
						if ((j+len1==len) || (!VYRAZ::cyfr(buf3[j+len1]) && !VYRAZ::bukv(buf3[j+len1])))
						{
							strcat(buf,"(");
							strcat(buf,buf2);
							j+=strlen(buf2);
							buf2[0]='\0';
							strcat(buf2,"k3");
							strcat(buf2,itoa(i+1,buf4,10));
							strcat(buf,"+");
							strcat(buf,buf2);
							strcat(buf,")");
							flag_ch2=true;
						}
					}
				}
			if ((!flag_ch1) && (!flag_ch2))
			{
				buf[strlen(buf)+1]='\0';
				buf[strlen(buf)]=buf3[j];
				j++;
			}
		}
		strcat(buf,")");
		buf2[0]='\0';
		strcat(buf2,"k4");
		strcat(buf2,itoa(i+1,buf4,10));
		cur=new perem_list;
		cur->next=first;
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		k4[i]=first->perem;
		cur=new perem_list;
		cur->next=first;
		buf2[0]='\0';
		strcat(buf2,"zakon");
		strcat(buf2,itoa(i+1,buf,10));
		buf[0]='\0';
		strcat(buf,"f");
		strcat(buf,itoa(i+1,buf3,10)); 
		strcat(buf,"+1/6*(k1");
		strcat(buf,itoa(i+1,buf3,10));
		strcat(buf,"+2*k2");
		strcat(buf,itoa(i+1,buf3,10));
		strcat(buf,"+2*k3");
		strcat(buf,itoa(i+1,buf3,10));
		strcat(buf,"+k4");
		strcat(buf,itoa(i+1,buf3,10));
		strcat(buf,")");
		cur->perem=new VYRAZ(buf2,buf);
		first=cur;
		zakon[i]=first->perem;
	}	
	fprintf(temp_file,"Входные данные:\n");
	fprintf(temp_file,"    Разрядность системы: %d\n",razr);
	fprintf(temp_file,"    Метод: Рунге-Кутта IV порядка\n");
	fprintf(temp_file,"    Имя переменной-аргумента: %s\n",x_name);
	fprintf(temp_file,"Система дифференциальных уравнений:\n");
	fprintf(log_file,"Входные данные:\n");
	fprintf(log_file,"    Разрядность системы: %d\n",razr);
	fprintf(log_file,"    Метод: Рунге-Кутта IV порядка\n");
	fprintf(log_file,"    Имя переменной-аргумента: %s\n",x_name);
	fprintf(log_file,"Система дифференциальных уравнений:\n");
	perem_list *cur_f;
	cur_f=first;
	while (strcmp(cur_f->perem->getName(),"df1"))
		cur_f=cur_f->next;
	cur=cur_f;
	// Вывод системы д.у. в файл
	for (int i=0;i<razr;i++)
	{
		fprintf(temp_file,"    df%d/d%s = %s\n",i+1,x_name,cur->perem->getFormula());
		fprintf(log_file,"    df%d/d%s = %s\n",i+1,x_name,cur->perem->getFormula());
		cur=cur->next;
	}
	fprintf(temp_file,"Отрезок интегрирования: [%g;%g]\n",a,b);
	fprintf(temp_file,"Количество точек на отрезке: %d\n",n);
	fprintf(temp_file,"Значение аргумента в задаче Коши: %s0=%g\n",x_name,ish_Koshi[0]);
	fprintf(temp_file,"Условия задачи Коши:\n");
	fprintf(log_file,"Отрезок интегрирования: [%g;%g]\n",a,b);
	fprintf(log_file,"Количество точек на отрезке: %d\n",n);
	fprintf(log_file,"Значение аргумента в задаче Коши: %s0=%g\n",x_name,ish_Koshi[0]);
	fprintf(log_file,"Условия задачи Коши:\n");
	for (int i=0;i<razr;i++)
	{
		fprintf(temp_file,"f%d(%s0) = %g\n",i+1,x_name,ish_Koshi[i+1]);
		fprintf(log_file,"f%d(%s0) = %g\n",i+1,x_name,ish_Koshi[i+1]);
	}
	if (cur)
	{
		char *buf=NULL;
		int j=0;
		fprintf(temp_file,"Дополнительные переменные\n");
		fprintf(log_file,"Дополнительные переменные\n");
		while (cur)
		{
			fprintf(temp_file,"%d\n",j+1);
			fprintf(log_file,"%d\n",j+1);
			cur->perem->Print_name();
			cur->perem->Print_formula();
			cur->perem->Print_result(&buf);
			fprintf(temp_file,"%s\n",cur->perem->getName());
			fprintf(temp_file,"%s\n",cur->perem->getFormula());
			fprintf(temp_file,"%s\n\n",buf);
			cur=cur->next;
		}
	}
	fflush(temp_file);
	fflush(log_file);
}
void RKIV::dop_solve(int j)
{
	k1[j]->setFlag_iter_leks(false);
	k2[j]->setFlag_iter_leks(false);
	k3[j]->setFlag_iter_leks(false);
	k4[j]->setFlag_iter_leks(false);
	perem_list *cur;
	cur=first;
	int len_cycle=7*razr+2;
	for (int i=0;i<len_cycle;i++)
		cur=cur->next;
	while (cur)
	{
		cur->perem->setFlag_iter_leks(false);
		cur=cur->next;
	}
}