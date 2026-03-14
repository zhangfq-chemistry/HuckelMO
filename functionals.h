#ifndef DATA_H
#define DATA_H


#include <string>
#include <QString>
#include "math/vector3.h"
#include <string>
#include <QString>
#include <QColor>
#include <string>
#include <sstream>
using namespace  std;



extern uint32_t ElementColors[110];
extern double elementMasses[119] ;
extern string ElementNames[119];
extern double  CovalentRadii[100];

uint Symbol2Z(QString symbol);
bool isNumber(const QString& );

bool isEnglish(QString &);
bool isEnglishOrNumber(QString &);
bool isFloat(QString & );
bool isInt(QString &);
bool isFloat(QString & );
bool isInt(QString &);

bool isInteger(double );

bool cmpPair(pair<int, double>, pair<int, double>);


vector3 buildColor(const QColor& );
vector3 MakeGlColor(uint32_t );
uint32_t buildColor1(vector3 );



int sign0(int x);

double sgnd(double x);
double powl(double val, int p);
double Factorial(int n);
double bang(int n);
double AssociatedLaguerre (int n, int lambda, double x) ;
double AssociatedLegendre (int l, int m, float x);

double binomial(int n, int k) ;
double binom(int n, int k) ;
int KroneckerDelta(int i,int j);


/*
sin2ϕ=2sinϕcosϕ
cos2ϕ=cos2ϕ−sin2ϕ
sin3ϕ=sinϕ(−sin2ϕ+3cos2ϕ)
cos3ϕ=cosϕ(cos2ϕ−3sin2ϕ)
sin4ϕ=4sinϕcosϕ(cos2ϕ−sin2ϕ)
cos4ϕ=1−8sin2ϕcos2ϕ
sin5ϕ=sinϕ[1+4cos2ϕ(cos2ϕ−3sin2ϕ)]
cos5ϕ=cosϕ[1+4sin2ϕ(sin2ϕ−3cos2ϕ)]
sin6ϕ=sinϕcosϕ[6−32sin2ϕcos2ϕ]
cos6ϕ=(cos2ϕ−sin2ϕ)(1−16sin2ϕcos2ϕ)

"^\d+$"　　//非负整数（正整数 + 0）
"^[0-9]*[1-9][0-9]*$"　　//正整数
"^((-\d+)|(0+))$"　　//非正整数（负整数 + 0）
"^-[0-9]*[1-9][0-9]*$"　　//负整数
"^-?\d+$"　　　　//整数
"^\d+(\.\d+)?$"　　//非负浮点数（正浮点数 + 0）
"^(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$"　　//正浮点数
"^((-\d+(\.\d+)?)|(0+(\.0+)?))$"　　//非正浮点数（负浮点数 + 0）
"^(-(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*)))$"　　//负浮点数
"^(-?\d+)(\.\d+)?$"　　//浮点数
"^[A-Za-z]+$"　　//由26个英文字母组成的字符串
"^[A-Z]+$"　　//由26个英文字母的大写组成的字符串
"^[a-z]+$"　　//由26个英文字母的小写组成的字符串
"^[A-Za-z0-9]+$"　　//由数字和26个英文字母组成的字符串
"^\w+$"　　//由数字、26个英文字母或者下划线组成的字符串
"^[\w-]+(\.[\w-]+)*@[\w-]+(\.[\w-]+)+$"　　　　//email地址
"^[a-zA-z]+://(\w+(-\w+)*)(\.(\w+(-\w+)*))*(\?\S*)?$"　　//url
"^(d{2}|d{4})-((0([1-9]{1}))|(1[1|2]))-(([0-2]([1-9]{1}))|(3[0|1]))$" // 年-月-日
"^((0([1-9]{1}))|(1[1|2]))/(([0-2]([1-9]{1}))|(3[0|1]))/(d{2}|d{4})$" // 月/日/年
"^([w-.]+)@(([[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.)|(([w-]+.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(]?)$"   //Email
"(d+-)?(d{4}-?d{7}|d{3}-?d{8}|^d{7,8})(-d+)?"     //电话号码
"^(d{1,2}|1dd|2[0-4]d|25[0-5]).(d{1,2}|1dd|2[0-4]d|25[0-5]).(d{1,2}|1dd|2[0-4]d|25[0-5]).(d{1,2}|1dd|2[0-4]d|25[0-5])$"   //IP地址


^([0-9A-F]{2})(-[0-9A-F]{2}){5}$   //MAC地址的正则表达式
^[-+]?\d+(\.\d+)?$ //值类型正则表达式
*/


#endif // DATA_H
