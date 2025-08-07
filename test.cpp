#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

typedef unsigned int digit;
typedef std::vector<digit> integer;

unsigned int BASE{10};

// Converts string to reversed digit vector (LSB first)
integer parseVector(const std::string& num) {
    integer result;
    for (int i = num.size() - 1; i >= 0; --i) {
        result.push_back(num[i] - '0');
    }
    return result;
}

// Converts reversed digit vector to string
std::string toString(const integer& num) {
    std::string result;
    for (int i = num.size() - 1; i >= 0; --i) {
        result += (num[i] + '0');
    }
    return result;
}

// Removes leading zeros
void clean(integer& num) {
    while (num.size() > 1 && num.back() == 0)
        num.pop_back();
}

// Adds two reversed-digit integers
integer add(const integer& a, const integer& b) {
    integer result;
    digit carry = 0;
    size_t n = std::max(a.size(), b.size());
    for (size_t i = 0; i < n || carry; ++i) {
        digit sum = carry;
        if (i < a.size()) sum += a[i];
        if (i < b.size()) sum += b[i];
        result.push_back(sum % BASE);
        carry = sum / BASE;
    }
    return result;
}

// Subtracts b from a (assuming a >= b)
integer sub(const integer& a, const integer& b) {
    integer result;
    digit borrow = 0;
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

// Pads two vectors to equal size
void padVectors(integer& v1, integer& v2) {
    while (v1.size() < v2.size()) v1.push_back(0);
    while (v2.size() < v1.size()) v2.push_back(0);
}

// Multiplies by BASE^n (shifts digits left)
integer shift(const integer& num, size_t n) {
    integer result(n, 0);
    result.insert(result.end(), num.begin(), num.end());
    return result;
}

// Karatsuba multiplication (reversed digits)
integer karatsuba(integer x, integer y) {
    clean(x);
    clean(y);
    padVectors(x, y);

    size_t n = x.size();
    if (n == 0) return {0};
    if (n == 1) {
        digit prod = x[0] * y[0];
        return {prod % BASE, prod / BASE};
    }

    size_t half = n / 2;

    integer x_low(x.begin(), x.begin() + half);
    integer x_high(x.begin() + half, x.end());
    integer y_low(y.begin(), y.begin() + half);
    integer y_high(y.begin() + half, y.end());

    integer z0 = karatsuba(x_low, y_low);
    integer z2 = karatsuba(x_high, y_high);
    integer z1 = karatsuba(add(x_low, x_high), add(y_low, y_high));
    z1 = sub(sub(z1, z0), z2);

    integer result = add(add(shift(z2, 2 * half), shift(z1, half)), z0);
    clean(result);
    return result;
}

int main() {
    std::string s1, s2;
    std::cin >> s1 >> s2 >> BASE;

    integer a = parseVector(s1);
    integer b = parseVector(s2);

    integer sum = add(a, b);
    integer product = karatsuba(a, b);

    std::cout << toString(sum) << " " << toString(product) << std::endl;
    return 0;
}
