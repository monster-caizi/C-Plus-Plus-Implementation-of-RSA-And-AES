#include <iostream>
#include <sstream>
#include "BigInt.h"
#include <stdlib.h> 
#include <time.h> 
using namespace std;

#define size 1 //若n为1024位，则size=16；若n为2048位，则size=32

void int2str(const int &int_temp,string &string_temp)  
{  
        stringstream stream;  
        stream<<int_temp;  
        string_temp=stream.str();   //此处也可以用 stream>>string_temp  
}

CBigInt newPrime()
{
	int aa;string myPrimestr="";
	 srand((unsigned)time(NULL));
	 bool flag=true,flag2 = true;
	 while(flag){

		 for(int i = 0; i < size;i++ ){ 
			aa=rand();
			int2str(aa,myPrimestr);
		 }
		 CBigInt myPrime(myPrimestr);
		 flag2 = true;
		 CBigInt ii("2");
		  if(myPrime%ii=="0"){
			  continue ;
		  }
		  ii=ii+1;
		 for(;ii<myPrime;ii=ii+2)
		 {
			 if(myPrime%ii=="0"){
				 flag2 = false;
				 break ;
			 }
		 }
		 if(flag2)
		 return myPrime;
	 }
   //  cout << endl; 
     return 0; 
}

CBigInt newPrime_e(CBigInt fyn)
{

	int aa;string myPrimestr="";
	 srand((unsigned)time(NULL));
	 bool flag=true,flag2 = true;
	 while(flag){
		 for(int i = 0; i < size;i++ ){ 
			aa=rand();
			int2str(aa,myPrimestr);
		 }
		 CBigInt myPrime(myPrimestr);
		 if(fyn<myPrime)
			 continue ;
		 flag2 = true;
		  CBigInt ii("2");
		  if(myPrime%ii=="0"){
			  continue ;
		  }
		  ii=ii+1;
		 for(;ii<myPrime;ii=ii+2)
		 {
			 if(myPrime%ii=="0"){
				 flag2 = false;
				 break ;
			 }
		 }
		 if(flag2)
		 return myPrime;
	 }
   //  cout << endl; 
     return 0; 
}


CBigInt Power( const CBigInt & x, const CBigInt & n,	// 求x^n mod p  利用整数幂计算方法
const CBigInt & p )
{
	if( n == "0" )
		return 1;

	CBigInt tmp = Power( ( x * x ) % p, n / CBigInt(2), p );

	if( n %CBigInt(2) == "0" )
		;
	else
	tmp = ( tmp * x ) % p;

	return tmp;
}


CBigInt ExGcd(CBigInt a, CBigInt b, CBigInt &x, CBigInt &y)
{
	if(b =="0")
	{
		x = 1;
		y = 0;
		return a;
	}
	CBigInt r = ExGcd(b, a % b, x, y);
	CBigInt t = x;
	x = y;
	y = t - a / b * y;
	return r;
}


CBigInt inverse( const CBigInt & fyn,	// 求d
const CBigInt & e )
{
	//int fyn = ( 1 - p ) * ( 1 - q );
	CBigInt x, y;

	ExGcd( fyn, e, x , y );
	if(y<CBigInt("0"))
		y = y+fyn;
	return y ;
}

int main( )
{
	cout<<"说明：本程序测试阶段只在小数据量中测试且随机产生时间较长，因未找到快速生成随机大素数的方法，无法进行大素数测试，但小规模素数测试正确。本程序只支持整数输入，该测试为1个int类型的数据，如需大数据，请修改宏定义size的值即可。";
	cout<<"请等待提示后输入！\n";
	CBigInt p = newPrime();
	CBigInt q = newPrime();
	CBigInt p1 = p-CBigInt(1);
	CBigInt q1 = q-CBigInt(1);
	CBigInt fyn =p1*q1;
	CBigInt n =p*q;
	CBigInt e =newPrime_e(fyn);
	CBigInt d = inverse( fyn, e);
	cout<<"The p is "<<p<<".The q is "<<q<<".The e is "<<e<<".The n is "<<n<<".The d is "<<d<<".(E is public key.D is primary key.)\n";
	string m;
	cout << "请输入需要加密的数据: " << endl;
	cin >> m;
	CBigInt m_big1(m);
	CBigInt C = Power( m_big1, e, n );
	cout << "加密后的数据为: " << C << endl;
	m_big1 = Power( C, d, n );
	cout <<"解密后的数据为: " << m_big1 << endl <<endl<<endl;
	cout << "请输入需要签名的数据: " << endl;
	cin >> m;
	CBigInt m_big2(m);
	C = Power( m_big2, d, n );
	cout << "签名后数据为: " << C << endl;
	m_big2 = Power( C, e, n );
	cout <<"签名验证后的数据为: " << m_big2 << endl <<endl<<endl;

	return 0;
}