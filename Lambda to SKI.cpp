#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool space(char c)
{
	return isspace(c);
}

bool not_space(char c)
{
	return !isspace(c);
}

vector<string> split(const string& str) {
	//�� ���� iterator�� ����� ��ȣ�� ã�� �ܾ��� ���۰� ���� ������ String�� ����.
	
	typedef string::const_iterator iter;
	vector<string> ret;
	int rbracenum = 0;

	iter it_b = str.begin();

	while (it_b != str.end()) {
		it_b = find_if(it_b, str.end(), not_space);

		iter it_e = find_if(it_b, str.end(), space);

		if (it_b != str.end()) {
			while (*it_b == '(') {
				ret.push_back(string(it_b, (it_b + 1)));
				it_b++;
			}
			while (*(it_e - 1) == ')') {
				rbracenum++;
				it_e--;
			}
			if (*it_b == '\\'){
				ret.push_back(string(it_b, (it_b + 1)));
				it_b++;
			}
			if(it_b != str.end()) ret.push_back(string(it_b, it_e));
			for (int i = 0; i < rbracenum; i++) {
				ret.push_back(")");
			}
			
		}

		it_b = it_e + rbracenum;
		rbracenum = 0;
	}

	return ret;
}

int tokenprint(vector<string> token) {
	int printedtokens = 0;
	int brace = 0;

	
	while (printedtokens != size(token)) {
		if (printedtokens == size(token))
			return 0;
		else if (printedtokens == size(token) - 1) {
			cout << "\"" << token[printedtokens++] << "\"";
		}
		else if (printedtokens == size(token) - 2) {
			cout << "App(\"" << token[printedtokens] << "\", \"" << token[printedtokens + 1] << "\")";
			brace++;
			printedtokens += 2;
		}
		else{
			cout << "App(App(\"" << token[printedtokens] << "\", \"" << token[printedtokens + 1] << "\"), ";
			brace += 2;
			printedtokens += 2;
		}
	}

	while (!(brace == 0 || brace == 1)) {
		cout << ")";
		brace--;
	}
}


int printtokens(vector<string> token1, vector<string> token2) {
	vector<string> print;

	if (size(token2) != 0) cout << "App(";
	tokenprint(token1);
	if(size(token2) != 0) cout << ", ";
	//if (size(token2) > 2) cout << "App(";

	tokenprint(token2);
	if(size(token2) > 1) cout << "))";
	else cout << ")";
	

	return 0;
}

int lamexp(vector<string> inputtoken) {
	string result;
	vector<string> token1, token2;
	int remainlambda = 0;
	int lambdanum = 0;
	int brace = 0;
	int brace0token = 0;
	
	
	//�Է� ��ū ����������� ������
	for (vector<string>::const_iterator iter = inputtoken.begin(); iter != inputtoken.end(); ++iter) {
		if (*iter == "\\") remainlambda++;
	}
	lambdanum = remainlambda;

	if (remainlambda > 1) cout << "App(";
	for (int iter = 0; iter < size(inputtoken); iter++) {
		if (inputtoken[iter] == "(") {
			brace++;
			continue;
		}
		else if (inputtoken[iter] == ")" || (iter + 1) == size(inputtoken)){
			brace--;
			if (inputtoken[iter] != ")" && (iter + 1) == size(inputtoken)) {
				brace++;
				token1.push_back(inputtoken[iter]);
			}
			if (brace == 0) {
				printtokens(token1, token2);
				if (--remainlambda > 0) cout << ", ";
				token1.clear();
				token2.clear();
			}
			continue;
		}
		//Lambdaǥ��
		else if (inputtoken[iter] == "\\") {
			cout << "Lam([\"" << inputtoken[++iter] << "\"";
			while (inputtoken[++iter] != "->") {
				cout << ", \"" << inputtoken[iter] << "\"";
			}
			cout << "], ";
			if (brace != 0) continue;
			else iter++;
		}


		//��ȣ�� ���� ���� �Է� term���� �з���.
		if (brace == 0) {
			token1.push_back(inputtoken[iter]);
			brace0token = 1;
			if (iter + 1 == size(inputtoken)) {
				printtokens(token1, token2);
				if (--remainlambda > 0) cout << ", ";
				token1.clear();
				token2.clear();
				brace0token = 0;
			}
		}
		else if (brace == 1) {
			if(brace0token == 1) token2.push_back(inputtoken[iter]);
			else token1.push_back(inputtoken[iter]);
		}
		else if (brace == 2) token2.push_back(inputtoken[iter]);
	}
	if (lambdanum > 1) cout << ")";

	return 0;
}

