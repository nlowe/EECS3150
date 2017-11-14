#include <iostream>
#include <fstream>
#include <link/LinkLayer.h>
#include <vector>

#define ERR_SYNTAX 1
#define ERR_INPUT  2
#define ERR_OUTPUT 3

#define BUFF_SIZE 65536

void printUsage()
{
    std::cout << "networkServer <-eh|-ec> <port>" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << std::endl;
    std::cout << "    -eh     use hamming for error correction" << std::endl;
    std::cout << "    -ec     use CRC for error correction" << std::endl;
    std::cout << "    <port>  The port to listen on" << std::endl;
    std::cout << std::endl;
    std::cout << "Listens on the specified port for clients to upload ";
    std::cout << "files to with virtual framing and error checking" << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc == 3)
    {
        auto arg = std::string(argv[1]);

        if(arg == "-h" || arg == "--help")
        {
            printUsage();
            return 0;
        }
        else
        {
            libsts::link::ErrorCorrectionType errorCorrectionType;
            if(arg == "-ec")
            {
                errorCorrectionType = libsts::link::ErrorCorrectionType::CRC_ANSI_16;
            }
            else if(arg == "-eh")
            {
                errorCorrectionType = libsts::link::ErrorCorrectionType::HAMMING;
            }
            else
            {
                std::cerr << "Unknown error-correction type: " << arg;
                return ERR_SYNTAX;
            }

            auto port = std::strtoul(argv[2], nullptr, 10);
            auto link = libsts::link::CreateServer(static_cast<const uint16_t>(port));
            try
            {
                link->open();
                link->setErrorCorrectionType(errorCorrectionType);

                std::vector<char> data;

                size_t len;
                char* buff = link->readAll(len);
                std::copy(buff, buff + len, std::back_inserter(data));

                std::cout << std::string(buff).substr(len);

                std::string response = "Received " + std::to_string(data.size()) + " bytes of data";
                link->write(response.c_str(), response.length());
                link->close();
            }
            catch(std::exception &ex)
            {
                link->close();
                std::cerr << "Encountered an error: " << ex.what() << std::endl;
                return ERR_INPUT;
            }
        }
    }
    else
    {
        std::cerr << "Syntax Error!" << std::endl;
        printUsage();
        return ERR_SYNTAX;
    }
}