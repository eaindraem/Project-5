# Project-5
This is a program that renders text from a plain text file into neatly arranged paragraphs with a particular maximum allowed line length.


---------------------------------------------------------------------------------------

Fall 2023 CS 31
Programming Assignment 5
Text Renderer
Part 1 due: 11:00 PM Monday, November 13
Part 2 due: 11:00 PM Monday, November 20

Part 1
Go through the following sections of the class zyBook, doing the Participation Activities and Challenge Activities. We will be looking at whether you have ever successfully completed them; it does not matter how many attempts you make before a successful completion (or how many attempts you make after a successful completion if you want to experiment).

6.6 through 6.9
7.2 and 7.3 (Part 2 does not depend on these)
8.1 and 8.2 (Part 2 does not depend on these)
You should complete this part of the assignment well before the stated Part 1 due date, to give you ample time for Part 2.

Part 2
Phase 1: Learn how to read text from a file
Read the File I/O writeup to learn how to work with files. Create a plain text file with a name of your choosing and put a few lines of text in it. Try running the last little program in the File I/O writeup (the program with the countLines function), replacing the file name data.txt with the name of your file.

In every past quarter that we've had projects that read from files, some students have trouble getting their program to successfully open a file. This is usually because they did not specify a complete path name to the file and did not understand what the File I/O writeup said about where the program would look for the file. Their problem was usually solved by their specifying the complete pathname to the file (such as C:/CS31/P5/myfile.txt on Windows or /Users/yourUsername/Desktop/myfile.txt on a Mac).

You will not be turning in anything for this phase of the project, but you should complete it as soon as possible to get past this potential stumbling block.

Phase 2: Write a text renderer function
Before you ask questions regarding this specification, see if your question has already been addressed by the Project 5 FAQ. And read the FAQ before you turn in this project, to be sure you didn't misinterpret anything. Use the Project 5 Renderer to test your understanding of the rendering rules.

When a browser renders text onto a screen or in a window of a certain width, or when a program like Microsoft Word formats text between margins of a certain width, it tries to fit as many words in each line as it can. For this project, you will implement a function that renders text from a plain text file into neatly arranged paragraphs with a particular maximum allowed line length. For example, if the desired maximum line length is 40 characters and the input text is

It always does seem to me that I am doing more work than
I should do. It is not that I object to the work, mind you;
I like work: it fascinates me.       I can sit and look at it for hours.
I love to keep     it by me: the idea of getting
rid
of it nearly breaks my heart. @P@ You cannot give me too
much work; to accumulate work has almost become


a passion with me: my study is so full of it now, that there is hardly
an inch of room for any more.
then the output text will be

It always does seem to me that I am
doing more work than I should do.  It is
not that I object to the work, mind you;
I like work:  it fascinates me.  I can
sit and look at it for hours.  I love to
keep it by me:  the idea of getting rid
of it nearly breaks my heart.

You cannot give me too much work; to
accumulate work has almost become a
passion with me:  my study is so full of
it now, that there is hardly an inch of
room for any more.
from Jerome K. Jerome's Three Men in a Boat (To Say Nothing of the Dog).

You are to implement the following function, whose job it is to do the rendering:

	int render(int lineLength, istream& inf, ostream& outf);
You may name the parameters something else, but there must be three parameters of the indicated types in the indicated order. (The File I/O tutorial explains istream and ostream.) The parameters are

an int with the desired maximum line length
an already-opened input source you will read from (probably a file the caller opened).
an already-opened output destination you will write to (probably either cout or an output file the caller created)
The function must return an int with one of the following values:

0 if is successful (i.e., neither of the following problems occurs)
1 if any input word portion (defined below) is longer than the maximum line length
2 if the desired maximum line length is less than 1
In the case of the error situation that would cause the function to return 2, it must return 2 without writing any output.

