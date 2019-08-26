#include "BigPrimeEngine.h"

BigInt BigPrimeEngine::operator()()
{
    // 子线程的最大数量
    static const uint64_t MAXTHREAD = std::thread::hardware_concurrency()/2;

    std::vector<std::thread> sub_threads;
    int i;
    big_prime = 0;

    sub_threads.reserve(MAXTHREAD);
    for (i=0; i<MAXTHREAD; ++i) sub_threads.emplace_back(sub_thread_find_prime, this);
    for (auto& th:sub_threads) th.join();

    return big_prime;
}

void BigPrimeEngine::find_prime()
{
    BigInt big;
    bool found;
    do {
        big_prime_mutex.lock();
        found = big_prime!=0;            // check if other threads have found a prime
        big_prime_mutex.unlock();
        if (found) return;

        big = big_rand();
    }
    while (!miller_rabin_test(big));

    big_prime_mutex.lock();
    big_prime = std::move(big);
    big_prime_mutex.unlock();
}

void sub_thread_find_prime(BigPrimeEngine* prime_eng) { prime_eng->find_prime(); }

BigInt BigPrimeEngine::big_rand()
{
    BigInt rand_big;
    int shift;
    for (shift = 0; shift < total_bit; shift += 32) {
        rand_big = rand_big + (BigInt(rand_eng()) << shift);            // “连接”不同随机数形成更大的随机数
    }
    if (!(rand_big & 1)) rand_big = rand_big - 1;                   // 若为偶数则将结果减1
    return rand_big;
}

/* 原型：CLRS P568 MILLER-RABIN */
bool BigPrimeEngine::miller_rabin_test(const BigInt &big)
{
    constexpr int CHECK = 50;         // 随机选取的用于检测的值的个数
    for (int i = 0; i < CHECK; ++i) {
        if (check_evidence(big, rand_eng() % 0xffffffff)) return false;
    }
    return true;
}

/* 原型：CLRS P560 MODULAR-EXPONENTIATION */
BigInt mod_expon(const BigInt& a, const BigInt& b, const BigInt& n)          // 反复平方法计算a**b%n的结果
{
    BigInt ans(1), one(1);
    for (int64_t shift = b.bit_size() - 1; shift >= 0; --shift) {
        ans = (ans * ans) % n;
        if (b & (one << shift)) ans = (ans * a) % n;
    }
    return ans;
}


/* 原型：CLRS P567 WITNESS */
bool BigPrimeEngine::check_evidence(const BigInt& big, unsigned evid)          // big应为奇数
{
    if (evid == 0) evid = 2;
    BigInt u, curr_x, last_x, one(1);
    uint64_t t = 0;
    while (!((big - 1) & (one << t))) ++t;
    u = (big - 1) >> t;

    curr_x = mod_expon(evid, u, big);
    for (int i = 0; i < t; ++i) {
        last_x = std::move(curr_x);
        curr_x = (last_x * last_x) % big;
        if (curr_x == 1 && last_x != 1 && last_x != big - 1)
            return true;
    }
    return curr_x != 1;
}

/* 原型：CLRS P547 EUCLID */
BigInt euclid_gcd(BigInt a, BigInt b)
{
    BigInt temp;
    while (b != 0) {
        temp = std::move(a);
        a = b;
        b = temp % b;
    }
    return a;
}
