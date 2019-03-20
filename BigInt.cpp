#include "BigInt.hpp"

// 算术操作辅助 Assistant to implement arithmetic
static bool abs_compare(const std::deque<uint64_t>&, const std::deque<uint64_t>&);
static std::deque<uint64_t> abs_add(const std::deque<uint64_t>&, const std::deque<uint64_t>&);           // 返回|a + b| Return abs(a + b)
static std::deque<uint64_t> abs_minus(const std::deque<uint64_t>&, const std::deque<uint64_t>&);         // 返回|a - b| Return abs(a - b)
// 求余辅助。返回尽量大的n使得a > (b << n) Assistant to calculate modulus, return maximum n which makes a > (
static uint64_t abs_max_lshift(const std::deque<uint64_t>&, const std::deque<uint64_t>&);

static bool abs_compare(const std::deque<uint64_t>& lhs, const std::deque<uint64_t>& rhs)       // 返回lhs < rhs
{
	if (lhs.size() < rhs.size()) return true;
	else if (lhs.size() > rhs.size()) return false;
	else {   // lhs.size() == rhs.size()
		for (int i = lhs.size() - 1; i >= 0; --i) {        // 从高位到低位依次比较
			if (lhs[i] < rhs[i]) return true;
			else if (lhs[i] > rhs[i]) return false;
		}
		return false;       // 二者相等
	}
}

static std::deque<uint64_t> abs_add(const std::deque<uint64_t>& lhs, const std::deque<uint64_t>& rhs)
{
	std::deque<uint64_t> ans;
	uint64_t carry_bit = 0;            // 记录进位
	for (int i = 0; i < lhs.size() && i < rhs.size(); ++i) {
		ans.push_back(lhs[i] + rhs[i] + carry_bit);
		if (lhs[i] + rhs[i] + carry_bit < lhs[i] || lhs[i] + rhs[i] + carry_bit < rhs[i]) carry_bit = 1;            // 检测上溢
		else carry_bit = 0;
	}
	// 最多执行其中一个for
	for (int i = lhs.size(); i < rhs.size(); ++i) {             // 若rhs更长（更长指deque有更高项）
		ans.push_back(rhs[i] + carry_bit);
		if (rhs[i] + carry_bit < rhs[i]) carry_bit = 1;               // 检测上溢
		else carry_bit = 0;
	}
	for (int i = rhs.size(); i < lhs.size(); ++i) {             // 若lhs更长
		ans.push_back(lhs[i] + carry_bit);
		if (lhs[i] + carry_bit < lhs[i]) carry_bit = 1;
		else carry_bit = 0;
	}
	if (carry_bit) ans.push_back(carry_bit);            // 最后一次加法可能有进位

	return ans;
}

static std::deque<uint64_t> abs_minus(const std::deque<uint64_t>& lhs, const std::deque<uint64_t>& rhs)
{
	if (abs_compare(lhs, rhs)) return abs_minus(rhs, lhs);        // lhs<rhs时
	// 此时lhs >= rhs

	std::deque<uint64_t> ans;
	uint64_t borrow_bit = 0;            // 记录借位
	for (int i = 0; i < rhs.size(); ++i) {
		ans.push_back(lhs[i] - borrow_bit - rhs[i]);
		if (lhs[i] - borrow_bit - rhs[i] > lhs[i] || (borrow_bit && rhs[i] + borrow_bit == 0)) borrow_bit = 1;         // 检测下溢
		else borrow_bit = 0;
	}
	for (int i = rhs.size(); i < lhs.size(); ++i) {          // 若lhs更长
		ans.push_back(lhs[i] - borrow_bit);
		if (lhs[i] - borrow_bit > lhs[i]) borrow_bit = 1;
		else borrow_bit = 0;
	}
	while (ans.size() > 1 && ans.back() == 0) ans.pop_back();         // 删除前缀0

	return ans;
}

static uint64_t abs_max_lshift(const std::deque<uint64_t>& lhs, const std::deque<uint64_t>& rhs)
{
	if (abs_compare(lhs, rhs)) return 0;    // 若lhs<rhs则返回0

	uint64_t lhs_top = 0, rhs_top = 0;         // 记录lhs与rhs的最高有效位位置
	for (int i = 63; i >= 0; --i) {
		if (lhs.back() & (1ll << i)) {
			lhs_top = (lhs.size() - 1) * 64 + i;
			break;
		}
	}
	for (int i = 63; i >= 0; --i) {
		if (rhs.back() & (1ll << i)) {
			rhs_top = (rhs.size() - 1) * 64 + i;
			break;
		}
	}
	return lhs_top - rhs_top > 1 ? lhs_top - rhs_top - 1 : 0;
}

