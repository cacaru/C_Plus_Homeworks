#include "std_lib_facilities.h"

class OutOfRangeException{};	// 과제용 클래스
class Not_correct_Input {};		// 입력과정에서 혹시나 하는 에러 방지용 클래스
// 평균 구하는 함수
float calculateAverage(int a, int b, int c, int d);
//값 입력받는 함수
void Inputnumber(int &a, int &b, int &c, int &d);

int main(void) {
	try
	{
		int a = 0, b = 0, c = 0, d = 0;
		cout << "Input four scores:" << endl;
		//값을 입력받아보자
		Inputnumber(a, b, c, d);

		//4가지 값이 평균 구한 값을 출력하기
		//calculateAverage 함수 호출하면서 조건 0-100 사이 검사 실행
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

//4가지 값 입력받는 함수
void Inputnumber(int &a, int &b, int &c, int &d)
{
	cin >> a;
	//입력받는데 값이 들어오지않으면 에러메세지 출력후 대기;
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

	//정상이라면 그냥 넘어가기!
}

//네 수의 평균을 구하는 함수
float calculateAverage(int a, int b, int c, int d)
{
	float average;
	//입력받은 a b c d 값을 검사
	//0-100사이가 아니라면 OutofRangeException을 스로우
	if (a < 0 || a > 100)
		throw OutOfRangeException{};
	if (b < 0 || b > 100)
		throw OutOfRangeException{};
	if (c < 0 || c > 100)
		throw OutOfRangeException{};
	if (d < 0 || d > 100)
		throw OutOfRangeException{};

	//정상이라면 !
	//입력받은 4가지 값의 평균 구하기
	average = ((float)a + (float)b + (float)c + (float)d) / 4;

	//구한 평균을 float 값으로 리턴;
	return average;
}