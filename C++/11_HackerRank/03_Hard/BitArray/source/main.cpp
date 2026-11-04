/**
 * Bit Array
 *
   Sample Input:
   3 1 1 1

   Sample Output:
   3
 *
 */

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include <inttypes.h>

const uint64_t MOD = (1ULL << 31);

uint64_t next_val(uint64_t a, uint64_t P, uint64_t Q)
{
  return (a * P + Q) % MOD;
}

int main(int argc, const char * argv[])
{
  int32_t N = 0;
  int32_t S = 0;
  int32_t P = 0;
  int32_t Q = 0;

  std::cin >> N >> S >> P >> Q;

  uint64_t slow = S % MOD;
  uint64_t fast = S % MOD;

  size_t count = 0;

  // ---------- F1: Loop Entrie ----------
  do
  {
    if (count == N)
    {
      std::cout << count << std::endl;
      return 0;
    }

    slow = next_val(slow, P, Q);
    fast = next_val(fast, P, Q);
    fast = next_val(fast, P, Q);

    count++;
  }
  while (slow != fast);

  // ---------- F2: Tail Length ----------
  int tail = 0;
  slow = S % MOD;

  while (slow != fast)
  {
    slow = next_val(slow, P, Q);
    fast = next_val(fast, P, Q);
    tail++;
  }

  // ---------- F3: Loop Length ----------
  int cycle = 1;
  fast = next_val(slow, P, Q);

  while (slow != fast)
  {
    fast = next_val(fast, P, Q);
    cycle++;
  }

  int result = tail + cycle;

  if (result > N)
  {
    result = N;
  }

  std::cout << result << std::endl;

  return 0;
}
