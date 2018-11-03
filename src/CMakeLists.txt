add_library(exchange SHARED book.cpp)

add_executable(play play.cpp)
target_link_libraries(play exchange)
