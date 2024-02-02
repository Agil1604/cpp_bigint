#include <iostream>
#include "bigfloat.h"


// литерал
bigfloat operator"" _mbf(const char* str)
{
	return bigfloat(std::string(str));
}

// конструкторы

bigfloat::bigfloat(int tolerance){
	num = "0";
	power = 0;
	tol = tolerance;
	sign = false;
}

bigfloat::bigfloat(std::string number, int tolerance){
	tol = tolerance;
	if (number[0] == '-') {
		sign = true;
		number.erase(0, 1); // удаление знака
	}
	else {sign = false;}
	while (number[0] == '0' &&  number[1] != '.' && number.length() > 1){number.erase(0, 1);}
	if (std::string::npos == number.find("."))
	{
		num = number;
		power = 0;
	}
	else
	{
		int point_pos = number.find(".");
		number = number.substr(0, point_pos + tolerance + 1);
		power = number.length() - point_pos - 1;
		num = number.erase(point_pos, 1);
	}	
}

//конструктор копирования
bigfloat::bigfloat(const bigfloat& bf){
	num = bf.num;
	power = bf.power;
	tol = bf.tol;
	sign = bf.sign;
}

//оператор присваивания
const bigfloat& bigfloat::operator=(const bigfloat& bf){
	if( &bf == this ) return *this;
	num = bf.num;
	power = bf.power;
	tol = bf.tol;
	sign = bf.sign;
	return *this;
}

//оператор ввода
std::istream& operator>>(std::istream& in, bigfloat& bf){
	std::string tmp;
	in >> tmp;
	bf = bigfloat(tmp, bf.tol);
	return in;
}

//оператор вывода
std::ostream& operator<<(std::ostream& out, const bigfloat& bf){
	if (bf.sign)
	{
		out<<'-';
	}
	if (bf.power == 0)
	{
		out<<bf.num;
	}
	else
	{
		int i, len = bf.num.length() - bf.power;
		for (i = 0; i < len; ++i)
		{
			out<<bf.num[i];
		}
		out<<'.';
		len += bf.power;
		for (; i < len; ++i)
		{
			out<<bf.num[i];
		}
	}
	return out;
}

void add_zeros(bigfloat& bf, int n)
{
	for (int i = 0; i < n; i++)
	{
		bf.num.append("0");
		bf.power++;
	}
	if (bf.power>bf.tol)
	{
		bf.tol = bf.power;
	}
}

bool is_null(bigfloat bf)
{
	int len = bf.num.length();
	for (int i = 0; i < len; ++i)
	{
		if (bf.num[i] != '0')
		{
			return false;
		}
	}
	return true;
}


bigfloat operator +(const bigfloat& bf1, const bigfloat& bf2)
{
	std::string c = {};
	bigfloat a = bf1;
	bigfloat b = bf2;
	bigfloat ans;
	if (a.sign == b.sign)
	{
		if (a.power > b.power)
		{
			add_zeros(b, a.power - b.power);
		}
		else if (a.power < b.power)
		{
			add_zeros(a, b.power - a.power);
		}
		
		int len1 = a.num.length(), len2 = b.num.length();
		int k = 0;
		int lendif;
		if (len1 < len2)
		{
			ans = a;
			a = b;
			b = ans;
		}
		lendif = len1 - len2;
		for (int i = len2 - 1; i>=0; --i)
		{
			k += a.num[i + lendif] + b.num[i] - '0';
			if (k>'9')
			{
				c.insert(0, 1, k - 10);
				k = 1;
			}
			else
			{
				c.insert(0, 1, k);
				k = 0;
			}
		}
		for (int i = lendif - 1; i>=0; --i)
		{
			k += a.num[i];
			if (k>'9')
			{
				c.insert(0, 1, k - 10);
				k = 1;
			}
			else
			{
				c.insert(0, 1, k);
				k = 0;
			}
		}
		if (k == 1)
		{
			c.insert(0, 1, '1');
		}
		ans.num = c;
		ans.power = a.power;
		ans.tol = a.tol;
		ans.sign = a.sign;
	}
	else
	{
		if (a.sign)
		{
			a.sign = false;
			ans = b - a;
		}
		else
		{
			b.sign = false;
			ans = a - b;
		}
	}
	return ans;
}

