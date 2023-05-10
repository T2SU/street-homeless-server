# Street Homeless Server
- **<가난과 노숙>** 크로스플랫폼 C++ MMORPG 서버

# 개요
- C++20
- `libuv`를 활용한 실시간 MMORPG 서버
- <가난과 노숙>을 위한 전용 서버로 개발

# 빌드
- 개발은 **MSVC 17.0 (Visual Studio 2022) + vcpkg** 로 했음.
## 필요 패키지 목록
- C++20 compiler (clang, msvc, ...)
- git
- cmake >= 3.19
- date::date (>= 3.0.1)
- libmariadb (>= 3.3.1)
- libpq (>= 15.2)
- libuv (>= 1.44.2)
- openssl(>= 3.1.0)
- protobuf (>= 3.21.12)
- sqlpp11 (>= 0.61)
- yaml-cpp (>= 0.7.0)