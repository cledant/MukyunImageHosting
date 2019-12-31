#include "file_saving.hpp"

#include <fstream>
#include <iostream>
#include <chrono>

namespace file_manipulation {

file_saving::file_saving()
  : _generator(std::random_device()())
{}

std::string
file_saving::save_file(std::string const &file_content,
                       std::string const &folder,
                       char const *type)
{
    std::uniform_int_distribution<uint32_t> int_distri(
      0, std::numeric_limits<uint32_t>::max());

    // Concat time since epoch + random uint64 for filename
    auto time = std::chrono::system_clock::now();
    std::string filename =
      std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
                       time.time_since_epoch())
                       .count()) +
      "_" + std::to_string(int_distri(_generator)) + type;

    // Saving file
    std::string save_filename = folder + "/" + filename;
    if (!_save_on_disk(file_content, save_filename)) {
        return (filename);
    }
    return (std::string());
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