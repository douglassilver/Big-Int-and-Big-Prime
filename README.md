# Big-Int-and-Big-Prime

Here is a simple implementation of large number class based on C++ STL deque. It support operation like addition, subtraction, division, multiplication and so on.
Unfortunately, it's not as efficient as the protogenetic big int algorithm in Python3. 

Thanks to the CLRS book, I can implement the BigPrimeEngine, a multi_threaded random large prime number generator based on Miller-Rabin test and the large number class.
Howerver, it runs quite slow now and take a long time to find a 1024 bits prime number.

After testing, multi-threading procedure runs faster than single-threading one only when it comes to 1024 bits prime number. So set sub-thread to 1 if you want a 512 or less bits prime number.
