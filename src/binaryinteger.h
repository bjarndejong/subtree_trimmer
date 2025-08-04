#ifndef BINARYINTEGER_H_INCLUDED
#define BINARYINTEGER_H_INCLUDED

#include <vector>      // for std::vector
#include <cstdint>     // for uint64_t
#include <cstddef>

class BinaryInteger
{
private:
    static std::size_t number_of_blocks;
    std::vector<uint64_t> blocks;
public:
    static void set_number_of_blocks(size_t number_of_bits);

    BinaryInteger();
   
    void set_bit(int i);

   
    BinaryInteger& operator|=(const BinaryInteger& other);

   
    bool operator==(const BinaryInteger& other) const;
   
    BinaryInteger operator|(const BinaryInteger& other) const;

    bool is_subset_of(const BinaryInteger& other) const;

};

#endif // BINARYINTEGER_H_INCLUDED