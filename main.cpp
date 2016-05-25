//
//  main.cpp
//  PrimeNumber
//
//  Created by DongVincent on 5/24/16.
//  Copyright © 2016 UCLA. All rights reserved.
//




//========================================================================
// Timer t;                 // create a timer and start it
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

//========================================================================



// Sieve of Eratosthenes

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

unsigned int indexToNumber(int i)
{
    return 6 * ((i / 2) + 1) + ((i % 2 == 0)?(-1):1);
}

int numberToIndex(unsigned int n)
{
    return (n - 2) / 6 * 2 + (((n + 1) % 6 == 0)?0:1);
}

void initialize(vector<bool>* numbers)
{
    vector<bool>::iterator p = numbers->begin();
    while (p != numbers->end())
    {
        *p = true;
        p++;
    }
}

double sieve(vector<bool>* numbers, unsigned int maxNumber) // return the time used
{
    Timer timer;
    
    vector<bool>::iterator p = numbers->begin();
    vector<bool>::iterator mult;
    unsigned int num;
    while (p != numbers->end())
    {
        if (*p)
        {
            num = indexToNumber(static_cast<int>(p - numbers->begin()));
            // any number x is in numbers iff x = 1or-1 (mod 6). So x*3, x*9, x*15, ... = 3or-3 not in numbers, while x*5, x*7, x*11, x*13, ... = 1or-1 are in numbers.
            if (num * 5 <= maxNumber)
            {
                for (unsigned int i = 5; i <= maxNumber / num; i+=2)
                    if (i % 3 != 0)
                    {
                        mult = numbers->begin() + numberToIndex(num * i);
                        if (*mult)
                            *mult = false;
                    }
            }
        }
        p++;
    }
    return timer.elapsed();
}

void print(vector<bool>* numbers, unsigned int maxNumber, double time)
{
    Timer timer;
  
    cout << "\nThe prime numbers less than and equal to " << maxNumber << " are\n\n2\n3\n";
    vector<bool>::iterator p = numbers->begin();
    int counter = 0;
    while (p != numbers->end() - 1)
    {
        if (*p)
        {
            cout << indexToNumber(static_cast<unsigned int>(p - numbers->begin())) << endl;
            counter++;
        }
        p++;
    }
    unsigned int num = indexToNumber(static_cast<int>(p - numbers->begin()));
    if (num <= maxNumber && *p)
    {
        cout << num << endl;
        counter++;
    }
    cout << "\nTime elapsed : " << time << " ms    or    " << time / 1000.0 << " s.\n";
    cout << "( Besides, time spent on printing : " << timer.elapsed() << " ms    or    " << timer.elapsed() / 1000.0 << " s. )\n\n";
 //   cout << counter << " prime numbers are found." << endl;
}


int findAllPrimeNumbers()
{
    cout << "======================== Find Prime Numbers ========================\n\n";
    cout << "find all prime numbers <= SOME INTEGER.\n\n";
    cout << "SOME INTEGER =    10,000,000  : .459 seconds (950  KB).\n";
    cout << "SOME INTEGER = 2,000,000,000  :  145 seconds ( 80  MB).\n";
    cout << "SOME INTEGER = 4,294,967,295  :  331 seconds (171.2MB).\n\n";
    cout << "Input the range :  from 0 to  ";
    unsigned int maxNumber;
    cin >> maxNumber;
    if (maxNumber < 5)
    {
        cout << "Max must be at least 5.\n";
        return 1;
    }
    if (maxNumber > 4294967295 )
    {
        cout << "Max must be at most 4,294,967,295.\n";
        return 2;
    }
    // since even numbers and multiples of 3 are of course not prime numbers, we donot consider them
    // vector[i] means whether 6 * ((i / 2) + 1) - 1 if i is even   is a prime number or not
    //                     and 6 * ((i / 2) + 1) + 1 if i is odd
    // 0  1  2  3  4  5  6  7  8  9  10 11
    // 5  7  11 13 17 19 23 25 29 31 35 37
    vector<bool> numbers;
    if (maxNumber == 4294967295) maxNumber--;
    numbers.resize((maxNumber+1) / 6 * 2);
    initialize(&numbers);
    double time = sieve(&numbers, maxNumber);
    print(&numbers, maxNumber, time);
    return 0;
}

void divide(unsigned int& number, int divisor, bool& isPrime)
{
    if (number % divisor == 0)
    {
        cout << " * " << divisor;
        number /= divisor;
        if (isPrime && number != 1) isPrime = false;
        if (number % divisor == 0)
        {
            cout << "^";
            int power = 1;
            while (number % divisor == 0)
            {
                number /= divisor;
                power++;
            }
            cout << power;
        }
    }
}

int determineAndFactorize()
{
    cout << "====================== Determine and Factorize ======================\n\n";
    cout << "Input the integer you want to check ( <= 4,294,967,295) :  ";
    unsigned int number;
    cin >> number;
    if (number < 1)
    {
        cout << "The number must be positive.\n";
        return 1;
    }
    if (number > 4294967295 )
    {
        cout << "The number must be at most 4,294,967,295.\n";
        return 2;
    }
    Timer timer;
    if (number == 1)
        cout << "\n1 is Not a Prime Number ! A Prime Number must be greater than 1.\n";
    else
    {
        unsigned int maxNumber = sqrt(number);
        vector<bool> numbers;
        numbers.resize((maxNumber+1) / 6 * 2);
        initialize(&numbers);
        sieve(&numbers, maxNumber);
    
        cout << endl << number << " = 1";
        unsigned int n = number;
        bool isPrime = true;
        divide(n, 2, isPrime);
        divide(n, 3, isPrime);
        for (int i = 0; indexToNumber(i) <= sqrt(n); i++)
        {
            if (numbers[i])
            {
                int divisor = indexToNumber(i);
                divide(n, divisor, isPrime);
                if (n == 1) break;
                if (n < maxNumber && numbers[n]) break;
            }
        }
        if (n != 1) cout << " * " << n;
        if (isPrime)
            cout<<"\n\n" << number << " is a Prime Number !\n";
        else
            cout<<"\n\n" << number << " is NOT a Prime Number !\n";
    }
    
    cout << "\nTime elapsed : " << timer.elapsed() << " ms    or    " << timer.elapsed() / 1000.0 << " s.\n";

    return 0;
}

int main()
{
    cout << "Do you want :\n\n1.  Find all prime numbers in a range.\n\n2.  Determine whether a number is a prime number, and factorize it if not.\n\n";
    int mode;
    cin >> mode;
    if (mode == 1)
        findAllPrimeNumbers();
    else if (mode == 2)
        determineAndFactorize();
}