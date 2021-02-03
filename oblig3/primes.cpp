#include <iostream>
#include <condition_variable>
#include <thread>
#include <functional>
#include <list>

using namespace std;

class Primes
{

  mutex primes_mutex;
  list<thread> threads;
  list<int> primes;

public:
  Primes(int start, int end, int threads_amount)
  {
    int low = start - 1;
    int num_of_elements = end - low;
    for (int i = 0; i < threads_amount; i++)
    {
      threads.emplace_back([this, i, num_of_elements, threads_amount, low] {
        pair<int, int> pair{
            (i * num_of_elements / threads_amount) + low,
            ((i + 1) * num_of_elements / threads_amount) + low};
        cout << "Thread " << (i + 1) << " will analyze from " << pair.first + 1 << " to " << pair.second << " - " << (pair.second - pair.first) << " numbers." << endl;
        for (int n = pair.first + 1; n < pair.second; n++)
        {
          if (isPrime(n))
          {
            primes_mutex.lock();
            primes.emplace_back(n);
            primes_mutex.unlock();
          }
        }
      });
    }
    for (auto &thread : threads)
    {
      thread.join();
    }
  };

  bool isPrime(int n)
  {
    if (n == 0 || n == 1)
    {
      return false;
    }
    else
    {
      for (int i = 2; i <= n / 2; ++i)
      {
        if (n % i == 0)
        {
          return false;
        }
      }
    }
    return true;
  }

  ~Primes()
  {
    threads.clear();
    primes.sort();
    for (auto &prime : primes)
    {
      cout << " " << prime;
    }
    cout << endl;
  }
};

int main()
{
  Primes(3, 50'000, 100);
}