Your render function may, of course, call other functions you write to help it do its job. You can test your render function by calling it from a main routine of your choosing. Our grading tool will ignore your main routine by renaming it to something harmless. If you wish, your main routine could prompt the user for the file names and the line length. Alternatively, it could hardcode the names of the files to use and prompt for only a line length. Do whatever works best for you to help you test your function. Here is an example:

    int main()
    {
        const int MAX_FILENAME_LENGTH = 100;
        for (;;)
        {
            cout << "Enter input file name (or q to quit): ";
            char filename[MAX_FILENAME_LENGTH];
            cin.getline(filename, MAX_FILENAME_LENGTH);
            if (strcmp(filename, "q") == 0)
                break;
            ifstream infile(filename);
            if (!infile)
            {
                cerr << "Cannot open " << filename << "!" << endl;
                continue;
            }
            cout << "Enter maximum line length: ";
            int len;
            cin >> len;
            cin.ignore(10000, '\n');
            int returnCode = render(len, infile, cout);
            cout << "Return code is " << returnCode << endl;
        }
    }
We use the following definitions in the rendering rules:

A word is a sequence of non-whitespace characters not immediately preceded or followed by a non-whitespace character. (The <cctype> function isspace tells you if a character is a whitespace character.) The string "swan's nest" contains two words, swan's and nest. Because of immediately preceding or following non-whitespace, swan, wa, 's, and nes are not words of that string.
A word can be viewed as a sequence of one or more word portions. The first word portion in a word starts at the start of the word; subsequent word portions in a word start just after the last character of the previous word portion. The last character of a word portion is the first hyphen at or after the start of that word portion, or the end of the word if there is no hyphen after the start of that word portion. Here are examples, including some pathological ones:
	Word                Word portions
	Thames              Thames
	so-called           so- and called
	Henley-on-Thames    Henley-, on-, and Thames
	so--called          so-, -, and called
	so-                 so-
	-so                 -,  and  so
Here are the rendering rules:

Fit as many word portions in an output line as you can without exceeding the line length. Output lines may well end up being different lengths, giving a "ragged-right" effect; your renderer will not try to right-justify the text.

Words in an output line must normally be separated by one blank. However, if the last character of an output word is a period, question mark, exclamation point, or colon, it must be separated from any following word on that output line by two blanks. The last word on an output line must not be followed by any blanks. The first word portion on an output line must not be preceded by any blanks. Blanks must not appear on an output line within any word. The last line of output must end with a newline, and there must be no output lines (not even empty ones) after the last word of the last paragraph. For example, if the last word in the input is bye, then the last four characters your function would write are 'b'  'y'  'e'  '\n'. That does not produce an empty last line. Writing this would produce an empty last line, in violation of this rule: 'b'  'y'  'e'  '\n'  '\n'.

Notice that this rule implies that if a word containing a hyphen is broken at a hyphen to fit on an output line, the hyphen must be the last character of one output line, and the character after the hyphen must be the first character of the next output line.

