#ifndef TEST_HTTP_SERVER_FILE_SAVING_HPP
#define TEST_HTTP_SERVER_FILE_SAVING_HPP

#include <string>
#include <random>

#include "nlohmann/json.hpp"

namespace file_manipulation {

class file_saving
{
  public:
    file_saving();
    virtual ~file_saving() = default;
    file_saving(file_saving const &src) = delete;
    file_saving &operator=(file_saving const &rhs) = delete;
    file_saving(file_saving &&src) = delete;
    file_saving &operator=(file_saving &&rhs) = delete;

    std::string save_file(std::string const &file_content,
                          std::string const &folder,
                          char const *type);

  private:
    std::mt19937_64 _generator;

    static uint8_t _save_on_disk(std::string const &file_content,
                                 std::string const &filename);
};

}

#endif // TEST_HTTP_SERVER_FILE_SAVING_HPP
