#ifndef VYRAZ_H
#define VYRAZ_H
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "locale.h"
#include "conio.h"
class VYRAZ;
struct perem_list
{
	VYRAZ *perem;
	perem_list *next;
};
extern FILE *log_file;
extern perem_list *first;
extern int perem_count_list;
class VYRAZ
{
public:
	struct elem
	{
		char *value;
		VYRAZ *adr;
		elem *next;
	};
	struct prov_func
	{
		int kol;
		prov_func *next;
	};

	struct temp_values
	{
		char *value;   //K в алгоритме 
		elem *el;
		temp_values *next;
	};
private:
	char *name;
	char *formula;
	char tokens[150];
	char buf[150];
	int kod_err;
	char *problem_in,*problem_perem,*problem_in_perem,*problem_vyraz;
	bool flag,flag_iter,flag_iter_leks;
	double *result;
	struct my_stek
	{
		char znak;
		int ind;
		my_stek *next;
	};
	elem *list_c, *list_p, *list_f, *list_p_simple;
	
public:
	void Clear()
	{
		tokens[0]='\0';
		kod_err=0;
		flag=true;
		flag_iter=false;
		flag_iter_leks=true;
		deleteResult();
		deleteProblems();
		elem *el;
		el=list_p;
		while (el)
		{
			list_p=list_p->next;
			delete [] el->value;
			delete el;
			el=list_p;
		}
		el=list_p_simple;
		while (el)
		{
			list_p_simple=list_p_simple->next;
			delete [] el->value;
			delete el;
			el=list_p_simple;
		}
		el=list_c;
		while (el)
		{
			list_c=list_c->next;
			delete [] el->value;
			delete el;
			el=list_c;
		}
		el=list_f;
		while (el)
		{
			list_f=list_f->next;
			delete [] el->value;
			delete el;
			el=list_f;
		}
	}
	void setFlag_iter(bool my_flag_iter)
	{
		flag_iter=my_flag_iter;
		elem *el_p;
		el_p=list_p_simple;
		while (el_p)
		{
			if ((el_p->adr) && (el_p->adr->flag_iter!=my_flag_iter))
				el_p->adr->setFlag_iter(my_flag_iter);
			el_p=el_p->next;
		}
	}
	bool getFlag()
	{
		return flag;
	}
	void setFlag_iter_leks(bool my_flag_iter_leks)
	{
		flag_iter_leks=my_flag_iter_leks;
	}
	VYRAZ(char *name_s,char *s)
	{
		name=new char[strlen(name_s)+1];
		name[0]='\0';
		strcat(name,name_s);
		formula=new char[strlen(s)+1];
		tokens[0]='\0';
		kod_err=0;
		strcpy(formula,s);
		flag=true;
		flag_iter=false;
		flag_iter_leks=true;
		result=NULL;
		problem_in=NULL;
		problem_perem=NULL;
		problem_in_perem=NULL;
		problem_vyraz=NULL;
		list_c=NULL;
		list_p=NULL;
		list_p_simple=NULL;
		list_f=NULL;
	}
	void Print()
	{
		char *buf_1=NULL;
		Print_name();
		Print_formula();
		Print_result(&buf_1);
		if (buf_1)
			delete [] buf_1;
		fprintf(log_file,"\n\n");
	}
	void setName(char *s_name)
	{
		if (name)
			delete [] name;
		name=new char[strlen(s_name)+1];
		name[0]='\0';
		strcat(name,s_name);
		flag=true;
		flag_iter=false;
		flag_iter_leks=true;
		if (result)
		{
			delete result;
			result=NULL;
		}
		kod_err=0;
		if (problem_in)
			delete [] problem_in;
		if (problem_perem)
			delete [] problem_perem;
		if (problem_in_perem)
			delete [] problem_in_perem;
		if (problem_vyraz)
			delete [] problem_vyraz;
		problem_in=NULL;
		problem_perem=NULL;
		problem_in_perem=NULL;
		problem_vyraz=NULL;
		elem *el_c,*el_p,*el_f;
		tokens[0]='\0';
		el_c=list_c;
		while (el_c)
		{
			list_c=list_c->next;
			delete [] el_c->value;
			delete el_c;
			el_c=list_c;
		}
		el_p=list_p;
		while (el_p)
		{
			list_p=list_p->next;
			delete [] el_p->value;
			delete el_p;
			el_p=list_p;
		}
		el_p=list_p_simple;
		while (el_p)
		{
			list_p_simple=list_p_simple->next;
			delete [] el_p->value;
			delete el_p;
			el_p=list_p_simple;
		}
		el_f=list_f;
		while (el_f)
		{
			list_f=list_f->next;
			delete [] el_f->value;
			delete el_f;
			el_f=list_f;
		}
	}
	void setFormula(char *s_formula)
	{
		if (formula)
			delete [] formula;
		formula=new char[strlen(s_formula)+1];
		formula[0]='\0';
		strcat(formula,s_formula);
		flag=true;
		if (result)
		{
			delete result;
			result=NULL;
		}
		kod_err=0;
		if (problem_in)
			delete [] problem_in;
		if (problem_perem)
			delete [] problem_perem;
		if (problem_in_perem)
			delete [] problem_in_perem;
		if (problem_vyraz)
			delete [] problem_vyraz;
		problem_in=NULL;
		problem_perem=NULL;
		problem_in_perem=NULL;
		problem_vyraz=NULL;
		if (!flag_iter)
		{
			elem *el_c,*el_p,*el_f;
			tokens[0]='\0';
			el_c=list_c;
			while (el_c)
			{
				list_c=list_c->next;
				delete [] el_c->value;
				delete el_c;
				el_c=list_c;
			}
			el_p=list_p;
			while (el_p)
			{
				list_p=list_p->next;
				delete [] el_p->value;
				delete el_p;
				el_p=list_p;
			}
			el_p=list_p_simple;
			while (el_p)
			{
				list_p_simple=list_p_simple->next;
				delete [] el_p->value;
				delete el_p;
				el_p=list_p_simple;
			}
			el_f=list_f;
			while (el_f)
			{
				list_f=list_f->next;
				delete [] el_f->value;
				delete el_f;
				el_f=list_f;
			}
		}
	}
	void Print_name()
	{
		fprintf(log_file,"ИМЯ ПЕРЕМЕННОЙ: ");
		fputs(name,log_file);
		fprintf(log_file,"\n");
	}
	void Print_formula()
	{
		fprintf(log_file,"ВЫРАЖЕНИЕ: ");
		fputs(formula,log_file);
		fprintf(log_file,"\n");
	}
	void Print_tokens()
	{
		fputs("Строка токенов",log_file);
		fprintf(log_file,"\n");
		fputs(tokens,log_file);
		fprintf(log_file,"\n");
	}
	void Print_result(char **res)
	{
		char buf2[350];
		if (result)
		{
			fprintf(log_file,"РЕЗУЛЬТАТ ВЫРАЖЕНИЯ: ");
			fputs(_gcvt(*result,10,buf),log_file);
			fprintf(log_file,"\n");
			fprintf(log_file,"\n");
			buf2[0]='\0';
			strcat(buf2,"РЕЗУЛЬТАТ ВЫРАЖЕНИЯ: ");
			strcat(buf2,buf);
		}
		else if (flag)
			{
				fputs("Значение выражения ещё не вычислено\n",log_file);
				fprintf(log_file,"\n");
				buf2[0]='\0';
				strcat(buf2,"Значение выражения ещё не вычислено");
			}
			else
			{
				fputs("В выражении есть ошибка\n",log_file);
				buf2[0]='\0';
				strcat(buf2,"В выражении есть ошибка.");
				fprintf(log_file,"\n");
				switch (kod_err)
				{
				case 1:
					{
						fputs("Ошибка на этапе лексического анализа",log_file);
						fprintf(log_file,"\n");
						fputs(problem_in,log_file);
						fprintf(log_file,"\n");
						strcat(buf2,"Ошибка на этапе лексического анализа - ");
						strcat(buf2,problem_in);
					}break;
				case 2:
					{
						fputs("Ошибка на этапе анализа переменных",log_file);
						fprintf(log_file,"\n");
						fprintf(log_file,"Ошибка в переменной %s\n",problem_perem);
						fprintf(log_file,"Проблема: %s\n",problem_in);
						strcat(buf2,"Ошибка на этапе анализа переменных - ");
						strcat(buf2,"Ошибка в переменной ");
						strcat(buf2,problem_perem);
						strcat(buf2,"; Проблема: ");
						strcat(buf2,problem_in);
					}break;
				case 3:
					{
						fputs("Ошибка на этапе вычисления выражения",log_file);
						strcat(buf2,"Ошибка на этапе вычисления выражения");
						fprintf(log_file,"\n");
						if (problem_vyraz)
						{
							fprintf(log_file,"Ошибка в выражении для переменной %s\n",problem_vyraz);
							strcat(buf2," - Ошибка в выражении для переменной ");
							strcat(buf2,problem_vyraz);
							fprintf(log_file,"Ошибка на этапе вычисления выражения переменной %s\n",problem_perem);
							strcat(buf2,"; Ошибка на этапе вычисления выражения переменной ");
							strcat(buf2,problem_perem);
						}
						fprintf(log_file,"Проблема: %s\n",problem_in);
						strcat(buf2,"; Проблема: ");
						strcat(buf2,problem_in);
					}break;
				}
			}
		if (*res)
			delete [] *res;
		*res=new char[strlen(buf2)+1];
		*res[0]='\0';
		strcat(*res,buf2);
	}
	void Leks_anal();
	void Parse();
	static bool provDopInt(char *s,char **my_res);
	static bool provDopVesh(char *s,char **my_res);
	static bool bukv(char symb)
	{
		if (((symb>='a') && (symb<='z')) || ((symb>='A') && (symb<='Z')))
			return true;
		else
			return false;
	}
	static bool cyfr(char c)
	{
		if ((c>='0') && (c<='9'))
			return true;
		else
			return false;
	}
	static bool form(char *str)
	{
		if (!strcmp(str,"abs") || !strcmp(str,"sin") || !strcmp(str,"cos") || !strcmp(str,"tg") || !strcmp(str,"ctg") || !strcmp(str,"arcsin") || !strcmp(str,"arccos") || !strcmp(str,"arctg") || !strcmp(str,"arcctg") || !strcmp(str,"log") || !strcmp(str,"lg") || !strcmp(str,"ln") || !strcmp(str,"pi") || !strcmp(str,"e") || !strcmp(str,"sqrt") || !strcmp(str,"sqr") || !strcmp(str,"pow") || !strcmp(str,"exp") || !strcmp(str,"fact"))
			return true;
		else
			return false;
	}
	char *getName()
	{
		return name;
	}
	char *getFormula()
	{
		return formula;
	}
	char *getResult()
	{
		char buffer[150];
		if (result)
			return _gcvt(*result,10,buffer);
		else
		{
			if (!kod_err)
				Parse();
			if (result)
				return _gcvt(*result,10,buffer);
			else
				return NULL;
		}
	}
	char *getProblemInPerem()
	{
		return problem_in_perem;
	}
	char *getProblemIn()
	{
		return problem_in;
	}
	int getKodErr()
	{
		return kod_err;
	}
	void setKodErr(int kod)
	{
		kod_err=kod;
	}
	void Bauer_Zamelzon();
	elem *getListPSimple()
	{
		return list_p_simple;
	}
	void setFlag(bool newflag)
	{
		flag=newflag;
	}
	void OpGeneration(temp_values **first_temp,my_stek **first_stek, prov_func **func_prov_listadr,perem_list *first);
	void PeremProv(perem_list *list_perem);
	void deleteResult()
	{
		if (result)
		{
			delete result;
			result=NULL;
		}
	}