bigfloat operator -(const bigfloat& bf1, const bigfloat& bf2)
{
	std::string c = {};
	bigfloat a = bf1;
	bigfloat b = bf2;
	bigfloat ans;
	if (!a.sign && !b.sign)
	{
		if (a < b)
		{
			ans = -(b - a);
		}
		else
		{
			if (a.power > b.power)
			{
				add_zeros(b, a.power - b.power);
			}
			else if (a.power < b.power)
			{
				add_zeros(a, b.power - a.power);
			}
			int len1 = a.num.length(), len2 = b.num.length();
			int k = 0;
			int lendif = len1 - len2;

			for (int i = len2 - 1; i>=0; --i)
			{
				k += a.num[i + lendif] - b.num[i] + '0';
				if (k >= '0')
				{
					c.insert(0, 1, k);
					k = 0;
				}
				else
				{
					c.insert(0, 1, k + 10);
					k = -1;
				}
			}
			for (int i = lendif - 1; i>=0; --i)
			{
				k += a.num[i];
				if (k >= '0')
				{
					c.insert(0, 1, k);
					k = 0;
				}
				else
				{
					c.insert(0, 1, k + 10);
					k = -1;
				}
			}
			while (c[0] == '0' && c.length() - a.power > 1){c.erase(0, 1);}
			ans.num = c;
			ans.power = a.power;
			ans.tol = a.tol;
			ans.sign = a.sign;
		}
		
	}
	else if (a.sign && b.sign)
	{
		b = -b;
		a = -a;
		ans = b - a;
	}
	else if (a.sign && !b.sign)
	{
		b = -b;
		ans = a + b;
	}
	else //if (!a.sign && b.sign)
	{
		b = -b;
		ans = a + b;
	}
	return ans;
}
bool operator >(const bigfloat& bf1, const bigfloat& bf2)
{
	if (is_null(bf1) && is_null(bf2)){return false;}
	if (!bf1.sign && bf2.sign)
	{
		return true;
	}
	else if (bf1.sign && !bf2.sign)
	{
		return false;
	}
	bigfloat a = bf1, b = bf2;
	if (a.power > b.power)
	{
		add_zeros(b, a.power - b.power);
	}
	else if (a.power < b.power)
	{
		add_zeros(a, b.power - a.power);
	}
	
	if (!a.sign)
	{
		if (a.num.length() - a.power > b.num.length() - b.power)
		{
			return true;
		}
		if (a.num.length() - a.power < b.num.length() - b.power)
		{
			return false;
		}
		int minlen;
		if (a.num.length() > b.num.length()){minlen = b.num.length();}
		else {minlen = a.num.length();}
		for (int i = 0; i < minlen; i++)
		{
			if (a.num[i] > b.num[i]){return true;}
			if (a.num[i] < b.num[i]){return false;}
		}
		if (a.num.length() > b.num.length()){return true;}
		else {return false;}
	}
	else //if (bf1.sign)
	{
		if (a.num.length() - a.power < b.num.length() - b.power)
		{
			return true;
		}
		if (a.num.length() - a.power > b.num.length() - b.power)
		{
			return false;
		}
		int minlen;
		if (a.num.length() < b.num.length()){minlen = b.num.length();}
		else {minlen = a.num.length();}
		for (int i = 0; i < minlen; i++)
		{
			if (a.num[i] < b.num[i]){return true;}
			if (a.num[i] > b.num[i]){return false;}
		}
		if (a.num.length() < b.num.length()){return true;}
		else {return false;}
	}
}

