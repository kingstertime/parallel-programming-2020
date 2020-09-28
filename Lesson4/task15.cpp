//
// Created by Кирилл Королёв on 28.09.2020.
//
#include <stdio.h>
#include <omp.h>
#include <cmath>
#include <iostream>
using namespace std;

bool isPrime(int num) {
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

int main()
{
    int numOne;
    int numTwo;
    cout << "Enter two numbers:"<<"\n";
    cin >> numOne >> numTwo;

    if (numOne < 2)
    {
        numOne = 2;
    }
    omp_set_num_threads(4);
    if (numOne <= numTwo) {
#pragma omp parallel for ordered schedule(dynamic)
        for (int i = numOne; i <= numTwo; i++) {
            if (isPrime(i))
            {
#pragma omp ordered
                cout << i << "\n";
            }
        }
    }
}


