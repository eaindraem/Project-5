//
//  main.cpp
//  Project5
//
//  Created by Melody Myae on 11/12/23.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <sstream>
#include <streambuf>
#include <cassert>
using namespace std;


int render(int lineLength, istream& inf, ostream& outf);
bool getToken(int lineLength, istream& infile, ostream& outfile, char& c, char token[], int& tokenLength, bool& hyphen, bool& printNewLineInTheEnd);
bool specialChar(const char *token, int& tokenLength);


void testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue)
{
    istringstream iss(input);
    ostringstream oss;
    ostringstream dummy;
    streambuf* origCout = cout.rdbuf(dummy.rdbuf());
    int retval = render(lineLength, iss, oss);
    cout.rdbuf(origCout);
    if ( ! dummy.str().empty())
        cerr << "WROTE TO COUT INSTEAD OF THIRD PARAMETER FOR: " << input << endl;
    else if (retval != expectedReturnValue)
        cerr << "WRONG RETURN VALUE FOR: " << input << endl;
    else if (retval == 2)
    {
        if ( ! oss.str().empty())
            cerr << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
    }
    else if (oss.str() != expectedOutput)
        cerr << "WRONG RESULT FOR: " << input << endl;
}

int main()
{
    //"proj5TestInput.txt"
    //"/Users/melodymyae/Library/Mobile Documents/com~apple~TextEdit/Documents/proj5TestInput.txt"
    ifstream infile("/Users/melodymyae/Library/Mobile Documents/com~apple~TextEdit/Documents/proj5TestInput.txt");
    if ( ! infile )
    {
        cerr << "Error: Cannot open data.txt!" << endl;
    }
    
    //"Proj5TestOutput.txt"
    //"/Users/melodymyae/Library/Mobile Documents/com~apple~TextEdit/Documents/Proj5TestOutput.txt"
    ofstream outfile( "/Users/melodymyae/Library/Mobile Documents/com~apple~TextEdit/Documents/Proj5TestOutput.txt");
    if ( ! outfile )           // Did the creation fail?
    {
        cerr << "Error: Cannot create results.txt!" << endl;
    }
    
    testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0);
    testRender(8, "  This is a test  \n", "This is\na test\n", 0);
    testRender(6, "Testing it\n", "Testin\ng it\n", 1);
    testRender(-5, "irrelevant", "irrelevant", 2);
    cerr << "Tests complete" << endl;
//    int returnValue = render(300, infile, outfile);
//    cerr << "return value: " << returnValue << endl;
}




