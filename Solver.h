#ifndef SOLVER_H
#define SOLVER_H
#include "VYRAZ.h"
class Solver
{
public:
	double a,b,*min,*max;
	int n_iter,n,razr;
	char buf[150], buf2[150], buf3[150], *x_name;
	VYRAZ **zakon, *xi, **yi, *h;
	char **yibuf;
	bool *flag_stop;
	struct point_type
	{
		char x[150],y[150];
	};
	point_type *results;
	double *ish_Koshi;
	FILE *temp_file;
	Solver(char *ma,char *mb,int mn, int mrazr, double *mish_Koshi, char *mx);
	void solve();
	virtual void dop_solve(int j){}
	void Print_results();
	virtual void delete_Temp_Variables(){}
	void delete_Temp_Common()
	{
		perem_list *cur;
		int len_cycle;
		len_cycle=2*razr+2;   // zakoni è yi, x, h
		for (int i=0;i<len_cycle;i++)  
		{
			cur=first->next;
			delete first;
			first=cur;
		}
	}
	virtual ~Solver()
	{
		delete [] results;
		delete [] min;
		delete [] max;
		delete [] ish_Koshi;
		delete [] yi;
		for (int i=0;i<razr;i++)
			if (yibuf[i])
			delete [] yibuf[i];
		delete [] yibuf;
		delete [] x_name;
		delete [] zakon;
		delete [] flag_stop;
		fclose(temp_file);
	}
};
class Eyler:public Solver
{
public:
	Eyler(char *ma,char *mb,int mn, int mrazr, double *mish_Koshi, char *mx);
	virtual void dop_solve(int j);
	static bool provX(char *name,int razr,char **res);
	virtual ~Eyler(){}
	virtual void delete_Temp_Variables()
	{
		delete_Temp_Common();
	}
};
class RKII:public Solver
{
public:
	VYRAZ **k1, **k2;
	RKII(char *ma,char *mb,int mn, int mrazr, double *mish_Koshi, char *mx);
	virtual void dop_solve(int j);
	static bool provX(char *name,int razr,char **res);
	virtual void delete_Temp_Variables()
	{
		perem_list *cur=first;
		int len_cycle=2*razr; 
		for (int i=0;i<len_cycle;i++)  
		{
			cur=first->next;
			delete first;
			first=cur;
		}
		delete_Temp_Common();
	}
	virtual ~RKII()
	{
		delete [] k1;
		delete [] k2;
	}
};
class RKIV:public Solver
{
public:
	VYRAZ **k1, **k2, **k3, **k4;
	RKIV(char *ma,char *mb,int mn, int mrazr, double *mish_Koshi, char *mx);
	virtual void dop_solve(int j);
	static bool provX(char *name,int razr,char **res);
	virtual void delete_Temp_Variables()
	{
		perem_list *cur;
		int len_cycle;
		len_cycle=4*razr; 
		for (int i=0;i<len_cycle;i++)  
		{
			cur=first->next;
			delete first;
			first=cur;
		}
		delete_Temp_Common();
	}
	virtual ~RKIV()
	{
		delete [] k1;
		delete [] k2;
		delete [] k3;
		delete [] k4;
	}
};
#endif