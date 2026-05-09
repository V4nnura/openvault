include(FetchContent)

FetchContent_Declare(fpattern
    GIT_REPOSITORY "https://github.com/alexbatalov/fpattern"
    GIT_TAG 992b9685380ebb6bfd037856ececec1da2e3742e # v1.9.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(fpattern)
