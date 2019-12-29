#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "http_server.hpp"

static constexpr char const *ENV_CFG_PATH = "MUKYUN_IH_CFG_PATH";
static constexpr char const *DEFAULT_CFG_FILENAME = "mukyun_ih_cfg.json";

static void
display_help()
{
    std::cout << "Usage: mukyun_image_hosting [-h] [--help]" << std::endl;
    std::cout << "\t-h | --help : display usage" << std::endl;
    std::cout << "\t--generate_default_cfg : generate config file with default "
                 "values in current directory"
              << std::endl;
    std::cout << ENV_CFG_PATH
              << " needs to be set in environment with path to configuration "
                 "file as value"
              << std::endl;
}

static void
load_cfg(http_server::server_config &cfg)
{
    char const *cfg_path = getenv(ENV_CFG_PATH);
    if (!cfg_path) {
        std::cout << ENV_CFG_PATH
                  << " is not defined in env. Using default settings"
                  << std::endl;
        return;
    }
    std::ifstream ifs(cfg_path);
    if (!ifs.is_open()) {
        std::cout << "Failed to open config file : " << cfg_path << std::endl;
        std::cout << "Using default settings" << std::endl;
        return;
    }
    try {
        nlohmann::json json;
        ifs >> json;
        cfg = json.get<http_server::server_config>();
    } catch (std::exception const &e) {
        std::cout << "Failed to parse config file json : " << cfg_path
                  << std::endl;
        std::cout << "Using default settings" << std::endl;
    }
    ifs.close();
}

static void
generate_default_cfg(http_server::server_config const &cfg)
{
    nlohmann::json json = cfg;
    std::ofstream ofs(DEFAULT_CFG_FILENAME);
    if (!ofs.is_open()) {
        std::cout << "Failed to generate config file" << std::endl;
        return;
    }
    ofs << std::setw(4) << json << std::endl;
    ofs.close();
    std::cout << "Saved config in : " << DEFAULT_CFG_FILENAME << std::endl;
}

int
main(int argc, char const **argv)
{
    http_server::server_config cfg;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            display_help();
            return (0);
        } else if (!strcmp(argv[i], "--generate_default_cfg")) {
            generate_default_cfg(cfg);
            return (0);
        }
    }
    load_cfg(cfg);
    http_server::server_endpoint srv(std::move(cfg));
    srv.start();
    return (0);
}