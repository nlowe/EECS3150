#include <iostream>
#include <fstream>
#include <link/LinkLayer.h>

#define ERR_SYNTAX 1
#define ERR_INPUT  2
#define ERR_OUTPUT 3

#define DEV_STDOUT "/dev/stdout"

void printUsage()
{
    std::cout << "server <input>|- [output]" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << std::endl;
    std::cout << R"(    <input>|-    The file to "serve". If "-" read from stdin instead)" << std::endl;
    std::cout << R"(    [output]   The file "socket" to serve on. If blank, write to stdout)" << std::endl;
    std::cout << std::endl;
    std::cout << R"("Serves" the specified file by encoding bytes with parity and virtual framing )";
    std::cout << R"(to the output file (or stdout) to be read by the "client" program)" << std::endl;
}

/**
 * Read the specified file for processing
 *
 * @param in the file to read
 * @param len set to the length of the file in bytes
 * @return a char array containing the data read
 */
char* getFile(const std::string &in, size_t &len)
{
    std::ifstream reader;
    reader.open(in, std::ios::ate);
    if(!reader.good())
    {
        reader.close();
        std::cerr << "Could not open file for read: " << in << std::endl;
        return nullptr;
    }

    len = static_cast<size_t>(reader.tellg());
    auto bytes = new char[len]{0};
    reader.seekg(0);
    reader.read(bytes, len);
    reader.close();

    return bytes;
}

/**
 * Read all input from stdin
 *
 * @param len set to the length of the file in bytes
 * @return a char array containing the data read
 */
char* getStdin(size_t &len)
{
    std::cin >> std::noskipws;
    std::istreambuf_iterator<char> it(std::cin);
    std::istreambuf_iterator<char> end;
    std::string buff(it, end);

    len = buff.length();
    auto data = new char[len]{0};
    std::copy(buff.begin(), buff.end(), &data[0]);

    return data;
}

/**
 * Encode the specified bytes to the link
 *
 * @param buff the bytes to encode
 * @param len the number of bytes to encode
 * @param out the output file to write to
 * @return 0 for success, nonzero otherwise
 */
int processFile(const char* buff, size_t len, const std::string &out)
{
    auto writer = libsts::link::CreateFileBasedWriter(out);
    try {
        writer->open();
    }
    catch (std::exception &ex)
    {
        std::cerr << "Could not create writer: " << ex.what() << std::endl;
        return ERR_OUTPUT;
    }

    try
    {
        writer->write(buff, len);
    }
    catch (std::exception &ex)
    {
        writer->close();

        std::cerr << "Error encoding input: " << ex.what() << std::endl;
        return ERR_OUTPUT;
    }

    writer->close();
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc == 2)
    {
        auto arg = std::string(argv[1]);

        if(arg == "-h" || arg == "--help")
        {
            printUsage();
            return 0;
        }
    }
    else if(argc < 2 || argc > 3)
    {
        std::cerr << "Syntax Error!" << std::endl;
        printUsage();
        return ERR_SYNTAX;
    }

    auto inputFile = std::string(argv[1]);
    size_t inputLen = 0;
    char* input = inputFile == "-" ? getStdin(inputLen) : getFile(inputFile, inputLen);

    if(input == nullptr || inputLen == 0)
    {
        std::cerr << "Could not read input" << std::endl;
        return ERR_INPUT;
    }

    auto ret = processFile(input, inputLen, argc == 2 ? DEV_STDOUT : std::string(argv[2]));

    delete[] input;
    return ret;
}