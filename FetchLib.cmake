function(FetchLib)
    # protobuf
    FetchContent_Declare(protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
        GIT_TAG        v23.0
        SOURCE_SUBDIR  cmake
        FIND_PACKAGE_ARGS NAMES protobuf
    )
    FetchContent_MakeAvailable(protobuf)

    # libuv

    # openssl

    # yaml-cpp

    # plog
endfunction()