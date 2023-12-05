int countLeadingZeros(uint64_t x) {
    int count = 0;
    for (int i = 63; i >= 0; i--) {
        if ((x >> i) & 1) {
            break;
        }
        count++;
    }
    return count;
}

int main() {
    // Hard-coded input values for testing

    // Calculate leading zeros
    uint64_t x = 5;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    x |= (x >> 32);

    x -= ((x >> 1) & 0x5555555555555555);
    x = ((x >> 2) & 0x3333333333333333) + (x & 0x3333333333333333);
    x = ((x >> 4) + x) & 0x0f0f0f0f0f0f0f0f;
    x = (x * 0x200040008001ULL & 0x1111111111111111);

    int leading_zero = 64 - countLeadingZeros(x);
    int priority = 4 - 1 - leading_zero;

    // Write the results to specified memory locations
    *((volatile int *) (4)) = leading_zero;
    *((volatile int *) (8)) = priority;

    return 0;
}
