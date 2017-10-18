#include <iostream>
#include <fstream>
#include <random>

#define ERR_SYNTAX 1
#define ERR_INPUT  2
#define ERR_OUTPUT 3

void printUsage()
{
    std::cout << "faulty-wire <file>|- <bits to flip>" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << std::endl;
    std::cout << R"(    <file>|-          The file to flip bits in. If "-" read from stdin instead)" << std::endl;
    std::cout <<   "    <bits to flip>    The number of bits to flip. Must be at least 1" << std::endl;
    std::cout << std::endl;
    std::cout << R"(Flips random "bits" in the input file. )";
    std::cout << R"(The file must be comprised of only "0" and "1" characters)" << std::endl;
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
 * Flip an arbitrary number of "bits" in the input file
 *
 * @param in the input file to read from. Use "-" for stdin
 *           The file is modified in-place. If stdin is used,
 *           the output is written to stdout
 * @param bits the number of bits to flip
 * @return 0 for success, nonzero otherwise
 */
int flipAndFlush(const std::string &in, size_t bits)
{
    size_t len = 0;
    char* data = in == "-" ? getStdin(len) : getFile(in, len);

    if(len == 0)
    {
        std::cerr << "Didn't get any input" << std::endl;
        return ERR_INPUT;
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<size_t > distribution(0,len-1);

    for(auto i = 0; i < bits; i++)
    {
        auto idx = distribution(rng);
        if(data[idx] == '0')
        {
            data[idx] = '1';
        }
        else if(data[idx] == '1')
        {
            data[idx] = '0';
        }
        else
        {
            std::cerr << "Input is not in the correct format. Expected a '0' or '1' but got " << std::to_string(data[idx]) << std::endl;
            delete[] data;
            return ERR_INPUT;
        }
    }

    if(in == "-")
    {
        std::cout << data;
    }
    else
    {
        std::ofstream writer;
        writer.open(in, std::ios::out | std::ios::trunc);

        if(!writer.good())
        {
            std::cerr << "Could not open file for write: " << in << std::endl;
            delete[] data;
            return ERR_OUTPUT;
        }

        writer.write(data, len);
        writer.flush();
        writer.close();
    }

    delete[] data;
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cerr << "Syntax Error!" << std::endl;
        printUsage();
        return ERR_SYNTAX;
    }

    try
    {
        std::string input(argv[1]);
        auto len = std::stoull(std::string(argv[2]));

        return flipAndFlush(input, len);
    }
    catch(std::exception &ex)
    {
        std::cerr << "Failed to parse arguments: " << ex.what() << std::endl;
        return ERR_INPUT;
    }
}
