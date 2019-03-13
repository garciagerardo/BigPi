#include <cmath>

#include <iostream>
#include <iomanip>
#include <string.h>
#include <chrono>
#include <mpir.h>
#include <mpirxx.h>

using namespace std;
using namespace std::chrono;

const int MAX_ITERATIONS = 5;
const int PLACES         = 1000;        // desired decimal places
const int PRECISION      = PLACES + 1;  // +1 for the digit 3 before the decimal

const int BASE       = 10;  // base 10 numbers
const int BIT_COUNT  = 8;   // bits per machine word

const int BLOCK_SIZE = 10;                // print digits in blocks
const int LINE_SIZE  = 100;               // digits to print per line
const int LINE_COUNT = PLACES/LINE_SIZE;  // lines to print
const int GROUP_SIZE = 5;                 // line grouping size

/**
 * Compute multiple-precision pi using functions
 * and return the elapsed time.
 * @param pi to be computed.
 * @return the compute time.
 */
long compute_pi(mpf_t& pi);


/**
 * Compute multiple-precision pi using overloaded operators
 * and return the elapsed time.
 * @param pi to be computed.
 * @return the compute time.
 */
//duration<double>
long compute_pi(mpf_class& pi);
//print pi created with operands
//@param string of pi
//@param time in milliseconds
void printPi(string str, long t);
//print pi created with functions
//@param cstring of pi
//@param time in milliseconds
void printPi(char * str, long t);

int main()
{
    mpf_set_default_prec(BIT_COUNT*PRECISION/2);  // precision in bits

    mpf_t pi2;
    long t2 = compute_pi(pi2);

//    char *str = ;
//    mp_exp_t *expptr;
//
//    str = mpf_get_str(str, expptr, BASE, PRECISION, pi2);

    mpf_class pi3;
    long t3 = compute_pi(pi3);

    string str3;
    mp_exp_t exp;
    str3 = pi3.get_str(exp, BASE, PRECISION);

//    cout << "to 1000 places calculated with functions:" << endl << endl;
//    printPi(str, t2);

    //print out pi
    cout << "pi to 1000 places calculated with operators:" << endl << endl;
    printPi(str3, t3);


    return 0;
}

long compute_pi(mpf_t& pi){
	steady_clock::time_point start_time = steady_clock::now();

	mpf_t a, y, y_top, y_bot, aF, aL;
	mpf_set_d(a, 6 - (4 * sqrt(2.0)));
	mpf_set_d(y, sqrt(2.0) - 1);

	for(int i = 1; i <= MAX_ITERATIONS; i++){
		//calculate y
		mpf_pow_ui(y_top, y, 4);
		mpf_ui_sub(y_top, 1, y_top);
		mpf_sqrt(y_top, y_top);
		mpf_sqrt(y_top, y_top);
		mpf_set(y_bot, y_top);//copy y_top to y_bot
		mpf_ui_sub(y_top, 1, y_top);//1-y_top
		mpf_add_ui(y_bot, y_bot, 1);//1+y_bot
		mpf_div(y, y_top, y_bot);
		//calculate a

		//calculate first part
		mpf_add_ui(aF, y, 1);
		mpf_pow_ui(aF, aF, 4);
		mpf_mul(aF, a, aF);
		//calculate second part
		mpf_pow_ui(aL, y, 2);
		mpf_add(aL, aL, y);
		mpf_add_ui(aL, aL, 1);
		int temp = pow(2, (2 * i) + 1);
		mpf_mul(aL, aL, y);
		mpf_mul_ui(aL, aL, temp);
		//subtract second part from first
		mpf_sub(a, aF, aL);

	}

	//inverse and assign to pi
	mpf_ui_div(pi, 1, a);

	steady_clock::time_point end_time = steady_clock::now();
	long elapsed_time = duration_cast<milliseconds>(end_time - start_time).count();
	return elapsed_time;
}

long compute_pi(mpf_class& pi){
	steady_clock::time_point start_time = steady_clock::now();

	mpf_class a, y, y_top, y_bot, aF, aL;
	a = 6 - (4 * sqrt(2.0));
	y = sqrt(2.0) - 1;

	for(int i = 1;i <= MAX_ITERATIONS;i++){
		//calculate y
		y_top = 1 - (y * y * y * y);
		mpf_sqrt(y_top.get_mpf_t(), y_top.get_mpf_t());
		mpf_sqrt(y_top.get_mpf_t(), y_top.get_mpf_t());
		y_bot = y_top;
		y_top = 1 - y_top;
		y_bot = 1 + y_bot;
		y = y_top / y_bot;
		//calculate a
		aF = y + 1;
		aF = a * (aF * aF * aF * aF);
		aL = y * (1 + y + (y * y));
		aL = pow(2.0, (2 * i) + 1) * aL;
		a = aF - aL;

	}
	pi = 1 / a;
	steady_clock::time_point end_time = steady_clock::now();
	long elapsed_time = duration_cast<milliseconds>(end_time - start_time).count();
	return elapsed_time;
}

void printPi(string str, long t){
	cout << str[0] << ".";
	for(int i = 1; i < str.length(); i++){
		cout << str[i];
		if(i % 500 == 0) cout << endl << endl << "  ";
		else if(i % LINE_SIZE == 0) cout << endl << "  ";
		else if(i % BLOCK_SIZE == 0) cout << " ";

	}
	cout << endl << endl << "  Elapsed time: " << t << " milliseconds" <<endl;
}

void printPi(char* str, long t){
	cout << str[0] << ".";
		for(int i = 1; i < PRECISION; i++){
			cout << str[i];
			if(i % 500 == 0) cout << endl << endl << "  ";
			else if(i % LINE_SIZE == 0) cout << endl << "  ";
			else if(i % BLOCK_SIZE == 0) cout << " ";

		}
		cout << endl << endl << "  Elapsed time: " << t << " milliseconds" <<endl;
}
