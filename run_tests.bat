@ECHO ON

:: TODO: Set parameters for config and check if test were built
cd build
ctest -C Debug --verbose
