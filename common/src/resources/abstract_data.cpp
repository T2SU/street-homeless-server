//
// Created by TSU on 2023-06-01.
//

#include "std_common.hpp"
#include "config/config.hpp"
#include "resources/abstract_data.hpp"

namespace hl
{
    abstract_data::abstract_data(const std::string& file_ath)
        : _data()
    {
        load(file_ath);
    }

    void abstract_data::load(const std::string& file_path)
    {
        const auto data_root = std::filesystem::path(hl::config::data_root);
        const auto path = data_root / file_path;

        const auto full_path = path.string() + ".yaml";
        _data = YAML::LoadFile(full_path);

        LOGV << "Loaded data: [" << full_path << "]";
    }
} // hl