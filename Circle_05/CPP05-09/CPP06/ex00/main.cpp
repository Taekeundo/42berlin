#include "Converter.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << RED << "Error: Format of the input is inconrrect" << EOC << std::endl
        << "ex) ./convert [String type input]" << std::endl;
    }
    else
    {
        try
        {
            Converter::convert(argv[1]);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    return (0);
}
