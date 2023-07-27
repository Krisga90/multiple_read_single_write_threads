# tests of shared_lock and multiple read single write access

std::shared_lock is slower than normal lock so it is ok to use it when you have multiple "read" function and single "modify" function

to start:
g++ -o app main.cpp -std=c++20 -lstdc++
./app