	void deleteProblems()
	{
		delete [] problem_in;
		delete [] problem_perem;
		delete [] problem_in_perem;
		if (problem_vyraz)
			delete [] problem_vyraz;
		problem_in=NULL;
		problem_perem=NULL;
		problem_in_perem=NULL;
		problem_vyraz=NULL;
	}
	void Save_err(char *s_in,char *s_perem,char *s_in_perem,char *s_vyraz)
	{
		problem_in=new char[strlen(s_in)+1];
		problem_in[0]='\0';
		strcat(problem_in,s_in);
		problem_perem=new char[strlen(s_perem)+1];
		problem_perem[0]='\0';
		strcat(problem_perem,s_perem);
		problem_in_perem=new char[strlen(s_in_perem)+1];
		problem_in_perem[0]='\0';
		strcat(problem_in_perem,s_in_perem);
		if (s_vyraz)
		{
			problem_vyraz=new char[strlen(s_vyraz)+1];
			problem_vyraz[0]='\0';
			strcat(problem_vyraz,s_vyraz);
		}
	}
	~VYRAZ()
	{
		elem *el;
		if (problem_in)
			delete [] problem_in;
		if (problem_perem)
			delete [] problem_perem;
		if (problem_in_perem)
			delete [] problem_in_perem;
		if (problem_vyraz)
			delete [] problem_vyraz;
		el=list_p;
		while (el)
		{
			list_p=list_p->next;
			delete [] el->value;
			delete el;
			el=list_p;
		}
		el=list_p_simple;
		while (el)
		{
			list_p_simple=list_p_simple->next;
			delete [] el->value;
			delete el;
			el=list_p_simple;
		}
		el=list_c;
		while (el)
		{
			list_c=list_c->next;
			delete [] el->value;
			delete el;
			el=list_c;
		}
		el=list_f;
		while (el)
		{
			list_f=list_f->next;
			delete [] el->value;
			delete el;
			el=list_f;
		}
		if (result) 
			delete result;
		delete [] name;
		delete [] formula;
	}
	static bool PeremTestName(char *s, char **res);
	static void provChange(char *s_name,perem_list *first);
};