If a word portion is longer than the line length, as much as will fit must be on an output line by itself. The rest of that word portion must begin the next output line (and, of course, is subject to similar splitting if it's too long). If this situation ever occurs, your function continues rendering, but it must eventually return 1 instead of 0 to its caller. Notice that this is the only situation where a word is allowed to be split across lines other than at a hyphen.

The input word @P@ is not to be processed as a word according to the above rules; instead, it indicates a paragraph break. The first word in the input following a paragraph break will be the first word of a new paragraph in the output. If a paragraph has already been output, the new paragraph must be separated from the one that precedes it by an empty line (i.e., a line with no characters other than the terminating newline). The very first output paragraph must not be preceded by an empty line. The very last output paragraph must not be followed by an empty line. Two or more consecutive @P@ words in the input are treated as just one paragraph break. Notice that banjo@P@ is one eight-character word; it does not cause a paragraph break, because in that string, @P@ is not a word because of the immediately preceding non-whitespace character o.

Notice that these rules imply that if the line length is valid but the input contains no words, render must return 0 without writing any output whatsoever.

Your render function and any functions you write that it calls directly or indirectly must not use any std::string objects. If you need to use a string, use a C string. Your function may assume that no input line will be 180 or more characters long (i.e., we will not test it with input lines that long).

This project is doable without assuming any upper limit for the output line length (the first parameter of render) — rather than storing the parts of an output line in a C string to be written later, you instead write them as soon as you can. However, some people may not figure out how to do that, so we'll give you a choice. If the first parameter of render is greater than 250, your implementation of render must either:

return 2 without writing any output; or
render the text using the indicated line length, returning 0 or 1 as appropriate (i.e., your algorithm doesn't impose any upper limit on the output line length). Implementing this choice correctly (instead of returning 2 and producing no output) is worth 5 bonus points.
(Although the program you turn in must not use any C++ strings, only C strings, you might want to consider this development strategy: Ignore this restriction at first, and develop a working solution that uses C++ strings. After you've ironed out the issues in writing the renderer, save a backup, and then convert your using C++ strings to using C strings instead. This approach helps you avoid confusing the mistakes in your use of C strings with the mistakes in your rendering algorithm, so might make debugging easier.)

Your render function and any functions you write that it calls directly or indirectly must not read from any source other than the istream passed as the second parameter and must not write to any destination other than the ostream passed as the third parameter, except that you may write anything you want to cerr. Our grading tool will discard anything written to cerr, so feel free to use it for debugging purposes.

Your program must not use any global variables whose values may be changed during execution. Global constants are OK.

The program you turn in must build successfully under both g31 and either Visual C++ or clang++, and its correctness must not depend on undefined program behavior. Your program could not, for example, assume anything about s's value in the following, or even whether or not the program crashes:

	void f()
	{
	    char s[6];
	    strcpy(s, "Thames");  // s is too short for 6+1 chars
	    …
What to turn in
You won't turn anything in through the CS 31 web site for Part 1; the zyBook system notes your successful completion of the PAs and CAs. For Part 2, turn in a zip file containing these two files and nothing more:

A text file named render.cpp that contains the source code for your C++ program. Your source code should have helpful comments that tell the purpose of the major segments of your function and explain any non-obvious code.

Try to ensure that your program does something that meets the spec for at least a few test cases. That way, you can get some partial credit for a program that does not meet the entire specification.

A file named report.docx (in Microsoft Word format), or report.txt (an ordinary text file) that contains:
A brief description of notable obstacles you overcame.
A description of the design of your project. Use pseudocode where it clarifies the presentation. Someone reading your description should be able to determine what the responsibilities of the functions you wrote are. If someone had to modify your code later, could they from your description readily find in your program the approximate location of the code that, for example, handles a word portion not fitting on a line?
A list of the test data that could be used to thoroughly test your function, along with the reason for each test case (e.g. "word broken at hyphen" or "two spaces after period"). You must note which test cases your program does not handle correctly. (This could happen if you didn't have time to write a complete solution, or if you ran out of time while still debugging a supposedly complete solution.)
By November 19, there will be links on the class webpage that will enable you to turn in your zip file electronically. Turn in the file by the due time above.

Notes for Visual C++ users
Microsoft made a controversial decision to issue by default an error or warning when your code uses certain functions from the standard C and C++ libraries (e.g., strcpy). These warnings call those functions unsafe and recommend using different functions in their place; those other functions, though, are not Standard C++ functions, so will cause a compilation failure when you try to build your program under clang++ g++. Therefore, for this class, we want to use functions like strcpy without getting those warnings from Visual C++; to eliminate them, put the following line in your program before any of your #includes:

	#define _CRT_SECURE_NO_DEPRECATE
It is OK and harmless to leave that line in when you build your program using g31 or clang++ and when you turn it in.

If you declare a large array in a function, Visual C++ gives a harmless warning C6262: Function uses 'NNNNN' bytes of stack: exceeds /analyze:stacksize '16384'. Consider moving some data to the heap., where NNNNN is some number. You can eliminate that warning by putting the following line in your program above the #define _CRT_SECURE_NO_DEPRECATE:

        #pragma warning(disable:6262)
It is OK and harmless to leave that line in when you build your program using g31 or clang++ and when you turn it in, even if you get a warning about the pragma being ignored.

Alternatively, in Visual Studio, select Project / yourProjectName properties, then select Configuration Properties / Code Analysis / General, and then in Code Analysis's stacksize, modify 16384 to, say, 100000.

If your program dies under Visual C++ with a dialog box appearing saying "Debug Assertion Failed! ... File: ...\src\isctype.c ... expression: (unsigned)(c+1)<=256", then you called one of the functions defined by <cctype>, such as isspace, with a character whose encoding is outside the range of 0 through 127. Since all the normal characters you would use (space, letters, punctuation, '\0', etc.) fall in that range, you're probably passing an uninitialized character to the function. Perhaps you're examining a character past the '\0' marking the end of a C string, or perhaps you built what you thought was a C string but forgot to end it with a '\0'.
