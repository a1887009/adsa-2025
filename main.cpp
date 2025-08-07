#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

typedef unsigned int digit;
typedef std::vector<digit> integer;

unsigned int BASE{10};

/* 
 * Reverses the string and puts it to a vector 
 * for better use, ie. vector[0] is now lowest digit
 */
integer parseVector(const std::string& num) 
{
    integer result;
    for (int i = num.size() - 1; i >= 0; --i) 
    {
        result.push_back(num[i] - '0');
    }
    return result;
}

/* 
 * Reverses the vector and puts it back to a string 
 * for delivery back to the user after functions have run
 */
std::string toString(const integer& num)
{
    std::string result;
    for (int i = num.size() - 1; i >=0; i--)
    {
        result += (num[i] + '0');
    }
    return result;
}

// Cleans leading zeros
void clean(integer& num) {
    while (num.size() > 1 && num.back() == 0)
        num.pop_back();
}

// Adds 0's to the smaller vector to compensate 
void padVectors(integer& v1, integer& v2) 
{
    while (v1.size() < v2.size()) v1.push_back(0);
    while (v2.size() < v1.size()) v2.push_back(0);
}

// Adds two integers
integer add(const integer& a, const integer& b) {
    integer result;
    int carry{0};
    size_t n = std::max(a.size(), b.size());
    for (size_t i = 0; i < n || carry; ++i) {
        int digitSum = carry;
        if (i < a.size()) digitSum += a[i];
        if (i < b.size()) digitSum += b[i];
        result.push_back(digitSum % BASE);
        carry = digitSum / BASE;
    }
    return result;
}

// Subtracts b from a (assuming a >= b)
integer sub(const integer& a, const integer& b) {
    integer result;
    int borrow{0};
    for (size_t i = 0; i < a.size(); ++i) {
        int diff = a[i] - borrow - (i < b.size() ? b[i] : 0);
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(diff);
    }
    clean(result);
    return result;
}

// Shifts a number left by n digits (multiply by BASE^n)
integer shift(const integer& num, size_t n) {
    integer result(n, 0);
    result.insert(result.end(), num.begin(), num.end());
    return result;
}

// Karatsuba multiplication
integer karatsuba(integer x, integer y) {
    clean(x);
    clean(y);
    padVectors(x, y);
    size_t n = x.size();

    if (n == 0) return {0};
    if (n == 1) return { (digit)(x[0] * y[0]) % BASE, (digit)(x[0] * y[0]) / BASE };

    size_t half = n / 2;

    // Splitting the vectors
    integer x_low(x.begin(), x.begin() + std::min(half, x.size()));
    integer x_high(x.begin() + std::min(half, x.size()), x.end());
    integer y_low(y.begin(), y.begin() + std::min(half, y.size()));
    integer y_high(y.begin() + std::min(half, y.size()), y.end());

    // Calculating karatsuba recursively
    integer p0 = karatsuba(x_low, y_low);
    integer p2 = karatsuba(x_high, y_high);
    integer p1 = karatsuba(add(x_low, x_high), add(y_low, y_high));
    p1 = sub(sub(p1, p0), p2);

    integer result = add(add(shift(p2, 2 * half), shift(p1, half)), p0);
    clean(result);
    return result;
}

int main() {
    std::string s1, s2;
    std::cin >> s1 >> s2 >> BASE;
    integer a = parseVector(s1);
    integer b = parseVector(s2);

    std::cout << toString(add(a, b)) << " " << toString(karatsuba(a, b)) << std::endl;
    return 0;
}
