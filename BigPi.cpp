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
duration<double> compute_pi(mpf_t& pi);


/**
 * Compute multiple-precision pi using overloaded operators
 * and return the elapsed time.
 * @param pi to be computed.
 * @return the compute time.
 */
//duration<double>
duration<double> compute_pi(mpf_class& pi);

/**
 * Compute the 4th root of a multiple-precision number x.
 * @param root4 the computed 4th root.
 * @param x the value whose root to compute.
 * @return the 4th root.
 */
inline void root4(mpf_t& root4, const mpf_t& x);

/**
 * Compute the 4th root of a multiple-precision number x.
 * @param root4 the computed 4th root.
 * @param x the value whose root to compute.
 * @return the 4th root.
 */
inline mpf_class& root4(mpf_class& root4, const mpf_class& x);

/**
 * Compute the 4th power of a multiple-precision number x.
 * @param xxxx the computed 4th root.
 * @param x the value whose root to compute.
 * @return the 4th power.
 */
inline void power4(mpf_t& xxxx, const mpf_t& x);

/**
 * Compute the 4th power of a multiple-precision number x.
 * @param xxxx the computed 4th root.
 * @param x the value whose root to compute.
 * @return the 4th power.
 */
inline mpf_class& power4(mpf_class& xxxx, const mpf_class& x);

//print pi created with functions
//@param cstring of pi
//@param time in milliseconds
void printPi(char * str);

int main(){
    mpf_set_default_prec(BIT_COUNT*PRECISION/2);  // precision in bits

    cout << "pi to " << PLACES << " places calculated with functions:" << endl;

    mpf_t pi;
    duration<double> elapsed_time = compute_pi(pi);
    mp_exp_t exp;
    char *str = NULL;

    char *s = mpf_get_str(str, &exp, BASE, PRECISION, pi);
    printPi(s);
    cout << "Elapsed time: " << elapsed_time.count() << " seconds" << endl;

    cout << endl;
    cout << "pi to " << PLACES << " places calculated with operators:" << endl;

    mpf_class pi_obj;
    elapsed_time = compute_pi(pi_obj);
    str = NULL;

    s = mpf_get_str(str, &exp, BASE, PRECISION, pi_obj.get_mpf_t());
    printPi(s);
    cout << "Elapsed time: " << elapsed_time.count() << " seconds" << endl;

    return 0;
}

duration<double> compute_pi(mpf_t& pi){
	steady_clock::time_point start_time = steady_clock::now();

	mpf_t one, two, four, six, sqrt2, sqrt2x4;
	mpf_t y, y_prev, a, a_prev, y4, yRoot4, aTerm, powers2, y2;

	// Multiple-precision constants.
	mpf_init(one);  mpf_set_str(one,  "1", BASE);
	mpf_init(two);  mpf_set_str(two,  "2", BASE);
	mpf_init(four); mpf_set_str(four, "4", BASE);
	mpf_init(six);  mpf_set_str(six,  "6", BASE);

	// The square root of 2 and four times the square root of 2.
	mpf_init(sqrt2);   mpf_sqrt(sqrt2, two);
	mpf_init(sqrt2x4); mpf_mul(sqrt2x4, four, sqrt2);

	// Multiple-precision variables.
	mpf_init(y);
	mpf_init(y_prev); mpf_sub(y_prev, sqrt2, one);
    mpf_init(a);
    mpf_init(a_prev); mpf_sub(a_prev, six, sqrt2x4);
    mpf_init(y4);
    mpf_init(yRoot4);
    mpf_init(aTerm);
    mpf_init(powers2); mpf_set_str(powers2, "2", BASE);
    mpf_init(y2);

    for (int i = 1; i <= MAX_ITERATIONS; i++){
    	// Compute y^4
        power4(y4, y_prev);

        mpf_t temp1, temp2;
        mpf_init(temp1);
        mpf_init(temp2);

        // Compute the fourth root of (1 - y^4)
        mpf_sub(temp1, one, y4);
        root4(yRoot4, temp1);

        // Compute y
        mpf_sub(temp1, one, yRoot4);
        mpf_add(temp2, one, yRoot4);
        mpf_div(y, temp1, temp2);

        // Compute a*(1 + y)^4
        mpf_add(temp1, one, y);
        power4(temp2, temp1);
        mpf_mul(aTerm, a_prev, temp2);

        // Compute 2^(2*i + 1)
        mpf_mul(powers2, four, powers2);

        // Compute y^2
        mpf_mul(y2, y, y);

        // Compute a
        mpf_add(temp1, one, y);
        mpf_add(temp1, temp1, y2);
        mpf_mul(temp1, temp1, y);
        mpf_mul(temp1, temp1, powers2);
        mpf_sub(a, aTerm, temp1);

        mpf_set(a_prev, a);
        mpf_set(y_prev, y);
    }

    mpf_init(pi);

	//inverse and assign to pi
	mpf_div(pi, one, a);

	steady_clock::time_point end_time = steady_clock::now();
	duration<double> elapsed_time = end_time - start_time;

	return elapsed_time;
}

void root4(mpf_t& root4, const mpf_t& x){
    mpf_t root2;
    mpf_init(root2);

    mpf_sqrt(root2, x);
    mpf_sqrt(root4, root2);
}

void power4(mpf_t& xxxx, const mpf_t& x){
    mpf_t xx;
    mpf_init(xx);

    mpf_mul(xx, x, x);
    mpf_mul(xxxx, xx, xx);
}

duration<double> compute_pi(mpf_class& pi){
	steady_clock::time_point start_time = steady_clock::now();

	const mpf_class ONE(1), TWO(2), FOUR(4), SIX(6);
	mpf_class y, y_prev, a, a_prev, y4, yRoot4, aTerm, powers2, temp;

    const mpf_class SQRT2 = sqrt(TWO);
    y_prev = SQRT2 - ONE;
    a_prev = SIX - FOUR*SQRT2;
    powers2 = 2;

    for (int i = 1; i <= MAX_ITERATIONS; i++){
        y4 = power4(y4, y_prev);
        yRoot4 = root4(yRoot4, ONE - y4);
        y = (ONE - yRoot4)/(ONE + yRoot4);
        aTerm = a_prev*power4(temp, ONE + y);
        powers2 = FOUR*powers2;
        a = aTerm - powers2*y*(ONE + y + y*y);

        a_prev = a;
        y_prev = y;
    }

    pi = ONE/a;

	steady_clock::time_point end_time = steady_clock::now();
	duration<double> elapsed_time = end_time - start_time;
	return elapsed_time;
}

mpf_class& root4(mpf_class& root4, const mpf_class& x){
    root4 = sqrt(sqrt(x));
    return root4;
}

mpf_class& power4(mpf_class& xxxx, const mpf_class& x){
    mpf_class xx = x*x;
    xxxx = xx*xx;
    return xxxx;
}

void printPi(char* str){
	cout << str[0] << ".";
	for(int i = 1; i < PRECISION; i++){
		cout << str[i];
		if(i % 500 == 0) cout << endl << endl << "  ";
		else if(i % LINE_SIZE == 0) cout << endl << "  ";
		else if(i % BLOCK_SIZE == 0) cout << " ";

	}

}
