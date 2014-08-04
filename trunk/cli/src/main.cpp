#include "core.h"
#include "launcher.h"


#include <boost/program_options.hpp>

int main(int argc, char** argv)
{
    (void) boost::program_options::arg;
    hello();
    std::string str1 = "abc";
    std::string str2 = std::move(str1);

    simbad::cli::Launcher l;
}
