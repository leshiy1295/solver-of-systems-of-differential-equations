#include "VYRAZ.h"
FILE *log_file;
perem_list *first=NULL;
int perem_count_list=0;
bool VYRAZ::PeremTestName(char *s, char **res)
{
	char str[20],buf[150];
	str[0]='\0';
	strcat(str,s);
	int len=strlen(str);
	bool flag=true;
	if (!len)
	{
		flag=false;
		fputs("������!!!\n��� �� ����� ���� ������",log_file);
		fprintf(log_file,"\n");
		buf[0]='\0';
		strcat(buf,"��� �� ����� ���� ������");
		if (*res)
			delete [] *res;
		*res=new char[strlen(buf)+1];
		*res[0]='\0';
		strcat(*res,buf);
		return flag;
	}
	if (!(VYRAZ::bukv(str[0])))
	{
		flag=false;
		fputs("������!!!\n������ ������ - �� �����",log_file);
		fprintf(log_file,"\n");
		buf[0]='\0';
		strcat(buf,"������ ������ - �� �����");
		if (*res)
			delete [] *res;
		*res=new char[strlen(buf)+1];
		*res[0]='\0';
		strcat(*res,buf);
		return flag;
	}
	else
	{
		int i=1;
		while (flag && (i<len))
		{
			if (!(VYRAZ::bukv(str[i]) || VYRAZ::cyfr(str[i])))
				flag=false;
			i++;
		}
		if (!flag)
		{
			fprintf(log_file,"������!!!\n'%c' - ������������ ������\n",str[i-1]);
			buf[0]='\'';
			buf[2]='\0';
			buf[1]=str[i-1];
			strcat(buf,"' - ������������ ������");
		}
		else
		{
			for (int m=0;m<len;m++)	
				if ((str[m]>='A') && (str[m]<='Z'))
					str[m]=str[m]-'A'+'a';
			if (VYRAZ::form(str))
			{
				fprintf(log_file,"%s �������� ����������������� ������ ��� �������\n",str);
				buf[0]='\0';
				strcat(buf,str);
				strcat(buf," �������� ����������������� ������ ��� �������");
				flag=false;
			}
		}
		if (flag)
		{
			buf[0]='\0';
			strcat(buf,"OK");
		}
		if (*res)
			delete [] *res;
		*res=new char[strlen(buf)+1];
		*res[0]='\0';
		strcat(*res,buf);
		return flag;
	}
}
void VYRAZ::provChange(char *s_name,perem_list *first)
{
	/*���������� ����� ���� ��������� ������� ����������. ���� ��� ��� ��������� ����������� ������ ������,
	�� ���� � �����-�� �� ���������� ���� ������������ ����������, �� result|->NULL,
	� ���� ��� ���� ��� � ��������, �� � ��� ��������� ��������� (flag,problems) ���������*/
	perem_list *stek_first=NULL,*stek_cur;
	VYRAZ::temp_values *all_stek=NULL,*all_cur;
	perem_list *cur, *cur2;
	bool flagprov;
	VYRAZ::prov_func *first_prov=NULL,*cur_prov;
	VYRAZ::prov_func *first_all_prov=NULL,*cur_all_prov;
	VYRAZ::elem *el_p;
	cur=first;
	if (cur)
	{
		stek_first=new perem_list;
		stek_first->perem=cur->perem;
		stek_first->next=NULL;
		all_stek=new VYRAZ::temp_values;
		all_stek->value=new char[strlen(cur->perem->getName())+1];
		all_stek->value[0]='\0';
		strcat(all_stek->value,cur->perem->getName());
		all_stek->next=NULL;
		first_all_prov=new VYRAZ::prov_func;
		first_all_prov->kol=0;
		first_all_prov->next=NULL;
		first_prov=new VYRAZ::prov_func;
		first_prov->kol=1;
		first_prov->next=NULL;
		el_p=cur->perem->getListPSimple();
	}
	while (cur)
	{
		if (el_p)
		{
			//�������� ������� � ����� �����
			all_cur=all_stek;
			cur_all_prov=first_all_prov;
			flagprov=true;
			while (flagprov && all_cur)
			{
				if (!strcmp(el_p->value,all_cur->value))
					flagprov=false;
				else
				{
					all_cur=all_cur->next;
					cur_all_prov=cur_all_prov->next;
				}
			}
			if (flagprov)
			{
				//���������� ��� �� ��������������
				//����� ����������
				cur2=first;
				flagprov=false;
				while (!flagprov && cur2)
				{
					if (!strcmp(el_p->value,cur2->perem->getName()))
						flagprov=true;
					else
						cur2=cur2->next;
				}
				if (flagprov)
				{
					//���������� ����������
					//�������� ������������ ��������� ����������
					if (!strcmp(el_p->value,s_name))
					{
						//���������� �������� ��� �������� �������� �����
						stek_cur=stek_first;
						cur_all_prov=first_all_prov;
						while (stek_cur)
						{
							stek_cur->perem->deleteResult();
							stek_cur->perem->setFlag(true);
							stek_cur->perem->setKodErr(0);
							stek_cur->perem->deleteProblems();
							stek_cur=stek_cur->next;
							cur_all_prov->kol=1;
							cur_all_prov=cur_all_prov->next;
						}
						first_prov->kol++;
					}
					else
					{
						all_cur=new VYRAZ::temp_values;
						all_cur->next=all_stek;
						all_cur->value=new char[strlen(el_p->value)+1];
						all_cur->value[0]='\0';
						strcat(all_cur->value,el_p->value);
						all_stek=all_cur;
						cur_all_prov=new VYRAZ::prov_func;
						cur_all_prov->next=first_all_prov;
						cur_all_prov->kol=0;
						first_all_prov=cur_all_prov;
						stek_cur=new perem_list;
						stek_cur->next=stek_first;
						stek_cur->perem=cur2->perem;
						stek_first=stek_cur;
						cur_prov=new VYRAZ::prov_func;
						cur_prov->next=first_prov;
						cur_prov->kol=1;
						first_prov=cur_prov;
					}
				}
				else
					first_prov->kol++;
			}
			else
			{
				if ((!strcmp(s_name,el_p->value)) || (cur_all_prov->kol==1))
				{
					stek_cur=stek_first;
					cur_all_prov=first_all_prov;
					while (stek_cur)
					{
						stek_cur->perem->deleteResult();
						stek_cur->perem->setFlag(true);
						stek_cur->perem->setKodErr(0);
						stek_cur->perem->deleteProblems();
						stek_cur=stek_cur->next;
						cur_all_prov->kol=1;
						cur_all_prov=cur_all_prov->next;
					}
				}
				first_prov->kol++;
			}
			el_p=stek_first->perem->getListPSimple();
			for (int kk=1;kk<first_prov->kol;kk++)
				el_p=el_p->next;
		}
		while (!el_p && stek_first)
		{
			//���� ��� ��������� �������������
			stek_cur=stek_first;
			stek_first=stek_first->next;
			delete stek_cur;
			cur_prov=first_prov;
			first_prov=first_prov->next;
			delete cur_prov;
			if (first_prov)
			{
				first_prov->kol++;
				el_p=stek_first->perem->getListPSimple();
				for (int kk=1;kk<first_prov->kol;kk++)
					el_p=el_p->next;
			}
		}
		if (!stek_first)
		{//���� ���� ���������� ��������� ��������, �� ��������� � ���������
			cur=cur->next;
			flagprov=false;
			while (cur && !flagprov)
			{
				all_cur=all_stek;
				flagprov=true;
				while (flagprov && all_cur)
				{
					if (!strcmp(cur->perem->getName(),all_cur->value))
						flagprov=false;
					all_cur=all_cur->next;
				}
				if (flagprov)
				{
					if (!strcmp(s_name,cur->perem->getName()))
					{
						cur=cur->next;
						flagprov=false;
					}
					else
					{
						//���������� ��� �� �������������
						stek_first=new perem_list;
						stek_first->perem=cur->perem;
						stek_first->next=NULL;
						all_cur=new VYRAZ::temp_values;
						all_cur->value=new char[strlen(cur->perem->getName())+1];
						all_cur->value[0]='\0';
						strcat(all_cur->value,cur->perem->getName());
						all_cur->next=all_stek;
						all_stek=all_cur;
						cur_all_prov=new VYRAZ::prov_func;
						cur_all_prov->next=first_all_prov;
						cur_all_prov->kol=0;
						first_all_prov=cur_all_prov;
						first_prov=new VYRAZ::prov_func;
						first_prov->kol=1;
						first_prov->next=NULL;
						el_p=cur->perem->getListPSimple();
					}
				}
				else
					cur=cur->next;
			}
		}
	}
	all_cur=all_stek;
	while (all_cur)
	{
		all_stek=all_stek->next;
		delete [] all_cur->value;
		delete all_cur;
		all_cur=all_stek;
	}
	cur_all_prov=first_all_prov;
	while (cur_all_prov)
	{
		first_all_prov=first_all_prov->next;
		delete cur_all_prov;
		cur_all_prov=first_all_prov;
	}
}
bool VYRAZ::provDopInt(char *s,char **my_res)
{
	bool flag=true;
	char buf[150];
	int i=0,len=strlen(s);
	if (len>=1)
	{
		if (len<=20)
		{
			if ((len>=2) && ((s[0]=='+') || (s[0]=='-')))
				i=1;
			while (flag && (i<len))
			{	
				if (!(VYRAZ::cyfr(s[i])))
				{
					fprintf(log_file,"������������ ������ - %c\n",s[i]);
					buf[0]='\0';
					strcat(buf,"������������ ������ - ");
					buf[strlen(buf)+1]='\0';
					buf[strlen(buf)]=s[i];
					flag=false;
				}
				else
					i++;
			}
		}
		else
		{
			flag=false;
			fputs("������������ ���������� �������� � ������ - 20!!!",log_file);
			fprintf(log_file,"\n");
			buf[0]='\0';
			strcat(buf,"������������ ���������� �������� � ������ - 20!!!");
		}
	}
	else
	{
		fputs("������� ������ ������",log_file);
		fprintf(log_file,"\n");
		buf[0]='\0';
		strcat(buf,"������� ������ ������");
		flag=false;
	}
	if (flag)
		if (atoi(s)==2147483647)
		{
			i=0;
			if (s[i]=='+')
				i=1;
			while (s[i]=='0')
				i++;
			if (strcmp(s+i,"2147483647"))
			{
				fputs("�������� �������� ������ ������������� ����������� ��� ���������� ��������",log_file);
				fprintf(log_file,"\n");
				buf[0]='\0';
				strcat(buf,"�������� �������� ������ ������������� ����������� ��� ���������� ��������");
				flag=false;
			}
		}
		else if (atoi(s)==-2147483648)
		{
			i=1;
			while (s[i]=='0')
				i++;
			if (strcmp(s+i+1,"2147483648"))
			{
				fputs("�������� �������� ������ ������������ ����������� ��� ���������� ��������",log_file);
				fprintf(log_file,"\n");
				buf[0]='\0';
				strcat(buf,"�������� �������� ������ ������������ ����������� ��� ���������� ��������");
				flag=false;
			}
		}
	if (flag)
	{
		if (*my_res)
			delete [] *my_res;
		*my_res=new char[strlen("OK")+1];
		*my_res[0]='\0';
		strcat(*my_res,"OK");
	}
	else
	{
		if (*my_res)
			delete [] *my_res;
		*my_res=new char[strlen(buf)+1];
		*my_res[0]='\0';
		strcat(*my_res,buf);
	}
	fflush(log_file);
	return flag;
}
bool VYRAZ::provDopVesh(char *s,char **my_res)
{
	char buf[150];
	bool flag;
	int i=0,j,len=strlen(s),kol=0;
	if (len>=1)
	{
		if (len<=20)
		{
			if ((len>=2) && ((s[0]=='+') || (s[0]=='-')))
				i=1;
			flag=true;
			if (!VYRAZ::cyfr(s[i]))
			{
				flag=false;
				fprintf(log_file,"������������ ������ '%c'\n",s[i]);
				buf[0]='\0';
				strcat(buf,"������������ ������ '");
				buf[strlen(buf)+1]='\0';
				buf[strlen(buf)]=s[i];
				strcat(buf,"'");
			}
			else
			{
				while ((i<len) && (s[i]=='0'))
					i++;
				if (i<len)
					if ((s[i]=='.'))
					{
						kol=2;//����� � ���� ����� ������
						i++;					
						while ((kol<15) && (i<len) && (VYRAZ::cyfr(s[i])))
						{
							i++;
							kol++;
						}
						if (i<len)
							if (VYRAZ::cyfr(s[i]))
							{//���������� ��������� ��� ���������� �������. ���� ��� 0, �� �Ѩ ��. ���� ��� - �� ������
								while (flag && (i<len) && (VYRAZ::cyfr(s[i])))
									if (s[i]!='0')
									{
										flag=false;
										fputs("��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15",log_file);
										fprintf(log_file,"\n");
										buf[0]='\0';
										strcat(buf,"��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15");
									}
									else
										i++;
								if (flag && (i<len))
									if ((s[i]=='e') || (s[i]=='E'))
									{
										i++;
										j=i;
										if ((s[i]=='+') || (s[i]=='-'))
											i++;
										if (!((i<len) && (VYRAZ::cyfr(s[i]))))
										{
											flag=false;
											if (i<len)
												fprintf(log_file,"������������ ������ '%c'\n",s[i]);
											else
												fprintf(log_file,"������������ ������ '%c'\n",s[i-1]);
											buf[0]='\0';
											strcat(buf,"������������ ������ '");
											buf[strlen(buf)+1]='\0';
											if (i<len)
												buf[strlen(buf)]=s[i];
											else
												buf[strlen(buf)]=s[i-1];
											strcat(buf,"'");
										}
										else
										{
											while (flag && (i<len))
												if (!(VYRAZ::cyfr(s[i])))
												{
													flag=false;
													fprintf(log_file,"������������ ������ '%c'\n",s[i]);
													buf[0]='\0';
													strcat(buf,"������������ ������ '");
													buf[strlen(buf)+1]='\0';
													buf[strlen(buf)]=s[i];
													strcat(buf,"'");
												}
												else
													i++;
											if (flag)
											{
												if ((atoi(s+j)<-307) || (atoi(s+j)>307))
												{
													flag=false;
													fputs("�������� ������� ������ ���������� � �������� [-307;307]",log_file);
													fprintf(log_file,"\n");
													buf[0]='\0';
													strcat(buf,"�������� ������� ������ ���������� � �������� [-307;307]");
												}
											}
										}
									}
									else
									{
										fprintf(log_file,"������������ ������ '%c'\n",s[i]);;
										flag=false;
										buf[0]='\0';
										strcat(buf,"������������ ������ '");
										buf[strlen(buf)+1]='\0';
										buf[strlen(buf)]=s[i];
										strcat(buf,"'");
									}
							}
							else
								if ((s[i]=='e') || (s[i]=='E'))
								{
									i++;
									j=i;
									if ((s[i]=='+') || (s[i]=='-'))
										i++;
									if (!((i<len) && (VYRAZ::cyfr(s[i]))))
									{
										flag=false;
										if (i<len)
											fprintf(log_file,"������������ ������ '%c'\n",s[i]);
										else
											fprintf(log_file,"������������ ������ '%c'\n",s[i-1]);
										buf[0]='\0';
										strcat(buf,"������������ ������ '");
										buf[strlen(buf)+1]='\0';
										if (i<len)
											buf[strlen(buf)]=s[i];
										else
											buf[strlen(buf)]=s[i-1];
										strcat(buf,"'");
									}
									else
									{
										while (flag && (i<len))
											if (!(VYRAZ::cyfr(s[i])))
											{
												flag=false;
												fprintf(log_file,"������������ ������ '%c'\n",s[i]);
												buf[0]='\0';
												strcat(buf,"������������ ������ '");
												buf[strlen(buf)+1]='\0';
												buf[strlen(buf)]=s[i];
												strcat(buf,"'");
											}
											else
												i++;
										if (flag)
										{
											if ((atoi(s+j)<-307) || (atoi(s+j)>307))
											{
												flag=false;
												fputs("�������� ������� ������ ���������� � �������� [-307;307]",log_file);
												fprintf(log_file,"\n");
												buf[0]='\0';
												strcat(buf,"�������� ������� ������ ���������� � �������� [-307;307]");
											}
										}
									}
								}
							else
							{
								flag=false;
								fprintf(log_file,"������������ ������ '%c'\n",s[i]);
								buf[0]='\0';
								strcat(buf,"������������ ������ '");
								buf[strlen(buf)+1]='\0';
								buf[strlen(buf)]=s[i];
								strcat(buf,"'");
							}
					}
					else
						if ((s[i]=='e') || (s[i]=='E'))
						{
							i++;
							j=i;
							if ((s[i]=='+') || (s[i]=='-'))
								i++;
							if (!((i<len) && (VYRAZ::cyfr(s[i]))))
							{
								flag=false;
								if (i<len)
									fprintf(log_file,"������������ ������ '%c'\n",s[i]);
								else
									fprintf(log_file,"������������ ������ '%c'\n",s[i-1]);
								buf[0]='\0';
								strcat(buf,"������������ ������ '");
								buf[strlen(buf)+1]='\0';
								if (i<len)
									buf[strlen(buf)]=s[i];
								else
									buf[strlen(buf)]=s[i-1];
								strcat(buf,"'");
							}
							else
							{
								while (flag && (i<len))
									if (!(VYRAZ::cyfr(s[i])))
									{
										flag=false;
										fprintf(log_file,"������������ ������ '%c'\n",s[i]);
										buf[0]='\0';
										strcat(buf,"������������ ������ '");
										buf[strlen(buf)+1]='\0';
										buf[strlen(buf)]=s[i];
										strcat(buf,"'");
									}
									else
										i++;
								if (flag)
								{
									if ((atoi(s+j)<-307) || (atoi(s+j)>307))
									{
										flag=false;
										fputs("�������� ������� ������ ���������� � �������� [-307;307]",log_file);
										fprintf(log_file,"\n");
										buf[0]='\0';
										strcat(buf,"�������� ������� ������ ���������� � �������� [-307;307]");
									}
								}
							}
						}
						else
							if (VYRAZ::cyfr(s[i]))
							{
								i++;
								kol=1;
								while ((kol<14) && (i<len) && (VYRAZ::cyfr(s[i])))
								{
									i++;
									kol++;
								}
								if (i<len)
								{
									if (VYRAZ::cyfr(s[i]))
									{//kol=14; ����� ���� �����, ��� ����� �� 15, ����� ��� ��������� �����.�����e�����, ������� ������ ���� 0
										i++;
										kol++;
										while (flag && (i<len) && (VYRAZ::cyfr(s[i])))
										{
										  	if (s[i]!='0')
											{
												flag=false;
												fputs("��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15",log_file);
												fprintf(log_file,"\n");
												buf[0]='\0';
												strcat(buf,"��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15");
											}
											else
												i++;
										}
										if (flag && (i<len))
											if (s[i]=='.')
											{
												if (kol<15)// �� ���� �� ���� ������ ������������ �����, ������� ����� ���� ���������
												{
													i++;
													kol++;//���� �����
													while (flag && (i<len) && (VYRAZ::cyfr(s[i])))
														if (s[i]!='0')
														{
															flag=false;
															fputs("��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15",log_file);
															fprintf(log_file,"\n");
															buf[0]='\0';
															strcat(buf,"��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15");
														}
														else
															i++;
													if (flag && (i<len))
														if ((s[i]=='e') || (s[i]=='E'))
														{
															i++;
															j=i;
															if ((s[i]=='+') || (s[i]=='-'))
																if (i+1<len)
																	i++;
															if (!((i<len) && (VYRAZ::cyfr(s[i]))))
															{
																flag=false;
																if (i<len)
																	fprintf(log_file,"������������ ������ '%c'\n",s[i]);
																else
																	fprintf(log_file,"������������ ������ '%c'\n",s[i-1]);
																buf[0]='\0';
																strcat(buf,"������������ ������ '");
																buf[strlen(buf)+1]='\0';
																if (i<len)
																	buf[strlen(buf)]=s[i];
																else
																	buf[strlen(buf)]=s[i-1];
																strcat(buf,"'");
															}
															else
															{
																while (flag && (i<len))
																	if (!(VYRAZ::cyfr(s[i])))
																	{
																		flag=false;
																		fprintf(log_file,"������������ ������ '%c'\n",s[i]);
																		buf[0]='\0';
																		strcat(buf,"������������ ������ '");
																		buf[strlen(buf)+1]='\0';
																		buf[strlen(buf)]=s[i];
																		strcat(buf,"'");
																	}
																	else
																		i++;
																if (flag)
																{
																	if ((atoi(s+j)<-307) || (atoi(s+j)>307))
																	{
																		flag=false;
																		fputs("�������� ������� ������ ���������� � �������� [-307;307]",log_file);
																		fprintf(log_file,"\n");
																		buf[0]='\0';
																		strcat(buf,"�������� ������� ������ ���������� � �������� [-307;307]");
																	}
																}
															}
														}
														else
														{
															fprintf(log_file,"������������ ������ '%c'\n",s[i]);;
															flag=false;
															buf[0]='\0';
															strcat(buf,"������������ ������ '");
															buf[strlen(buf)+1]='\0';
															buf[strlen(buf)]=s[i];
															strcat(buf,"'");
														}
												}
												else
												{
													flag=false;
													fputs("��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15",log_file);
													fprintf(log_file,"\n");
													buf[0]='\0';
													strcat(buf,"��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15");
												}
											}
											else
												if ((s[i]=='e') || (s[i]=='E'))
												{
													i++;
													j=i;
													if ((s[i]=='+') || (s[i]=='-'))
														i++;
													if (!((i<len) && (VYRAZ::cyfr(s[i]))))
													{
														flag=false;
														if (i<len)
															fprintf(log_file,"������������ ������ '%c'\n",s[i]);
														else
															fprintf(log_file,"������������ ������ '%c'\n",s[i-1]);
														buf[0]='\0';
														strcat(buf,"������������ ������ '");
														buf[strlen(buf)+1]='\0';
														if (i<len)
															buf[strlen(buf)]=s[i];
														else
															buf[strlen(buf)]=s[i-1];
														strcat(buf,"'");
													}
													else
													{
														while (flag && (i<len))
															if (!(VYRAZ::cyfr(s[i])))
															{
																flag=false;
																fprintf(log_file,"������������ ������ '%c'\n",s[i]);
																buf[0]='\0';
																strcat(buf,"������������ ������ '");
																buf[strlen(buf)+1]='\0';
																buf[strlen(buf)]=s[i];
																strcat(buf,"'");
															}
															else
																i++;
														if (flag)
														{
															if ((atoi(s+j)<-307) || (atoi(s+j)>307))
															{
																flag=false;
																fputs("�������� ������� ������ ���������� � �������� [-307;307]",log_file);
																fprintf(log_file,"\n");
																buf[0]='\0';
																strcat(buf,"�������� ������� ������ ���������� � �������� [-307;307]");
															}
														}
													}
												}
												else
												{
													flag=false;
													fprintf(log_file,"������������ ������ '%c'\n",s[i]);
													buf[0]='\0';
													strcat(buf,"������������ ������ '");
													buf[strlen(buf)+1]='\0';
													buf[strlen(buf)]=s[i];
													strcat(buf,"'");
												}
									}
									else
										if (s[i]=='.')
										{
											kol++;//���� �����
											i++;	
											while ((kol<15) && (i<len) && (VYRAZ::cyfr(s[i])))
											{
												kol++;
												i++;
											}
											if (i<len)
											{
												if (VYRAZ::cyfr(s[i]))
												{
													//���� ��������� ��� ���� �� e
													while (flag && (i<len) && (VYRAZ::cyfr(s[i])))
														if (s[i]!='0')
														{
															flag=false;
															fputs("��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15",log_file);
															fprintf(log_file,"\n");
															buf[0]='\0';
															strcat(buf,"��������� ���������� �������� ���� ����� � ������� ������ � ������ '.' ������ ���� �� ����� 15");
														}
														else
															i++;
													if (flag && (i<len))
														if ((s[i]=='e') || (s[i]=='E'))
														{
															i++;
															j=i;
															if ((s[i]=='+') || (s[i]=='-'))
																i++;
															if (!((i<len) && (VYRAZ::cyfr(s[i]))))
															{
																flag=false;
																if (i<len)
																	fprintf(log_file,"������������ ������ '%c'\n",s[i]);
																else
																	fprintf(log_file,"������������ ������ '%c'\n",s[i-1]);
																buf[0]='\0';
																strcat(buf,"������������ ������ '");
																buf[strlen(buf)+1]='\0';
																if (i<len)
																	buf[strlen(buf)]=s[i];
																else
																	buf[strlen(buf)]=s[i-1];
																strcat(buf,"'");
															}
															else
															{
																while (flag && (i<len))
																	if (!(VYRAZ::cyfr(s[i])))
																	{
																		flag=false;
																		fprintf(log_file,"������������ ������ '%c'\n",s[i]);
																		buf[0]='\0';
																		strcat(buf,"������������ ������ '");
																		buf[strlen(buf)+1]='\0';
																		buf[strlen(buf)]=s[i];
																		strcat(buf,"'");
																	}
																	else
																		i++;
																if (flag)
																{
																	if ((atoi(s+j)<-307) || (atoi(s+j)>307))
																	{
																		flag=false;
																		fputs("�������� ������� ������ ���������� � �������� [-307;307]",log_file);
																		fprintf(log_file,"\n");
																		buf[0]='\0';
																		strcat(buf,"�������� ������� ������ ���������� � �������� [-307;307]");
																	}
																}
															}
														}
														else
														{
															fprintf(log_file,"������������ ������ '%c'\n",s[i]);;
															flag=false;
															buf[0]='\0';
															strcat(buf,"������������ ������ '");
															buf[strlen(buf)+1]='\0';
															buf[strlen(buf)]=s[i];
															strcat(buf,"'");
														}
												}
												else
													if ((s[i]=='e') || (s[i]=='E'))
													{
														i++;
														j=i;
														if ((s[i]=='+') || (s[i]=='-'))
															i++;
														if (!((i<len) && (VYRAZ::cyfr(s[i]))))
														{
															flag=false;
															if (i<len)
																fprintf(log_file,"������������ ������ '%c'\n",s[i]);
															else
																fprintf(log_file,"������������ ������ '%c'\n",s[i-1]);
															buf[0]='\0';
															strcat(buf,"������������ ������ '");
															buf[strlen(buf)+1]='\0';
															if (i<len)
																buf[strlen(buf)]=s[i];
															else
																buf[strlen(buf)]=s[i-1];
															strcat(buf,"'");
														}
														else
														{
															while (flag && (i<len))
																if (!(VYRAZ::cyfr(s[i])))
																{
																	flag=false;
																	fprintf(log_file,"������������ ������ '%c'\n",s[i]);
																	buf[0]='\0';
																	strcat(buf,"������������ ������ '");
																	buf[strlen(buf)+1]='\0';
																	buf[strlen(buf)]=s[i];
																	strcat(buf,"'");
																}
																else
																	i++;
															if (flag)
															{
																if ((atoi(s+j)<-307) || (atoi(s+j)>307))
																{
																	flag=false;
																	fputs("�������� ������� ������ ���������� � �������� [-307;307]",log_file);
																	fprintf(log_file,"\n");
																	buf[0]='\0';
																	strcat(buf,"�������� ������� ������ ���������� � �������� [-307;307]");
																}
															}
														}
													}
													else
													{
														flag=false;
														fprintf(log_file,"������������ ������ '%c'\n",s[i]);
														buf[0]='\0';
														strcat(buf,"������������ ������ '");
														buf[strlen(buf)+1]='\0';
														buf[strlen(buf)]=s[i];
														strcat(buf,"'");
													}
											}
										}
										else
											if ((s[i]=='e') || (s[i]=='E'))
											{
												i++;
												j=i;
												if ((s[i]=='+') || (s[i]=='-'))
													i++;
												if (!((i<len) && (VYRAZ::cyfr(s[i]))))
												{
													flag=false;
													if (i<len)
														fprintf(log_file,"������������ ������ '%c'\n",s[i]);
													else
														fprintf(log_file,"������������ ������ '%c'\n",s[i-1]);
													buf[0]='\0';
													strcat(buf,"������������ ������ '");
													buf[strlen(buf)+1]='\0';
													if (i<len)
														buf[strlen(buf)]=s[i];
													else
														buf[strlen(buf)]=s[i-1];
													strcat(buf,"'");
												}
												else
												{
													while (flag && (i<len))
														if (!(VYRAZ::cyfr(s[i])))
														{
															flag=false;
															fprintf(log_file,"������������ ������ '%c'\n",s[i]);
															buf[0]='\0';
															strcat(buf,"������������ ������ '");
															buf[strlen(buf)+1]='\0';
															buf[strlen(buf)]=s[i];
															strcat(buf,"'");
														}
														else
															i++;
													if (flag)
													{
														if ((atoi(s+j)<-307) || (atoi(s+j)>307))
														{
															flag=false;
															fputs("�������� ������� ������ ���������� � �������� [-307;307]",log_file);
															fprintf(log_file,"\n");
															buf[0]='\0';
															strcat(buf,"�������� ������� ������ ���������� � �������� [-307;307]");
														}
													}
												}
											}
											else
											{
												flag=false;
												fprintf(log_file,"������������ ������ '%c'\n",s[i]);
												buf[0]='\0';
												strcat(buf,"������������ ������ '");
												buf[strlen(buf)+1]='\0';
												buf[strlen(buf)]=s[i];
												strcat(buf,"'");
											}
								}
							}
							else
							{
								flag=false;
								fprintf(log_file,"������������ ������ '%c'\n",s[i]);
								buf[0]='\0';
								strcat(buf,"������������ ������ '");
								buf[strlen(buf)+1]='\0';
								buf[strlen(buf)]=s[i];
								strcat(buf,"'");
							}
						}
		}
		else
		{
			flag=false;
			fputs("������������ ���������� �������� � ������ - 20!!!",log_file);
			fprintf(log_file,"\n");
			buf[0]='\0';
			strcat(buf,"������������ ���������� �������� � ������ - 20!!!");
		}
	}
	else
	{
		flag=false;
		fputs("������� ������ ������",log_file);
		fprintf(log_file,"\n");
		buf[0]='\0';
		strcat(buf,"������� ������ ������");
	}
	if (flag)
	{
		_gcvt(atof(s),10,buf);
		if (strncmp(buf,"-1.#INF",7))
			if (strncmp(buf,"1.#INF",6))
			{
				buf[0]='\0';
				strcat(buf,"OK");
			}
			else
			{
				flag=false;
				buf[0]='\0';
				strcat(buf,"�������� ");
				strcat(buf,s);
				strcat(buf," ������ ������������� ����������� ��� ���������� ��������");
			}
		else
		{
			flag=false;
			buf[0]='\0';
			strcat(buf,"�������� ");
			strcat(buf,s);
			strcat(buf," ������ ������������ ����������� ��� ���������� ��������");
		}
	}
	if (*my_res)
		delete [] *my_res;
	*my_res=new char[strlen(buf)+1];
	*my_res[0]='\0';
	strcat(*my_res,buf);
	fflush(log_file);
	return flag;
}
void VYRAZ::Leks_anal()
{
	fputs("����������� ������",log_file);
	fprintf(log_file,"\n");
	/* �� ������ ����� ����������
		1) ��������� ��� ������� �� ������������
		2) ��������� ������ ��������, ���������� � ������
		3) ��������� ������ �������
	*/
	char *prov_buf=NULL;
	elem *el_c,*el_p,*el_f;
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
	el_c=list_c;
	while (el_c)
	{
		list_c=list_c->next;
		delete [] el_c->value;
		delete el_c;
		el_c=list_c;
	}
	el_f=list_f;
	while (el_f)
	{
		list_f=list_f->next;
		delete [] el_f->value;
		delete el_f;
		el_f=list_f;
	}
	//if (result)
//		delete result
	tokens[0]='\0';
	Print_name();
	Print_formula();
	char buf2[150];
	bool flag_func=false; // ��� ���������� �������� ������� ;
	bool flag_e;
	el_c=list_c;
	el_p=list_p;
	el_f=list_f;
	int i=0,len=strlen(formula),j;
	while (flag && (i<len))
	{
		if ((formula[i]=='(') || (formula[i]==')') || ((formula[i]=='+') && !(((i==0) || (formula[i-1]==';') || (formula[i-1]=='(')) && (i<len-1) && cyfr(formula[i+1]))) || ((formula[i]=='-') && !(((i==0) || (formula[i-1]==';') || (formula[i-1]=='(')) && (i<len-1) && cyfr(formula[i+1]))) || (formula[i]=='*') || (formula[i]=='/') || (flag_func && (formula[i]==';')))
		{
			if (flag_func && (formula[i]==';'))
				flag_func=false;
			buf[0]=formula[i];
			buf[1]='\0';
			strcat(tokens,buf);
			i++;
		}
		else
			if (cyfr(formula[i]) || ((formula[i]=='+') && (((i==0) || (formula[i-1]==';') || (formula[i-1]=='(')) && (i<len-1) && cyfr(formula[i+1]))) || ((formula[i]=='-') && (((i==0) || (formula[i-1]==';') || (formula[i-1]=='(')) && (i<len-1) && cyfr(formula[i+1]))))
			{
				strcat(tokens,"C");
				if (!list_c)
				{
					list_c=new elem;
					list_c->value=new char[len+1];
					list_c->value[0]='\0';
					list_c->next=NULL;
					el_c=list_c;
				}
				else
				{
					el_c->next=new elem;
					el_c=el_c->next;
					el_c->value=new char[len+1];
					el_c->value[0]='\0';
					el_c->next=NULL;
				}
				if ((formula[i]=='+') || (formula[i]=='-'))
				{
					if (formula[i]=='-')
					{
						buf[0]=formula[i];
						buf[1]='\0';
						strcat(el_c->value,buf);
					}
					i++;
				}
				while ((i<len) && cyfr(formula[i]))
				{
					buf[0]=formula[i];
					buf[1]='\0';
					strcat(el_c->value,buf);
					i++;
				}
				if ((i<len) && (formula[i]=='.'))
				{
					buf[0]=formula[i];
					buf[1]='\0';
					strcat(el_c->value,buf);
					i++;
					while ((i<len) && cyfr(formula[i]))
					{	
						buf[0]=formula[i];
						buf[1]='\0';
						strcat(el_c->value,buf);
						i++;
					}
				}
				if ((i<len) && (bukv(formula[i])))
				{
					flag_e=false;
					if ((formula[i]=='e') || (formula[i]=='E'))
					{
						if (((i+1)<len) && (((formula[i+1]=='+') || (formula[i+1]=='-')) && ((i+2)<len) && (cyfr(formula[i+2]))) || cyfr(formula[i+1]))
						{
							flag_e=true;
							buf[0]=formula[i];
							buf[1]='\0';
							strcat(el_c->value,buf);
							buf[0]=formula[i+1];
							buf[1]='\0';
							strcat(el_c->value,buf);
							i=i+2;
							while ((i<len) && cyfr(formula[i]))
							{	
								buf[0]=formula[i];
								buf[1]='\0';
								strcat(el_c->value,buf);
								i++;
							}
							if ((i<len) && ((bukv(formula[i])) || (formula[i]=='.')))
								flag_e=false;
						}
						else
							i=i+1; //����� ������� �������� ������
					}
					if (!flag_e)
					{
						buf2[0]='\0';
						strcat(buf2,"������������ ������ '");
						buf2[strlen(buf2)+1]='\0';
						buf2[strlen(buf2)]=formula[i];
						strcat(buf2,"'\n");
						fputs(buf2,log_file);
						fprintf(log_file,"\n");
						if (problem_in)
							delete [] problem_in;
						problem_in=new char[strlen(buf2)+1];
						problem_in[0]='\0';
						strcat(problem_in,buf2);
						flag=false;
					}
				}
				if (!provDopVesh(el_c->value,&prov_buf))
				{
					buf2[0]='\0';
					strcat(buf2,prov_buf);
					if (problem_in)
						delete [] problem_in;
					problem_in=new char[strlen(buf2)+1];
					problem_in[0]='\0';
					strcat(problem_in,buf2);
					flag=false;
				}
			}
			else
				if (bukv(formula[i]))
				{
					buf[0]='\0';
					while ((i<len) && bukv(formula[i]))
					{
						buf2[0]=formula[i];
						buf2[1]='\0';
						strcat(buf,buf2);
						i++;
					}
					if (form(buf) && (formula[i]=='('))
					{
						// ������ ��� �������
						strcat(tokens,"F");
						j=strlen(buf);
						for (int p=0;p<j;p++)
							if ((buf[p]>='A') && (buf[p]<='Z'))
								buf[p]='a'+buf[p]-'A';
						flag_func=true;
						if (!list_f)
						{
							list_f=new elem;
							list_f->value=new char[strlen(buf)+1];
							list_f->value[0]='\0';
							strcat(list_f->value,buf);
							list_f->next=NULL;
							el_f=list_f;
						}
						else
						{
							el_f->next=new elem;
							el_f=el_f->next;
							el_f->value=new char[strlen(buf)+1];
							el_f->value[0]='\0';
							strcat(el_f->value,buf);
							el_f->next=NULL;
						}
					}
					else
					{
							strcat(tokens,"P");
							while ((i<len) && (cyfr(formula[i]) || bukv(formula[i])))
							{
								buf2[0]=formula[i];
								buf2[1]='\0';
								strcat(buf,buf2);
								i++;
							}
							if (!form(buf))
							{
								if (!list_p)
								{
									list_p=new elem;
									list_p->adr=NULL;
									list_p->value=new char[strlen(buf)+1];
									list_p->value[0]='\0';
									strcat(list_p->value,buf);
									list_p->next=NULL;
									el_p=list_p;
								}
								else
								{
									el_p->next=new elem;
									el_p=el_p->next;
									el_p->adr=NULL;
									el_p->value=new char[strlen(buf)+1];
									el_p->value[0]='\0';
									strcat(el_p->value,buf);
									el_p->next=NULL;
								}
							}
							else
							{
								buf2[0]='\0';
								strcat(buf2,"������� ");
								strcat(buf2,buf);
								strcat(buf2," �� ����� ����������\n");
								fputs(buf2,log_file);
								fprintf(log_file,"\n");
								if (problem_in)
									delete [] problem_in;
								problem_in=new char[strlen(buf2)+1];
								problem_in[0]='\0';
								strcat(problem_in,buf2);
								flag=false;
							}
					}
				}
				else
				{
					flag=false;
					buf2[0]='\0';
					strcat(buf2,"������������ ������ '");
					buf2[strlen(buf2)+1]='\0';
					buf2[strlen(buf2)]=formula[i];
					strcat(buf2,"'\n");
					fputs(buf2,log_file);
					fprintf(log_file,"\n");
					if (problem_in)
						delete [] problem_in;
					problem_in=new char[strlen(buf2)+1];
					problem_in[0]='\0';
					strcat(problem_in,buf2);
				}
	}
	if (flag)
	{
		fputs("����������� ������ ��������",log_file);
		fprintf(log_file,"\n");
		el_c=list_c;
		fputs("���������",log_file);
		fprintf(log_file,"\n");
		while (el_c)
		{
			fputs(el_c->value,log_file);
			fprintf(log_file,"\n");
			el_c=el_c->next;
		}
		el_p=list_p;
		fputs("����������",log_file);
		fprintf(log_file,"\n");
		while (el_p)
		{
			fputs(el_p->value,log_file);
			fprintf(log_file,"\n");
			el_p=el_p->next;
		}
		//����������� ������ ������ ����������, � ������� ��� ����������
		if (list_p)
		{
			elem *curel,*last;
			bool flagel;
			list_p_simple=new elem;
			list_p_simple->next=NULL;
			list_p_simple->adr=NULL;
			list_p_simple->value=new char[strlen(list_p->value)+1];
			list_p_simple->value[0]='\0';
			strcat(list_p_simple->value,list_p->value);
			el_p=list_p->next;
			last=list_p_simple;
			while (el_p)
			{
				//��������� ������� �������� �������� � ��� ������������ ������� ������
				flagel=true;
				curel=list_p_simple;
				while (flagel && curel)
				{
					if (!strcmp(curel->value,el_p->value))
						flagel=false;
					curel=curel->next;
				}
				if (flagel)
				{
					last->next=new elem;
					last=last->next;
					last->next=NULL;
					last->adr=NULL;
					last->value=new char[strlen(el_p->value)+1];
					last->value[0]='\0';
					strcat(last->value,el_p->value);
				}
				el_p=el_p->next;	
			}
		}
		fputs("������� ������ ����������",log_file);
		fprintf(log_file,"\n");
		el_p=list_p_simple;
		while (el_p)
		{
			fputs(el_p->value,log_file);
			fprintf(log_file,"\n");
			el_p=el_p->next;
		}
		el_f=list_f;
		fputs("�������",log_file);
		fprintf(log_file,"\n");
		while (el_f)
		{
			fputs(el_f->value,log_file);
			fprintf(log_file,"\n");
			el_f=el_f->next;
		}
		Print_tokens();
	}
	else
	{
		fputs("������ �� ����� ������������ �������",log_file);
		fprintf(log_file,"\n");
		kod_err=1;
		if (problem_perem)
			delete [] problem_perem;
		problem_perem=new char[strlen(getName())+1];
		problem_perem[0]='\0';
		strcat(problem_perem,getName());
		if (problem_in_perem)
			delete [] problem_in_perem;
		problem_in_perem=new char[strlen(getName())+1];
		problem_in_perem[0]='\0';
		strcat(problem_in_perem,getName());
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
		el_c=list_c;
		while (el_c)
		{
			list_c=list_c->next;
			delete [] el_c->value;
			delete el_c;
			el_c=list_c;
		}
		el_f=list_f;
		while (el_f)
		{
			list_f=list_f->next;
			delete [] el_f->value;
			delete el_f;
			el_f=list_f;
		}
		tokens[0]='\0';
	}
	if (prov_buf)
		delete [] prov_buf;
}
void VYRAZ::Parse()
{
	fputs("������ �������",log_file);
	fprintf(log_file,"\n");
	if (flag_iter_leks)
		Leks_anal();
	if (flag)
	{
		if (!flag_iter)
			PeremProv(first);
		if (flag)
			Bauer_Zamelzon();
	}
}



