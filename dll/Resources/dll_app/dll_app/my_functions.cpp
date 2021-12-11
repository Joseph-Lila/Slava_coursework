//#include <iostream>
#include <regex>
#include <string>
#include <stack>
#include "pch.h"
#include "my_library.h"
#include "windows.h"
#include "lagrange_interpolator.h"

using namespace std;

bool checkCorrectInput(string s, double x, vector<string>& splitedString, regex regexFunc);
vector<string> prefixNotation(vector<string> splitedString);
void goBackByBrakets(vector<string>& reversePrefixStr, stack<string>& st);
void goBackByOperator(vector<string>& reversePrefixStr, stack<string>& st, int priority);
int getPriority(char ch);
bool isFunc(string s);
bool isBracket(string s);
bool isOperator(string s);
double calculate(vector<string> reversePrefixStr, bool& flag);
double calculateFunc(double value, string s);
double calculateOperation(double a, double b, string s);
// �������� ���������� ������� ��������� ������, ����� ���������� �������� �����������.
// ��� ��� ������� �������� main.
//double calculateExpression(const char* inputStr, int strLen, double x);

double my_function() {
	double res = 0;
	double a = 1;
	double b = 3;
	__asm {
		fld a;
		fadd b;
		fstp res;
	}
	//res = a + b;
	return res;
}

int my_sum(int a, int b) {
	return a + b;
}

double my_double_multiply(double a, double b) {
	return a * b;
}

/// <summary>
/// ���������� �������� ��������� �� ����������
/// </summary>
/// <param name="inputStr">������ � ����������</param>
/// <param name="strLen">����� ������ � ����������</param>
/// <param name="startX">��������� �������� X � ����������</param>
/// <param name="finishX">�������� �������� X � ����������</param>
/// <param name="amountX">���������� �������� � ����������</param>
/// <param name="buffer">��������� �� ������ double �������� amountX</param>
/// <returns>��� ����������. 1 - �������.</returns>
double* calculateExpressionInRange(char* inputStr, int strLen, double startX, double finishX, int amountX, double* buffer) {
	double tempX = startX;
	// ���������� ������ ��������. ������ ������� � � �� ���� ������ ������, �� ������ ����  ������ ���.
	double deltaX = (finishX - startX) / (amountX - 1);
	for (int i = 0; i <= amountX; i++) {
		buffer[i] = calculateExpression(inputStr, strLen, tempX);
		tempX += deltaX;
	}
	return buffer;
}



/// <summary>
/// ��������� ��������� � ����������� ���� � �������� ���������� x. ���������� ����������� �������� ���������.
/// </summary>
/// <param name="inputStr">������ � ����������</param>
/// <param name="strLen">����� ������ � ����������</param>
/// <param name="x">�������� ����������</param>
/// <returns>�������� ��������� � ����� x</returns>
double calculateExpression(char *inputStr, int strLen, double x) {
	string tempStr = string(inputStr, strLen);
	tempStr = "(" + tempStr + ")";
	regex regexFunc("(\\d+(\\.|,\\d+)?)|[\\+\\-\\^\\*\\/\\(\\)]|sin\\(|cos\\(|tg\\(|ctg\\(|exp\\(|log\\(|log10\\(|sqrt\\(");
	vector<string> splitedString;
	
	if (!checkCorrectInput(tempStr, x, splitedString, regexFunc)) {
		return NULL;
	}
	else {
		vector<string> reversePrefixStr = prefixNotation(splitedString);
		for (int i = 0; i < reversePrefixStr.size(); i++)
		{
			if (reversePrefixStr[i] == "x")
				reversePrefixStr[i] = to_string(x);
		}
		bool flag = true;
		double answer = calculate(reversePrefixStr, flag);
		return answer;
	}
}

// ��������� �� ������������ ������ ���������. 
bool checkCorrectInput(string s, double x, vector<string>& splitedString, regex regexFunc) {
	s = regex_replace(s, regex("\\."), ",");
	s = regex_replace(s, regex(" "), "");
	s = regex_replace(s, regex("\\(-"), "(0-");
	s = regex_replace(s, regex("\\+{2,}"), "+");
	s = regex_replace(s, regex("\\-{2,}"), "-");
	s = regex_replace(s, regex("\\*{2,}"), "*");
	s = regex_replace(s, regex("\\/{2,}"), "/");

	int startLength = s.length();
	int length = 0;
	smatch m;

	while (regex_search(s, m, regexFunc) == true) {
		if (s[0] == 'x') {
			splitedString.push_back(m.prefix());
			length += m.prefix().length();
		}
		splitedString.push_back(m[0]);
		length += m[0].length();
		s = m.suffix();
	}

	if (length != startLength) {
		return false;
	}
	else
		return true;
}

// ���������� �������� ��������� ��������.
int getPriority(char ch) {
	switch (ch) {
	case '(':
	case ')': return 0;
	case '+': return 1;
	case '-': return 2;
	case '*':
	case '/': return 3;
	case '^': return 4;
	default:  return 0;
	}
}

