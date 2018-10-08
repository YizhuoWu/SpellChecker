// WordChecker.cpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include <iostream>
#include "WordChecker.hpp"
#include <vector>
using namespace std;


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return this->words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    vector<string> suggestions;
    string all_letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    ///----------------------------------Swapping each adjacent pair of characters in the word-------------------
    for (int i = 0; i < word.size()-1; i++)
    {
        string result_1 = word;
        swap(result_1[i],result_1[i+1]);
        if (wordExists(result_1)==true && find(suggestions.begin(),suggestions.end(), result_1) == suggestions.end())
        {
            suggestions.push_back(result_1);
        }
    }
    ///-----------------------------------Each letter from 'A' through 'Z' is inserted---------------------------
    for(int m = 0; m <= word.size(); m++)
    {
        for (int n = 0; n < all_letter.size(); n++)
        {
            string result_2 = word;
            result_2.insert(m,all_letter.substr(n,1));
            if (wordExists(result_2)==true && find(suggestions.begin(),suggestions.end(), result_2) == suggestions.end())
            {
                suggestions.push_back(result_2);
            }
        }
    }
    ///-----------------------------------Deleting each character from the word----------------------------------
    for(int p = 0; p < word.size(); p++)
    {
        string result_3 = word;
        result_3 = result_3.erase(p,1);
        if (wordExists(result_3)==true && find(suggestions.begin(),suggestions.end(), result_3) == suggestions.end())
        {
            suggestions.push_back(result_3);
        }

    }
    ///-----------------------------------Replacing each word with 'A' through 'Z'-------------------------------
    for(int i = 0; i < word.size(); i++)
    {
        for(int x = 0; x < all_letter.size();x++)
        {
            string result_4 = word;
            result_4[i] = all_letter[x];
            if (wordExists(result_4)==true && find(suggestions.begin(),suggestions.end(), result_4) == suggestions.end())
            {
                suggestions.push_back(result_4);
            }

        }
    }
    ///-----------------------------------Splitting the word into a pair adding space----------------------------
    string temp1;
    string temp2;
    for(int m = 0;m<word.size()-1;m++)
    {
        temp1 = word.substr(0,m);
        temp2 = word.substr(m);
        string result_5;
        result_5 = temp1 + " " + temp2;
        if (wordExists(temp1)==true&&wordExists(temp2)==true && find(suggestions.begin(),suggestions.end(), result_5) == suggestions.end())
        {
            suggestions.push_back(result_5);
        }

    }


    return suggestions;
}

