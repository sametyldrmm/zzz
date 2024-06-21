cmake_minimum_required(VERSION 3.10)

# Proje adını ve sürümünü belirtin
project(MyProject VERSION 1.0)

# C++ standardını ayarlayın
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Başlık dosyalarının bulunduğu dizini belirtin
include_directories(include)

# Tüm kaynak dosyalarını src klasöründen alın
file(GLOB SOURCES "src/*.cpp")

# Pthread kütüphanesini bulun ve ekleyin
find_package(Threads REQUIRED)

# Yürütülebilir dosya oluşturun
add_executable(MyExecutable ${SOURCES})

# Pthread kütüphanesini yürütülebilir dosyaya bağlayın
target_link_libraries(MyExecutable ${CMAKE_THREAD_LIBS_INIT})
