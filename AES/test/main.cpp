
#include "RijndaelAlgFst.h"
#include<time.h>
#include<fstream>
#include<iostream>

using namespace std;

void Enc()
{
	u8 cipherKey[16]={0x3b ,0x8e, 0x45, 0x76, 0x18, 0xa7, 0x22, 0x11, 0x45, 0xf7, 0xe5, 0xd8, 0xc9, 0xcf, 0x4f, 0x3f};//密钥
	u32 rk[44];
	int Nr,i=0,j=0,k=0,p=0;
	u8 pt[16],ct[16];
	char ch;
	int fin1=0,fin2,time=0,max;
	bool flag = true;
	//利用buffer整块读入与写出
	char buffer1[1024],buffer2[1024];
	//////////////////////////////////////////////////////////////////////
	//加密过程
	Nr=rijndaelKeySetupEnc(rk,cipherKey,128);//生成的（Nr+1）个轮加密密钥
	//从mingwen.txt中读取明文存放到pt中
	ifstream in("mingwen.txt",ios::in||ios::binary);
	if(!in)
	{
		cerr<<"file mingwen cannot be opened/n";
		exit(1); 
	}

	//将生成的密文存放到miwen.txt中
	ofstream out("miwen.txt",ios::out);
	if(!out)
	{
		cerr<<"file miwen cannot be opened/n";
		exit(1);
	}
	//将文件长度保存于length.txt中
	ofstream out2("length.txt",ios::out);
	if(!out2)
	{
		cerr<<"file length cannot be opened/n";
		exit(1);
	}
	//获取明文文件长度，并将其写出保存
	in.seekg(0,ios::end);
	fin1=in.tellg();
	fin2=fin1;
	in.seekg(0,ios::beg);
	out2<<fin1<<" ";
	
	while(flag)
	{
		//获得所需数据
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
		//在内存中循环操作
		for(k=0;k<max;k++)
		{
			pt[i]=buffer1[k];
			i++;
			if(i==16)
			{
				i=0;
				rijndaelEncrypt(rk,Nr,pt,ct);//生成密文
				//将生成的密文存放到miwen.txt中
				for(j=0;j<16;j++)
				{
					buffer2[p]=ct[j];
					p++;
					if(p==1024)
					{
						p=0;
						//当写出buffer满时，将其写出
						out.write(buffer2,1024);
					}
				}
			}
		}
	}
	if(i>0&&i<16)
	{
		rijndaelEncrypt(rk,Nr,pt,ct);//生成密文
		fin2+=(16-i);
		//将生成的密文存放到miwen.txt中
		for(j=0;j<16;j++)
		{
			buffer2[p]=ct[j];
			p++;
			if(p==1024)
			{
				p=0;
				//当写出buffer满时，将其写出
				out.write(buffer2,1024);
			}
		}
	}
	//处理最后剩余数据
	if(p>0)
	{
		out.write(buffer2,p);
	}
	//保存最后密文长度
	out2<<fin2;
	in.close();
	out.close();
	out2.close();
}

void Dec()
{
	u8 cipherKey[16]={0x3b ,0x8e, 0x45, 0x76, 0x18, 0xa7, 0x22, 0x11, 0x45, 0xf7, 0xe5, 0xd8, 0xc9, 0xcf, 0x4f, 0x3f};//密钥
	u32 rk[44];
	int Nr,i=0,j=0,k=0,p=0;
	u8 pt[16],ct[16];
	char ch;
	int fin1,fin2,time=0,max;
	bool flag = true;
	//利用buffer整块读入与写出
	char buffer1[1024],buffer2[1024];

	//////////////////////////////////////////////////////////////////////
	//解密过程
	Nr=rijndaelKeySetupDec(rk,cipherKey,128);//生成的（Nr+1）个轮密钥
	//从mingwen.txt中读取明文存放到pt中
	ifstream in("miwen.txt",ios::in||ios::binary);
	if(!in)
	{
		cerr<<"file miwen cannot be opened/n";
		exit(1); 
	}
	//从length.txt中读取文件长度以及保存长度
	ifstream in2("length.txt",ios::in);
	if(!in2)
	{
		cerr<<"file length cannot be opened/n";
		exit(1); 
	}
	
	//将生成的密文存放到jiemi.txt中
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
		//获得所需数据
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
		//在内存中循环操作
		for(k=0;k<max;k++)
		{
			pt[i]=buffer1[k];
			i++;
			if(i==16)
			{
				i=0;
				rijndaelDecrypt(rk,Nr,pt,ct);//生成明文
				//将生成的明文存放到jiemi.txt中
				for(j=0;j<16;j++)
				{
					buffer2[p]=ct[j];
					p++;
					if(p==1024)
					{
						p=0;
						//当写出buffer满时，将其写出
						out.write(buffer2,1024);
					}
				}
			}
		}
	}
	//处理最后剩余数据
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
	//计算加密时间
	start=clock();
	Enc();
	finish=clock();
	duration1=(double)(finish-start);
	//获得加密长度
	ifstream in("length.txt",ios::in);
	if(!in)
	{
		cerr<<"file length cannot be opened/n";
		exit(1); 
	}

	in>>length;
	in.close();
	//计算机解密时间
	start=clock();
	Dec();
	finish=clock();
	duration2=(double)(finish-start);
	//输出结果
	cout<<"加密长度为："<<length<<"个字节。\n";
	cout<<"加密时间为："<<duration1<<" ms!"<<endl;
	cout<<"解密时间为："<<duration2<<" ms!"<<endl;
	cout<<"文件mingwen.txt为明文输入文件，文件miwen.txt为密文输出文件，文件jiemi.txt为解密文件！\n";
	cout<<"输入任意字符以结束";
	char c;
	cin.get(c);
}