/* Print CPUID the right way for IA-64 (Itanium)
 *
 * Copyright 2014, Grigory Rechistov
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <climits>

uint64_t do_cpuid(uint32_t leaf) {
    uint64_t res = 0x1122334455667788;
    __asm__ __volatile__ (
        "mov %0=cpuid[%1];; \n"
        : "=r" (res)
        : "r" (leaf)
        : 
        );

    return res;
}

int main(int argc, char **argv) {
    
    std::cout << "Leaf              Value" << std::endl;
    std::cout << "-----------------------" << std::endl;

    uint8_t max_leaf = UCHAR_MAX; // no limit is known at this point
    uint8_t leaf = 0;
    while (leaf <= max_leaf) {
        uint64_t result = do_cpuid(leaf);
        std::cout << std::setw(3) 
                  << +leaf 
                  << std::hex << std::showbase << std::setfill(' ') << std::setw(20)
                  << result 
                  << std::endl;
        if (leaf == 3) { // result[0:7] contains maximum supported leaf number
            max_leaf = (uint8_t)(result & 0xffull);
        }
        leaf++;
    }

    return 0;
}
