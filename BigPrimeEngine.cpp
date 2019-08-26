#include "BigPrime.hpp"

BigInt BigPrime::operator()()
{
    BigInt big = big_rand();
    while (!miller_rabin_test(big)) big = big_rand();
    return big;
}

BigInt BigPrime::big_rand()
{
    constexpr int total_bits = 1024;              // 生成的随机数最大位数
    BigInt big;

    for (int shift = 0; shift < total_bits; shift += 32) {
        big = big + (BigInt(rand_eng()) << shift);            // “连接”不同随机数形成更大的随机数
    }
    if (!(big & 1)) big = big - 1;                   // 若为偶数则将结果减1
    return big;
}

/* 原型：CLRS P568 MILLER-RABIN */
bool BigPrime::miller_rabin_test(const BigInt &big)
{
    constexpr int check_time = 50;         // 随机选取的用于检测的值的个数
    for (int i = 0; i < check_time; ++i) {
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
bool BigPrime::check_evidence(const BigInt& big, unsigned evid)          // big应为奇数
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
