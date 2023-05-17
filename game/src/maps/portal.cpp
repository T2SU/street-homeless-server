//
// Created by T2SU on 2023/05/16.
//

#include <utility>

#include "std.hpp"
#include "maps/portal.hpp"

static hl::game::portal_type get_portal_type(const std::string& t)
{
    if (t == "ChangeMap") return hl::game::portal_type::change_map;
    else if (t == "Script") return hl::game::portal_type::script;
    else return hl::game::portal_type::starting_point;
}

hl::game::portal::portal(const YAML::Node& node)
        : _name(node["name"].as<std::string>())
        , _type(get_portal_type(node["type"].as<std::string>()))
        , _pos(node["position"]["x"].as<float>(), node["position"]["y"].as<float>(), node["position"]["z"].as<float>())
        , _rot(node["rotation"]["x"].as<float>(), node["rotation"]["y"].as<float>(), node["rotation"]["z"].as<float>(), node["rotation"]["w"].as<float>())
        , _target_scene(node["target_scene"].as<std::string>())
        , _target_portal_name(node["target_portal_name"].as<std::string>())
        , _script_name(node["script_name"].as<std::string>())
{}

const std::string &hl::game::portal::get_name() const
{
    return _name;
}

const hl::vector3 &hl::game::portal::get_pos() const
{
    return _pos;
}

const hl::quaternion &hl::game::portal::get_rot() const
{
    return _rot;
}

hl::game::portal_type hl::game::portal::get_type() const
{
    return _type;
}

const std::string &hl::game::portal::get_target_scene() const
{
    return _target_scene;
}

const std::string &hl::game::portal::get_target_portal_name() const
{
    return _target_portal_name;
}

const std::string &hl::game::portal::get_script_name() const
{
    return _script_name;
}