BigInt::BigInt(): sign(true) { number.push_back(0); }

BigInt::BigInt(int64_t n): sign(n >= 0)
{
	number.push_back(std::abs(n));
}

BigInt::BigInt(const BigInt& num): number(num.number), sign(num.sign) { }

BigInt::BigInt(BigInt&& num) noexcept: number(std::move(num.number)), sign(num.sign) { }

BigInt& BigInt::operator=(const BigInt& num)
{
	number = num.number;
	sign = num.sign;
}

BigInt& BigInt::operator=(BigInt&& num) noexcept
{
	number = std::move(num.number);
	sign = num.sign;
}

bool operator==(const BigInt& lhs, const BigInt& rhs)
{
	if (lhs.sign != rhs.sign || lhs.number.size() != rhs.number.size()) return false;
	for (int i = lhs.number.size() - 1; i >= 0; --i) {
		if (lhs.number[i] != rhs.number[i]) return false;
	}
	return true;
}

bool operator!=(const BigInt& lhs, const BigInt& rhs) { return !(lhs == rhs); }

bool operator<(const BigInt& lhs, const BigInt& rhs)
{
	if (!lhs.sign && rhs.sign) return true;     // lhs为负，rhs为正
	else if (lhs.sign && !rhs.sign) return false;      // lhs为正，rhs为负
	else if (lhs.sign && rhs.sign) {               // lhs与rhs同为正
		if (lhs.number.size() < rhs.number.size()) return true;
		else if (lhs.number.size() > rhs.number.size()) return false;
		else {   // lhs.number.size() == rhs.number.size()
			for (int i = lhs.number.size() - 1; i >= 0; --i) {        // 从高位到低位依次比较 
				if (lhs.number[i] < rhs.number[i]) return true;
				else if (lhs.number[i] > rhs.number[i]) return false;
			}
			return false;       // 二者相等 
		}
	} else {    // !lhs.sign && !rhs.sign。与上一种情况对称
		if (lhs.number.size() < rhs.number.size()) return false;
		else if (lhs.number.size() > rhs.number.size()) return true;
		else {
			for (int i = lhs.number.size() - 1; i >= 0; --i) { 
				if (lhs.number[i] < rhs.number[i]) return false;
				else if (lhs.number[i] > rhs.number[i]) return true;
			}
			return false;
		}
	}
}

bool operator>(const BigInt& lhs, const BigInt& rhs) { return rhs < lhs; }

bool operator<=(const BigInt& lhs, const BigInt& rhs) { return !(lhs > rhs); }

bool operator>=(const BigInt& lhs, const BigInt& rhs) { return !(lhs < rhs); }

BigInt BigInt::operator<<(uint64_t shift) const
{
	BigInt ans(*this);
	const uint64_t s = shift % 64;         // 实际须移动的位数
	if (s) {          // 段内移位
		ans.number.push_back(ans.number.back() >> (64 - s));         // 从高位开始处理
		for (int i = ans.number.size() - 2; i > 0; --i) {
			ans.number[i] = (ans.number[i] << s) + (ans.number[i-1] >> (64 - s));
		}
		ans.number[0] <<= s;
	}
	for (int i = 0; i < shift / 64; ++i) ans.number.push_front(0);        // 整段移位
	while (ans.number.size() > 1 && ans.number.back() == 0) ans.number.pop_back();             // 去除前缀0

	return ans;
}

BigInt BigInt::operator>>(uint64_t shift) const
{
    BigInt ans(*this);
    for (int i = 0; i < shift / 64 && ans.number.size() > 1; ++i) ans.number.pop_front();
    const uint64_t s = shift % 64;
    if (s) {
        for (int i = 0; i < ans.number.size() - 1; ++i) {           // 从低位开始处理
            ans.number[i] = (ans.number[i] >> s) + (ans.number[i+1] << (64 - s));
        }
        ans.number.back() >>= s;
    }
    while (ans.number.size() > 1 && ans.number.back() == 0) ans.number.pop_back();             // 去除前缀0

    return ans;
}

uint64_t BigInt::bit_size() const
{
    int top = 63;
    while (!((number.back() >> top) & 1)) --top;
    return (number.size() - 1) * 64 + top + 1;
}

