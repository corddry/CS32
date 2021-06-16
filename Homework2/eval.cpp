#include "Map.h"
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
#include <iostream>

using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result) {
//infix to postfix
	string workingPostfix;
	stack<char> opstack;

	KeyType ch;
	for (int i = 0; i < infix.size(); i++) {
		ch = infix[i];

		if (ch == ' ') {

		}
		else if (islower(ch)) {
			workingPostfix += ch;
		}
		else if (ch == '(') {
			opstack.push('(');
		}
		else if (ch == ')') {
			if (opstack.empty() || opstack.top() == '(') {
				return 1;
			}
			while (opstack.top() != '(') {
				if (opstack.size() == 1) {
					return 1;
				}
				workingPostfix += opstack.top();
				opstack.pop();
			}
			opstack.pop();
		}
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
			while (!opstack.empty() && opstack.top() != '('
			&& !((ch == '*' || ch == '/') && (opstack.top() == '+' || opstack.top() == '-')) ) {
				postfix += opstack.top();
				opstack.pop();
			}
			opstack.push(ch);
		}
		else {
			return 1;
		}
	}
	while (!opstack.empty()) {
		workingPostfix += opstack.top();
		opstack.pop();
	}

	postfix = workingPostfix;

//postfix evaluation
	ValueType val = 0, operand1 = 0, operand2 = 0;
	stack<ValueType> operandStack;
	for (int i = 0; i < postfix.length(); i++) {
		ch = postfix[i];

		if (islower(ch)) {
			if (!values.contains(ch)) {
				return 2;
			}
			values.get(ch, val);
			operandStack.push(val);
		}
		else {
			operand2 = operandStack.top();
			operandStack.pop();
			if (operandStack.empty()) {
				return 1;
			}
			operand1 = operandStack.top();
			operandStack.pop();

			if (ch == '*') {
				operandStack.push(operand1 * operand2);
			} else if (ch == '/') {
				if (operand2 == 0) {
					return 3;
				}
				operandStack.push(operand1 / operand2);
			} else if (ch == '+') {
				operandStack.push(operand1 + operand2);
			}
			else /*(ch == '-')*/ {
				operandStack.push(operand1 - operand2);
			}
		}
	}
	if (operandStack.size() != 1) {
		return 1;
	}
	result = operandStack.top();
	return 0;
}


int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0 &&
		pf == "ae+" && answer == -6);
	cout << ".";
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	assert(evaluate("()o", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	cout << ".";
//	assert(evaluate("y(*o)", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	cout << ".";
	cout << evaluate("a*b", m, pf, answer) << pf;
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*" && answer == 999);
	cout << ".";
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+" && answer == -1);
	cout << ".";
	answer = 999;
	cout << ".";
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/" && answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a" && answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a" && answer == 3);
	cout << "Passed all tests" << endl;
}