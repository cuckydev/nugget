/*

MIT License

Copyright (c) 2021 PCSX-Redux authors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <stdlib.h>

#include "common/compiler/stdint.h"
#include "common/hardware/pcsxhw.h"
#include "common/syscalls/syscalls.h"
#include "openbios/patches/patches.h"

#define JRRA 0x03e00008

int g_patch_permissive = 0;

static const uint32_t hash_prime = 0xb503198f;

static inline uint32_t hashone(uint32_t a) {
    a = (a ^ 61) ^ (a >> 16);
    a += (a << 3);
    a ^= (a >> 4);
    a *= 0x27d4eb2f;
    a ^= (a >> 15);
    return a;
}

static inline uint32_t hash(const uint32_t* ptr, uint32_t mask, unsigned len) {
    uint32_t hash = 0x5810d659;

    while (len--) {
        uint32_t n = *ptr++;
        switch (mask & 3) {
            case 1:
                n &= 0xffff0000;
                break;
            case 2:
            case 3:
                n &= 0xfc000000;
                break;
        }
        mask >>= 2;

        hash += hashone(n);
        hash *= hash_prime;
    }

    return hash;
}

struct patch {
    uint32_t hash;
    void (*execute)(uint32_t* ra);
    const char* name;
};

void remove_ChgclrPAD_execute(uint32_t* ra);
void patch_pad_execute(uint32_t* ra);

// We will need to reduce these to the least common denominator,
// as we make progress on implementing new patches. Note that this
// alters the hashes.
static const uint32_t generic_hash_mask = 0x14880000;
static const unsigned generic_hash_len = 16;

static struct patch B0patches[] = {{
    .hash = 0x54d4eeaa,
    .execute = remove_ChgclrPAD_execute,
    .name = "_remove_ChgclrPAD",
}};
static struct patch C0patches[] = {};

void patch_hook(uint32_t* ra, enum patch_table table) {
    // already patched, bail out
    if ((ra[0] == JRRA) && (ra[1] == 0)) return;

    uint32_t h = hash(ra, generic_hash_mask, generic_hash_len);

    struct patch* patches = NULL;
    unsigned size = 0;
    char t = 'x';
    switch (table) {
        case PATCH_TABLE_B0:
            patches = B0patches;
            size = sizeof(B0patches) / sizeof(struct patch);
            t = 'B';
            break;
        case PATCH_TABLE_C0:
            patches = C0patches;
            size = sizeof(C0patches) / sizeof(struct patch);
            t = 'C';
            break;
    }

    while (size--) {
        if (patches->hash == h) {
            romsyscall_printf("Found %c0 patch hash %08x \"%s\", issued from %p, executing...\n", t, h, patches->name, ra);
            patches->execute(ra);
            ra[0] = JRRA;
            ra[1] = 0;
            return;
        }
    }

    romsyscall_printf("Couldn't find %c0 patch hash %08x issued from %p!\n", t, h, ra);
    if (g_patch_permissive) {
        romsyscall_printf("Permissive mode activated, continuing.\n", t, h);
    } else {
        romsyscall_printf("Stopping.\n", t, h);
        enterCriticalSection();
        pcsx_debugbreak();
        while (1)
            ;
    }
}
