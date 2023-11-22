// This is the standard main file for the repl and command line parsing.
// DO NOT CHANGE THIS FILE without talking with me first.
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "include/cxxopts.hpp"

#include "helpers.hpp"
#include "linkedlist.hpp"
#include "linkedlisttest.hpp"

using namespace std;

bool quit(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine);
bool help(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine);
bool test(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine);

/// @brief Processes a line of input.
/// @param line The line to process.
/// @param commandString The allOutput parameter to store the command itself.
/// @param allOutput The allOutput parameter to store the allOutput of the command.
/// @param expectedOutput The allOutput parameter to store the expected allOutput of the command.
/// @param comment The allOutput parameter to store the comment of the command.
/// @param interactive Whether the command is being run in an interactive session.
/// @param currentLine The current line number in the input stream.
/// @return Whether the command was successful.
bool ProcessLineCommand(const string &line, std::string &commandString, std::string &allOutput, std::string &expectedOutput, std::string &comment, bool interactive, int currentLine);

/// @brief Processes a command.
/// @param command The command to process.
/// @param params The parameters to pass to the command.
/// @param outputStream The allOutput stream to write the command's allOutput to.
/// @param interactive Whether the command is being run in an interactive session.
/// @param currentLine The current line number in the input stream.
/// @return Whether the command was successful.
bool ProcessCommand(const string &command, const vector<string> &params, string &allOutput, bool interactive, int currentLine);

// Create an array of pairs, each containing a command name and its associated function
vector<TestFunctionEntry> baseCommands = {
    {"quit", "quit", quit},
    {"help", "help [command] - gives help for the optional command.", help},
    {"test", "test <input file> - tests a file.", test},
    {"?", "? [command] - gives help for the optional command.", help},
};

std::map<std::string, TestFunctionEntry> commandMap;

