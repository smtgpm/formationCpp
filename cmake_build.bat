@ECHO OFF
set BUILD_TYPE=Release
IF NOT EXIST build mkdir build
cd build
IF EXIST "CMakeCache.txt" DEL /F "CMakeCache.txt"
cmake -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
if "%BUILD_TYPE%" == "Debug" (
  cmake --build . --target ALL_BUILD --config Debug
) else (
  cmake --build . --target ALL_BUILD --config Release
)
cd ..