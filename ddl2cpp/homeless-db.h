// generated by ddl2cpp.py homeless.ddl homeless-db db
#ifndef DB_HOMELESS_DB_H
#define DB_HOMELESS_DB_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace db
{
  namespace PublicCharacters_
  {
    struct Pid
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "pid";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T pid;
            T& operator()() { return pid; }
            const T& operator()() const { return pid; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Name
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "name";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T name;
            T& operator()() { return name; }
            const T& operator()() const { return name; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct Health
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "health";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T health;
            T& operator()() { return health; }
            const T& operator()() const { return health; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Tiredness
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "tiredness";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T tiredness;
            T& operator()() { return tiredness; }
            const T& operator()() const { return tiredness; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct MaxHealth
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "max_health";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T maxHealth;
            T& operator()() { return maxHealth; }
            const T& operator()() const { return maxHealth; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct MaxTiredness
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "max_tiredness";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T maxTiredness;
            T& operator()() { return maxTiredness; }
            const T& operator()() const { return maxTiredness; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Money
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "money";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T money;
            T& operator()() { return money; }
            const T& operator()() const { return money; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Created
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "created";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T created;
            T& operator()() { return created; }
            const T& operator()() const { return created; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
    };
    struct LastConnected
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "last_connected";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T lastConnected;
            T& operator()() { return lastConnected; }
            const T& operator()() const { return lastConnected; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
    };
  } // namespace PublicCharacters_

  struct PublicCharacters: sqlpp::table_t<PublicCharacters,
               PublicCharacters_::Pid,
               PublicCharacters_::Name,
               PublicCharacters_::Health,
               PublicCharacters_::Tiredness,
               PublicCharacters_::MaxHealth,
               PublicCharacters_::MaxTiredness,
               PublicCharacters_::Money,
               PublicCharacters_::Created,
               PublicCharacters_::LastConnected>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "public.characters";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T publicCharacters;
        T& operator()() { return publicCharacters; }
        const T& operator()() const { return publicCharacters; }
      };
    };
  };
  namespace PublicUsers_
  {
    struct Uid
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "uid";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T uid;
            T& operator()() { return uid; }
            const T& operator()() const { return uid; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Name
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "name";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T name;
            T& operator()() { return name; }
            const T& operator()() const { return name; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct Password
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "password";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T password;
            T& operator()() { return password; }
            const T& operator()() const { return password; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct Salt
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "salt";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T salt;
            T& operator()() { return salt; }
            const T& operator()() const { return salt; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
    struct Created
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "created";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T created;
            T& operator()() { return created; }
            const T& operator()() const { return created; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
    };
    struct LastConnected
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "last_connected";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T lastConnected;
            T& operator()() { return lastConnected; }
            const T& operator()() const { return lastConnected; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
    };
    struct SessionIp
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "session_ip";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T sessionIp;
            T& operator()() { return sessionIp; }
            const T& operator()() const { return sessionIp; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
  } // namespace PublicUsers_

  struct PublicUsers: sqlpp::table_t<PublicUsers,
               PublicUsers_::Uid,
               PublicUsers_::Name,
               PublicUsers_::Password,
               PublicUsers_::Salt,
               PublicUsers_::Created,
               PublicUsers_::LastConnected,
               PublicUsers_::SessionIp>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "public.users";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T publicUsers;
        T& operator()() { return publicUsers; }
        const T& operator()() const { return publicUsers; }
      };
    };
  };
} // namespace db
#endif