bool operator ==(const bigfloat& bf1, const bigfloat& bf2)
{
	if (is_null(bf1) && is_null(bf2)){return true;}
	bigfloat a = bf1, b = bf2;
	if (a.power > b.power)
	{
		add_zeros(b, a.power - b.power);
	}
	else if (a.power < b.power)
	{
		add_zeros(a, b.power - a.power);
	}
	
	if (!a.num.compare(b.num) && a.sign == b.sign && a.power == b.power)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void erase_last_nulls(bigfloat& bf)
{
	int len = bf.num.length();
	while (bf.num[len-1] == '0' && len>1 && bf.power > 0)
	{
		bf.num.pop_back();
		bf.power--;
		len--;
	}
}

bool operator !=(const bigfloat& bf1, const bigfloat& bf2)
{
	return !(bf1 == bf2);
}

bool operator >=(const bigfloat& bf1, const bigfloat& bf2)
{
	return (bf1 == bf2 || bf1 > bf2);
}

bool operator <(const bigfloat& bf1, const bigfloat& bf2)
{
	return !(bf1 >= bf2);
}

bool operator <=(const bigfloat& bf1, const bigfloat& bf2)
{
	return !(bf1 > bf2);
}

bigfloat operator +(const bigfloat& bf)
{
	return bf;
}

bigfloat operator -(const bigfloat& bf)
{
	bigfloat a = bf;
	a.sign = !a.sign;
	return a;
}

bigfloat operator ++(bigfloat& bf)
{
	bf = bf + 1_mbf;
	return bf;
}

bigfloat operator ++(bigfloat& bf, int)
{
	bigfloat a = bf;
	bf = bf + 1_mbf;
	return a;
}

bigfloat operator --(bigfloat& bf)
{
	bf = bf - 1_mbf;
	return bf;
}

bigfloat operator --(bigfloat& bf, int)
{
	bigfloat a = bf;
	bf = bf - 1_mbf;
	return a;
}

bigfloat operator *(const bigfloat& bf1, const bigfloat& bf2)
{
	std::string c = {};
	bigfloat ans;
	int len1 = bf1.num.length(), len2 = bf2.num.length();
	for (int i = len1 + len2 - 1; i >= 0; --i)
	{
		c.append("0");
	}
	int k;
	for (int i = len2 - 1; i >= 0; --i)
	{
		
		for (int j = len1 - 1; j >= 0; --j)
		{
			k = (bf2.num[i] - '0') * (bf1.num[j] - '0');
			c[i + j + 1] += k%10;
			if (c[i + j + 1] > '9')
			{
				c[i + j] += 1;
				c[i + j + 1] -= 10;
			}
			c[i + j] += k/10;
		}
	}
	ans.power = bf1.power + bf2.power;
	while (c[0] == '0' && c.length() - ans.power > 1){c.erase(0, 1);}
	ans.tol = (bf1.tol > bf2.tol ? bf1.tol : bf2.tol);
	ans.num = c;
	ans.sign = (bf1.sign == bf2.sign ? false : true);
	return ans;
}

bigfloat operator /(const bigfloat& bf1, const bigfloat& bf2)
{
	if (is_null(bf2))
	{
		std::cout << "wrong input" << std::endl;
		exit(1);
	}
	bigfloat op1 = bf1, op2 = bf2;
	bigfloat ans;
	bigfloat a;
	bigfloat k;
		
	std::string c = {};
	op1.power = 0;
	op2.power = 0;

	int pow = bf1.power - bf2.power;
	add_zeros(op1, op1.tol - pow);
	pow = op1.power + pow;
	int len = op1.num.length();
	a.num = "";

	for (int i = 0; i < len; i++)
	{
		k.num = "0";
		if (is_null(a))
		{a.num = op1.num.substr(i, 1);}
		else{a.num.append(op1.num.substr(i, 1));}

		while ((k + 1_mbf)*op2 <= a){++k;}
		a = a - k*op2;
		c.append(k.num);
	}
	ans.power = pow;
	while (c[0] == '0' && c.length() - ans.power > 1){c.erase(0, 1);}
	ans.num = c;
	ans.sign = (bf1.sign == bf2.sign ? false : true);
	ans.tol = bf1.tol;
	
	erase_last_nulls(ans);
	return ans;
}

std::string get_str(bigfloat& bf)
{
	std:: string str;
	if (bf.sign)
	{
		str.append("-");
	}
	if (bf.power == 0)
	{
		str.append(bf.num);
	}
	else
	{
		int i, len = bf.num.length() - bf.power;
		for (i = 0; i < len; ++i)
		{
			str.push_back(bf.num[i]);
		}
		str.append(".");
		len += bf.power;
		for (; i < len; ++i)
		{
			str.push_back(bf.num[i]);
		}
	}
	return str;
}

int main(){
	bigfloat a, e;
	// a = 15.132412341234_mbf;
	// e = 1.13241_mbf;
	std::cin >> a >> e; 
	bigfloat c = a / e;
	// std::cout << c << std::endl;
	std::string f = get_str(c);
	std::cout << f << std::endl;
	// std::cout << e.num << ' ' << e.power << ' ' << e.sign << ' ' << e.tol << std::endl;
	// std::cout << e <<  std::endl;
	return 0;
}
