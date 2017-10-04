#include <iostream>
#include <fstream>
#include <link/LinkLayer.h>

#define ERR_SYNTAX 1
#define ERR_INPUT  2
#define ERR_OUTPUT 3

void printUsage()
{
    std::cout << "client <input>|- [output]" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << std::endl;
    std::cout << R"(    <input>   The file to "download")" << std::endl;
    std::cout <<   "    [output]  The file write to. If blank, write to stdout" << std::endl;
    std::cout << std::endl;
    std::cout << R"("Downloads" the specified file by decoding bytes with parity and virtual framing)";
    std::cout << "from the input file to the output file or stdout if no output file is specified" << std::endl;
}

char* decode(const std::string &in, size_t &len)
{
    try
    {
        auto reader = libsts::link::CreateFileBasedLink(in);
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

int processToStandardOut(const std::string &in)
{
    size_t len;
    auto buff = decode(in, len);

    if(buff == nullptr)
    {
        std::cerr << "Could not decode input" << std::endl;
        return ERR_INPUT;
    }

    std::cout << buff << std::endl;

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
            return processToStandardOut(std::string(argv[1]));
        }
    }
    else if(argc == 3)
    {
        return processToFile(std::string(argv[1]), std::string(argv[2]));
    }
    else
    {
        std::cerr << "Syntax Error!" << std::endl;
        printUsage();
        return ERR_SYNTAX;
    }
}