int render(int lineLength, istream& infile, ostream& outfile)
{
    const int MAXWORDLENGTH = 10000;
    char c;
    char token[MAXWORDLENGTH];
    int tokenLength = 0; //to check if whole word is printtable
    int outputLineIndex = 0;
    int ConsecutiveParaBreak = 0;
    bool paragraphBreak = false;
    bool firstWord = true; //if firstword is true, don't print paragraph break
    int spaceCount = 0; //this amount of space should be printed out before the word.
    bool hyphen = 0;
    bool printNewLineInTheEnd = false;
    bool wordLongerThanMaxLength = false;

    
    while (infile)
    {
        bool wordOrNot = getToken(lineLength, infile, outfile, c, token, tokenLength, hyphen, printNewLineInTheEnd);
        cerr << "outputlineindex: " << outputLineIndex<< endl;
        cerr << "spacecount: " << spaceCount << endl;
        cerr << "wordornot(1 for true) : " << wordOrNot << endl;
        
        if (lineLength < 1)
        {
            return 2;
        }
        if (wordOrNot == true)
        {
            //if @P@ --> paragraph break
            if (strcmp(token, "@P@") == 0 && ConsecutiveParaBreak == 0)
            {
                paragraphBreak = true;
                token[0] = '\0';
                ConsecutiveParaBreak++;
                tokenLength = 0;
                outputLineIndex = 0;
                cerr << "place 1" << endl << endl;
                continue;
            }
            //if consecutive paragraph break, clear the token, and don't print new line
            else if (strcmp(token, "@P@") == 0 && ConsecutiveParaBreak != 0)
            {
                ConsecutiveParaBreak++;
                tokenLength = 0;
                outputLineIndex = 0;
                cerr << "place 2" << endl << endl;
                
                continue;
            }
            
            
            //EXTRA LONG WORD SITUTATION
            //if token bigger than line length, cut it up to fit as many as possible in one line
            if (tokenLength > lineLength)
            {
                wordLongerThanMaxLength = true;
                if (specialChar(token, tokenLength) == true)
                {
                    spaceCount++;
                }
                
                cerr << "place 8" << endl;
                int Nloop = tokenLength/ lineLength; //determine how many lines needed to print the word
                if (tokenLength % lineLength != 0)
                {
                    Nloop++;
                }
                
                cerr << endl << endl;
                cerr << "outputLineIndex: " << outputLineIndex << endl;
                cerr << "nloop: " << Nloop << endl;
                
                
                int i = 0; //how many lines have been output
                int j = 0; //token index
                
                if (outputLineIndex != 0)
                {
                    outfile << endl;
                    spaceCount = 0;
                    outputLineIndex = 0;
                }
                
                //print the first line of extra long word
                while (outputLineIndex < lineLength && token[j] != '\0')
                {
                    outfile << token[j];
                    j++;
                    outputLineIndex++;
                }
                outputLineIndex=0;
                
                //print new line before printing the next lines
                for (i = 1; i < Nloop ; i++)
                {
                    outfile << endl;
                    outputLineIndex = 0;
                    while (outputLineIndex < lineLength && token[j] != '\0')
                    {
                        outfile << token[j];
                        j++;
                        outputLineIndex++;
                    }
                }
                if (hyphen == true)
                {
                    spaceCount--;
                }
                tokenLength = 0;
                spaceCount++;

            }
            
        
            //print only token if first word of a newline
            else if (tokenLength <= lineLength && (outputLineIndex + tokenLength) <= lineLength && outputLineIndex == 0)
            {
                if (paragraphBreak== true && firstWord == true)//if firstword is true and paragraph break is true, don't print paragraph break
                {
                    paragraphBreak = false;
                    firstWord = false;
                }
                else if (paragraphBreak == true)
                {
                    outfile << endl << endl;
                    paragraphBreak = false;
                    cerr << "Place 7" << endl;
                }
                
                spaceCount = 0;
                outfile << token;
                spaceCount++;
                //if it ends with special punctuation, add another space
                if (specialChar(token, tokenLength) == true)
                {
                    spaceCount++;
                    //outputLineIndex++;
                }
                outputLineIndex += tokenLength;
                tokenLength = 0;
                if (token[0] != '\0')
                {
                    ConsecutiveParaBreak = 0;
                }
                if (hyphen == true)
                {
                    cerr << "came into here" << endl;
                    spaceCount--;
                }
                firstWord = false;
                cerr << "outputlineindex: " << outputLineIndex << endl;
                cerr << "place 3" << endl << endl;
                
            }
            
            //if not the first word of the line, print space before printing token
            else if ( tokenLength <= lineLength && (outputLineIndex + spaceCount + tokenLength) <= lineLength)
            {
                if (paragraphBreak && firstWord)//if firstword is true, don't print paragraph break
                {
                    continue;
                }
                else if (paragraphBreak)
                {
                    outfile << endl << endl;
                    paragraphBreak = false;
                }
                
                for (int i = 0; i < spaceCount; i++)
                {
                    cerr << "spaceCount: " << spaceCount << endl;
                    outfile << " " ;
                    outputLineIndex++;
                }
                spaceCount = 0;
                outfile << token;
                spaceCount++;
                //if it ends with special punctuation, add another space
                if (specialChar(token, tokenLength) == true)
                {
                    spaceCount++;
                }
                outputLineIndex += tokenLength;
                tokenLength = 0;
                if (token[0] != '\0')
                {
                    ConsecutiveParaBreak = 0;
                }
                firstWord = false;
                if (hyphen == true)
                {
                    spaceCount--;
                }
                hyphen = false;
                cerr << "place 4" << endl << endl;
                
                
            }
            
            //print token on new line when the whole word doesn't fit
            else if ((tokenLength <= lineLength || hyphen == true ) && (outputLineIndex + tokenLength+ spaceCount) > lineLength)
            {
                
                if (paragraphBreak && firstWord)//if firstword is true, don't print paragraph break
                {
                    continue;
                }
                else if (paragraphBreak)
                {
                    outfile << endl << endl;
                    paragraphBreak = false;
                }
                
                //if the index isn't already at start, print newline
                if (outputLineIndex != 0)
                {
                    outfile << endl;
                    outputLineIndex = 0;
                    spaceCount = 0;

                }
                outfile << token;
                spaceCount++;
            
                //if it ends with special punctuation, add another space
                if (specialChar(token, tokenLength) == true)
                {
                    spaceCount++;
                }
                outputLineIndex = tokenLength;
                tokenLength = 0;
                if (token[0] != '\0')
                {
                    ConsecutiveParaBreak = 0;
                }
                if (hyphen == true)
                {
                    spaceCount--;
                }
                firstWord = false;
                
                cerr << "place 5" << endl << endl;
                
            }
            
            
        }
        else
        {
            continue;
        }
    }
    
    
    if (printNewLineInTheEnd == true) //true if at least one word was printed in the program
    {
        outfile << endl;
    }
        
    if (wordLongerThanMaxLength == true)
        {
            return 1;
        }
    return 0;
}

    
    
    //if the token ends with a special character, return true
    //separated by two blanks: period, question mark, !, or colon
bool specialChar(const char *token, int& tokenLength)
{
    
    char lastChar = token[tokenLength-1];
    switch (lastChar)
    {
        case '.':
        case '?':
        case '!':
        case ':':
            // Last character is one of the specified punctuation characters
            return true;
        default:
            // Last character is not a specified punctuation character
            return false;
    }
}
    
    
    
    
bool getToken(int lineLength, istream& infile, ostream& outfile, char& c, char token[], int& tokenLength, bool& hyphen, bool& printNewLineInTheEnd)
{
    int tokenIndex = 0;
    
    //if end of file, return false
    if (!infile.get(c))
    {
        return false;
    }
    
    // Skip leading whitespace
    if (isspace(c))
    {
        return false;
    }
    
    //if word, copy the word into token, print, return true
    do
    {
        token[tokenIndex] = c; //copy character into token
        tokenLength++;
        tokenIndex++;
        printNewLineInTheEnd = true;
    }
    while (infile.get(c) && (!isspace(c) && c != '-'));
    
    if (c == '-')
    {
        hyphen = true;
        token[tokenIndex] = '-';
        token[tokenIndex+1] = '\0';
        tokenLength++;
        tokenIndex = 0;
    }
    else {
        // Null-terminate the token string
        token[tokenIndex] = '\0';
        tokenIndex = 0;
    }
    
    cerr << endl << endl<< "this is from token function " << endl;
    cerr << "Token: " << token << endl;
    cerr << "TokenLength: " << tokenLength << endl;
    cerr << "hyphen(1 for true) :" << hyphen << endl;
    cerr << "hyphen amount: " << hyphen << endl << endl;
    
    return true;
}


