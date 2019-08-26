#ifndef BIGPRIME_HPP
#define BIGPRIME_HPP


#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include "BigInt.h"


class BigPrimeEngine {           // 随机生成基于BigInt的大素数
public:
	BigPrimeEngine(): rand_eng(static_cast<unsigned>(time(nullptr))) { }
	BigInt operator()();                 // 调用返回一个大素数

private:
	unsigned total_bit = 512;                // 生成素数的位数
	std::default_random_engine rand_eng;          // 随机数生成器，调用返回unsigned类型的随机值
	BigInt big_prime;
	std::mutex big_prime_mutex;
	void find_prime();
	friend void sub_thread_find_prime(BigPrimeEngine* prime_eng);

	BigInt big_rand();          // 生成随机大奇数
	bool miller_rabin_test(const BigInt &);             // Miller-Rabin素数测试，若测试为素数则返回true
	bool check_evidence(const BigInt &, unsigned);              // 检查给定数是大数为合数的证据则返回true
};

void sub_thread_find_prime(BigPrimeEngine* prime_eng);

BigInt mod_expon(const BigInt&, const BigInt&, const BigInt&);           // 求模取幂
BigInt euclid_gcd(BigInt, BigInt);                // 欧几里得算法计算最大公约数


#endif      // BIGPRIME_HPP
