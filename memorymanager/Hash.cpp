#include "Hash.h"

unsigned int Hash::hash(const void* Key, BYTE Length, const unsigned int Modifier)
{
	// fix C4100: unreferenced formal parameter
	UNREFERENCED_PARAMETER(Length);
	UNREFERENCED_PARAMETER(Key);
	UNREFERENCED_PARAMETER(Modifier);

    unsigned int a, b, c;                                          /* internal state */
    union { const void* ptr; size_t i; } u;     /* needed for Mac Powerbook G4 */

    /* Set up the internal state */
    a = b = c = 0xdeadbeef + static_cast<unsigned int>(Length) + Modifier;

    u.ptr = Key;
    if ((u.i & 0x3) == 0) {
        const unsigned int* k = static_cast<const unsigned int*>(Key);         /* read 32-bit chunks */

        /*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
        while (Length > 12)
        {
            a += k[0];
            b += k[1];
            c += k[2];
            mix(a, b, c);
            Length -= 12;
            k += 3;
        }

        /*----------------------------- handle the last (probably partial) block */
        /*
         * "k[2]&0xffffff" actually reads beyond the end of the string, but
         * then masks off the part it's not allowed to read.  Because the
         * string is aligned, the masked-off tail is in the same word as the
         * rest of the string.  Every machine with memory protection I've seen
         * does it on word boundaries, so is OK with this.  But VALGRIND will
         * still catch it and complain.  The masking trick does make the hash
         * noticably faster for short strings (like English words).
         */
#ifndef VALGRIND

        switch (Length)
        {
        case 12: c += k[2]; b += k[1]; a += k[0]; break;
        case 11: c += k[2] & 0xffffff; b += k[1]; a += k[0]; break;
        case 10: c += k[2] & 0xffff; b += k[1]; a += k[0]; break;
        case 9: c += k[2] & 0xff; b += k[1]; a += k[0]; break;
        case 8: b += k[1]; a += k[0]; break;
        case 7: b += k[1] & 0xffffff; a += k[0]; break;
        case 6: b += k[1] & 0xffff; a += k[0]; break;
        case 5: b += k[1] & 0xff; a += k[0]; break;
        case 4: a += k[0]; break;
        case 3: a += k[0] & 0xffffff; break;
        case 2: a += k[0] & 0xffff; break;
        case 1: a += k[0] & 0xff; break;
        case 0: return c;              /* zero length strings require no mixing */
        }

#else /* make valgrind happy */

        k8 = (const uint8_t*)k;
        switch (length)
        {
        case 12: c += k[2]; b += k[1]; a += k[0]; break;
        case 11: c += ((uint32_t)k8[10]) << 16;  /* fall through */
        case 10: c += ((uint32_t)k8[9]) << 8;    /* fall through */
        case 9: c += k8[8];                   /* fall through */
        case 8: b += k[1]; a += k[0]; break;
        case 7: b += ((uint32_t)k8[6]) << 16;   /* fall through */
        case 6: b += ((uint32_t)k8[5]) << 8;    /* fall through */
        case 5: b += k8[4];                   /* fall through */
        case 4: a += k[0]; break;
        case 3: a += ((uint32_t)k8[2]) << 16;   /* fall through */
        case 2: a += ((uint32_t)k8[1]) << 8;    /* fall through */
        case 1: a += k8[0]; break;
        case 0: return c;
        }

#endif /* !valgrind */

    }
    else if ((u.i & 0x1) == 0) {
        const unsigned short* k = static_cast<const unsigned short*>(Key);         /* read 16-bit chunks */
        const unsigned char* k8;

        /*--------------- all but last block: aligned reads and different mixing */
        while (Length > 12)
        {
            a += k[0] + (static_cast<unsigned int>(k[1]) << 16);
            b += k[2] + (static_cast<unsigned int>(k[3]) << 16);
            c += k[4] + (static_cast<unsigned int>(k[5]) << 16);
            mix(a, b, c);
            Length -= 12;
            k += 6;
        }

        /*----------------------------- handle the last (probably partial) block */
        k8 = reinterpret_cast<const unsigned char*>(k);
        switch (Length)
        {
        case 12: c += k[4] + (static_cast<unsigned int>(k[5]) << 16);
            b += k[2] + (static_cast<unsigned int>(k[3]) << 16);
            a += k[0] + (static_cast<unsigned int>(k[1]) << 16);
            break;
        case 11: c += (static_cast<unsigned int>(k8[10]) << 16);     /* fall through */
        case 10: c += k[4];
            b += k[2] + (static_cast<unsigned int>(k[3]) << 16);
            a += k[0] + (static_cast<unsigned int>(k[1]) << 16);
            break;
        case 9: c += k8[8];                      /* fall through */
        case 8: b += k[2] + (static_cast<unsigned int>(k[3]) << 16);
            a += k[0] + (static_cast<unsigned int>(k[1]) << 16);
            break;
        case 7: b += (static_cast<unsigned int>(k8[6]) << 16);      /* fall through */
        case 6: b += k[2];
            a += k[0] + (static_cast<unsigned int>(k[1]) << 16);
            break;
        case 5: b += k8[4];                      /* fall through */
        case 4: a += k[0] + (static_cast<unsigned int>(k[1]) << 16);
            break;
        case 3: a += (static_cast<unsigned int>(k8[2]) << 16);      /* fall through */
        case 2: a += k[0];
            break;
        case 1: a += k8[0];
            break;
        case 0: return c;                     /* zero length requires no mixing */
        }

    }
    else {                        /* need to read the key one byte at a time */
        const unsigned char* k = static_cast<const unsigned char*>(Key);

        /*--------------- all but the last block: affect some 32 bits of (a,b,c) */
        while (Length > 12)
        {
            a += k[0];
            a += static_cast<unsigned int>(k[1]) << 8;
            a += static_cast<unsigned int>(k[2]) << 16;
            a += static_cast<unsigned int>(k[3]) << 24;
            b += k[4];
            b += static_cast<unsigned int>(k[5]) << 8;
            b += static_cast<unsigned int>(k[6]) << 16;
            b += static_cast<unsigned int>(k[7]) << 24;
            c += k[8];
            c += static_cast<unsigned int>(k[9]) << 8;
            c += static_cast<unsigned int>(k[10]) << 16;
            c += static_cast<unsigned int>(k[11]) << 24;
            mix(a, b, c);
            Length -= 12;
            k += 12;
        }

        /*-------------------------------- last block: affect all 32 bits of (c) */
        switch (Length)                   /* all the case statements fall through */
        {
        case 12: c += static_cast<unsigned int>(k[11]) << 24;
        case 11: c += static_cast<unsigned int>(k[10]) << 16;
        case 10: c += static_cast<unsigned int>(k[9]) << 8;
        case 9: c += k[8];
        case 8: b += static_cast<unsigned int>(k[7]) << 24;
        case 7: b += static_cast<unsigned int>(k[6]) << 16;
        case 6: b += static_cast<unsigned int>(k[5]) << 8;
        case 5: b += k[4];
        case 4: a += static_cast<unsigned int>(k[3]) << 24;
        case 3: a += static_cast<unsigned int>(k[2]) << 16;
        case 2: a += static_cast<unsigned int>(k[1]) << 8;
        case 1: a += k[0];
            break;
        case 0: return c;
        }
    }

    final(a, b, c);
    return c;

}

unsigned int Hash::rot(unsigned int x, unsigned int k)
{
    return ((x) << (k) | (x) >> (32 - (k)));
}

void Hash::mix(unsigned int& a, unsigned int& b, unsigned int& c)
{
  a -= c;  a ^= rot(c, 4);  c += b; 
  b -= a;  b ^= rot(a, 6);  a += c; 
  c -= b;  c ^= rot(b, 8);  b += a; 
  a -= c;  a ^= rot(c,16);  c += b; 
  b -= a;  b ^= rot(a,19);  a += c; 
  c -= b;  c ^= rot(b, 4);  b += a; 
}

void Hash::final(unsigned int& a, unsigned int& b, unsigned int& c)
{
  c ^= b; c -= rot(b,14);
  a ^= c; a -= rot(c,11);
  b ^= a; b -= rot(a,25);
  c ^= b; c -= rot(b,16);
  a ^= c; a -= rot(c,4);
  b ^= a; b -= rot(a,14);
  c ^= b; c -= rot(b,24);
}