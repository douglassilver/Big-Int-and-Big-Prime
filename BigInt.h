#ifndef BIGINT_HPP
#define BIGINT_HPP


#include <deque>
#include <utility>
#include <iostream>
#include <sstream>


typedef long long int64_t;
typedef unsigned long long uint64_t;

class BigInt;

// 实现位于、位或、位异或
typedef uint64_t (*BitOperate)(uint64_t, uint64_t);
static BigInt bit_operation(const BigInt&, const BigInt&, BitOperate);

// 实现除法与求余
static std::pair<BigInt, BigInt> divide_with_remainder(const BigInt &, const BigInt &);


// deque实现支持简单操作的大整数 Using deque to implement big int supporting simple operations
class BigInt {
public:
	// 构造函数 Constructors
	BigInt();            // 默认初始化为0 Default initialize to zero
	BigInt(int64_t);           // 允许算术类型隐式转换 Allowing arithmetic type convert to BigInt implicitly
	BigInt(const BigInt&) = default;           // 拷贝构造 Copy constructor
	BigInt(BigInt&&) noexcept;          // 移动构造 Move-constructor
	// 赋值函数 Mutator
	BigInt& operator=(const BigInt&) = default;           // 拷贝赋值 Copy assignment
	BigInt& operator=(BigInt&&) noexcept;        // 移动赋值 Move-assignment
	
	// 比较 Compare
	friend bool operator==(const BigInt&, const BigInt&);
	friend bool operator<(const BigInt&, const BigInt&);
	
	// 位操作符 Bit operations
	BigInt operator<<(uint64_t) const;            // 左移，帮助实现乘法运算 Left shift, helping implementing multiplication
	BigInt operator>>(uint64_t) const;            // 类似无符号右移
	friend BigInt bit_operation(const BigInt&, const BigInt&, BitOperate);
	
	// 类型转换 Type converter
	explicit operator bool() const;

	uint64_t bit_size() const;     // 返回二进制有效位数
	
	// 算术
	friend BigInt operator+(const BigInt&, const BigInt&);
	friend BigInt operator-(const BigInt&, const BigInt&);
	friend std::pair<BigInt, BigInt> divide_with_remainder(const BigInt &, const BigInt &);
	friend BigInt operator*(const BigInt&, const BigInt&);
	// friend BigInt operator/(const BigInt&, const BigInt&);        // 待实现除法操作
	
	// 输出 
	friend std::ostream& operator<<(std::ostream&, const BigInt&);          // 输出16进制形式
	
private:
	std::deque<uint64_t> number;       // 无符号大整数，表示绝对值，秩从小到大对应低位到高位
	bool sign;                // 符号，指示正(true)负(false)
};

// 比较操作 Comparison operation 
bool operator==(const BigInt&, const BigInt&);
bool operator!=(const BigInt&, const BigInt&);
bool operator<(const BigInt&, const BigInt&);
bool operator>(const BigInt&, const BigInt&);
bool operator<=(const BigInt&, const BigInt&);
bool operator>=(const BigInt&, const BigInt&);

// 位操作 Bit operations
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);

// 算术操作 Arithmetical operation
BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);

std::ostream& operator<<(std::ostream&, const BigInt&);


#endif      // BIGINT_HPP
