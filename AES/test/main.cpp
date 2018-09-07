
#include "RijndaelAlgFst.h"
#include<time.h>
#include<fstream>
#include<iostream>

using namespace std;

void Enc()
{
	u8 cipherKey[16]={0x3b ,0x8e, 0x45, 0x76, 0x18, 0xa7, 0x22, 0x11, 0x45, 0xf7, 0xe5, 0xd8, 0xc9, 0xcf, 0x4f, 0x3f};//��Կ
	u32 rk[44];
	int Nr,i=0,j=0,k=0,p=0;
	u8 pt[16],ct[16];
	char ch;
	int fin1=0,fin2,time=0,max;
	bool flag = true;
	//����buffer���������д��
	char buffer1[1024],buffer2[1024];
	//////////////////////////////////////////////////////////////////////
	//���ܹ���
	Nr=rijndaelKeySetupEnc(rk,cipherKey,128);//���ɵģ�Nr+1�����ּ�����Կ
	//��mingwen.txt�ж�ȡ���Ĵ�ŵ�pt��
	ifstream in("mingwen.txt",ios::in||ios::binary);
	if(!in)
	{
		cerr<<"file mingwen cannot be opened/n";
		exit(1); 
	}

	//�����ɵ����Ĵ�ŵ�miwen.txt��
	ofstream out("miwen.txt",ios::out);
	if(!out)
	{
		cerr<<"file miwen cannot be opened/n";
		exit(1);
	}
	//���ļ����ȱ�����length.txt��
	ofstream out2("length.txt",ios::out);
	if(!out2)
	{
		cerr<<"file length cannot be opened/n";
		exit(1);
	}
	//��ȡ�����ļ����ȣ�������д������
	in.seekg(0,ios::end);
	fin1=in.tellg();
	fin2=fin1;
	in.seekg(0,ios::beg);
	out2<<fin1<<" ";
	
	while(flag)
	{
		//�����������
		in.seekg(1024*time,ios::beg);
		time++;
		if(fin1>1024)
		{
			in.read(buffer1,1024);
			fin1-=1024;
			max=1024;
		}
		else
		{
			in.read(buffer1,fin1);
			max=fin1;
			flag=false;
		}
		//���ڴ���ѭ������
		for(k=0;k<max;k++)
		{
			pt[i]=buffer1[k];
			i++;
			if(i==16)
			{
				i=0;
				rijndaelEncrypt(rk,Nr,pt,ct);//��������
				//�����ɵ����Ĵ�ŵ�miwen.txt��
				for(j=0;j<16;j++)
				{
					buffer2[p]=ct[j];
					p++;
					if(p==1024)
					{
						p=0;
						//��д��buffer��ʱ������д��
						out.write(buffer2,1024);
					}
				}
			}
		}
	}
	if(i>0&&i<16)
	{
		rijndaelEncrypt(rk,Nr,pt,ct);//��������
		fin2+=(16-i);
		//�����ɵ����Ĵ�ŵ�miwen.txt��
		for(j=0;j<16;j++)
		{
			buffer2[p]=ct[j];
			p++;
			if(p==1024)
			{
				p=0;
				//��д��buffer��ʱ������д��
				out.write(buffer2,1024);
			}
		}
	}
	//�������ʣ������
	if(p>0)
	{
		out.write(buffer2,p);
	}
	//����������ĳ���
	out2<<fin2;
	in.close();
	out.close();
	out2.close();
}

void Dec()
{
	u8 cipherKey[16]={0x3b ,0x8e, 0x45, 0x76, 0x18, 0xa7, 0x22, 0x11, 0x45, 0xf7, 0xe5, 0xd8, 0xc9, 0xcf, 0x4f, 0x3f};//��Կ
	u32 rk[44];
	int Nr,i=0,j=0,k=0,p=0;
	u8 pt[16],ct[16];
	char ch;
	int fin1,fin2,time=0,max;
	bool flag = true;
	//����buffer���������д��
	char buffer1[1024],buffer2[1024];

	//////////////////////////////////////////////////////////////////////
	//���ܹ���
	Nr=rijndaelKeySetupDec(rk,cipherKey,128);//���ɵģ�Nr+1��������Կ
	//��mingwen.txt�ж�ȡ���Ĵ�ŵ�pt��
	ifstream in("miwen.txt",ios::in||ios::binary);
	if(!in)
	{
		cerr<<"file miwen cannot be opened/n";
		exit(1); 
	}
	//��length.txt�ж�ȡ�ļ������Լ����泤��
	ifstream in2("length.txt",ios::in);
	if(!in2)
	{
		cerr<<"file length cannot be opened/n";
		exit(1); 
	}
	
	//�����ɵ����Ĵ�ŵ�jiemi.txt��
	ofstream out("jiemi.txt",ios::out);
	if(!out)
	{
		cerr<<"file jiemi cannot be opened/n";
		exit(1);
	}
	
	in2>>fin2>>fin1;
	fin2=fin1-fin2; 
	in2.close();
	while(flag)
	{
		//�����������
		in.seekg(1024*time,ios::beg);
		time++;
		if(fin1>1024)
		{
			in.read(buffer1,1024);
			fin1-=1024;
			max=1024;
		}
		else
		{
			in.read(buffer1,fin1);
			max=fin1;
			flag=false;
		}
		//���ڴ���ѭ������
		for(k=0;k<max;k++)
		{
			pt[i]=buffer1[k];
			i++;
			if(i==16)
			{
				i=0;
				rijndaelDecrypt(rk,Nr,pt,ct);//��������
				//�����ɵ����Ĵ�ŵ�jiemi.txt��
				for(j=0;j<16;j++)
				{
					buffer2[p]=ct[j];
					p++;
					if(p==1024)
					{
						p=0;
						//��д��buffer��ʱ������д��
						out.write(buffer2,1024);
					}
				}
			}
		}
	}
	//�������ʣ������
	if(p>0)
	{
		out.write(buffer2,p-fin2);
	}
	in.close();
	out.close();
}

void main()
{
	clock_t start,finish;
	double duration1,duration2;
	int length;
	//�������ʱ��
	start=clock();
	Enc();
	finish=clock();
	duration1=(double)(finish-start);
	//��ü��ܳ���
	ifstream in("length.txt",ios::in);
	if(!in)
	{
		cerr<<"file length cannot be opened/n";
		exit(1); 
	}

	in>>length;
	in.close();
	//���������ʱ��
	start=clock();
	Dec();
	finish=clock();
	duration2=(double)(finish-start);
	//������
	cout<<"���ܳ���Ϊ��"<<length<<"���ֽڡ�\n";
	cout<<"����ʱ��Ϊ��"<<duration1<<" ms!"<<endl;
	cout<<"����ʱ��Ϊ��"<<duration2<<" ms!"<<endl;
	cout<<"�ļ�mingwen.txtΪ���������ļ����ļ�miwen.txtΪ��������ļ����ļ�jiemi.txtΪ�����ļ���\n";
	cout<<"���������ַ��Խ���";
	char c;
	cin.get(c);
}