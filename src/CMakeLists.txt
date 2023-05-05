# CMakeList.txt : CMake project for xna++, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (xna++ 
"csharp/integralnumeric.cpp"
"csharp/numeric.cpp"
"csharp/type.cpp"
"csharp/uri/uri.cpp"
"csharp/stream/reader.cpp"
"csharp/io/path.cpp" 
"csharp/floatnumeric.cpp"
"csharp/timespan.cpp"
"csharp/stopwatch.cpp"
"input/mouse.cpp"
"input/enumerations.cpp" 
"input/keyboard.cpp"
"input/joystick.cpp"
"graphics/packedvector.cpp"
"utilities/stringhelper.cpp"
"mathhelper.cpp"
"utilities/filehelpers.cpp"
"xna++.cpp"
"basic-structs.cpp"
"csharp/nullable.cpp" 
"enumerations.cpp" 
"curve.cpp"
"color.cpp"
"gameclock.cpp"
"content/contentreader.cpp"
"csharp/stream/stream.cpp"
"csharp/array.cpp"
"content/contentmanager.cpp" 
"titlecontainer.cpp" 
"content/decompress-stream.cpp"
"content/lzxdecoder.cpp"
"collision.cpp"
"csharp/enumerations.cpp"
"csharp/io/file.cpp"
 "input/gamepad.cpp" "graphics/texture.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xna++ PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
