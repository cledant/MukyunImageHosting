#include "file_saving.hpp"

#include <fstream>
#include <iostream>

namespace file_manipulation {

file_saving::file_saving()
  : _generator(std::random_device()())
{}

uint64_t
file_saving::save_file(std::string const &file_content,
                       std::string const &folder,
                       char const *type)
{
    std::uniform_int_distribution<uint64_t> int_distri(
      1, std::numeric_limits<uint64_t>::max());

    uint64_t nb_filename = int_distri(_generator);
    std::string file_name = folder + "/" + std::to_string(nb_filename) + type;
    if (!_save_on_disk(file_content, file_name)) {
        return (nb_filename);
    }
    return (0);
}

uint8_t
file_saving::_save_on_disk(std::string const &file_content,
                           std::string const &filename)
{
    std::ofstream ofs;

    ofs.open(filename, std::ios::binary);
    if (!ofs.is_open()) {
        return (1);
    }
    ofs << file_content;
    if (ofs.fail() || ofs.bad()) {
        ofs.close();
        return (1);
    }
    ofs.close();
    std::cout << "Saved image in : " << filename << std::endl;
    return (0);
}

}