// ������������ ���������� ������ � ������� �������.
vector<string> prefixNotation(vector<string> splitedString) {
	stack<string> stackStr;
	vector<string> reversePrefixStr;

	for (int i = 0; i < splitedString.size(); i++) {
		if (isFunc(splitedString[i])) {
			stackStr.push(splitedString[i]);
			continue;
		}

		if (i == splitedString.size() - 1 && stackStr.top() == "^") {
			string tmp = stackStr.top();
			stackStr.pop();
			reversePrefixStr.push_back(tmp);
		}

		if (isBracket(splitedString[i])) {
			if (splitedString[i] == ")") {
				goBackByBrakets(reversePrefixStr, stackStr);
			}
			else stackStr.push(splitedString[i]);
			continue;
		}

		if (isOperator(splitedString[i])) {
			if (getPriority(splitedString[i].c_str()[0]) < getPriority(stackStr.top().c_str()[0])) {
				goBackByOperator(reversePrefixStr, stackStr, getPriority(splitedString[i].c_str()[0]));
			}
			else if (stackStr.top() == "/" || stackStr.top() == "^") {
				string tmp = stackStr.top();
				stackStr.pop();
				reversePrefixStr.push_back(tmp);
			}
			stackStr.push(splitedString[i]);
			continue;
		}

		reversePrefixStr.push_back(splitedString[i]);
	}
	return reversePrefixStr;
}

// ��������� ���������� �������, ��������������� ����������������� � �������� �������.
double calculate(vector<string> reversePrefixStr, bool& flag) {
	stack<double> stackDouble;

	for (int i = 0; i < reversePrefixStr.size(); i++) {
		if (isFunc(reversePrefixStr[i])) {
			double value = stackDouble.top();
			stackDouble.pop();
			stackDouble.push(calculateFunc(value, reversePrefixStr[i]));
			continue;
		}

		if (isOperator(reversePrefixStr[i])) {
			double b = stackDouble.top();
			stackDouble.pop();
			double a = stackDouble.top();
			stackDouble.pop();
			stackDouble.push(calculateOperation(a, b, reversePrefixStr[i]));
			continue;
		}

		double q = atof(reversePrefixStr[i].c_str());
		stackDouble.push(q);
	}
	return stackDouble.top();
}

// ��������� �������������� � ����������������� �������� � ���������� ��������� � ��������.
double calculateFunc(double value, string s) {
	double y = 0;
	if (s == "sin(")
		__asm {
		fld value
		fsin
		fstp y
	}
	else if (s == "cos(")
		__asm {
		fld value
		fcos
		fstp y
	}
	else if (s == "tg(")
		__asm {
		fld value
		fptan
		fxch st(1)
		fstp y
	}
	else if (s == "ctg(")
		__asm {
		fld value
		fptan
		fdiv
		fstp y
	}
	else if (s == "sqrt(")
		__asm {
		fld value
		fsqrt
		fstp y
	}
	return y;

	throw invalid_argument("");
}

// ��������� �������������� �������� � ����� ���������� ��������� � ����������.
double calculateOperation(double a, double b, string s) {
	double y = 0;
	if (s == "*") {
		__asm {
			fld a;
			fmul b;
			fstp y;
		}
	}
	if (s == "/") {
		__asm {
			fld a;
			fdiv b;
			fstp y;
		}
	}
	if (s == "+") {
		__asm {
			fld a;
			fadd b;
			fstp y;
		}
	}
	if (s == "-") {
		__asm {
			fld a;
			fsub b;
			fstp y;
		}
	}
	if (s == "^") {
		;
		__asm {
			fld a
			fld b
			fxch st(1)
			fldln2
			fxch st(1)
			fyl2x
			fmulp st(1), st(0)
			fldl2e
			fmul
			fld st
			frndint
			fsub st(1), st
			fxch st(1)
			f2xm1
			fld1
			fadd
			fscale
			fstp st(1)
			fstp y
		}

		// ���� ������� �� �������������� �����, �� ������ ������ ����.
		if (isnan(y)) {
			y = 0;
		}
	}
	return y;
	throw invalid_argument("");
}

// �������� ������ �� ������������ ��������� �������.
bool isFunc(string s) {
	if (s == "sin(" || s == "cos(" || s == "tg(" || s == "ctg(" ||
		s == "acos(" || s == "asin(" || s == "atan(" ||
		s == "sinh(" || s == "cosh(" || s == "tanh(" ||
		s == "exp(" || s == "log(" || s == "log10(" || s == "sqrt(")
	{
		return true;
	}
	else
		return false;
}

// �������� ������ �� ������������ ������� ������.
bool isBracket(string s) {
	if (s == "(" || s == ")")
		return true;
	else
		return false;
}

// �������� ������ �� ������������ ��������������� ���������.
bool isOperator(string s) {
	if (s == "*" || s == "/" || s == "+" || s == "-" || s == "^")
		return true;
	else
		return false;
}

// ���� ������ ������ � �������� �������.
void goBackByBrakets(vector<string>& reversePrefixstr, stack<string>& str) {
	int pos = str.top().find('(', 0);
	while (pos < 0) {
		reversePrefixstr.push_back(str.top());
		str.pop();
		pos = str.top().find('(', 0);
	}

	if (isFunc(str.top())) reversePrefixstr.push_back(str.top());
	str.pop();
}

// ���� ������� ��������� � �������� �������. 
void goBackByOperator(vector<string>& reversePrefixstr, stack<string>& str, int priority) {
	while (getPriority(str.top().c_str()[0]) > priority) {
		reversePrefixstr.push_back(str.top());
		str.pop();
	}
}
