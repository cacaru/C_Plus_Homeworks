#include "std_lib_facilities.h"

class OutOfRangeException{};	// ������ Ŭ����
class Not_correct_Input {};		// �Է°������� Ȥ�ó� �ϴ� ���� ������ Ŭ����
// ��� ���ϴ� �Լ�
float calculateAverage(int a, int b, int c, int d);
//�� �Է¹޴� �Լ�
void Inputnumber(int &a, int &b, int &c, int &d);

int main(void) {
	try
	{
		int a = 0, b = 0, c = 0, d = 0;
		cout << "Input four scores:" << endl;
		//���� �Է¹޾ƺ���
		Inputnumber(a, b, c, d);

		//4���� ���� ��� ���� ���� ����ϱ�
		//calculateAverage �Լ� ȣ���ϸ鼭 ���� 0-100 ���� �˻� ����
		cout << "The average score is " << calculateAverage(a, b, c, d) << '\n';
		keep_window_open();
	}
	catch (OutOfRangeException)
	{
		cout << "One or more scores are out of range" << endl;
		keep_window_open();
	}
	catch (Not_correct_Input)
	{
		cout << "Mis InPut !!!" << endl;
		keep_window_open();
	}
	catch (...)
	{
		cout << "What's wrong... " << endl;
		keep_window_open();
	}

}

//4���� �� �Է¹޴� �Լ�
void Inputnumber(int &a, int &b, int &c, int &d)
{
	cin >> a;
	//�Է¹޴µ� ���� ������������ �����޼��� ����� ���;
	if (!cin)
		throw Not_correct_Input{};
	cin >> b;
	if (!cin)
		throw Not_correct_Input{};
	cin >> c;
	if (!cin)
		throw Not_correct_Input{};
	cin >> d;
	if (!cin)
		throw Not_correct_Input{};

	//�����̶�� �׳� �Ѿ��!
}

//�� ���� ����� ���ϴ� �Լ�
float calculateAverage(int a, int b, int c, int d)
{
	float average;
	//�Է¹��� a b c d ���� �˻�
	//0-100���̰� �ƴ϶�� OutofRangeException�� ���ο�
	if (a < 0 || a > 100)
		throw OutOfRangeException{};
	if (b < 0 || b > 100)
		throw OutOfRangeException{};
	if (c < 0 || c > 100)
		throw OutOfRangeException{};
	if (d < 0 || d > 100)
		throw OutOfRangeException{};

	//�����̶�� !
	//�Է¹��� 4���� ���� ��� ���ϱ�
	average = ((float)a + (float)b + (float)c + (float)d) / 4;

	//���� ����� float ������ ����;
	return average;
}