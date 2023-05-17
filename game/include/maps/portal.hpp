//
// Created by T2SU on 2023/05/16.
//

#ifndef STREET_HOMELESS_SERVER_0B64770214D54C11850242972DC80943
#define STREET_HOMELESS_SERVER_0B64770214D54C11850242972DC80943

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

namespace hl::game
{
    enum class portal_type { starting_point, change_map, script };

    class portal
    {
    private:
        const std::string _name;
        const vector3 _pos;
        const quaternion _rot;
        const portal_type _type;
        const std::string _target_scene;
        const std::string _target_portal_name;
        const std::string _script_name;

    public:
        explicit portal(const YAML::Node& node);

        [[nodiscard]] const std::string &get_name() const;
        [[nodiscard]] const vector3 &get_pos() const;
        [[nodiscard]] const quaternion &get_rot() const;
        [[nodiscard]] portal_type get_type() const;
        [[nodiscard]] const std::string &get_target_scene() const;
        [[nodiscard]] const std::string &get_target_portal_name() const;
        [[nodiscard]] const std::string &get_script_name() const;
    };
}

#endif //STREET_HOMELESS_SERVER_0B64770214D54C11850242972DC80943
