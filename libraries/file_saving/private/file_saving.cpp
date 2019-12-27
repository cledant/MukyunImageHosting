#include "file_saving.hpp"

#include <fstream>
#include <iostream>

namespace file_manipulation {

file_saving_config::file_saving_config()
  : _save_folder(DEFAULT_SAVE_FOLDER)
  , _link_address(DEFAULT_LINK_ADDRESS)
{}

file_saving::file_saving()
  : _generator(std::random_device()())
{}

file_manipulation::file_saving::file_saving(
  file_manipulation::file_saving_config &&cfg)
  : _config(cfg)
  , _generator(std::random_device()())
{}

std::string
file_saving::save_file(std::string const &file_content, char const *type)
{
    std::uniform_int_distribution<uint64_t> int_distri;
    std::string file_name = std::to_string(int_distri(_generator)) + type;
    if (!_save_on_disk(file_content, file_name)) {
        std::string url = _config._link_address + "/" + file_name;
        return (url);
    }
    return ("Failed to save file");
}

uint8_t
file_saving::_save_on_disk(std::string const &file_content,
                           std::string const &filename)
{
    std::ofstream ofs;

    ofs.open(_config._save_folder + filename, std::ios::binary);
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