void ProcessStream(istream *sourceStream, ostream *outputStream, bool interactive)
{
    if (sourceStream == NULL)
    {
        sourceStream = &cin;
    }

    if (outputStream == NULL)
    {
        outputStream = &cout;
    }

    if (interactive)
    {
        *outputStream << "Simple C++ REPL - Enter an arithmetic expression or 'quit' to quit." << endl;
    }

    int currentLine = 0;

    while (true)
    {
        if (interactive)
        {
            *outputStream << "> ";
        }

        string input;
        if (!getline(*sourceStream, input))
        {
            break;
        }

        currentLine++;

        string command;
        string allOutput;
        string expectedOutput;
        string comment;

        input = trim(input);

        bool b = ProcessLineCommand(input, command, allOutput, expectedOutput, comment, interactive, currentLine);
        *outputStream << allOutput << endl;

        if (!b)
        {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    istream *sourceStream = NULL;
    ostream *outputStream = NULL;
    bool interactive = true;

    cxxopts::Options options("REPL", "A simple C++ REPL");

    options.add_options()("e,expression", "Evaluate the provided arithmetic expression", cxxopts::value<string>())("h,help", "Display this help message")("s,source", "Source file to evaluate line by line.", cxxopts::value<string>())("i,interactive", "Source file to evaluate line by line.")("o,allOutput", "Output file to write results to.", cxxopts::value<string>());

    AddCommands(baseCommands, commandMap);
    AddCommands(linkedListTestCommands, commandMap);

    try
    {
        cxxopts::ParseResult result = options.parse(argc, argv);

        if (result.count("help"))
        {
            cout << options.help() << endl;
            return EXIT_SUCCESS;
        }

        // See if we have an allOutput file to write to.
        if (result.count("allOutput"))
        {
            string filename = result["allOutput"].as<string>();
            outputStream = new ofstream(filename);

            if (outputStream == NULL || outputStream->fail())
            {
                cerr << "Error creating allOutput stream \"" << filename << "\"" << endl;
                return EXIT_FAILURE;
            }
        }

        // If interactive is set then ignore the other source options.
        if (result.count("interactive"))
        {
            interactive = true;
            sourceStream = NULL;
        }
        // Next prioritize setting a command line expression.
        else if (result.count("expression"))
        {
            interactive = false;
            string expression = result["expression"].as<string>();

            sourceStream = new stringstream(expression);

            if (sourceStream == NULL || sourceStream->fail())
            {
                cerr << "Error creating source stream from expression." << endl;
                return EXIT_FAILURE;
            }
        }
        // Next prioritize setting a source file.
        else if (result.count("source"))
        {
            interactive = false;
            string filename = result["source"].as<string>();
            sourceStream = new ifstream(filename);

            if (sourceStream == NULL || sourceStream->fail())
            {
                cerr << "Error creating source stream \"" << filename << "\"" << endl;
                return EXIT_FAILURE;
            }
        }
        // Otherwise none were set so make it interactive.
        else
        {
            interactive = true;
            sourceStream = NULL;
        }

        ProcessStream(sourceStream, outputStream, interactive);
    }
    catch (const cxxopts::exceptions::exception &e)
    {
        cerr << "Error parsing options: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    // Cleanup

    if (sourceStream != NULL)
    {
        delete sourceStream;
        sourceStream = NULL;
    }

    if (outputStream != NULL)
    {
        delete outputStream;
        outputStream = NULL;
    }

    return 0;
}

bool help(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine)
{
    allOutput = "";

    if (params.size() == 0)
    {
        cout << "Available commands: ";

        for (const auto &pair : commandMap)
        {
            cout << pair.first << " ";
        }

        cout << endl;

        return true;
    }
    else if (params.size() == 1)
    {
        auto match = commandMap.find(params[0]);

        if (match == commandMap.end())
        {
            PrintError(currentLine, 0, "Unknown command '" + params[0] + "'");
            return true;
        }

        cout << match->second._name << ": " << match->second._help << endl;
    }
    else
    {
        throw invalid_argument("help requires 0 or 1 parameters");
    }

    return true;
}

bool quit(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine)
{
    if (interactive)
    {
        cout << "Goodbye!" << endl;
    }

    allOutput = "";

    return false;
}

bool test(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine)
{
    ifstream testInputFile;

    if (params.size() != 1)
    {
        throw std::invalid_argument("test requires one parameter");
    }

    testInputFile.open(params[0]);

    if (testInputFile.fail())
    {
        PrintError(currentLine, 0, "Could not open input file '" + params[0] + "'");
        allOutput = "";
        return true;
    }

    int fileCurrentLine = 0;
    int errorCount = 0;
    allOutput = "";

    while (true)
    {
        string input;
        if (!getline(testInputFile, input))
        {
            break;
        }

        fileCurrentLine++;

        string command;
        string output;
        string expectedOutput;
        string comment;

        input = trim(input);

        bool b = ProcessLineCommand(input, command, output, expectedOutput, comment, interactive, fileCurrentLine);

        // Compare allOutput and expectedOutput ignoring leading and trailing spaces
        string outputTrim = trim(output);
        string expectedOutputTrim = trim(expectedOutput);

        string matchString = (outputTrim != expectedOutputTrim) ? "differs" : "same";
        {
            stringstream sstr;
            sstr << "Line " << fileCurrentLine << " " << matchString << ": Command: \'" << command << "\'; Result: \'" << outputTrim << "\'; Expected: \'" << expectedOutputTrim << "\'" << endl;
            allOutput += sstr.str();
            cout << sstr.str();

            errorCount++;
        }

        if (!b)
        {
            break;
        }
    }

    if (errorCount == 0)
    {
        allOutput += "\nAll tests passed\n";
    }
    else
    {
        allOutput += "\nFailed test count - " + to_string(errorCount) + "\n";
    }

    return true;
}

bool ProcessLineCommand(const string &line, std::string &commandString, std::string &allOutput, std::string &expectedOutput, std::string &comment, bool interactive, int currentLine)
{
    ParseLine(line, commandString, expectedOutput, comment, ';', '#');

    vector<string> splitLine = SplitString(commandString);
    if (splitLine.empty())
    {
        allOutput = "";
        return true;
    }

    // Remove the command
    string command = trim(splitLine.front());
    splitLine.erase(splitLine.begin());

    return ProcessCommand(command, splitLine, allOutput, interactive, currentLine);
}

// Return true to continue false to exit
bool ProcessCommand(const string &command, const vector<string> &params, string &allOutput, bool interactive, int currentLine)
{
    vector<string> matches = FindPrefixMatch(commandMap, command);

    if (matches.empty())
    {
        ostringstream stringStream;
        stringStream << "Invalid command '" << command << "'"
                     << ". Type '?' or 'help' for a list of commands.";
        PrintError(currentLine, 0, stringStream.str());

        allOutput = "error";
        return true;
    }
    else if (matches.size() == 1 || matches.front() == command)
    {
        try
        {
            return commandMap[matches.front()]._function(params, allOutput, interactive, currentLine);
        }
        catch (const std::invalid_argument &e)
        {
            PrintError(currentLine, 0, "Invalid argument: " + std::string(e.what()));
            allOutput = "error";

            return true;
        }
        catch (const std::out_of_range &e)
        {
            PrintError(currentLine, 0, "Out of range: " + std::string(e.what()));
            allOutput = "error";
            return true;
        }
        catch (const LinkedListException &e)
        {
            PrintError(currentLine, 0, "LinkedList Error: " + std::string(e.what()));
            allOutput = "error";
            return true;
        }
        catch (...)
        {
            PrintError(currentLine, 0, "Unknown exception occurred");
            allOutput = "error";
            return true;
        }
    }
    else
    {
        ostringstream stringStream;
        stringStream << "Ambiguous command '" << command << "'"
                     << ". Did you mean one of these: ";
        for (const auto &match : matches)
        {
            stringStream << match << " ";
        }
        PrintError(currentLine, 0, stringStream.str());
        allOutput = "error";
        return true;
    }
}
