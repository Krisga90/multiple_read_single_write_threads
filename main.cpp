#include <chrono>
#include <exception>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <thread>
#include <vector>

using namespace std::literals;

std::shared_mutex SHARED_MUTEX;

void read_fun(const std::string &str, bool throw_exc);
void write_fun(std::string &str, bool throw_exc);

int main() {


  std::string text = "Fizzz Buzzz!!\n";
  bool throw_except_argument = false;

  std::vector<std::thread> threads;

  for(int i =0; i<20; i++)
  {
    if((i-2)% 10 == 0)
      threads.push_back(std::thread(write_fun, std::ref(text), throw_except_argument));
    threads.push_back(std::thread(read_fun, std::cref(text), throw_except_argument));
  }
  threads.push_back(std::thread(write_fun, std::ref(text), throw_except_argument));

  for (auto& thread: threads)
    thread.join();

  return 0;
}


void read_fun(const std::string &str, bool throw_exc)
{
  try
  {
    std::shared_lock<std::shared_mutex> lock(SHARED_MUTEX);
    std::cout << "Read, thread ID: " << std::this_thread::get_id() << " text: " << str;

    if (throw_exc) {
      // throw std::exception();
      throw std::runtime_error("Forced error");
    }
    std::this_thread::sleep_for(100ms);
  } 
  catch (const std::exception &e) 
  {
    std::cerr << e.what() << '\n';
  }
}

void write_fun(std::string &str, bool throw_exc)
{
  try
  {
    std::lock_guard<std::shared_mutex> lock(SHARED_MUTEX);
    std::stringstream ss;
    str.pop_back();
    ss << str << "<" << std::this_thread::get_id() << ">\n";

    str = ss.str();
    std::cout << "\nWrite, thread ID: " << std::this_thread::get_id() << " text: " << str <<"\n";

    if (throw_exc) {
      // throw std::exception();
      throw std::runtime_error("Forced error");
    }
    std::this_thread::sleep_for(100ms);
  } 
  catch (const std::exception &e) 
  {
    std::cerr << e.what() << '\n';
  }
}
