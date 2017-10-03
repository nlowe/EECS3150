#include <iostream>
#include <fstream>
#include <link/LinkLayer.h>

#define ERR_SYNTAX 1
#define ERR_INPUT  2
#define ERR_OUTPUT 3

void printUsage()
{
    std::cout << "server <input>|- [output]" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << std::endl;
    std::cout << R"(    <input>|-    The file to "serve". If "-" read from stdin instead)" << std::endl;
    std::cout << R"(    [output]   The file "socket" to serve on. If blank, write to stdout)" << std::endl;
    std::cout << std::endl;
    std::cout << R"("Serves" the specified file by encoding bytes with parity and virtual framing)";
    std::cout << R"(to the output file (or stdout) to be read by the "client" program)" << std::endl;
}

int processFile(const std::string &in, const std::string &out)
{
    std::ifstream reader;
    reader.open(in, std::ios::ate);
    if(!reader.good())
    {
        reader.close();
        std::cerr << "Could not open file for read: " << in << std::endl;
        return ERR_INPUT;
    }

    auto writer = libsts::link::CreateFileBasedWriter(out);
    try {
        writer->open();
    }
    catch (std::exception &ex)
    {
        reader.close();
        std::cerr << "Could not create writer: " << ex.what() << std::endl;
        return ERR_OUTPUT;
    }

    auto len = reader.tellg();
    auto bytes = new char[len]{0};
    reader.seekg(0);
    reader.read(bytes, len);
    reader.close();

    try
    {
        writer->write(bytes, len);
    }
    catch (std::exception &ex)
    {
        reader.close();
        writer->close();

        delete[] bytes;
        std::cerr << "Error encoding input: " << ex.what() << std::endl;
        return ERR_OUTPUT;
    }

    std::cout << "Encoded '" << in << "' to '" << out << "'" << std::endl;

    writer->close();
    delete[] bytes;
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc == 2 && std::string(argv[1]) == "-h")
    {
        printUsage();
        return 0;
    }

    if(argc != 3)
    {
        std::cerr << "Syntax Error!" << std::endl;
        printUsage();
        return ERR_SYNTAX;
    }

    return processFile(std::string(argv[1]), std::string(argv[2]));
}