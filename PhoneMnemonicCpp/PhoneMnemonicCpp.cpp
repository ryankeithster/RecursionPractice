// PhoneMnemonicCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <exception>
#include <array>
#include <chrono>

const std::unordered_map<int, std::string> keyVals = { {0, ""}, {1, ""}, {2, "abc"}, {3, "def"}, {4, "ghi"}, {5, "jkl"}, {6, "mno"}, {7, "pqrs"}, {8, "tuv"}, {9, "wxyz"}};
const std::array<std::string, 10> keyValsArr = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

/// <summary>
/// Helper function that recursively constructs all possible sequences of words that correspond to the given phone number.
/// This is the more efficient version that avoids unnecessary string copying and manipulation.
/// </summary>
/// <param name="phoneNumber"></param>
/// <param name="resultStr"></param>
/// <param name="digitCount"></param>
/// <param name="newCharacterIndex"></param>
/// <param name="results"></param>
void phoneMnemonicHelper(std::string& phoneNumber, std::string& resultStr, int digitCount, int newCharacterIndex, std::vector<std::string>& results)
{
    // If we've finished a word, add it to the list and return
    if (digitCount == 7)
    {
        results.emplace_back(resultStr);
        return;
    }

    char nextNumber = phoneNumber[digitCount];
    std::string possibleLetters = keyValsArr[static_cast<int>(nextNumber) - 48];
    ++digitCount;

    for (char c : possibleLetters)
    {
        resultStr[newCharacterIndex] = c;
        phoneMnemonicHelper(phoneNumber, resultStr, digitCount, newCharacterIndex + 1, results);
    }

    // Both 0 and 1 on the keypad have no letters associated with them
    if (possibleLetters.length() == 0)
    {
        // Since the current digit has no letters associated with it, we know our
        // resulting string will be one character smaller than the original allocation
        resultStr.resize(resultStr.length() - 1);
        
        phoneMnemonicHelper(phoneNumber, resultStr, digitCount, newCharacterIndex, results);
    }
}

/// <summary>
/// Helper function that recursively constructs all possible sequences of words that correspond to the given phone number.
/// This version is less efficient than the version above it.
/// </summary>
/// <param name="phoneNumber"></param>
/// <param name="phoneNumberSoFar"></param>
/// <param name="strSoFar"></param>
/// <param name="results"></param>
void phoneMnemonicHelperLessEfficient(std::string& phoneNumber, std::string phoneNumberSoFar, std::string& strSoFar, std::vector<std::string>& results)
{
    // If we've finished a word, add it to the list and return
    if (phoneNumber == phoneNumberSoFar)
    {
        results.emplace_back(strSoFar);
        return;
    }

    char nextNumber = phoneNumber[phoneNumberSoFar.length()];
    std::string possibleLetters = keyVals.at(static_cast<int>(nextNumber) - 48);
    phoneNumberSoFar += nextNumber;

    if (possibleLetters.length() > 0)
    {
        for (char c : possibleLetters)
        {
            strSoFar += c;
            phoneMnemonicHelperLessEfficient(phoneNumber, phoneNumberSoFar, strSoFar, results);
            strSoFar = strSoFar.erase(strSoFar.length() - 1);
        }
    }
    else // Special case for numbers 0 and 1, which have no letters associated with it on the keypad
    {
        phoneMnemonicHelperLessEfficient(phoneNumber, phoneNumberSoFar, strSoFar, results);
    }
}

/// <summary>
/// Constructs all possible sequences of words that correspond to the given phone number. Words
/// will be up to 7 characters long and will not be limited to dictionary words.
/// </summary>
/// <param name="phoneNumber"></param>
/// <param name="useEfficientVersion"></param>
/// <returns></returns>
std::vector<std::string> phoneMnemonic(std::string& phoneNumber, bool useEfficientVersion)
{
    if (phoneNumber.length() != 7)
    {
        throw "Not a valid phone number";
    }

    std::vector<std::string> results;

    if (useEfficientVersion)
    {
        int digitCount = 0;
        int newCharacterIndex = 0;
        std::string resultStr(7, 'a'); // doesn't matter what character we fill with as it will be overwritten
        phoneMnemonicHelper(phoneNumber, resultStr, digitCount, newCharacterIndex, results);
    }
    else
    {
        std::string phoneNumberSoFar = "";
        std::string strSoFar = "";

        phoneMnemonicHelperLessEfficient(phoneNumber, phoneNumberSoFar, strSoFar, results);
    }

    return results;
}

int main()
{
    std::string phoneNumber = "2276696";
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::string> words = phoneMnemonic(phoneNumber, false);
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::string> words2 = phoneMnemonic(phoneNumber, true);
    
    stop = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Less efficient execution time (us): " << duration1.count() << std::endl;
    std::cout << "More efficient execution time (us): " << duration2.count() << std::endl;

    std::string phoneNumber2 = "1234567";
    std::vector<std::string> words3 = phoneMnemonic(phoneNumber2, false);
    std::vector<std::string> words4 = phoneMnemonic(phoneNumber2, true);
}


