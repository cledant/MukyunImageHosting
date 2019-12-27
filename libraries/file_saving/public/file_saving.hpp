#ifndef TEST_HTTP_SERVER_FILE_SAVING_HPP
#define TEST_HTTP_SERVER_FILE_SAVING_HPP

#include <string>
#include <random>

namespace file_manipulation {

struct file_saving_config
{
    file_saving_config();
    virtual ~file_saving_config() = default;
    file_saving_config(file_saving_config const &src) = default;
    file_saving_config &operator=(file_saving_config const &rhs) = default;
    file_saving_config(file_saving_config &&src) = default;
    file_saving_config &operator=(file_saving_config &&rhs) = default;

    std::string _save_folder;
    std::string _link_address;

    static constexpr char const *DEFAULT_SAVE_FOLDER = "./";
    static constexpr char const *DEFAULT_LINK_ADDRESS = "http://127.0.0.0.10";
};

class file_saving
{
  public:
    file_saving();
    explicit file_saving(file_saving_config &&cfg);
    virtual ~file_saving() = default;
    file_saving(file_saving const &src) = delete;
    file_saving &operator=(file_saving const &rhs) = delete;
    file_saving(file_saving &&src) = delete;
    file_saving &operator=(file_saving &&rhs) = delete;

    std::string save_file(std::string const &file_content, char const *type);

  private:
    file_saving_config _config;
    std::mt19937_64 _generator;

    uint8_t _save_on_disk(std::string const &file_content,
                          std::string const &filename);
};

}

#endif // TEST_HTTP_SERVER_FILE_SAVING_HPP