int SKI(vector<string> inputtoken) {
	
	//���ٵ��� ������ �� �ְ� �տ� \�� ���̰� ->�� ������.
	int lam = 0;
	for (int i = 0; i < size(inputtoken); i++) {
		if (inputtoken[i] == "\\") {
			inputtoken.erase(inputtoken.begin() + i);
			while (inputtoken[i] != "->") {
				inputtoken[i] = "\\" + inputtoken[i];
				i++;
			}
			inputtoken.erase(inputtoken.begin() + i);
		}
	}

	//���⼭���� SKI Convert����
	int highestlam = 0;
	int finish = 0;
	int repeat = 0;
	while (!finish) {
		//�˰��� �̿ϼ����� ���� ���ѷ��� ����
		if (repeat++ > 1000) {
			cout << "\n���: ";
			for (int i = 0; i < size(inputtoken); i++) {
				cout << inputtoken[i];
			}
			cout << "\n<�� �Ŀ� ���� ��ȯ���� ���� �ϼ����� ���߽��ϴ�.>" << endl;
			
			return 0;
		}


		for (int i = 0; i < size(inputtoken); i++) {
			if (inputtoken[i].at(0) == '\\')
				highestlam = i;
		}
		//I�Ǻ�(4��Ģ)
		if (inputtoken[highestlam].substr(1) == inputtoken[highestlam + 1]) {
			inputtoken[highestlam] = "I";
			inputtoken.erase(inputtoken.begin() + highestlam + 1);
		}
		//���� �ι�° ���� Term�� ������ ��ȣ�� �������� �ʴ� ���� S�Ǻ�(6��Ģ ��ȯ)
		else if ((highestlam + 2 < size(inputtoken)) && (inputtoken[highestlam].substr(1) == inputtoken[highestlam + 2])) {
			string temp1 = inputtoken[highestlam];
			string temp2 = inputtoken[highestlam + 1];
			string temp3 = inputtoken[highestlam + 2];

			inputtoken.erase(inputtoken.begin() + highestlam, inputtoken.begin() + highestlam + 3);

			inputtoken.insert(inputtoken.begin() + highestlam, ")");
			inputtoken.insert(inputtoken.begin() + highestlam, temp3);
			inputtoken.insert(inputtoken.begin() + highestlam, temp1);
			inputtoken.insert(inputtoken.begin() + highestlam, "(");
			inputtoken.insert(inputtoken.begin() + highestlam, ")");
			inputtoken.insert(inputtoken.begin() + highestlam, temp2);
			inputtoken.insert(inputtoken.begin() + highestlam, temp1);
			inputtoken.insert(inputtoken.begin() + highestlam, "(");
			inputtoken.insert(inputtoken.begin() + highestlam, "S");
		}
		//�ٷ� ��ȣ�� ���� S�Ǻ�(6��Ģ)
		//else if (1) { ; }
		//K�Ǻ�(3��Ģ)
		else if (inputtoken[highestlam].substr(1) != inputtoken[highestlam + 1]) {
			inputtoken[highestlam] = "K";
		}
			
		
		//KSI�� ��ȣ�� ������ ��� ����
		for (int i = 0; i < size(inputtoken); i++) {
			if (inputtoken[i] == "K" || inputtoken[i] == "S" || inputtoken[i] == "I" || inputtoken[i] == ")" || inputtoken[i] == "(") finish = 1;
			else {
				finish = 0;
				break;
			}
		}
	}

	cout << "\n���: ";
	for (int i = 0; i < size(inputtoken); i++) {
		cout << inputtoken[i];
	}

	return 0;
}

int showexample() {
	//Example1
	cout << "//Example 1\n" << endl;
	cout << "�Է�: add 1 1 => (\\n f x -> f (n f x)) (\\f x -> f x)" << endl;
	string input = "(\\m n f x -> m f (n f x)) (\\f x -> f x) (\\f x -> f x)";
	vector<string> inputtoken = split(input);
	cout << "���� ǥ����: ";
	lamexp(inputtoken);
	SKI(inputtoken);

	//Example2
	cout << "\n\n//Example 2\n" << endl;
	cout << "�Է�: succ 1 => (\\n f x -> f (n f x)) (\\f x -> f x)" << endl;
	input = "(\\n f x -> f (n f x)) (\\f x -> f x)";
	inputtoken = split(input);
	cout << "���� ǥ����: ";
	lamexp(inputtoken);
	SKI(inputtoken);

	//Example3
	cout << "\n\n//Example 3\n" << endl;
	cout << "�Է�: 0 => \\f x -> x" << endl;
	input = "\\f x -> x";
	inputtoken = split(input);
	cout << "���� ǥ����: ";
	lamexp(inputtoken);
	SKI(inputtoken);
	cout << endl;

	//Example4
	cout << "\n\n//Example 4\n" << endl;
	cout << "�Է�: 1 => \\f x -> f x" << endl;
	input = "\\f x -> f x";
	inputtoken = split(input);
	cout << "���� ǥ����: ";
	lamexp(inputtoken);
	SKI(inputtoken);

	return 0;
}

int main() {
	showexample();

	string input;
	cout << "\n\n";
	cout << "Lambda Expression�� �Է��� �ּ���." << endl;
	cout << "�� ��ū�� ����� �����մϴ�." << endl;
	cout << "EX.	(\\n f x -> f(n f x)) (\\f x -> f x)			-----> X" << endl;
	cout << "   	(\\n f x -> f (n f x)) (\\f x -> f x)			-----> O" << endl;
	cout << "   	(\\m n f x -> m f (n f x)) (\\f x -> f x) (\\f x -> f x)   -----> O" << endl;
	cout << "\n�Է�: ";
	
	getline(cin, input);
	vector<string> inputtoken = split(input);
	lamexp(inputtoken);
	cout << endl;

	SKI(inputtoken);

	return 0;
}