void VYRAZ::OpGeneration(temp_values **first_tempadr,my_stek **first_stekadr,prov_func **func_prov_listadr,perem_list *first)
{
	temp_values *cur_temp,*first_temp=*first_tempadr;
	my_stek *cur_stek,*first_stek=*first_stekadr;
	prov_func *cur_func_prov,*func_prov_list=*func_prov_listadr;
	int func_prov_count1,func_prov_count2,func_prov_count=0;
	char *result1=NULL,*result2=NULL,err_buffer[150];
	cur_temp=first_temp;
	while (cur_temp)
	{
		func_prov_count++;
		cur_temp=cur_temp->next;
	}
	func_prov_count1=func_prov_list->kol-func_prov_list->next->kol; //������ �������
	func_prov_count2=func_prov_count-func_prov_list->kol; //������ �������
	cur_func_prov=func_prov_list;
	func_prov_list=func_prov_list->next;
	delete cur_func_prov;
	switch (first_stek->znak)
	{
	case '+':
	{
		if ((func_prov_count1==1) && (func_prov_count2==1))
		{	
			if (first_temp->next->el)
			{
				//����������
				if (!first_temp->next->el->adr->getResult())
				{
					flag=false;
					kod_err=3;
					if (!first_temp->next->el->adr->problem_vyraz)
						Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->name);
					else
						Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->problem_vyraz);
				}
				else
				{
					if (result1)
						delete [] result1;
					result1=new char[strlen(first_temp->next->el->adr->getResult())+1];
					result1[0]='\0';
					strcat(result1,first_temp->next->el->adr->getResult());
				}
			}
			else
			{
				if (result1)
					delete [] result1;
				result1=new char[strlen(first_temp->next->value)+1];
				result1[0]='\0';
				strcat(result1,first_temp->next->value);
			}
			if (first_temp->el)
			{
				//����������
				if (!first_temp->el->adr->getResult())
				{
					flag=false;
					kod_err=3;
					if (!first_temp->el->adr->problem_vyraz)
						Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
					else
						Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
				}
				else
				{
					if (result2)
						delete [] result2;
					result2=new char[strlen(first_temp->el->adr->getResult())+1];
					result2[0]='\0';
					strcat(result2,first_temp->el->adr->getResult());
				}
			}
			else
			{
				if (result2)
					delete [] result2;
				result2=new char[strlen(first_temp->value)+1];
				result2[0]='\0';
				strcat(result2,first_temp->value);
			}
			if (flag)
			{
				buf[0]='\0';
				strcat(buf,myMath::PLUS(result1,result2));
				if (!strncmp(buf,"-1.#INF",7))
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"��������� ");
						strcat(err_buffer,result1);
						strcat(err_buffer,"+");
						strcat(err_buffer,result2);
						strcat(err_buffer," ������ ������������ ����������� ��� ���������� ��������");
						Save_err(err_buffer,name,name,NULL);
					}
					else
						if (!strncmp(buf,"1.#INF",6))
						{
							flag=false;
							kod_err=3;
							err_buffer[0]='\0';
							strcat(err_buffer,"��������� ");
							strcat(err_buffer,result1);
							strcat(err_buffer,"+");
							strcat(err_buffer,result2);
							strcat(err_buffer," ������ ������������� ����������� ��� ���������� ��������");
							Save_err(err_buffer,name,name,NULL);
						}
						else
							if (bukv(first_temp->next->value[0]) && bukv(first_temp->value[0]))
								fprintf(log_file,"P+P=%s+%s=%s\n",result1,result2,buf);
							else
								if (bukv(first_temp->next->value[0]))
									fprintf(log_file,"P+C=%s+%s=%s\n",result1,result2,buf);
								else
									if (bukv(first_temp->value[0]))
										fprintf(log_file,"C+P=%s+%s=%s\n",result1,result2,buf);
									else
										fprintf(log_file,"C+C=%s+%s=%s\n",result1,result2,buf);
				cur_temp=first_temp;
				first_temp=first_temp->next;
				delete [] cur_temp->value;
				delete cur_temp;
				delete [] first_temp->value;
				first_temp->value=new char[strlen(buf)+1];
				first_temp->value[0]='\0';
				strcat(first_temp->value,buf);
				first_temp->el=NULL;
			}
		}	
		else	
			if ((func_prov_count1==0) && (func_prov_count2==1))
			{	
				if ((first_stek->next->znak=='f') || (first_stek->next->znak=='(') || (first_stek->next->znak==';'))
				{	
					if (first_temp->el)
					{
						//����������
						if (!first_temp->el->adr->getResult())
						{
							flag=false;
							kod_err=3;
							if (!first_temp->el->adr->problem_vyraz)
								Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
							else
								Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
						}
						else
						{
							if (result1)
								delete [] result1;
							result1=new char[strlen(first_temp->el->adr->getResult())+1];
							result1[0]='\0';
							strcat(result1,first_temp->el->adr->getResult());
						}
					}
					else
					{
						if (result1)
							delete [] result1;
						result1=new char[strlen(first_temp->value)+1];
						result1[0]='\0';
						strcat(result1,first_temp->value);
					}
					if (flag)
					{
						buf[0]='\0';
						strcat(buf,myMath::PLUS("0",result1));
						if (bukv(first_temp->value[0]))
							fprintf(log_file,"+P=+%s=%s\n",result1,buf);
						else	
							fprintf(log_file,"+C=+%s=%s\n",result1,buf);
						delete [] first_temp->value;
						first_temp->value=new char[strlen(buf)+1];
						first_temp->value[0]='\0';
						strcat(first_temp->value,buf);
						first_temp->el=NULL;
					}
				}
				else
				{
					flag=false;
					kod_err=3;
					err_buffer[0]='\0';
					strcat(err_buffer,"� �������� '+' ��� ������ ��������");
					fputs(err_buffer,log_file);
					fprintf(log_file,"\n");
					Save_err(err_buffer,name,name,NULL);
				}
			}
			else
				if ((func_prov_count2==0) && (func_prov_count1==1))
				{
					flag=false;
					kod_err=3;
					err_buffer[0]='\0';
					strcat(err_buffer,"� �������� '+' ��� ������� ��������");
					fputs(err_buffer,log_file);
					fprintf(log_file,"\n");
					Save_err(err_buffer,name,name,NULL);
				}
				else
					if ((func_prov_count2==0) && (func_prov_count1==0))
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"� �������� '+' ��� ����� ���������");
						fputs(err_buffer,log_file);
						fprintf(log_file,"\n");
						Save_err(err_buffer,name,name,NULL);
					}
					else
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� �������� '+'");
						fputs(err_buffer,log_file);
						fprintf(log_file,"\n");
						Save_err(err_buffer,name,name,NULL);
					}
		}break;
	case '-':
	{
		if ((func_prov_count1==1) && (func_prov_count2==1))
		{	
			if (first_temp->next->el)
			{
				//����������
				if (!first_temp->next->el->adr->getResult())
				{
					flag=false;
					kod_err=3;
					if (!first_temp->next->el->adr->problem_vyraz)
						Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->name);
					else
						Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->problem_vyraz);
				}
				else
				{
					if (result1)
						delete [] result1;
					result1=new char[strlen(first_temp->next->el->adr->getResult())+1];
					result1[0]='\0';
					strcat(result1,first_temp->next->el->adr->getResult());
				}
			}
			else
			{
				if (result1)
					delete [] result1;
				result1=new char[strlen(first_temp->next->value)+1];
				result1[0]='\0';
				strcat(result1,first_temp->next->value);
			}
			if (first_temp->el)
			{
				//����������
				if (!first_temp->el->adr->getResult())
				{
					flag=false;
					kod_err=3;
					if (!first_temp->el->adr->problem_vyraz)
						Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
					else
						Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
				}
				else
				{
					if (result2)
						delete [] result2;
					result2=new char[strlen(first_temp->el->adr->getResult())+1];
					result2[0]='\0';
					strcat(result2,first_temp->el->adr->getResult());
				}
			}
			else
			{
				if (result2)
					delete [] result2;
				result2=new char[strlen(first_temp->value)+1];
				result2[0]='\0';
				strcat(result2,first_temp->value);
			}
			if (flag)
			{
				buf[0]='\0';
				strcat(buf,myMath::MINUS(result1,result2));
				if (!strncmp(buf,"-1.#INF",7))
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"��������� ");
						strcat(err_buffer,result1);
						strcat(err_buffer,"-");
						strcat(err_buffer,result2);
						strcat(err_buffer," ������ ������������ ����������� ��� ���������� ��������");
						Save_err(err_buffer,name,name,NULL);
					}
					else
						if (!strncmp(buf,"1.#INF",6))
						{
							flag=false;
							kod_err=3;
							err_buffer[0]='\0';
							strcat(err_buffer,"��������� ");
							strcat(err_buffer,result1);
							strcat(err_buffer,"-");
							strcat(err_buffer,result2);
							strcat(err_buffer," ������ ������������� ����������� ��� ���������� ��������");
							Save_err(err_buffer,name,name,NULL);
						}
						else
							if (bukv(first_temp->next->value[0]) && bukv(first_temp->value[0]))
								fprintf(log_file,"P-P=%s-%s=%s\n",result1,result2,buf);
							else
								if (bukv(first_temp->next->value[0]))
									fprintf(log_file,"P-C=%s-%s=%s\n",result1,result2,buf);
								else
									if (bukv(first_temp->value[0]))
										fprintf(log_file,"C-P=%s-%s=%s\n",result1,result2,buf);
									else
										fprintf(log_file,"C-C=%s-%s=%s\n",result1,result2,buf);
				cur_temp=first_temp;
				first_temp=first_temp->next;
				delete [] cur_temp->value;
				delete cur_temp;
				delete [] first_temp->value;
				first_temp->value=new char[strlen(buf)+1];
				first_temp->value[0]='\0';
				strcat(first_temp->value,buf);
				first_temp->el=NULL;
			}
		}	
		else	
			if ((func_prov_count1==0) && (func_prov_count2==1))
			{	
				if ((first_stek->next->znak=='f') || (first_stek->next->znak=='(') || (first_stek->next->znak==';'))
				{	
					if (first_temp->el)
					{
						//����������
						if (!first_temp->el->adr->getResult())
						{
							flag=false;
							kod_err=3;
							if (!first_temp->el->adr->problem_vyraz)
								Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
							else
								Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
						}
						else
						{
							if (result1)
								delete [] result1;
							result1=new char[strlen(first_temp->el->adr->getResult())+1];
							result1[0]='\0';
							strcat(result1,first_temp->el->adr->getResult());
						}
					}
					else
					{
						if (result1)
							delete [] result1;
						result1=new char[strlen(first_temp->value)+1];
						result1[0]='\0';
						strcat(result1,first_temp->value);
					}
					if (flag)
					{
						buf[0]='\0';
						strcat(buf,myMath::MINUS("0",result1));
						if (bukv(first_temp->value[0]))
							fprintf(log_file,"-P=-%s=%s\n",result1,buf);
						else	
							fprintf(log_file,"-C=-%s=%s\n",result1,buf);
						delete [] first_temp->value;
						first_temp->value=new char[strlen(buf)+1];
						first_temp->value[0]='\0';
						strcat(first_temp->value,buf);
						first_temp->el=NULL;
					}
				}
				else
				{
					flag=false;
					kod_err=3;
					err_buffer[0]='\0';
					strcat(err_buffer,"� �������� '-' ��� ������ ��������");
					fputs(err_buffer,log_file);
					fprintf(log_file,"\n");
					Save_err(err_buffer,name,name,NULL);
				}
			}
			else
				if ((func_prov_count2==0) && (func_prov_count1==1))
				{
					flag=false;
					kod_err=3;
					err_buffer[0]='\0';
					strcat(err_buffer,"� �������� '-' ��� ������� ��������");
					fputs(err_buffer,log_file);
					fprintf(log_file,"\n");
					Save_err(err_buffer,name,name,NULL);
				}
				else
					if ((func_prov_count2==0) && (func_prov_count1==0))
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"� �������� '-' ��� ����� ���������");
						fputs(err_buffer,log_file);
						fprintf(log_file,"\n");
						Save_err(err_buffer,name,name,NULL);
					}
					else
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� �������� '-'");
						fputs(err_buffer,log_file);
						fprintf(log_file,"\n");
						Save_err(err_buffer,name,name,NULL);
					}
		}break;
	case '*':
	{
		if ((func_prov_count1==1) && (func_prov_count2==1))
		{	
			if (first_temp->next->el)
			{
				//����������
				if (!first_temp->next->el->adr->getResult())
				{
					flag=false;
					kod_err=3;
					if (!first_temp->next->el->adr->problem_vyraz)
						Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->name);
					else
						Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->problem_vyraz);
				}
				else
				{
					if (result1)
						delete [] result1;
					result1=new char[strlen(first_temp->next->el->adr->getResult())+1];
					result1[0]='\0';
					strcat(result1,first_temp->next->el->adr->getResult());
				}
			}
			else
			{
				if (result1)
					delete [] result1;
				result1=new char[strlen(first_temp->next->value)+1];
				result1[0]='\0';
				strcat(result1,first_temp->next->value);
			}
			if (first_temp->el)
			{
				//����������
				if (!first_temp->el->adr->getResult())
				{
					flag=false;
					kod_err=3;
					if (!first_temp->el->adr->problem_vyraz)
						Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
					else
						Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
				}
				else
				{
					if (result2)
						delete [] result2;
					result2=new char[strlen(first_temp->el->adr->getResult())+1];
					result2[0]='\0';
					strcat(result2,first_temp->el->adr->getResult());
				}
			}
			else
			{
				if (result2)
					delete [] result2;
				result2=new char[strlen(first_temp->value)+1];
				result2[0]='\0';
				strcat(result2,first_temp->value);
			}
			if (flag)
			{
				buf[0]='\0';
				strcat(buf,myMath::UMN(result1,result2));
				if (!strncmp(buf,"-1.#INF",7))
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"��������� ");
						strcat(err_buffer,result1);
						strcat(err_buffer,"*");
						strcat(err_buffer,result2);
						strcat(err_buffer," ������ ������������ ����������� ��� ���������� ��������");
						Save_err(err_buffer,name,name,NULL);
					}
					else
						if (!strncmp(buf,"1.#INF",6))
						{
							flag=false;
							kod_err=3;
							err_buffer[0]='\0';
							strcat(err_buffer,"��������� ");
							strcat(err_buffer,result1);
							strcat(err_buffer,"*");
							strcat(err_buffer,result2);
							strcat(err_buffer," ������ ������������� ����������� ��� ���������� ��������");
							Save_err(err_buffer,name,name,NULL);
						}
						else
							if (bukv(first_temp->next->value[0]) && bukv(first_temp->value[0]))
								fprintf(log_file,"P*P=%s*%s=%s\n",result1,result2,buf);	
							else
								if (bukv(first_temp->next->value[0]))
									fprintf(log_file,"P*C=%s*%s=%s\n",result1,result2,buf);
								else
									if (bukv(first_temp->value[0]))
										fprintf(log_file,"C*P=%s*%s=%s\n",result1,result2,buf);
									else
										fprintf(log_file,"C*C=%s*%s=%s\n",result1,result2,buf);
				cur_temp=first_temp;
				first_temp=first_temp->next;
				delete [] cur_temp->value;
				delete cur_temp;
				delete [] first_temp->value;
				first_temp->value=new char[strlen(buf)+1];
				first_temp->value[0]='\0';
				strcat(first_temp->value,buf);
				first_temp->el=NULL;
			}
		}	
		else	
			if ((func_prov_count1==0) && (func_prov_count2==1))
			{	
				flag=false;
				kod_err=3;
				err_buffer[0]='\0';
				strcat(err_buffer,"� �������� '*' ��� ������ ��������");
				fputs(err_buffer,log_file);
				fprintf(log_file,"\n");
				Save_err(err_buffer,name,name,NULL);
			}
			else
				if ((func_prov_count2==0) && (func_prov_count1==1))
				{
					flag=false;
					kod_err=3;
					err_buffer[0]='\0';
					strcat(err_buffer,"� �������� '*' ��� ������� ��������");
					fputs(err_buffer,log_file);
					fprintf(log_file,"\n");
					Save_err(err_buffer,name,name,NULL);
				}
				else
					if ((func_prov_count2==0) && (func_prov_count1==0))
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"� �������� '*' ��� ����� ���������");
						fputs(err_buffer,log_file);
						fprintf(log_file,"\n");
						Save_err(err_buffer,name,name,NULL);
					}
					else
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� �������� '*'");
						fputs(err_buffer,log_file);
						fprintf(log_file,"\n");
						Save_err(err_buffer,name,name,NULL);
					}
		}break;
	case '/':
	{
		if ((func_prov_count1==1) && (func_prov_count2==1))
		{	
			if (first_temp->next->el)
			{
				//����������
				if (!first_temp->next->el->adr->getResult())
				{
					flag=false;
					kod_err=3;
					if (!first_temp->next->el->adr->problem_vyraz)
						Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->name);
					else
						Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->problem_vyraz);
				}
				else
				{
					if (result1)
						delete [] result1;
					result1=new char[strlen(first_temp->next->el->adr->getResult())+1];
					result1[0]='\0';
					strcat(result1,first_temp->next->el->adr->getResult());
				}
			}
			else
			{
				if (result1)
					delete [] result1;
				result1=new char[strlen(first_temp->next->value)+1];
				result1[0]='\0';
				strcat(result1,first_temp->next->value);
			}
			if (first_temp->el)
			{
				//����������
				if (!first_temp->el->adr->getResult())
				{
					flag=false;
					kod_err=3;
					if (!first_temp->el->adr->problem_vyraz)
						Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
					else
						Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
				}
				else
				{
					if (result2)
						delete [] result2;
					result2=new char[strlen(first_temp->el->adr->getResult())+1];
					result2[0]='\0';
					strcat(result2,first_temp->el->adr->getResult());
				}
			}
			else
			{
				if (result2)
					delete [] result2;
				result2=new char[strlen(first_temp->value)+1];
				result2[0]='\0';
				strcat(result2,first_temp->value);
			}
			if (flag)
			{
				buf[0]='\0';
				strcat(buf,myMath::DEL(result1,result2));
				if (!strncmp(buf,"-1.#IND",7))
				{
					flag=false;
					kod_err=3;
					err_buffer[0]='\0';
					strcat(err_buffer,"������� �� 0 - ");
					strcat(err_buffer,result1);
					strcat(err_buffer,"/");
					strcat(err_buffer,result2);
					Save_err(err_buffer,name,name,NULL);
				}
				else
					if (!strncmp(buf,"-1.#INF",7))
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"��������� ");
						strcat(err_buffer,result1);
						strcat(err_buffer,"/");
						strcat(err_buffer,result2);
						strcat(err_buffer," ������ ������������ ����������� ��� ���������� ��������");
						Save_err(err_buffer,name,name,NULL);
					}
					else
						if (!strncmp(buf,"1.#INF",6))
						{
							flag=false;
							kod_err=3;
							err_buffer[0]='\0';
							strcat(err_buffer,"��������� ");
							strcat(err_buffer,result1);
							strcat(err_buffer,"/");
							strcat(err_buffer,result2);
							strcat(err_buffer," ������ ������������� ����������� ��� ���������� ��������");
							Save_err(err_buffer,name,name,NULL);
						}
						else
							if (bukv(first_temp->next->value[0]) && bukv(first_temp->value[0]))
								fprintf(log_file,"P/P=%s/%s=%s\n",result1,result2,buf);
							else
								if (bukv(first_temp->next->value[0]))
									fprintf(log_file,"P/C=%s/%s=%s\n",result1,result2,buf);
								else
									if (bukv(first_temp->value[0]))
										fprintf(log_file,"C/P=%s/%s=%s\n",result1,result2,buf);
									else
										fprintf(log_file,"C/C=%s/%s=%s\n",result1,result2,buf);
				cur_temp=first_temp;
				first_temp=first_temp->next;
				delete [] cur_temp->value;
				delete cur_temp;
				delete [] first_temp->value;
				first_temp->value=new char[strlen(buf)+1];
				first_temp->value[0]='\0';
				strcat(first_temp->value,buf);
				first_temp->el=NULL;
			}
		}	
		else	
			if ((func_prov_count1==0) && (func_prov_count2==1))
			{	
				flag=false;
				kod_err=3;
				err_buffer[0]='\0';
				strcat(err_buffer,"� �������� '/' ��� ������ ��������");
				fputs(err_buffer,log_file);
				fprintf(log_file,"\n");
				Save_err(err_buffer,name,name,NULL);
			}
			else
				if ((func_prov_count2==0) && (func_prov_count1==1))
				{
					flag=false;
					kod_err=3;
					err_buffer[0]='\0';
					strcat(err_buffer,"� �������� '/' ��� ������� ��������");
					fputs(err_buffer,log_file);
					fprintf(log_file,"\n");
					Save_err(err_buffer,name,name,NULL);
				}
				else
					if ((func_prov_count2==0) && (func_prov_count1==0))
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"� �������� '/' ��� ����� ���������");
						fputs(err_buffer,log_file);
						fprintf(log_file,"\n");
						Save_err(err_buffer,name,name,NULL);
					}
					else
					{
						flag=false;
						kod_err=3;
						err_buffer[0]='\0';
						strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� �������� '/'");
						fputs(err_buffer,log_file);
						fprintf(log_file,"\n");
						Save_err(err_buffer,name,name,NULL);
					}
		}break;
	}
	*first_tempadr=first_temp;
	*first_stekadr=first_stek;
	*func_prov_listadr=func_prov_list;
}
void VYRAZ::Bauer_Zamelzon()
{
	fputs("���������� �������� ���������",log_file);
	fprintf(log_file,"\n");
	char mas[5][6]={{'1','1','?','1','?','+'},
				    {'2','1','4','1','4','4'},
				    {'4','2','4','1','4','4'},
				    {'1','1','?','1','6','?'},
					{'1','1','5','1','7','?'}};
	temp_values *first_temp=NULL, *cur_temp;
	my_stek *first_stek, *cur_stek;
	prov_func *func_prov_list, *cur_func_prov;
	elem *cur;
	char *result1=NULL,*result2=NULL;
	char err_buffer[150];
	prov_func *indf_stek=NULL;
	int func_prov_count,func_prov_count1,func_prov_count2;
	int i=0,len=strlen(tokens),indc=-1,indp=-1,indf=-1,j;
	int curind;
	char curvar;
	func_prov_list=new prov_func;
	func_prov_list->kol=0;
	func_prov_list->next=NULL;
	first_stek=new my_stek;
	first_stek->znak='f';
	first_stek->ind=0;
	first_stek->next=NULL;
	if (flag_iter && (list_p) && (!strcmp(list_p->value,"ERROR")))
	{
		flag=false;
		kod_err=3;
		err_buffer[0]='\0';
		strcat(err_buffer,"������������ ������ �� ���������� ���������");
		fputs(err_buffer,log_file);
		fprintf(log_file,"\n");
		Save_err(err_buffer,name,name,NULL);
	}
	while (flag && (i<=len))
	{
		if (tokens[i+1] && (tokens[i+1]=='(') && ((tokens[i]=='P') || (tokens[i]=='C') || (tokens[i]==')')))   // ������, ������� �� ������������� �������� ������ �������
		{
			flag=false;
			kod_err=3;
			err_buffer[0]='\0';
			strcat(err_buffer,"����������� �������� ����� '('");
			fputs(err_buffer,log_file);
			fprintf(log_file,"\n");
			Save_err(err_buffer,name,name,NULL);
		}
		else
		{
			switch (tokens[i])
			{
				case '+': curind=0; break;
				case '-': curind=0; break;
				case '*': curind=1; break;
				case '/': curind=1; break;
				case ';': curind=2; break;
				case '(': curind=3; break;
				case ')': curind=4; break;
				case '\0': curind=5; break;
				default: curind=6; break;
			}
			if (curind==6)
			{
				// ������� ������ - ����� C,P ��� F
				if (tokens[i]=='F')
				{
					cur_stek=new my_stek;
					cur_stek->znak='F';
					cur_stek->next=first_stek;
					cur_stek->ind=first_stek->ind;
					first_stek=cur_stek;
					cur_func_prov=new prov_func;
					indf++;
					cur_func_prov->kol=indf;
					cur_func_prov->next=indf_stek;
					indf_stek=cur_func_prov;
				}
				else
				{
					if (tokens[i]=='C')
					{
						indc++;
						cur=list_c;
						for (j=0;j<indc;j++)
							cur=cur->next;
						cur_temp=new temp_values;
						cur_temp->next=first_temp;
						cur_temp->value=new char[strlen(cur->value)+1];
						cur_temp->value[0]='\0';
						strcat(cur_temp->value,cur->value);
						cur_temp->el=NULL;
						first_temp=cur_temp;
					}
					else
					{
						indp++;
						cur=list_p;
						for (j=0;j<indp;j++)
							cur=cur->next;
						cur_temp=new temp_values;
						cur_temp->next=first_temp;
						cur_temp->value=new char[strlen(cur->value)+1];
						cur_temp->value[0]='\0';
						strcat(cur_temp->value,cur->value);
						cur_temp->el=cur;
						first_temp=cur_temp;
					}
				}
				i++;
			}
			else
			{
				curvar=mas[first_stek->ind][curind];
				switch (curvar)
				{
				case '1':{//������� �������� � ���� �������� � ������ ��������� ������
							cur_stek=new my_stek;
							cur_stek->znak=tokens[i];
							cur_stek->ind=curind+1;
							cur_stek->next=first_stek;
							first_stek=cur_stek;
							i++;
							cur_func_prov=new prov_func;
							cur_func_prov->next=func_prov_list;
							func_prov_list=cur_func_prov;
							cur_temp=first_temp;
							func_prov_count=0;
							while (cur_temp)
							{
								func_prov_count++;
								cur_temp=cur_temp->next;
							}
							func_prov_list->kol=func_prov_count;
						 }break;
				case '2':{//������������ ���������� ��������, ������� �������� � ���� �������� � ������ ��������� ������  
							OpGeneration(&first_temp,&first_stek,&func_prov_list,first);
							first_stek->znak=tokens[i];
							cur_temp=first_temp;
							func_prov_count=0;
							while (cur_temp)
							{
								func_prov_count++;
								cur_temp=cur_temp->next;
							}
							cur_func_prov=new prov_func;
							cur_func_prov->next=func_prov_list;
							func_prov_list=cur_func_prov;
							func_prov_list->kol=func_prov_count;
							i++;
						 }break;
					case '4':{//������������ �������� � ������������� ��� �� ������� ������
								OpGeneration(&first_temp,&first_stek,&func_prov_list,first);
								cur_stek=first_stek;
								first_stek=first_stek->next;
								delete cur_stek;
							 }break;
					case '+':
						{/*� ����� �������� ������ ������ ������� ������, � ������������ ������� - \0. 
							� ����� ������ � temp_values ������������ ������� - ���������.*/
							if ((first_stek->znak=='f') && (first_stek->next==NULL) && (first_temp) && (first_temp->next==NULL) && (func_prov_list->kol==0) && (func_prov_list->next==NULL))
							{
								if (first_temp->el)
									if (first_temp->el->adr->getResult())
									{
										buf[0]='\0';
										strcat(buf,first_temp->el->adr->getResult());
										result=new double;
										*result=atof(buf);
										delete [] first_temp->value;
										first_temp->value=new char[strlen(buf)+1];
										first_temp->value[0]='\0';
										strcat(first_temp->value,buf);
									}
									else
									{
										flag=false;
										kod_err=3;
										i++;
										if (!first_temp->el->adr->problem_vyraz)
											Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
										else
											Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
									}
								else
								{
									result=new double;
									*result=atof(first_temp->value);
								}
								if (flag)
								{
									delete [] first_temp->value;
									delete first_temp;
									delete first_stek;
									i++; // ��� ������ �� ��������� �����
								}
							}
							else
							{
								flag=false;
								kod_err=3;
								err_buffer[0]='\0';
								strcat(err_buffer,"��������� �����������");
								fputs(err_buffer,log_file);
								fprintf(log_file,"\n");
								Save_err(err_buffer,name,name,NULL);
							}
						}break;
					case '5':
						{
							/* 1 - ���� � ����� F(
							? - ���� � ����� ��� ���������� F(*/
							if ((first_stek->znak=='(') && (first_stek->next->znak=='F'))
							{
								cur_stek=new my_stek;
								cur_stek->znak=tokens[i];
								cur_stek->ind=curind+1;
								cur_stek->next=first_stek;
								first_stek=cur_stek;
								i++;
								cur_func_prov=new prov_func;
								cur_func_prov->next=func_prov_list;
								func_prov_list=cur_func_prov;
								cur_temp=first_temp;
								func_prov_count=0;
								while (cur_temp)
								{
									func_prov_count++;
									cur_temp=cur_temp->next;
								}
								func_prov_list->kol=func_prov_count;
							}
							else
							{
								flag=false;
								kod_err=3;
								err_buffer[0]='\0';
								strcat(err_buffer,"���� ';' ��������� �� �������� �������");
								fputs(err_buffer,log_file);
								fprintf(log_file,"\n");
								Save_err(err_buffer,name,name,NULL);
							}
						}break;
					case '6':
						{//������������ �������, �� ����� ������ "F(;", ������ ��������� ������
							cur=list_f;
							for (j=0;j<indf_stek->kol;j++)
								cur=cur->next;
							cur_func_prov=indf_stek;
							indf_stek=indf_stek->next;
							delete cur_func_prov;
							buf[0]='\0';
							strcat(buf,cur->value);
							cur_stek=first_stek;
							for (j=0;j<3;j++)
							{
								first_stek=cur_stek->next;
								delete cur_stek;
								cur_stek=first_stek;
							}
							cur_temp=first_temp;
							func_prov_count=0;
							while (cur_temp)
							{
								func_prov_count++;
								cur_temp=cur_temp->next;
							}
							func_prov_count1=func_prov_list->kol-func_prov_list->next->kol;// ������ ��������
							func_prov_count2=func_prov_count-func_prov_list->kol; // ������ ��������
							cur_func_prov=func_prov_list;
							func_prov_list=func_prov_list->next;
							delete cur_func_prov;
							if (!strcmp(buf,"sin"))
							{
								flag=false;
								kod_err=3;
								err_buffer[0]='\0';
								strcat(err_buffer,"������� sin ������� ������ ������ ���������!!!");
								fputs(err_buffer,log_file);
								fprintf(log_file,"\n");
								Save_err(err_buffer,name,name,NULL);
							}
							else
								if (!strcmp(buf,"cos"))
								{
									flag=false;
									kod_err=3;
									err_buffer[0]='\0';
									strcat(err_buffer,"������� cos ������� ������ ������ ���������!!!");
									fputs(err_buffer,log_file);
									fprintf(log_file,"\n");
									Save_err(err_buffer,name,name,NULL);
								}
								else
									if (!strcmp(buf,"tg"))
									{
										flag=false;
										kod_err=3;
										err_buffer[0]='\0';
										strcat(err_buffer,"������� tg ������� ������ ������ ���������!!!");
										fputs(err_buffer,log_file);
										fprintf(log_file,"\n");
										Save_err(err_buffer,name,name,NULL);
									}
									else
										if (!strcmp(buf,"ctg"))
										{
											flag=false;
											kod_err=3;
											err_buffer[0]='\0';
											strcat(err_buffer,"������� ctg ������� ������ ������ ���������!!!");
											fputs(err_buffer,log_file);
											fprintf(log_file,"\n");
											Save_err(err_buffer,name,name,NULL);
										}
										else
											if (!strcmp(buf,"arcsin"))
											{
												flag=false;
												kod_err=3;
												err_buffer[0]='\0';
												strcat(err_buffer,"������� arcsin ������� ������ ������ ���������!!!");
												fputs(err_buffer,log_file);
												fprintf(log_file,"\n");
												Save_err(err_buffer,name,name,NULL);
											}
											else
												if (!strcmp(buf,"arccos"))
												{
													flag=false;
													kod_err=3;
													err_buffer[0]='\0';
													strcat(err_buffer,"������� arccos ������� ������ ������ ���������!!!");
													fputs(err_buffer,log_file);
													fprintf(log_file,"\n");
													Save_err(err_buffer,name,name,NULL);
												}
												else
													if (!strcmp(buf,"arctg"))
													{
														flag=false;
														kod_err=3;
														err_buffer[0]='\0';
														strcat(err_buffer,"������� arctg ������� ������ ������ ���������!!!");
														fputs(err_buffer,log_file);
														fprintf(log_file,"\n");
														Save_err(err_buffer,name,name,NULL);
													}
													else
														if (!strcmp(buf,"arcctg"))
														{
															flag=false;
															kod_err=3;
															err_buffer[0]='\0';
															strcat(err_buffer,"������� arcctg ������� ������ ������ ���������!!!");
															fputs(err_buffer,log_file);
															fprintf(log_file,"\n");
															Save_err(err_buffer,name,name,NULL);
														}
														else
															if (!strcmp(buf,"log"))
															{
																if ((func_prov_count1==1) && (func_prov_count2==1))
																{
																	if (first_temp->next->el)
																	{
																		//����������
																		if (!first_temp->next->el->adr->getResult())
																		{
																			flag=false;
																			kod_err=3;
																			if (!first_temp->next->el->adr->problem_vyraz)
																				Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->name);
																			else
																				Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->problem_vyraz);
																		}
																		else
																		{
																			if (result1)
																				delete [] result1;
																			result1=new char[strlen(first_temp->next->el->adr->getResult())+1];
																			result1[0]='\0';
																			strcat(result1,first_temp->next->el->adr->getResult());
																		}
																	}
																	else
																	{
																		if (result1)
																			delete [] result1;
																		result1=new char[strlen(first_temp->next->value)+1];
																		result1[0]='\0';
																		strcat(result1,first_temp->next->value);
																	}
																	if (first_temp->el)
																	{
																		//����������
																		if (!first_temp->el->adr->getResult())
																		{
																			flag=false;
																			kod_err=3;
																			if (!first_temp->el->adr->problem_vyraz)
																				Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																			else
																				Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																		}
																		else
																		{
																			if (result2)
																				delete [] result2;
																			result2=new char[strlen(first_temp->el->adr->getResult())+1];
																			result2[0]='\0';
																			strcat(result2,first_temp->el->adr->getResult());
																		}
																	}
																	else
																	{
																		if (result2)
																			delete [] result2;
																		result2=new char[strlen(first_temp->value)+1];
																		result2[0]='\0';
																		strcat(result2,first_temp->value);
																	}
																	if (flag)
																	{
																		buf[0]='\0';
																		strcat(buf,myMath::LOG(result1,result2));
																		if (!strncmp(buf,"-1.#IND",7))
																		{
																			flag=false;
																			kod_err=3;
																			err_buffer[0]='\0';
																			strcat(err_buffer,"������������ �������� ���������� ������� log - log(");
																			strcat(err_buffer,result1);
																			strcat(err_buffer,";");
																			strcat(err_buffer,result2);
																			strcat(err_buffer,")");
																			Save_err(err_buffer,name,name,NULL);
																		}
																		else
																			if (bukv(first_temp->value[0]) && bukv(first_temp->next->value[0]))
																				fprintf(log_file,"F(P;P)=log(%s;%s)=log(%s;%s)=%s\n",first_temp->next->value,first_temp->value,result1,result2,buf);
																			else 
																				if (bukv(first_temp->value[0]))
																					fprintf(log_file,"F(C;P)=log(%s;%s)=log(%s;%s)=%s\n",first_temp->next->value,first_temp->value,result1,result2,buf);
																				else 
																					if (bukv(first_temp->next->value[0]))
																						fprintf(log_file,"F(P;C)=log(%s;%s)=log(%s;%s)=%s\n",first_temp->next->value,first_temp->value,result1,result2,buf);
																					else
																						fprintf(log_file,"F(C;C)=log(%s;%s)=%s\n",first_temp->next->value,first_temp->value,buf);
																		delete [] first_temp->value;
																		cur_temp=first_temp;
																		first_temp=first_temp->next;
																		delete cur_temp;
																		delete [] first_temp->value;
																		first_temp->value=new char[strlen(buf)+1];
																		first_temp->value[0]='\0';
																		strcat(first_temp->value,buf);
																		first_temp->el=NULL;
																		cur_func_prov=func_prov_list;
																		func_prov_list=func_prov_list->next;
																		delete cur_func_prov;
																	}
																}
																else
																{
																	flag=false;
																	kod_err=3;
																	err_buffer[0]='\0';
																	if ((func_prov_count1==1) && (func_prov_count2==0))
																		strcat(err_buffer,"� ������� log ����������� ������ ��������");
																	else
																		if ((func_prov_count1==0) && (func_prov_count2==1))
																			strcat(err_buffer,"� ������� log ����������� ������ ��������");
																		else
																			if ((func_prov_count1==0) && (func_prov_count2==0))
																				strcat(err_buffer,"� ������� log ����������� ��� ���������");
																			else
																				strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� log");	
																	fputs(err_buffer,log_file);
																	fprintf(log_file,"\n");
																	Save_err(err_buffer,name,name,NULL);
																}
															}
															else
																if (!strcmp(buf,"lg"))
																{
																	flag=false;
																	kod_err=3;
																	err_buffer[0]='\0';
																	strcat(err_buffer,"������� lg ������� ������ ������ ���������!!!");
																	fputs(err_buffer,log_file);
																	fprintf(log_file,"\n");
																	Save_err(err_buffer,name,name,NULL);
																}
																else
																	if (!strcmp(buf,"ln"))
																	{
																		flag=false;
																		kod_err=3;
																		err_buffer[0]='\0';
																		strcat(err_buffer,"������� ln ������� ������ ������ ���������!!!");
																		fputs(err_buffer,log_file);
																		fprintf(log_file,"\n");
																		Save_err(err_buffer,name,name,NULL);
																	}
																	else
																		if (!strcmp(buf,"pi"))
																		{
																			flag=false;
																			kod_err=3;
																			err_buffer[0]='\0';
																			strcat(err_buffer,"������� pi �� ������� ����������!!!");
																			fputs(err_buffer,log_file);
																			fprintf(log_file,"\n");
																			Save_err(err_buffer,name,name,NULL);
																		}
																		else
																			if (!strcmp(buf,"e"))
																			{
																				flag=false;
																				kod_err=3;
																				err_buffer[0]='\0';
																				strcat(err_buffer,"������� e �� ������� ����������!!!");
																				fputs(err_buffer,log_file);
																				fprintf(log_file,"\n");
																				Save_err(err_buffer,name,name,NULL);
																			}
																			else
																				if (!strcmp(buf,"sqrt"))
																				{
																					flag=false;
																					kod_err=3;
																					err_buffer[0]='\0';
																					strcat(err_buffer,"������� sqrt ������� ������ ������ ���������!!!");
																					fputs(err_buffer,log_file);
																					fprintf(log_file,"\n");
																					Save_err(err_buffer,name,name,NULL);
																				}
																				else
																					if (!strcmp(buf,"sqr"))
																					{
																						flag=false;
																						kod_err=3;
																						err_buffer[0]='\0';
																						strcat(err_buffer,"������� sqr ������� ������ ������ ���������!!!");
																						fputs(err_buffer,log_file);
																						fprintf(log_file,"\n");
																						Save_err(err_buffer,name,name,NULL);
																					}
																					else
																						if (!strcmp(buf,"pow"))
																						{
																							if ((func_prov_count1==1) && (func_prov_count2==1))
																							{
																								if (first_temp->next->el)
																								{
																									//����������
																									if (!first_temp->next->el->adr->getResult())
																									{
																										flag=false;
																										kod_err=3;
																										if (!first_temp->next->el->adr->problem_vyraz)
																											Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->name);
																										else
																											Save_err(first_temp->next->el->adr->problem_in,first_temp->next->el->adr->name,first_temp->next->el->adr->problem_in_perem,first_temp->next->el->adr->problem_vyraz);
																									}
																									else
																									{
																										if (result1)
																											delete [] result1;
																										result1=new char[strlen(first_temp->next->el->adr->getResult())+1];
																										result1[0]='\0';
																										strcat(result1,first_temp->next->el->adr->getResult());
																									}
																								}
																								else
																								{
																									if (result1)
																										delete [] result1;
																									result1=new char[strlen(first_temp->next->value)+1];
																									result1[0]='\0';
																									strcat(result1,first_temp->next->value);
																								}
																								if (first_temp->el)
																								{
																									//����������
																									if (!first_temp->el->adr->getResult())
																									{
																										flag=false;
																										kod_err=3;
																										if (!first_temp->el->adr->problem_vyraz)
																											Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																										else
																											Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																									}
																									else
																									{
																										if (result2)
																											delete [] result2;
																										result2=new char[strlen(first_temp->el->adr->getResult())+1];
																										result2[0]='\0';
																										strcat(result2,first_temp->el->adr->getResult());
																									}
																								}
																								else
																								{
																									if (result2)
																										delete [] result2;
																									result2=new char[strlen(first_temp->value)+1];
																									result2[0]='\0';
																									strcat(result2,first_temp->value);
																								}
																								if (flag)
																								{
																									buf[0]='\0';
																									strcat(buf,myMath::POW(result1,result2));
																									if (!strncmp(buf,"-1.#IND",7))
																									{
																										flag=false;
																										kod_err=3;
																										err_buffer[0]='\0';
																										strcat(err_buffer,"������������ �������� ���������� ������� pow - pow(");
																										strcat(err_buffer,result1);
																										strcat(err_buffer,";");
																										strcat(err_buffer,result2);
																										strcat(err_buffer,")");
																										Save_err(err_buffer,name,name,NULL);
																									}
																									else
																										if (!strncmp(buf,"-1.#INF",7))
																										{
																											flag=false;
																											kod_err=3;
																											err_buffer[0]='\0';
																											strcat(err_buffer,"��������� pow(");
																											strcat(err_buffer,result1);
																											strcat(err_buffer,";");
																											strcat(err_buffer,result2);
																											strcat(err_buffer,") ������ ������������ ����������� ��� ���������� ��������");
																											Save_err(err_buffer,name,name,NULL);
																										}
																										else
																											if (!strncmp(buf,"1.#INF",6))
																											{
																												flag=false;
																												kod_err=3;
																												err_buffer[0]='\0';
																												strcat(err_buffer,"��������� pow(");
																												strcat(err_buffer,result1);
																												strcat(err_buffer,";");
																												strcat(err_buffer,result2);
																												strcat(err_buffer,") ������ ������������� ����������� ��� ���������� ��������");
																												Save_err(err_buffer,name,name,NULL);
																											}
																											else
																												if (bukv(first_temp->value[0]) && bukv(first_temp->next->value[0]))
																													fprintf(log_file,"F(P;P)=pow(%s;%s)=pow(%s;%s)=%s\n",first_temp->next->value,first_temp->value,result1,result2,buf);
																												else 
																													if (bukv(first_temp->value[0]))
																														fprintf(log_file,"F(C;P)=pow(%s;%s)=pow(%s;%s)=%s\n",first_temp->next->value,first_temp->value,result1,result2,buf);
																													else 
																														if (bukv(first_temp->next->value[0]))
																															fprintf(log_file,"F(P;C)=pow(%s;%s)=pow(%s;%s)=%s\n",first_temp->next->value,first_temp->value,result1,result2,buf);
																														else
																															fprintf(log_file,"F(C;C)=pow(%s;%s)=%s\n",first_temp->next->value,first_temp->value,buf);
																									delete [] first_temp->value;
																									cur_temp=first_temp;
																									first_temp=first_temp->next;
																									delete cur_temp;
																									delete [] first_temp->value;
																									first_temp->value=new char[strlen(buf)+1];
																									first_temp->value[0]='\0';
																									strcat(first_temp->value,buf);
																									first_temp->el=NULL;
																									cur_func_prov=func_prov_list;
																									func_prov_list=func_prov_list->next;
																									delete cur_func_prov;
																								}
																							}
																							else
																							{
																								flag=false;
																								kod_err=3;
																								err_buffer[0]='\0';
																								if ((func_prov_count1==1) && (func_prov_count2==0))
																									strcat(err_buffer,"� ������� pow ����������� ������ ��������");
																								else
																									if ((func_prov_count1==0) && (func_prov_count2==1))
																										strcat(err_buffer,"� ������� pow ����������� ������ ��������");
																									else
																										if ((func_prov_count1==0) && (func_prov_count2==0))
																											strcat(err_buffer,"� ������� pow ����������� ��� ���������");
																										else
																											strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� pow");	
																								fputs(err_buffer,log_file);
																								fprintf(log_file,"\n");
																								Save_err(err_buffer,name,name,NULL);						
																							}
																						}
																						else
																							if (!strcmp(buf,"exp"))
																							{
																								flag=false;
																								kod_err=3;
																								err_buffer[0]='\0';
																								strcat(err_buffer,"������� exp ������� ������ ������ ���������!!!");
																								fputs(err_buffer,log_file);
																								fprintf(log_file,"\n");
																								Save_err(err_buffer,name,name,NULL);
																							}
																							else
																								if (!strcmp(buf,"fact"))
																								{
																									flag=false;
																									kod_err=3;
																									err_buffer[0]='\0';
																									strcat(err_buffer,"������� fact ������� ������ ������ ���������!!!");
																									fputs(err_buffer,log_file);
																									fprintf(log_file,"\n");
																									Save_err(err_buffer,name,name,NULL);
																								}
																								else
																									if (!strcmp(buf,"abs"))
																									{
																										flag=false;
																										kod_err=3;
																										err_buffer[0]='\0';
																										strcat(err_buffer,"������� abs ������� ������ ������ ���������!!!");
																										fputs(err_buffer,log_file);
																										fprintf(log_file,"\n");
																										Save_err(err_buffer,name,name,NULL);
																									}
							i++;
						}break;
					case '7':
						{/* 3 - ���� ��� ���������� F( - ������� ������� ������ �� ����� � ������ ��������� ������
						    7 - ���� ���� ���������� F(, �� ������������ �������, �� ����� ������ F( � ������ ��������� ������*/
							if ((first_stek->znak=='(') && (first_stek->next->znak!='F'))
							{
								if (first_temp)
								{
									if (bukv(first_temp->value[0]))
										fprintf(log_file,"(P)=(%s)=%s\n",first_temp->value,first_temp->value);
									else
										fprintf(log_file,"(C)=(%s)=%s\n",first_temp->value,first_temp->value);
									cur_stek=first_stek;
									first_stek=cur_stek->next;
									delete cur_stek;
									cur_func_prov=func_prov_list;
									func_prov_list=func_prov_list->next;
									delete cur_func_prov;
								}
								else
								{
									flag=false;
									kod_err=3;
									err_buffer[0]='\0';
									strcat(err_buffer,"��������� � ������� �����������");
									fputs(err_buffer,log_file);
									fprintf(log_file,"\n");
									Save_err(err_buffer,name,name,NULL);
								}
							}
							else
								if ((first_stek->znak=='(') && (first_stek->next->znak=='F'))
								{
									cur_stek=first_stek;
									for (j=0;j<2;j++)
									{
										first_stek=first_stek->next;
										delete cur_stek;
										cur_stek=first_stek;
									}
									cur=list_f;
									for (j=0;j<indf_stek->kol;j++)
										cur=cur->next;
									cur_func_prov=indf_stek;
									indf_stek=indf_stek->next;
									delete cur_func_prov;
									buf[0]='\0';
									strcat(buf,cur->value);
									cur_temp=first_temp;
									func_prov_count=0;
									while (cur_temp)
									{
										func_prov_count++;
										cur_temp=cur_temp->next;
									}
									func_prov_count-=func_prov_list->kol;
									cur_func_prov=func_prov_list;
									func_prov_list=func_prov_list->next;
									delete cur_func_prov;
									if (!strcmp(buf,"sin"))
									{
										if (func_prov_count==1)
										{
											if (first_temp->el)
											{
												//����������
												if (!first_temp->el->adr->getResult())
												{
													flag=false;
													kod_err=3;
													if (!first_temp->el->adr->problem_vyraz)
														Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
													else
														Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
												}
												else
												{
													if (result1)
														delete [] result1;
													result1=new char[strlen(first_temp->el->adr->getResult())+1];
													result1[0]='\0';
													strcat(result1,first_temp->el->adr->getResult());
												}
											}
											else
											{
												if (result1)
													delete [] result1;
												result1=new char[strlen(first_temp->value)+1];
												result1[0]='\0';
												strcat(result1,first_temp->value);
											}
											if (flag)
											{
												buf[0]='\0';
												strcat(buf,myMath::SIN(result1));
												if (bukv(first_temp->value[0]))
													fprintf(log_file,"F(P)=sin(%s)=sin(%s)=%s\n",first_temp->value,result1,buf);
												else
													fprintf(log_file,"F(C)=sin(%s)=%s\n",first_temp->value,buf);
												delete [] first_temp->value;
												first_temp->value=new char[strlen(buf)+1];
												first_temp->value[0]='\0';
												strcat(first_temp->value,buf);
												first_temp->el=NULL;
											}
										}
										else
										{
											flag=false;
											kod_err=3;
											err_buffer[0]='\0';
											if ((func_prov_count>1) || (func_prov_count<0))
												strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� sin");
											else 
												strcat(err_buffer,"� ������� sin ������� �������� ���������� ����������");											
											fputs(err_buffer,log_file);
											fprintf(log_file,"\n");
											Save_err(err_buffer,name,name,NULL);
										}
									}
									else
										if (!strcmp(buf,"cos"))
										{
											if (func_prov_count==1)
											{
												if (first_temp->el)
												{
													//����������
													if (!first_temp->el->adr->getResult())
													{
														flag=false;
														kod_err=3;
														if (!first_temp->el->adr->problem_vyraz)
															Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
														else
															Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
													}
													else
													{
														if (result1)
															delete [] result1;
														result1=new char[strlen(first_temp->el->adr->getResult())+1];
														result1[0]='\0';
														strcat(result1,first_temp->el->adr->getResult());
													}
												}
												else
												{
													if (result1)
														delete [] result1;
													result1=new char[strlen(first_temp->value)+1];
													result1[0]='\0';
													strcat(result1,first_temp->value);
												}
												if (flag)
												{
													buf[0]='\0';
													strcat(buf,myMath::COS(result1));
													if (bukv(first_temp->value[0]))
														fprintf(log_file,"F(P)=cos(%s)=cos(%s)=%s\n",first_temp->value,result1,buf);
													else
														fprintf(log_file,"F(C)=cos(%s)=%s\n",first_temp->value,buf);
													delete [] first_temp->value;
													first_temp->value=new char[strlen(buf)+1];
													first_temp->value[0]='\0';
													strcat(first_temp->value,buf);
													first_temp->el=NULL;
												}
											}
											else
											{
												flag=false;
												kod_err=3;
												err_buffer[0]='\0';
												if ((func_prov_count>1) || (func_prov_count<0))
													strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� cos");
												else 
													strcat(err_buffer,"� ������� cos ������� �������� ���������� ����������");											
												fputs(err_buffer,log_file);
												fprintf(log_file,"\n");
												Save_err(err_buffer,name,name,NULL);
											}	
										}
										else
											if (!strcmp(buf,"tg"))
											{
												if (func_prov_count==1)
												{
													if (first_temp->el)
													{
														//����������
														if (!first_temp->el->adr->getResult())
														{
															flag=false;
															kod_err=3;
															if (!first_temp->el->adr->problem_vyraz)
																Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
															else
																Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
														}
														else
														{
															if (result1)
																delete [] result1;
															result1=new char[strlen(first_temp->el->adr->getResult())+1];
															result1[0]='\0';
															strcat(result1,first_temp->el->adr->getResult());
														}
													}
													else
													{
														if (result1)
															delete [] result1;
														result1=new char[strlen(first_temp->value)+1];
														result1[0]='\0';
														strcat(result1,first_temp->value);
													}
													if (flag)
													{
														buf[0]='\0';
														strcat(buf,myMath::TG(result1));
														if (!strncmp(buf,"-1.#IND",7))
														{
															flag=false;
															kod_err=3;
															err_buffer[0]='\0';
															strcat(err_buffer,"������������ �������� ��������� ������� tg - tg(");
															strcat(err_buffer,result1);
															strcat(err_buffer,")");
															Save_err(err_buffer,name,name,NULL);
														}
														else
															if (!strncmp(buf,"-1.#INF",7))
															{
																flag=false;
																kod_err=3;
																err_buffer[0]='\0';
																strcat(err_buffer,"��������� tg(");
																strcat(err_buffer,result1);
																strcat(err_buffer,") ������ ������������ ����������� ��� ���������� ��������");
																Save_err(err_buffer,name,name,NULL);
															}
															else
																if (!strncmp(buf,"1.#INF",6))
																{
																	flag=false;
																	kod_err=3;
																	err_buffer[0]='\0';
																	strcat(err_buffer,"��������� tg(");
																	strcat(err_buffer,result1);
																	strcat(err_buffer," ������ ������������� ����������� ��� ���������� ��������");
																	Save_err(err_buffer,name,name,NULL);
																}
																else
																	if (bukv(first_temp->value[0]))
																		fprintf(log_file,"F(P)=tg(%s)=tg(%s)=%s\n",first_temp->value,result1,buf);
																	else
																		fprintf(log_file,"F(C)=tg(%s)=%s\n",first_temp->value,buf);
														delete [] first_temp->value;
														first_temp->value=new char[strlen(buf)+1];
														first_temp->value[0]='\0';
														strcat(first_temp->value,buf);
														first_temp->el=NULL;
													}
												}
												else
												{
													flag=false;
													kod_err=3;
													err_buffer[0]='\0';
													if ((func_prov_count>1) || (func_prov_count<0))
														strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� tg");
													else 
														strcat(err_buffer,"� ������� tg ������� �������� ���������� ����������");											
													fputs(err_buffer,log_file);
													fprintf(log_file,"\n");
													Save_err(err_buffer,name,name,NULL);
												}	
											}
											else
												if (!strcmp(buf,"ctg"))
												{
													if (func_prov_count==1)
													{
														if (first_temp->el)
														{
															//����������
															if (!first_temp->el->adr->getResult())
															{
																flag=false;
																kod_err=3;
																if (!first_temp->el->adr->problem_vyraz)
																	Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																else
																	Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
															}
															else
															{
																if (result1)
																	delete [] result1;
																result1=new char[strlen(first_temp->el->adr->getResult())+1];
																result1[0]='\0';
																strcat(result1,first_temp->el->adr->getResult());
															}
														}
														else
														{
															if (result1)
																delete [] result1;
															result1=new char[strlen(first_temp->value)+1];
															result1[0]='\0';
															strcat(result1,first_temp->value);
														}
														if (flag)
														{
															buf[0]='\0';
															strcat(buf,myMath::CTG(result1));
															if (!strncmp(buf,"-1.#IND",7))
															{
																flag=false;
																kod_err=3;
																err_buffer[0]='\0';
																strcat(err_buffer,"������������ �������� ��������� ������� ctg - ctg(");
																strcat(err_buffer,result1);
																strcat(err_buffer,")");
																Save_err(err_buffer,name,name,NULL);
															}
															else
																if (!strncmp(buf,"-1.#INF",7))
																{
																	flag=false;
																	kod_err=3;
																	err_buffer[0]='\0';
																	strcat(err_buffer,"��������� ctg(");
																	strcat(err_buffer,result1);
																	strcat(err_buffer,") ������ ������������ ����������� ��� ���������� ��������");
																	Save_err(err_buffer,name,name,NULL);
																}
																else
																	if (!strncmp(buf,"1.#INF",6))
																	{
																		flag=false;
																		kod_err=3;
																		err_buffer[0]='\0';
																		strcat(err_buffer,"��������� ctg(");
																		strcat(err_buffer,result1);
																		strcat(err_buffer," ������ ������������� ����������� ��� ���������� ��������");
																		Save_err(err_buffer,name,name,NULL);
																	}
																	else
																		if (bukv(first_temp->value[0]))
																			fprintf(log_file,"F(P)=ctg(%s)=ctg(%s)=%s\n",first_temp->value,result1,buf);
																		else
																			fprintf(log_file,"F(C)=ctg(%s)=%s\n",first_temp->value,buf);
															delete [] first_temp->value;
															first_temp->value=new char[strlen(buf)+1];
															first_temp->value[0]='\0';
															strcat(first_temp->value,buf);
															first_temp->el=NULL;
														}
													}
													else
													{
														flag=false;
														kod_err=3;
														err_buffer[0]='\0';
														if ((func_prov_count>1) || (func_prov_count<0))
															strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� ctg");
														else 
															strcat(err_buffer,"� ������� ctg ������� �������� ���������� ����������");											
														fputs(err_buffer,log_file);
														fprintf(log_file,"\n");
														Save_err(err_buffer,name,name,NULL);
													}	
												}
												else
													if (!strcmp(buf,"arcsin"))
													{
														if (func_prov_count==1)
														{
															if (first_temp->el)
															{
																//����������
																if (!first_temp->el->adr->getResult())
																{
																	flag=false;
																	kod_err=3;
																	if (!first_temp->el->adr->problem_vyraz)
																		Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																	else
																		Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																}
																else
																{
																	if (result1)
																		delete [] result1;
																	result1=new char[strlen(first_temp->el->adr->getResult())+1];
																	result1[0]='\0';
																	strcat(result1,first_temp->el->adr->getResult());
																}
															}
															else
															{
																if (result1)
																	delete [] result1;
																result1=new char[strlen(first_temp->value)+1];
																result1[0]='\0';
																strcat(result1,first_temp->value);
															}
															if (flag)
															{
																buf[0]='\0';
																strcat(buf,myMath::ARCSIN(result1));
																if (!strncmp(buf,"-1.#IND",7))
																{
																	flag=false;
																	kod_err=3;
																	err_buffer[0]='\0';
																	strcat(err_buffer,"������������ �������� ��������� ������� arcsin - arcsin(");
																	strcat(err_buffer,result1);
																	strcat(err_buffer,")");
																	Save_err(err_buffer,name,name,NULL);
																}
																else
																	if (bukv(first_temp->value[0]))
																		fprintf(log_file,"F(P)=arcsin(%s)=arcsin(%s)=%s\n",first_temp->value,result1,buf);
																	else
																		fprintf(log_file,"F(C)=arcsin(%s)=%s\n",first_temp->value,buf);
																delete [] first_temp->value;
																first_temp->value=new char[strlen(buf)+1];
																first_temp->value[0]='\0';
																strcat(first_temp->value,buf);
																first_temp->el=NULL;
															}
														}
														else
														{
															flag=false;
															kod_err=3;
															err_buffer[0]='\0';
															if ((func_prov_count>1) || (func_prov_count<0))
																strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� arcsin");
															else 
																strcat(err_buffer,"� ������� arcsin ������� �������� ���������� ����������");											
															fputs(err_buffer,log_file);
															fprintf(log_file,"\n");
															Save_err(err_buffer,name,name,NULL);
														}	
													}
													else
														if (!strcmp(buf,"arccos"))
														{
															if (func_prov_count==1)
															{
																if (first_temp->el)
																{
																	//����������
																	if (!first_temp->el->adr->getResult())
																	{
																		flag=false;
																		kod_err=3;
																		if (!first_temp->el->adr->problem_vyraz)
																			Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																		else
																			Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																	}
																	else
																	{
																		if (result1)
																			delete [] result1;
																		result1=new char[strlen(first_temp->el->adr->getResult())+1];
																		result1[0]='\0';
																		strcat(result1,first_temp->el->adr->getResult());
																	}
																}
																else
																{
																	if (result1)
																		delete [] result1;
																	result1=new char[strlen(first_temp->value)+1];
																	result1[0]='\0';
																	strcat(result1,first_temp->value);
																}
																if (flag)
																{
																	buf[0]='\0';
																	strcat(buf,myMath::ARCCOS(result1));
																	if (!strncmp(buf,"-1.#IND",7))
																	{
																		flag=false;
																		kod_err=3;
																		err_buffer[0]='\0';
																		strcat(err_buffer,"������������ �������� ��������� ������� arccos - arccos(");
																		strcat(err_buffer,result1);
																		strcat(err_buffer,")");
																		Save_err(err_buffer,name,name,NULL);
																	}
																	else
																		if (bukv(first_temp->value[0]))
																			fprintf(log_file,"F(P)=arccos(%s)=arccos(%s)=%s\n",first_temp->value,result1,buf);
																		else
																			fprintf(log_file,"F(C)=arccos(%s)=%s\n",first_temp->value,buf);
																	delete [] first_temp->value;
																	first_temp->value=new char[strlen(buf)+1];
																	first_temp->value[0]='\0';
																	strcat(first_temp->value,buf);
																	first_temp->el=NULL;
																}
															}
															else
															{
																flag=false;
																kod_err=3;
																err_buffer[0]='\0';
																if ((func_prov_count>1) || (func_prov_count<0))
																	strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� arccos");
																else 
																	strcat(err_buffer,"� ������� arccos ������� �������� ���������� ����������");											
																fputs(err_buffer,log_file);
																fprintf(log_file,"\n");
																Save_err(err_buffer,name,name,NULL);
															}	
														}
														else
															if (!strcmp(buf,"arctg"))
															{
																if (func_prov_count==1)
																{
																	if (first_temp->el)
																	{
																		//����������
																		if (!first_temp->el->adr->getResult())
																		{
																			flag=false;
																			kod_err=3;
																			if (!first_temp->el->adr->problem_vyraz)
																				Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																			else
																				Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																		}
																		else
																		{
																			if (result1)
																				delete [] result1;
																			result1=new char[strlen(first_temp->el->adr->getResult())+1];
																			result1[0]='\0';
																			strcat(result1,first_temp->el->adr->getResult());
																		}
																	}
																	else
																	{
																		if (result1)
																			delete [] result1;
																		result1=new char[strlen(first_temp->value)+1];
																		result1[0]='\0';
																		strcat(result1,first_temp->value);
																	}
																	if (flag)
																	{
																		buf[0]='\0';
																		strcat(buf,myMath::ARCTG(result1));
																		if (bukv(first_temp->value[0]))
																			fprintf(log_file,"F(P)=arctg(%s)=arctg(%s)=%s\n",first_temp->value,result1,buf);
																		else
																			fprintf(log_file,"F(C)=arctg(%s)=%s\n",first_temp->value,buf);
																		delete [] first_temp->value;
																		first_temp->value=new char[strlen(buf)+1];
																		first_temp->value[0]='\0';
																		strcat(first_temp->value,buf);
																		first_temp->el=NULL;
																	}
																}
																else
																{
																	flag=false;
																	kod_err=3;
																	err_buffer[0]='\0';
																	if ((func_prov_count>1) || (func_prov_count<0))
																		strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� arctg");
																	else 
																		strcat(err_buffer,"� ������� arctg ������� �������� ���������� ����������");											
																	fputs(err_buffer,log_file);
																	fprintf(log_file,"\n");
																	Save_err(err_buffer,name,name,NULL);
																}	
															}
															else
																if (!strcmp(buf,"arcctg"))
																{
																	if (func_prov_count==1)
																	{
																		if (first_temp->el)
																		{
																			//����������
																			if (!first_temp->el->adr->getResult())
																			{
																				flag=false;
																				kod_err=3;
																				if (!first_temp->el->adr->problem_vyraz)
																					Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																				else
																					Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																			}
																			else
																			{
																				if (result1)
																					delete [] result1;
																				result1=new char[strlen(first_temp->el->adr->getResult())+1];
																				result1[0]='\0';
																				strcat(result1,first_temp->el->adr->getResult());
																			}
																		}
																		else
																		{
																			if (result1)
																				delete [] result1;
																			result1=new char[strlen(first_temp->value)+1];
																			result1[0]='\0';
																			strcat(result1,first_temp->value);
																		}
																		if (flag)
																		{
																			buf[0]='\0';
																			strcat(buf,myMath::ARCCTG(result1));
																			if (bukv(first_temp->value[0]))
																				fprintf(log_file,"F(P)=arcctg(%s)=arcctg(%s)=%s\n",first_temp->value,result1,buf);
																			else
																				fprintf(log_file,"F(C)=arcctg(%s)=%s\n",first_temp->value,buf);
																			delete [] first_temp->value;
																			first_temp->value=new char[strlen(buf)+1];
																			first_temp->value[0]='\0';
																			strcat(first_temp->value,buf);
																			first_temp->el=NULL;
																		}
																	}
																	else
																	{
																		flag=false;
																		kod_err=3;
																		err_buffer[0]='\0';
																		if ((func_prov_count>1) || (func_prov_count<0))
																			strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� arcctg");
																		else 
																			strcat(err_buffer,"� ������� arcctg ������� �������� ���������� ����������");											
																		fputs(err_buffer,log_file);
																		fprintf(log_file,"\n");
																		Save_err(err_buffer,name,name,NULL);
																	}	
																}
																else
																	if (!strcmp(buf,"log"))
																	{
																		flag=false;
																		kod_err=3;
																		err_buffer[0]='\0';
																		strcat(err_buffer,"������� log ������� ���� ����������!!!");
																		fputs(err_buffer,log_file);
																		fprintf(log_file,"\n");
																		Save_err(err_buffer,name,name,NULL);
																	}
																	else
																		if (!strcmp(buf,"lg"))
																		{
																			if (func_prov_count==1)
																			{
																				if (first_temp->el)
																				{
																					//����������
																					if (!first_temp->el->adr->getResult())
																					{
																						flag=false;
																						kod_err=3;
																						if (!first_temp->el->adr->problem_vyraz)
																							Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																						else
																							Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																					}
																					else
																					{
																						if (result1)
																							delete [] result1;
																						result1=new char[strlen(first_temp->el->adr->getResult())+1];
																						result1[0]='\0';
																						strcat(result1,first_temp->el->adr->getResult());
																					}
																				}
																				else
																				{
																					if (result1)
																						delete [] result1;
																					result1=new char[strlen(first_temp->value)+1];
																					result1[0]='\0';
																					strcat(result1,first_temp->value);
																				}
																				if (flag)
																				{
																					buf[0]='\0';
																					strcat(buf,myMath::LG(result1));
																					if (!strncmp(buf,"-1.#IND",7))
																					{
																						flag=false;
																						kod_err=3;
																						err_buffer[0]='\0';
																						strcat(err_buffer,"������������ �������� ��������� ������� lg - lg(");
																						strcat(err_buffer,result1);
																						strcat(err_buffer,")");
																						Save_err(err_buffer,name,name,NULL);
																					}
																					else
																						if (bukv(first_temp->value[0]))
																							fprintf(log_file,"F(P)=lg(%s)=lg(%s)=%s\n",first_temp->value,result1,buf);
																						else
																							fprintf(log_file,"F(C)=lg(%s)=%s\n",first_temp->value,buf);
																					delete [] first_temp->value;
																					first_temp->value=new char[strlen(buf)+1];
																					first_temp->value[0]='\0';
																					strcat(first_temp->value,buf);
																					first_temp->el=NULL;
																				}
																			}
																			else
																			{
																				flag=false;
																				kod_err=3;
																				err_buffer[0]='\0';
																				if ((func_prov_count>1) || (func_prov_count<0))
																					strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� lg");
																				else 
																					strcat(err_buffer,"� ������� lg ������� �������� ���������� ����������");											
																				fputs(err_buffer,log_file);
																				fprintf(log_file,"\n");
																				Save_err(err_buffer,name,name,NULL);
																			}	
																		}
																		else
																			if (!strcmp(buf,"ln"))
																			{
																				if (func_prov_count==1)
																				{
																					if (first_temp->el)
																					{
																						//����������
																						if (!first_temp->el->adr->getResult())
																						{
																							flag=false;
																							kod_err=3;
																							if (!first_temp->el->adr->problem_vyraz)
																								Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																							else
																								Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																						}
																						else
																						{
																							if (result1)
																								delete [] result1;
																							result1=new char[strlen(first_temp->el->adr->getResult())+1];
																							result1[0]='\0';
																							strcat(result1,first_temp->el->adr->getResult());
																						}
																					}
																					else
																					{
																						if (result1)
																							delete [] result1;
																						result1=new char[strlen(first_temp->value)+1];
																						result1[0]='\0';
																						strcat(result1,first_temp->value);
																					}
																					if (flag)
																					{
																						buf[0]='\0';
																						strcat(buf,myMath::LN(result1));
																						if (!strncmp(buf,"-1.#IND",7))
																						{
																							flag=false;
																							kod_err=3;
																							err_buffer[0]='\0';
																							strcat(err_buffer,"������������ �������� ��������� ������� ln - ln(");
																							strcat(err_buffer,result1);
																							strcat(err_buffer,")");
																							Save_err(err_buffer,name,name,NULL);
																						}
																						else
																							if (bukv(first_temp->value[0]))
																								fprintf(log_file,"F(P)=ln(%s)=ln(%s)=%s\n",first_temp->value,result1,buf);
																							else
																								fprintf(log_file,"F(C)=ln(%s)=%s\n",first_temp->value,buf);
																						delete [] first_temp->value;
																						first_temp->value=new char[strlen(buf)+1];
																						first_temp->value[0]='\0';
																						strcat(first_temp->value,buf);
																						first_temp->el=NULL;
																					}
																				}
																				else
																				{
																					flag=false;
																					kod_err=3;
																					err_buffer[0]='\0';
																					if ((func_prov_count>1) || (func_prov_count<0))
																						strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� ln");
																					else 
																						strcat(err_buffer,"� ������� ln ������� �������� ���������� ����������");											
																					fputs(err_buffer,log_file);
																					fprintf(log_file,"\n");
																					Save_err(err_buffer,name,name,NULL);
																				}	
																			}
																			else
																				if (!strcmp(buf,"pi"))
																				{
																					if (func_prov_count==0)
																					{
																						buf[0]='\0';
																						strcat(buf,myMath::PI());
																						fprintf(log_file,"F()=pi()=%s\n",buf);
																						cur_temp=new temp_values;
																						cur_temp->next=first_temp;
																						first_temp=cur_temp;
																						first_temp->value=new char[strlen(buf)+1];
																						first_temp->value[0]='\0';
																						strcat(first_temp->value,buf);
																						first_temp->el=NULL;
																					}
																					else
																					{
																						flag=false;
																						kod_err=3;
																						err_buffer[0]='\0';
																						if ((func_prov_count>1) || (func_prov_count<0))
																							strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� pi");
																						else 
																							strcat(err_buffer,"������� pi �� ������� ����������");											
																						fputs(err_buffer,log_file);
																						fprintf(log_file,"\n");
																						Save_err(err_buffer,name,name,NULL);
																					}
																				}
																				else
																					if (!strcmp(buf,"e"))
																					{
																						if (func_prov_count==0)
																						{
																							buf[0]='\0';
																							strcat(buf,myMath::E());
																							fprintf(log_file,"F()=e()=%s\n",buf);
																							cur_temp=new temp_values;
																							cur_temp->next=first_temp;
																							first_temp=cur_temp;
																							first_temp->value=new char[strlen(buf)+1];
																							first_temp->value[0]='\0';
																							strcat(first_temp->value,buf);
																							first_temp->el=NULL;	
																						}
																						else
																						{
																							flag=false;
																							kod_err=3;
																							err_buffer[0]='\0';
																							if ((func_prov_count>1) || (func_prov_count<0))
																								strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� e");
																							else 
																								strcat(err_buffer,"������� e �� ������� ����������");											
																							fputs(err_buffer,log_file);
																							fprintf(log_file,"\n");
																							Save_err(err_buffer,name,name,NULL);
																						}
																					}
																					else
																						if (!strcmp(buf,"sqrt"))
																						{
																							if (func_prov_count==1)
																							{
																								if (first_temp->el)
																								{
																									//����������
																									if (!first_temp->el->adr->getResult())
																									{
																										flag=false;
																										kod_err=3;
																										if (!first_temp->el->adr->problem_vyraz)
																											Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																										else
																											Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																									}
																									else
																									{
																										if (result1)
																											delete [] result1;
																										result1=new char[strlen(first_temp->el->adr->getResult())+1];
																										result1[0]='\0';
																										strcat(result1,first_temp->el->adr->getResult());
																									}
																								}
																								else
																								{
																									if (result1)
																										delete [] result1;
																									result1=new char[strlen(first_temp->value)+1];
																									result1[0]='\0';
																									strcat(result1,first_temp->value);
																								}
																								if (flag)
																								{
																									buf[0]='\0';
																									strcat(buf,myMath::SQRT(result1));
																									if (!strncmp(buf,"-1.#IND",7))
																									{
																										flag=false;
																										kod_err=3;
																										err_buffer[0]='\0';
																										strcat(err_buffer,"������������ �������� ��������� ������� sqrt - sqrt(");
																										strcat(err_buffer,result1);
																										strcat(err_buffer,")");
																										Save_err(err_buffer,name,name,NULL);
																									}
																									else
																										if (bukv(first_temp->value[0]))
																											fprintf(log_file,"F(P)=sqrt(%s)=sqrt(%s)=%s\n",first_temp->value,result1,buf);
																										else
																											fprintf(log_file,"F(C)=sqrt(%s)=%s\n",first_temp->value,buf);
																									delete [] first_temp->value;
																									first_temp->value=new char[strlen(buf)+1];
																									first_temp->value[0]='\0';
																									strcat(first_temp->value,buf);
																									first_temp->el=NULL;
																								}
																							}
																							else
																							{
																								flag=false;
																								kod_err=3;
																								err_buffer[0]='\0';
																								if ((func_prov_count>1) || (func_prov_count<0))
																									strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� sqrt");
																								else 
																									strcat(err_buffer,"� ������� sqrt ������� �������� ���������� ����������");											
																								fputs(err_buffer,log_file);
																								fprintf(log_file,"\n");
																								Save_err(err_buffer,name,name,NULL);
																							}	
																						}
																						else
																							if (!strcmp(buf,"sqr"))
																							{
																								if (func_prov_count==1)
																								{
																									if (first_temp->el)
																									{
																										//����������
																										if (!first_temp->el->adr->getResult())
																										{
																											flag=false;
																											kod_err=3;
																											if (!first_temp->el->adr->problem_vyraz)
																												Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																											else
																												Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																										}
																										else
																										{
																											if (result1)
																												delete [] result1;
																											result1=new char[strlen(first_temp->el->adr->getResult())+1];
																											result1[0]='\0';
																											strcat(result1,first_temp->el->adr->getResult());
																										}
																									}
																									else
																									{
																										if (result1)
																											delete [] result1;
																										result1=new char[strlen(first_temp->value)+1];
																										result1[0]='\0';
																										strcat(result1,first_temp->value);
																									}
																									if (flag)
																									{
																										buf[0]='\0';
																										strcat(buf,myMath::SQR(result1));
																										if (!strncmp(buf,"1.#INF",6))
																										{
																											flag=false;
																											kod_err=3;
																											err_buffer[0]='\0';
																											strcat(err_buffer,"��������� sqr(");
																											strcat(err_buffer,result1);
																											strcat(err_buffer,") ������ ������������� ����������� ��� ���������� ��������");
																											Save_err(err_buffer,name,name,NULL);
																										}
																										else
																											if (bukv(first_temp->value[0]))
																												fprintf(log_file,"F(P)=sqr(%s)=sqr(%s)=%s\n",first_temp->value,result1,buf);
																											else
																												fprintf(log_file,"F(C)=sqr(%s)=%s\n",first_temp->value,buf);
																										delete [] first_temp->value;
																										first_temp->value=new char[strlen(buf)+1];
																										first_temp->value[0]='\0';
																										strcat(first_temp->value,buf);
																										first_temp->el=NULL;
																									}
																								}
																								else
																								{
																									flag=false;
																									kod_err=3;
																									err_buffer[0]='\0';
																									if ((func_prov_count>1) || (func_prov_count<0))
																										strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� sqr");
																									else 
																										strcat(err_buffer,"� ������� sqr ������� �������� ���������� ����������");											
																									fputs(err_buffer,log_file);
																									fprintf(log_file,"\n");
																									Save_err(err_buffer,name,name,NULL);
																								}	
																							}
																							else
																								if (!strcmp(buf,"pow"))
																								{
																									flag=false;
																									kod_err=3;
																									err_buffer[0]='\0';
																									strcat(err_buffer,"������� pow ������� ���� ����������!!!");											
																									fputs(err_buffer,log_file);
																									fprintf(log_file,"\n");
																									Save_err(err_buffer,name,name,NULL);
																								}
																								else
																									if (!strcmp(buf,"exp"))
																									{
																										if (func_prov_count==1)
																										{
																											if (first_temp->el)
																											{
																												//����������
																												if (!first_temp->el->adr->getResult())
																												{
																													flag=false;
																													kod_err=3;
																													if (!first_temp->el->adr->problem_vyraz)
																														Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																													else
																														Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																												}
																												else
																												{
																													if (result1)
																														delete [] result1;
																													result1=new char[strlen(first_temp->el->adr->getResult())+1];
																													result1[0]='\0';
																													strcat(result1,first_temp->el->adr->getResult());
																												}
																											}
																											else
																											{
																												if (result1)
																													delete [] result1;
																												result1=new char[strlen(first_temp->value)+1];
																												result1[0]='\0';
																												strcat(result1,first_temp->value);
																											}
																											if (flag)
																											{
																												buf[0]='\0';
																												strcat(buf,myMath::EXP(result1));
																												if (!strncmp(buf,"1.#INF",6))
																												{
																													flag=false;
																													kod_err=3;
																													err_buffer[0]='\0';
																													strcat(err_buffer,"��������� exp(");
																													strcat(err_buffer,result1);
																													strcat(err_buffer,") ������ ������������� ����������� ��� ���������� ��������");
																													Save_err(err_buffer,name,name,NULL);
																												}
																												else
																													if (bukv(first_temp->value[0]))
																														fprintf(log_file,"F(P)=exp(%s)=exp(%s)=%s\n",first_temp->value,result1,buf);
																													else
																														fprintf(log_file,"F(C)=exp(%s)=%s\n",first_temp->value,buf);
																												delete [] first_temp->value;
																												first_temp->value=new char[strlen(buf)+1];
																												first_temp->value[0]='\0';
																												strcat(first_temp->value,buf);
																												first_temp->el=NULL;
																											}
																										}
																										else
																										{
																											flag=false;
																											kod_err=3;
																											err_buffer[0]='\0';
																											if ((func_prov_count>1) || (func_prov_count<0))
																												strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� exp");
																											else 
																												strcat(err_buffer,"� ������� exp ������� �������� ���������� ����������");											
																											fputs(err_buffer,log_file);
																											fprintf(log_file,"\n");
																											Save_err(err_buffer,name,name,NULL);
																										}	
																									}
																									else
																										if (!strcmp(buf,"fact"))
																										{
																											if (func_prov_count==1)
																											{
																												if (first_temp->el)
																												{
																													//����������
																													if (!first_temp->el->adr->getResult())
																													{
																														flag=false;
																														kod_err=3;
																														if (!first_temp->el->adr->problem_vyraz)
																															Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																														else
																															Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																													}
																													else
																													{
																														if (result1)
																															delete [] result1;
																														result1=new char[strlen(first_temp->el->adr->getResult())+1];
																														result1[0]='\0';
																														strcat(result1,first_temp->el->adr->getResult());
																													}
																												}
																												else
																												{
																													if (result1)
																														delete [] result1;
																													result1=new char[strlen(first_temp->value)+1];
																													result1[0]='\0';
																													strcat(result1,first_temp->value);
																												}
																												if (flag)
																												{
																													buf[0]='\0';
																													strcat(buf,myMath::FACT(result1));
																													if (!strncmp(buf,"-1.#IND",7))
																													{
																														flag=false;
																														kod_err=3;
																														err_buffer[0]='\0';
																														strcat(err_buffer,"������������ �������� ��������� ������� fact - fact(");
																														strcat(err_buffer,result1);
																														strcat(err_buffer,")");
																														Save_err(err_buffer,name,name,NULL);
																													}
																													else
																														if (!strncmp(buf,"1.#INF",6))
																														{
																															flag=false;
																															kod_err=3;
																															err_buffer[0]='\0';
																															strcat(err_buffer,"��������� fact(");
																															strcat(err_buffer,result1);
																															strcat(err_buffer,") ������ ������������� ����������� ��� ���������� ��������");
																															Save_err(err_buffer,name,name,NULL);
																														}
																														else
																															if (bukv(first_temp->value[0]))
																																fprintf(log_file,"F(P)=fact(%s)=fact(%s)=%s\n",first_temp->value,result1,buf);
																															else
																																fprintf(log_file,"F(C)=fact(%s)=%s\n",first_temp->value,buf);
																													delete [] first_temp->value;
																													first_temp->value=new char[strlen(buf)+1];
																													first_temp->value[0]='\0';
																													strcat(first_temp->value,buf);
																													first_temp->el=NULL;
																												}
																											}
																											else
																											{
																												flag=false;
																												kod_err=3;
																												err_buffer[0]='\0';
																												if ((func_prov_count>1) || (func_prov_count<0))
																													strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� fact");
																												else 
																													strcat(err_buffer,"� ������� fact ������� �������� ���������� ����������");											
																												fputs(err_buffer,log_file);
																												fprintf(log_file,"\n");
																												Save_err(err_buffer,name,name,NULL);
																											}	
																										}
																										else
																											if (!strcmp(buf,"abs"))
																											{
																												if (func_prov_count==1)
																												{
																													if (first_temp->el)
																													{
																														//����������
																														if (!first_temp->el->adr->getResult())
																														{
																															flag=false;
																															kod_err=3;
																															if (!first_temp->el->adr->problem_vyraz)
																																Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->name);
																															else
																																Save_err(first_temp->el->adr->problem_in,first_temp->el->adr->name,first_temp->el->adr->problem_in_perem,first_temp->el->adr->problem_vyraz);
																														}
																														else
																														{
																															if (result1)
																																delete [] result1;
																															result1=new char[strlen(first_temp->el->adr->getResult())+1];
																															result1[0]='\0';
																															strcat(result1,first_temp->el->adr->getResult());
																														}
																													}
																													else
																													{
																														if (result1)
																															delete [] result1;
																														result1=new char[strlen(first_temp->value)+1];
																														result1[0]='\0';
																														strcat(result1,first_temp->value);
																													}
																													if (flag)
																													{
																														buf[0]='\0';
																														strcat(buf,myMath::ABS(result1));
																														if (bukv(first_temp->value[0]))
																															fprintf(log_file,"F(P)=abs(%s)=abs(%s)=%s\n",first_temp->value,result1,buf);
																														else
																															fprintf(log_file,"F(C)=abs(%s)=%s\n",first_temp->value,buf);
																														delete [] first_temp->value;
																														first_temp->value=new char[strlen(buf)+1];
																														first_temp->value[0]='\0';
																														strcat(first_temp->value,buf);
																														first_temp->el=NULL;
																													}
																												}
																												else
																												{
																													flag=false;
																													kod_err=3;
																													err_buffer[0]='\0';
																													if ((func_prov_count>1) || (func_prov_count<0))
																														strcat(err_buffer,"��������� �����������. ������ ��������� �� ��� � ����� ������� abs");
																													else 
																														strcat(err_buffer,"� ������� abs ������� �������� ���������� ����������");											
																													fputs(err_buffer,log_file);
																													fprintf(log_file,"\n");
																													Save_err(err_buffer,name,name,NULL);
																												}	
																											}
								}
								else
								{
									cur=list_f;
									for (j=0;j<indf_stek->kol;j++)
										cur=cur->next;
									cur_func_prov=indf_stek;
									indf_stek=indf_stek->next;
									delete cur_func_prov;
									buf[0]='\0';
									strcat(buf,cur->value);
									flag=false;
									kod_err=3;
									err_buffer[0]='\0';
									strcat(err_buffer,"��������� �����������. ������ �������� ��������� �� ��� � ����� ������� ");
									strcat(err_buffer,buf);
									strcat(err_buffer,"\n");
									fputs(err_buffer,log_file);
									fprintf(log_file,"\n");
									Save_err(err_buffer,name,name,NULL);
								}
							i++;
						}break;
					case '?':
						{
							flag=false;
							kod_err=3;
							err_buffer[0]='\0';
							strcat(err_buffer,"��������� �����������");
							fputs(err_buffer,log_file);
							fprintf(log_file,"\n");
							Save_err(err_buffer,name,name,NULL);
						}break;
				}
			}
		}
	}
	if (flag)
	{
		fputs("������� ��������",log_file);
		fprintf(log_file,"\n");
	}
	else
	{
		fputs("������ � ���������",log_file);
		fprintf(log_file,"\n");
		cur_stek=first_stek;
		while (cur_stek)
		{
			first_stek=cur_stek->next;
			delete cur_stek;
			cur_stek=first_stek;
		}
		cur_func_prov=indf_stek;
		while (cur_func_prov)
		{
			indf_stek=indf_stek->next;
			delete cur_func_prov;
			cur_func_prov=indf_stek;
		}
		cur_temp=first_temp;
		while (cur_temp)
		{
			first_temp=cur_temp->next;
			delete [] cur_temp->value;
			delete cur_temp;
			cur_temp=first_temp;
		}
		cur_func_prov=func_prov_list;
		while (cur_func_prov)
		{
			func_prov_list=cur_func_prov->next;
			delete cur_func_prov;
			cur_func_prov=func_prov_list;
		}
	}
}

