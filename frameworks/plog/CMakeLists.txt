include(FetchContent)

FetchContent_Declare(
    PLOG
    GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
    GIT_BRANCH     origin/master
)

FetchContent_MakeAvailable(PLOG)