class myMath
{
public:
	static char *ABS(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		return _gcvt(abs(value),10,buf);
	}
	static char *SIN(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		return _gcvt(sin(value),10,buf);
	}
	static char *COS(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		return _gcvt(cos(value),10,buf);
	}
	static char *TG(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		_gcvt(tan(value),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргумента функции tg - tg(%s)\n",arg);
		if (!strncmp(buf,"-1.#INF",7))
			fprintf(log_file,"ОШИБКА!!! Результат tg(%s) меньше минимального допустимого для вычислений значения\n",arg);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат tg(%s) больше максимального допустимого для вычислений значения\n",arg);
		return buf;
	}
	static char *CTG(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		_gcvt(1/tan(value),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргумента функции ctg - ctg(%s)\n",arg);
		if (!strncmp(buf,"-1.#INF",7))
			fprintf(log_file,"ОШИБКА!!! Результат ctg(%s) меньше минимального допустимого для вычислений значения\n",arg);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат ctg(%s) больше максимального допустимого для вычислений значения\n",arg);
		return buf;
	}
	static char *ARCSIN(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		_gcvt(asin(value),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргумента функции arcsin - arcsin(%s)\n",arg);
		return buf;
	}
	static char *ARCCOS(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		_gcvt(acos(value),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргумента функции arccos - arccos(%s)\n",arg);
		return buf;
	}
	static char *ARCTG(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		return _gcvt(atan(value),10,buf);
	}
	static char *ARCCTG(char *arg)
	{
		double value;
		char buf[150];
		value=atof(arg);
		return _gcvt(M_PI_2-atan(value),10,buf);
	}
	static char *LOG(char *osn,char *st)
	{
		double value1,value2;
		char buf[150];
		value1=atof(osn);
		value2=atof(st);
		_gcvt(log(value2)/log(value1),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргументов функции log - log(%s;%s)\n",osn,st);
		return buf;
	}
	static char *LG(char *st)
	{
		double value;
		char buf[150];
		value=atof(st);
		_gcvt(log10(value),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргумента функции lg - lg(%s)\n",st);
		return buf;
	}
	static char *LN(char *st)
	{
		double value;
		char buf[150];
		value=atof(st);
		_gcvt(log(value),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргумента функции ln - ln(%s)\n",st);
		return buf;
	}
	static char *PI()
	{
		char buf[150];
		return _gcvt(M_PI,10,buf);
	}
	static char *E()
	{
		char buf[150];
		return _gcvt(M_E,10,buf);
	}
	static char *SQRT(char *m)
	{
		double value;
		char buf[150];
		value=atof(m);
		_gcvt(sqrt(value),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргумента функции sqrt - sqrt(%s)\n",m);
		return buf;
	}
	static char *SQR(char *m)
	{
		double value;
		char buf[150];
		value=atof(m);
		_gcvt(value*value,10,buf);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат sqr(%s) больше максимального допустимого для вычислений значения\\n",m);
		return buf;
	}
	static char *POW(char *st,char *pok)
	{
		double value1,value2;
		char buf[150];
		value1=atof(st);
		value2=atof(pok);
		_gcvt(pow(value1,value2),10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргументов функции pow - pow(%s;%s)\n",st,pok);
		if (!strncmp(buf,"-1.#INF",7))
			fprintf(log_file,"ОШИБКА!!! Результат pow(%s;%s) меньше минимального допустимого для вычислений значения\n",st,pok);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат pow(%s;%s) больше максимального допустимого для вычислений значения\n",st,pok);
		return buf;
	}
	static char *EXP(char *st)
	{
		double value;
		char buf[150];
		value=atof(st);
		_gcvt(exp(value),10,buf);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат exp(%s) больше максимального допустимого для вычислений значения\n",st);
		return buf;
	}

	static char *FACT(char *n)
	{
		int value,i;
		bool flag_correct=true;
		char buf[150];
		char *ukaz=NULL;
		value=strlen(n);
		i=0;
		flag_correct=VYRAZ::provDopInt(n,&ukaz);
		if ((!flag_correct) || ((value>1) && (n[0]=='-')))
		{
			fprintf(log_file,"ОШИБКА!!! Недопустимое значение аргумента функции fact - fact(%s)\n",n);
			return "-1.#IND";
		}
		value=atoi(n);
		i=value-1;
		if (!value)
			return "1";
		else
		{
			while (i>0)
			{
				if ((2147483647/value)>=i)
					value*=i--;
				else
				{
					fprintf(log_file,"ОШИБКА!!! Результат fact(%s) больше максимального допустимого для вычислений значения\n",n);
					return "1.#INF";
				}
			}
			return _gcvt(value,10,buf);
		}
	}
	static char *PLUS(char *n, char *m)
	{
		double value1,value2,value;
		char buf[150];
		value1=atof(n);
		value2=atof(m);
		value=value1+value2;
		_gcvt(value,10,buf);
		if (!strncmp(buf,"-1.#INF",7))
			fprintf(log_file,"ОШИБКА!!! Результат %s+%s меньше минимального допустимого для вычислений значения\n",n,m);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат %s+%s больше максимального допустимого для вычислений значения\n",n,m);
		return buf;
	}
	static char *MINUS(char *n, char *m)
	{
		double value1,value2,value;
		char buf[150];
		value1=atof(n);
		value2=atof(m);
		value=value1-value2;
		_gcvt(value,10,buf);
		if (!strncmp(buf,"-1.#INF",7))
			fprintf(log_file,"ОШИБКА!!! Результат %s-%s меньше минимального допустимого для вычислений значения\n",n,m);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат %s-%s больше максимального допустимого для вычислений значения\n",n,m);
		return buf;
	}
	static char *UMN(char *n, char *m)
	{
		double value1,value2,value;
		char buf[150];
		value1=atof(n);
		value2=atof(m);
		value=value1*value2;
		_gcvt(value,10,buf);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат %s*%s больше максимального допустимого для вычислений значения\n",n,m);
		if (!strncmp(buf,"-1.#INF",7))
			fprintf(log_file,"ОШИБКА!!! Результат %s*%s меньше минимального допустимого для вычислений значения\n",n,m);
		return buf;
	}
	static char *DEL(char *n, char *m)
	{
		double value1,value2,value;
		char buf[150];
		value1=atof(n);
		value2=atof(m);
		value=value1/value2;
		_gcvt(value,10,buf);
		if (!strncmp(buf,"-1.#IND",7))
			fprintf(log_file,"ОШИБКА!!! Деление на 0 - %s/%s\n",n,m);
		if (!strncmp(buf,"-1.#INF",7))
			fprintf(log_file,"ОШИБКА!!! Результат %s/%s меньше минимального допустимого для вычислений значения\n",n,m);
		if (!strncmp(buf,"1.#INF",6))
			fprintf(log_file,"ОШИБКА!!! Результат %s/%s больше максимального допустимого для вычислений значения\n",n,m);
		return buf;
	}
};
#endif