void VYRAZ::PeremProv(perem_list *list_perem)
{
	perem_list *stek=NULL,*cur_stek,*cur_perem;
	elem *el_p;
	char buf2[150],buf3[150];
	bool flag1,flag_leks=false;
	buf2[0]='\0';
	buf3[0]='\0';
	prov_func *first,*cur;//���������� ��� ����� ������ ������������� � list_p
	cur_perem=list_perem;
	el_p=list_p_simple;
	if (el_p) //���� � ��������� ���� ����������
	{
		//���������� ������������� ����������-���������
		stek=new perem_list;
		stek->next=NULL;
		stek->perem=this;
		first=new prov_func;
		first->next=NULL;
		first->kol=1; // ����� ������� ������������� � list_p_simple
		el_p=stek->perem->list_p_simple;
		//�������� ������
		while (stek && flag)
		{
			flag1=false;
			cur_perem=list_perem;
			//�������� �������������
			while (!flag1 && cur_perem)
			{
				if (!strcmp(el_p->value,cur_perem->perem->getName()))
					flag1=true;
				else
					cur_perem=cur_perem->next;
			}
			flag=flag1;		
			if (flag)
			{
				fprintf(log_file,"���������� %s ����������\n",el_p->value);
				cur_perem->perem->Print();
				el_p->adr=cur_perem->perem;//���������� � list_p ������ ����������
				if ((!cur_perem->perem->flag) && (cur_perem->perem->kod_err!=3))
				{
					//� ������������ ���� ������
					flag=false; 
					flag_leks=true;
					buf[0]='\0';
					strcat(buf,cur_perem->perem->problem_perem);
					buf3[0]='\0';
					strcat(buf3,cur_perem->perem->problem_in_perem);
					buf2[0]='\0';
					strcat(buf2,cur_perem->perem->problem_in);
				}
				else
				{
					if (!strlen(cur_perem->perem->tokens) && (cur_perem->perem->flag))
						cur_perem->perem->Leks_anal();//���� ����������� ������ ��� �� ����������, �� �������� ��� ����������� ������ ����������
					if ((!cur_perem->perem->flag) && (cur_perem->perem->kod_err==1))
					{
						flag=false; //������� ����������� ������ 
						flag_leks=true;
						buf[0]='\0';
						strcat(buf,cur_perem->perem->getName());
						buf3[0]='\0';
						strcat(buf3,cur_perem->perem->getName());
						buf2[0]='\0';
						strcat(buf2,cur_perem->perem->getProblemIn());
					}
					else
					{
						//� ��������� ���������� ���� ������ ����������
						//�������� �� ����������� �����������
						cur_stek=stek;
						while (flag && cur_stek)
						{
							if (!strcmp(el_p->value,cur_stek->perem->getName()))
								flag=false;							
							cur_stek=cur_stek->next;
						}
						//���������� ��������� ���������� � ���� ��� ����������� �������
						if (flag)
						{
							cur_stek=new perem_list;
							cur_stek->next=stek;
							cur_stek->perem=cur_perem->perem;
							stek=cur_stek;
							cur=new prov_func;
							cur->next=first;
							cur->kol=1;
							first=cur;
							el_p=stek->perem->list_p_simple;
							for (int j=1;j<first->kol;j++)
								el_p=el_p->next;
							while (stek && !el_p)
							{
								//����� �� ����� ����� �� �������
								cur_stek=stek;
								stek=stek->next;
								delete cur_stek;
								cur_stek=stek;
								cur=first;
								first=first->next;
								delete cur;
								cur=first;
								//����������� ����� �������������
								if (first)
								{
									first->kol++;
									el_p=stek->perem->list_p_simple;
									for (int j=1;j<first->kol;j++)
										el_p=el_p->next;
								}
							}
						}
						else
						{
							buf2[0]='\0';
							strcat(buf2,"���������� ����������� ����������� ���������� ");
							strcat(buf2,el_p->value);
							strcat(buf2,"\n");
							fputs(buf2,log_file);
							fprintf(log_file,"\n");
							buf3[0]='\0';
							strcat(buf3,stek->perem->getName());
						}
					}
				}
			}
			else
			{
				buf2[0]='\0';
				strcat(buf2,"���������� ");
				strcat(buf2,el_p->value);
				strcat(buf2," �� ����������\n");
				fputs(buf2,log_file);
				fprintf(log_file,"\n");
				buf3[0]='\0';
				strcat(buf3,el_p->value);
			}		
		}
	}
	if (flag)
	{
		fputs("������ ���������� ����������. ����������� ����������� �� ����������",log_file);
		fprintf(log_file,"\n");
		//����������� ������� �� ������� list_p_simple � list_p
		elem *curel;
		el_p=list_p_simple;
		while (el_p)
		{
			curel=list_p;
			while (curel)
			{
				if (!strcmp(curel->value,el_p->value))
					curel->adr=el_p->adr;
				curel=curel->next;
			}
			el_p=el_p->next;
		}
	}
	else
	{
		fputs("���������� ������ �� ����� ������� ����������",log_file);
		fprintf(log_file,"\n");
		cur_stek=stek;
		if (!flag_leks)
		{
			buf[0]='\0';
			strcat(buf,stek->perem->getName());
		}
		while (cur_stek)
		{
			stek=stek->next;
			cur_stek->perem->flag=false;
			if (cur_stek->perem->kod_err==0)
			{
				cur_stek->perem->kod_err=2;
				if (cur_stek->perem->problem_perem)
					delete [] cur_stek->perem->problem_perem;
				cur_stek->perem->problem_perem=new char[strlen(buf)+1];
				cur_stek->perem->problem_perem[0]='\0';
				strcat(cur_stek->perem->problem_perem,buf);
				if (cur_stek->perem->problem_in)
					delete [] cur_stek->perem->problem_in;
				cur_stek->perem->problem_in=new char[strlen(buf2)+1];
				cur_stek->perem->problem_in[0]='\0';
				strcat(cur_stek->perem->problem_in,buf2);
				if (cur_stek->perem->problem_in_perem)
					delete [] cur_stek->perem->problem_in_perem;
				cur_stek->perem->problem_in_perem=new char[strlen(buf3)+1];
				cur_stek->perem->problem_in_perem[0]='\0';
				strcat(cur_stek->perem->problem_in_perem,buf3);
			}
			delete cur_stek;
			cur_stek=stek;
		}
	}
}