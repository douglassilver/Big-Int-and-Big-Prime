#ifndef BIGINT_HPP
#define BIGINT_HPP


#include <deque>
#include <iostream>
#include <sstream>

typedef long long int64_t;
typedef unsigned long long uint64_t;


// deque实现支持简单操作的大整数 
class BigInt {
public:
	// 构造函数 
	BigInt();            // 默认初始化为0
	BigInt(int64_t);           // 允许隐式转换
	BigInt(const BigInt&);           // 拷贝构造 
	BigInt(BigInt&&) noexcept;          // 移动构造
	// 赋值函数 
	BigInt& operator=(const BigInt&);           // 拷贝赋值
	BigInt& operator=(BigInt&&) noexcept;        // 移动赋值
	
	// 比较 
	friend bool operator==(const BigInt&, const BigInt&);          // 允许uint64_t或int64_t隐式转换为BigInt进而支持比较 
	friend bool operator<(const BigInt&, const BigInt&);
	
	// 位操作 
	BigInt operator<<(uint64_t) const;            // 左移，帮助实现乘法运算
	BigInt operator>>(uint64_t) const;
	friend BigInt operator&(const BigInt&, const BigInt&);
	
	// 类型转换
	explicit operator bool() const;

	uint64_t bit_size() const;     // 返回位数，帮助实现乘法
	
	// 算术
	friend BigInt operator+(const BigInt&, const BigInt&);
	friend BigInt operator-(const BigInt&, const BigInt&);
	friend BigInt operator%(const BigInt&, const BigInt&);
	friend BigInt operator*(const BigInt&, const BigInt&);
	
	// 输出 
	friend std::ostream& operator<<(std::ostream&, const BigInt&);          // 输出16进制形式
	
private:
	std::deque<uint64_t> number;       // 二进制观点。绝对值，秩从小到大对应低位到高位 
	bool sign;                // 符号，指示正(true)负(false) 

	uint64_t count_ones() const;   	// 乘法辅助
};

// 比较操作
bool operator==(const BigInt&, const BigInt&);
bool operator!=(const BigInt&, const BigInt&);
bool operator<(const BigInt&, const BigInt&);
bool operator>(const BigInt&, const BigInt&);
bool operator<=(const BigInt&, const BigInt&);
bool operator>=(const BigInt&, const BigInt&);
// 算术操作
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);

std::ostream& operator<<(std::ostream&, const BigInt&);


#endif      // BIGINT_HPP
