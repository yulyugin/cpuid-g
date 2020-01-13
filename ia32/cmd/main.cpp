/* Print CPUID the right way
 *
 * Copyright (c) 2014, Evgeny Yulyugin and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * The names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <getopt.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include "cpuid-g.h"

using namespace std;

static void
print_help() {
    cout << "cpuid--g\n\n";
    cout << "USAGE: cpuid-g [options]\n\n";
    cout << "Options:\n";
    cout << "\t-h, --help\tPrint usage and exit.\n";
    cout << "\t-l, --leaf\tPrint just this leaf\n";
    cout << "\t-s, --subleaf\tUse this particular subleaf\n";
}

int main(int argc, char **argv) {
    // Parse command line arguments
    int opt = 0, opt_idx = 0;
    const char *short_options = "hl:s:";
    uint32_t leaf = 0xffffffff, subleaf = 0xffffffff;
    static struct option long_opt[] = {
        {"help", no_argument, NULL, 'h'},
        {"leaf", required_argument, NULL, 'l'},
        {"subleaf", required_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };
    while ((opt = getopt_long(argc, argv, short_options,
                              long_opt, &opt_idx)) != -1) {
        switch (opt) {
            case 'l':
                leaf = strtol(optarg, NULL, 16);
                break;
            case 's':
                subleaf = strtol(optarg, NULL, 16);
                break;
            case '?':
                cout << "Use -h, --help options to get usage.\n";
                return 0;
            case 'h':
            default:
                print_help();
                return 0;
        }
    }

    cout << "Leaf           Subleaf         EAX         EBX        ECX          EDX\n";
    cout << "----------------------------------------------------------------------\n";

    if (leaf != 0xffffffff) {
        if (subleaf != 0xffffffff) {
            cout << cpuid_subleaf(leaf, subleaf);
        } else {
            cout << cpuid_leaf(leaf);
        }
    } else {
        cout << cpuid_all();
    }

    return 0;
}
