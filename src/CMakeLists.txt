# CMakeList.txt : CMake project for xna++, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (xna++ 
"content/decompress-stream.cpp"
"content/lzxdecoder.cpp"
"content/contentreader.cpp"
"content/contentmanager.cpp" 
"csharp/integralnumeric.cpp"
"csharp/numeric.cpp"
"csharp/type.cpp"
"csharp/uri/uri.cpp"
"csharp/stream/reader.cpp"
"csharp/io/path.cpp" 
"csharp/floatnumeric.cpp"
"csharp/timespan.cpp"
"csharp/stopwatch.cpp"
"csharp/stream/stream.cpp"
"csharp/array.cpp"
"csharp/io/file.cpp"
"csharp/enumerations.cpp"
"input/mouse.cpp"
"input/enumerations.cpp" 
"input/keyboard.cpp"
"input/joystick.cpp"
"input/gamepad.cpp"
"input/input.cpp"
"graphics/packedvector.cpp"
"graphics/texture.cpp"
"utilities/stringhelper.cpp"
"utilities/filehelpers.cpp"
"mathhelper.cpp"
"xna++.cpp"
"basic-structs.cpp"
"csharp/nullable.cpp" 
"enumerations.cpp" 
"curve.cpp"
"color.cpp"
"gameclock.cpp"
"titlecontainer.cpp" 
"collision.cpp"
"gamewindow.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xna++ PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
