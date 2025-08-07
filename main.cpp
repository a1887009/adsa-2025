#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/* 
 * Reverses the string and puts it to a vector 
 * for better use, ie. vector[0] is now lowest digit
 */
std::vector<int> parseVector(const std::string& numStr)
{
    std::vector<int> digits;
    for (int i = numStr.length() - 1; i >=0; i--)
    {
        digits.push_back(numStr[i] - '0');
    }
    return digits;
}

std::string parseString(const std::vector<int>& numVec)
{
    std::string digits;
    for (int i = numVec.size() - 1; i >=0; i--)
    {
        digits += (numVec[i] + '0');
    }
    return digits;
}

// i1, i2 are integers and b is the base
std::string add(const std::string& i1, const std::string& i2, int base)
{
    std::vector<int> v1 = parseVector(i1);
    std::vector<int> v2 = parseVector(i2);
    std::vector<int> result;
    int n1 = v1.size();
    int n2 = v2.size();

    int carry{0};
    for (int i = 0; i < std::max(n1, n2); i++)
    {
        int d1 = (i < n1) ? v1[i] : 0;
        int d2 = (i < n2) ? v2[i] : 0;

        int temp = d1 + d2 + carry;
        result.push_back(temp % base);
        carry = temp / base;
    }
    if (carry > 0) {result.push_back(carry);}

    return parseString(result);
}


int main()
{
    std::string num1;
    std::string num2;
    int base;
    std::cin >> num1 >> num2 >> base;
    std::cout << add(num1, num2, base) << '\n';
    return 0;
}