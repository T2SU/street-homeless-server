//
// Created by TSU on 2023-06-01.
//

#ifndef STREET_HOMELESS_SERVER_5C995A12C7184C3293DEE1BBEDF68D9B
#define STREET_HOMELESS_SERVER_5C995A12C7184C3293DEE1BBEDF68D9B

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl
{
    class abstract_data
    {
    protected:
        YAML::Node _data;

        explicit abstract_data(const std::string& file_path);

    public:
        inline auto operator[](const std::string& key) const { return _data[key]; }
        inline const auto& get_data() const { return _data; }

    private:
        void load(const std::string& file_path);
    };
}

#endif //STREET_HOMELESS_SERVER_5C995A12C7184C3293DEE1BBEDF68D9B
