# CMakeList.txt : CMake project for xna++, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (xna++ 
"csharp/integralnumeric.cpp"
"csharp/numeric.cpp"
"mathhelper.cpp"
"xna++.cpp"
"point.cpp"
"matrix.cpp"
"quaternion.cpp"
"vector.cpp"
"rectangle.cpp"
"csharp/nullable.cpp"
"boundingbox.cpp"
"boundingsphere.cpp"
"boundingfrustum.cpp" 
"enumerations.cpp" 
"plane.cpp" 
"ray.cpp"
"curvekey.cpp" 
"curvekeycollection.cpp"
"curve.cpp"
"color.cpp"
"graphics/packedvector.cpp"
"csharp/floatnumeric.cpp"
"csharp/timespan.cpp"
"gameclock.cpp"
"csharp/stopwatch.cpp"
"content/contentreader.cpp"
"csharp/stream/stream.cpp"
"csharp/array.cpp"
"content/contentmanager.cpp" 
"titlecontainer.cpp" 
"csharp/uri/uri.cpp"
"csharp/stream/reader.cpp"
"csharp/io/path.cpp" 
"helpers/stringhelper.cpp"
"content/decompress-stream.cpp"  "csharp/type.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xna++ PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
