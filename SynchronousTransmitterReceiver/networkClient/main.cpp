#include <iostream>
#include <fstream>
#include <link/LinkLayer.h>

#define ERR_SYNTAX 1
#define ERR_INPUT  2
#define ERR_OUTPUT 3

#define DEV_STDIN "/dev/stdin"

void printUsage()
{
    std::cout << "networkClient <-ec|-eh> <host> <port> <input>|-" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << std::endl;
    std::cout <<   "    -ec       Use CRC for error correction" << std::endl;
    std::cout <<   "    -eh       Use Hamming for error correction"  << std::endl;
    std::cout <<   "    <host>    The remote host to connect to" << std::endl;
    std::cout <<   "    <port>    The port to connect to" << std::endl;
    std::cout << R"(    <input>|- The file to "upload". If "-" read from stdin instead)" << std::endl;
    std::cout << std::endl;
    std::cout << R"("Uploads" the specified file to the specified remote host with the specified mode error correction )";
    std::cout << "to the specified remote host" << std::endl;
}

int uploadFile(libsts::link::ErrorCorrectionType correctionType, std::string &host, uint16_t port, std::string &file)
{
    std::ifstream reader;
    reader.open(file, std::ios::in);
    if(!reader.good())
    {
        reader.close();
        std::cerr << "Could not open file for read: " << file << std::endl;
        return ERR_INPUT;
    }

    std::vector<char> bytes;

    while(!reader.eof())
    {
        char slice[4096];
        reader.read(slice, 4096);
        auto realLen = reader.gcount();

        std::copy(&slice[0], &slice[0] + realLen, std::back_inserter(bytes));
    }

    auto link = libsts::link::CreateClient(host, port);
    try
    {
        link->open();
        link->setErrorCorrectionType(correctionType);

        link->write(bytes.data(), bytes.size());

        size_t len = 0;
        char* buff = link->readAll(len);

        std::cout << "Response: " << buff << std::endl;
        delete[] buff;

        link->close();
        return 0;
    }
    catch(std::exception &ex)
    {
        link->close();
        std::cerr << "Failed to upload file: " << ex.what() << std::endl;
        return ERR_OUTPUT;
    }
}

int main(int argc, char* argv[])
{
    if(argc != 5)
    {
        printUsage();
        return ERR_SYNTAX;
    }
    else
    {
        libsts::link::ErrorCorrectionType correctionType;
        auto ecArg = std::string(argv[1]);
        if(ecArg == "-h" || ecArg == "--help")
        {
            printUsage();
            return 0;
        }
        else if(ecArg == "-ec")
        {
            correctionType = libsts::link::ErrorCorrectionType::CRC_ANSI_16;
        }
        else if(ecArg == "-eh")
        {
            correctionType = libsts::link::ErrorCorrectionType::HAMMING;
        }
        else
        {
            printUsage();
            return ERR_SYNTAX;
        }

        auto host = std::string(argv[2]);
        uint16_t port;
        try
        {
            port = static_cast<uint16_t>(strtoul(argv[3], nullptr, 0));
        }
        catch (std::exception &ex)
        {
            std::cerr << "Failed to parse port number: " << ex.what() << std::endl;
            printUsage();
            return ERR_SYNTAX;
        }

        auto input = std::string(argv[4]);
        if(input == "-") input = DEV_STDIN;

        return uploadFile(correctionType, host, port, input);
    }
}