BigInt operator&(const BigInt& lhs, const BigInt& rhs)
{
	uint64_t common_size = lhs.number.size() <= rhs.number.size() ? lhs.number.size() : rhs.number.size();
	BigInt ans;
	ans.number[0] = lhs.number[0] & rhs.number[0];           // 最低字段初始化时已存在
	for (uint64_t i = 1; i < common_size; ++i) ans.number.push_back(lhs.number[i] & rhs.number[i]);

	while (ans.number.size() > 1 && ans.number.back() == 0) ans.number.pop_back();             // 去除前缀0

	return ans;
}

BigInt::operator bool() const
{
	return !(number.size() == 1 && number.front() == 0);           // 绝对值为0
}

uint64_t BigInt::count_ones() const
{
	uint64_t ans = 0;
	for (auto seg : number) {
		for (int i = 0; i < 64; ++i) {
			if (seg & (1 << i)) ++ans;
		}
	}
	return ans;
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs)
{
	BigInt ans;
	if (lhs.sign == rhs.sign) {        // 加数同号 
		ans.number = abs_add(lhs.number, rhs.number);
		ans.sign = lhs.sign;
	} else {         // 加数异号 
		if (lhs.sign) {
			ans.sign = !abs_compare(lhs.number, rhs.number);
			ans.number = abs_minus(lhs.number, rhs.number);
		} else {
			ans.sign = !abs_compare(rhs.number, lhs.number);
			ans.number = abs_minus(rhs.number, lhs.number);
		}
	}
	return ans;
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs)
{
	BigInt ans;
	if (lhs.sign != rhs.sign) {       // 对应绝对值相加 
		ans.sign = lhs.sign;
		ans.number = abs_add(lhs.number, rhs.number);
	} else {             // 对应绝对值相减 
		if (lhs.sign) {
			ans.sign = !abs_compare(lhs.number, rhs.number);
			ans.number = abs_minus(lhs.number, rhs.number);
		} else {
			ans.sign = !abs_compare(rhs.number, lhs.number);
			ans.number = abs_minus(rhs.number, lhs.number);
		}
	}
	return ans;
}

BigInt operator%(const BigInt& lhs, const BigInt& rhs)        // 由加减法实现 
{
	BigInt ans = lhs;
	uint64_t shift;
	if (rhs.sign) {        // ans应与rhs同号 
		if (ans.sign)  while (ans >= rhs) {
			shift = abs_max_lshift(ans.number, rhs.number);
			ans = ans - (rhs << shift);
		} else while (!ans.sign) {
			shift = abs_max_lshift(ans.number, rhs.number);
			ans = ans + (rhs << shift);
		}
	} else {
		if (!ans.sign) while (ans <= rhs) {
			shift = abs_max_lshift(ans.number, rhs.number);
			ans = ans - (rhs << shift);
		} else while (ans.sign) {
			shift = abs_max_lshift(ans.number, rhs.number);
			ans = ans + (rhs << shift);
		}
	}
	return ans;
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs)
{
	if (lhs == 0 || rhs == 0) return 0;                   // 特殊情况
	else if (lhs.count_ones() < rhs.count_ones()) return rhs * lhs;       // 减少所需的加法操作
	// 此时rhs二进制表示中有效位较少 
	
	BigInt ans(0), one(1);
	ans.sign = (lhs.sign == rhs.sign);     // 同号为正，异号为负
	
	uint64_t rhs_top_top = 63;              // rhs最高字段的最高有效位位置
	while (!(rhs.number.back() & (1ull << rhs_top_top))) --rhs_top_top;
	for (uint64_t i = 0; i <= (rhs.number.size() - 1) * 64 + rhs_top_top; ++i) {
		if (rhs & (one << i)) {
			ans = ans + (lhs << i);
		}
	}
	return ans;
}

std::ostream& operator<<(std::ostream& os, const BigInt& num)
{
	if (!num.sign) os << '-';           // 打印符号
	os << std::hex << num.number.back();
	std::ostringstream ostr;
	for (int i = num.number.size() - 2; i >= 0; --i) {
		os << ' ';          // 将输出按每64bit间隔输出
		ostr.str("");         //  清空ostringstream
		ostr << std::hex << num.number[i];
		int rest = 16 - ostr.str().size();          // 计算该段前缀0的个数
		while (rest--) os << '0';
		os << std::hex << num.number[i];
	}
	return os;
}
