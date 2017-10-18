#include <iostream>
#include <fstream>
#include <link/LinkLayer.h>

#define ERR_SYNTAX 1
#define ERR_INPUT  2
#define ERR_OUTPUT 3

#define DEV_STDIN "/dev/stdin"

void printUsage()
{
    std::cout << "client <input>|- [output]" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << std::endl;
    std::cout << R"(    <input>|- The file to "download". If "-" read from stdin instead)" << std::endl;
    std::cout <<   "    [output]  The file write to. If blank, write to stdout" << std::endl;
    std::cout << std::endl;
    std::cout << R"("Downloads" the specified file by decoding bytes with parity and virtual framing )";
    std::cout << "from the input file to the output file or stdout if no output file is specified" << std::endl;
}

/**
 * Decodes the specified file back to text
 *
 * @param in the file to decode
 * @param len set to the length of the decoded text
 * @return a char array contianing the decoded data. The callling
 *         method is responsible for deleting the array
 */
char* decode(const std::string &in, size_t &len)
{
    try
    {
        auto reader = libsts::link::CreateFileBasedReader(in);
        reader->open();
        return reader->readAll(len);
    }
    catch(std::exception &ex)
    {
        std::cerr << "Failed to decode file: " << ex.what() << std::endl;
        len = 0;
        return nullptr;
    }
}

/**
 * Decodes the specified file to the specified output file
 *
 * @param in the file to decode
 * @param out the file to write the results to
 * @return 0 for success, nonzero otherwise
 */
int processToFile(const std::string &in, const std::string &out)
{
    size_t len;
    auto buff = decode(in, len);

    if(buff == nullptr)
    {
        std::cerr << "Could not decode input" << std::endl;
        return ERR_INPUT;
    }

    std::ofstream writer;
    writer.open(out);

    if(!writer.good())
    {
        writer.close();
        std::cerr << "Could not open file for write: " << out << std::endl;
        return ERR_OUTPUT;
    }

    writer.write(buff, len);
    writer.close();

    return 0;
}

/**
 * Decodes the specified file to standard output
 *
 * @param in the file to decode
 * @return 0 for success, nonzero otherwise
 */
int processToStandardOut(const std::string &in)
{
    size_t len;
    auto buff = decode(in, len);

    if(buff == nullptr)
    {
        std::cerr << "Could not decode input" << std::endl;
        return ERR_INPUT;
    }

    std::string sanity(buff);
    sanity = sanity.substr(0, len);

    std::cout << sanity << std::endl;

    delete[] buff;

    return 0;
}

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        auto arg = std::string(argv[1]);

        if(arg == "-h" || arg == "--help")
        {
            printUsage();
            return 0;
        }
        else
        {
            auto inputFile = std::string(argv[1]);
            return processToStandardOut(inputFile == "-" ? DEV_STDIN : inputFile);
        }
    }
    else if(argc == 3)
    {
        auto inputFile = std::string(argv[1]);
        return processToFile(inputFile == "-" ? DEV_STDIN : inputFile, std::string(argv[2]));
    }
    else
    {
        std::cerr << "Syntax Error!" << std::endl;
        printUsage();
        return ERR_SYNTAX;
    }
}