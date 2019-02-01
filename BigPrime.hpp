#ifndef BIGPRIME_HPP
#define BIGPRIME_HPP


#include <random>
#include <ctime>
#include "BigInt.hpp"


class BigPrime {           // 随机生成1024位大素数
public:
	BigPrime(): rand_eng(time(0)) { }
	BigInt operator()();                 // 调用返回一个大素数

private:
	uint64_t count = 0;     // 用于测试时计数
	std::default_random_engine rand_eng;          // 随机数生成器，调用返回unsigned类型的随机值
	BigInt big_rand();          // 生成1024位随机大奇数
	bool check_evidence(const BigInt &, unsigned);              // 检查给定数是大数为合数的证据则返回true
	bool miller_rabin_test(const BigInt &);             // Miller-Rabin素数测试，若测试为素数则返回true
};

BigInt mod_expon(const BigInt& a, const BigInt& b, const BigInt& n);           // 求模取幂
BigInt euclid_gcd(BigInt, BigInt);                // 欧几里得算法计算最大公约数


#endif      // BIGPRIME_HPP
