#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

typedef unsigned int digit;
typedef std::vector<digit> integer;
unsigned int BASE{10};

/* 
 * Reverses the string and puts it to a vector 
 * for better use, ie. vector[0] is now lowest digit
 */
integer parseVector(const std::string& numStr)
{
    integer digits;
    for (int i = numStr.length() - 1; i >=0; i--)
    {
        digits.push_back(numStr[i] - '0');
    }
    return digits;
}

/* 
 * Reverses the vector and puts it back to a string 
 * for delivery back to the user after functions have run
 */
std::string parseString(const integer& numVec)
{
    std::string digits;
    for (int i = numVec.size() - 1; i >=0; i--)
    {
        digits += (numVec[i] + '0');
    }
    return digits;
}

// Adds 0's to the smaller vector to compensate 
void padVectors(integer& v1, integer& v2) 
{
    while (v1.size() < v2.size()) v1.push_back(0);
    while (v2.size() < v1.size()) v2.push_back(0);
}

integer addVectors(integer v1, integer v2) 
{
    // Apply padding to ensure they have same size
    padVectors(v1, v2);
    integer result;
    digit carry = 0;

    // Main addition loop
    int n = v1.size();
    for (int i = 0; i < n; i++)
    {
        digit sum = v1[i] + v2[i] + carry;
        result.push_back(sum % BASE);
        carry = sum / BASE;
    }

    // Add leftover carry
    if (carry) result.push_back(carry);
    return result;
}

// i1, i2 are integers and b is the base
std::string add(const std::string& i1, const std::string& i2)
{
    // Initialise the vectors ready for computation
    integer v1 = parseVector(i1);
    integer v2 = parseVector(i2);

    // Pass the addition of the two vectors to the result
    integer result = addVectors(v1, v2);
    return parseString(result);
}



int main()
{
    std::string num1;
    std::string num2;
    std::cin >> num1 >> num2 >> BASE;
    std::cout << add(num1, num2) << '\n';
    return 0;
}