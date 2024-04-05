#include <stdint.h>
#include "pgmspace.h"
#define KYBER_Q 3329
#define QINV 62209
#define KYBER_N 256

const int16_t zetas[128] = {
        2285, 2571, 2970, 1812, 1493, 1422,  287, 202,  3158,  622, 1577,  182,  962, 2127, 1855, 1468,
        573, 2004,  264,  383, 2500, 1458, 1727, 3199, 2648, 1017,  732,  608, 1787,  411, 3124, 1758,
        1223,  652, 2777, 1015, 2036, 1491, 3047, 1785,  516, 3321, 3009, 2663, 1711, 2167,  126, 1469,
        2476, 3239, 3058,  830,  107, 1908, 3082, 2378, 2931,  961, 1821, 2604,  448, 2264,  677, 2054,
        2226,  430,  555,  843, 2078,  871, 1550,  105,  422,  587,  177, 3094, 3038, 2869, 1574, 1653,
        3083,  778, 1159, 3182, 2552, 1483, 2727, 1119, 1739,  644, 2457,  349,  418,  329, 3173, 3254,
        817, 1097,  603,  610, 1322, 2044, 1864, 384, 2114, 3193,  1218, 1994, 2455,  220, 2142, 1670,
        2144, 1799, 2051,  794, 1819, 2475, 2459, 478, 3221, 3021,   996,  991,  958, 1869, 1522, 1628
};





int16_t montgomery(int32_t a)
{
    int32_t t;
    int16_t u;

    u = (int16_t) a*QINV;
    t = (int32_t)u*KYBER_Q;
    t = a - t;
    t >>= 16;
    return (int16_t) t;
}

int16_t barrett_reduce(int16_t a) {
    int16_t t;
    const int16_t v = ((1U << 26) + KYBER_Q/2)/KYBER_Q;

    t  = (int32_t)v*a >> 26;
    t *= KYBER_Q;
    return a - t;
}


static int16_t fqmul(int16_t a, int16_t b) 
	{
    return montgomery((int32_t)a*b);
}
	

void ntt(int16_t r[256])
	{
    unsigned int len, start, j, k;
    int16_t t, zeta;
    k = 1;
		#pragma unroll_completely
    for (len = 128; len >= 2; len >>= 1) {
			#pragma unroll_completely
        for (start = 0; start < 256; start = j + len) {
            zeta = zetas[k++];
						#pragma unroll_completely
            for (j = start; j < start + len; ++j) {
                t = fqmul(zeta, r[j + len]);
                r[j + len] = r[j] - t;
                r[j] = r[j] + t;
            }
        }
    }
			for(j=0; j<KYBER_N; j++)
		r[j]=barrett_reduce(r[j]);
  }

int main(){

   int16_t poly_A[256] =
            {1344, 3325, 1821,  2175,  2841,   562,  2452,  1304,  3126,  3297,  193, 1281, 1726, 1865, 2920, 1688,  917, 1862, 1667, 2431,  776, 990,
             2842, 1060,  423,  2312,  2337,  2613,  3086,  3238,  1093,  2089, 3235,  573, 1923,  406, 1135, 1047, 2698,  932, 2003, 2891, 3201, 400,
             2415, 452,  3077,     3,  3302,  1415,    94,   749,    65,  2936, 1810,  488, 2907, 1806, 3101,  323, 1715,  865, 2413, 1621, 2427, 1995,
             233,  701,  2384,  2154,   364,  2934,  2026,   764,  2021,   137, 1500, 3012,  111,  575,  765,  860,  640,  372,  329, 2116, 3280, 2135,
             274,  522,   412,   346,  3131,  2839,     1,  2818,   732,   702, 2861,  545, 1066, 2467,  230, 2819, 2147,  368, 1978, 2818,  479, 212,
             1840, 1615, 2657,   627,  2554,  1463,   174,   488,  2973,   587,  834, 2776, 1085, 1823, 3253, 2805,  185,  444,   21, 2239,  570, 252,
             2717, 1608, 1367,  3195,  2087,  2567,   121,  2073,  1079,  1736, 1401, 1706,  962,  524, 1881, 2438, 1156,  127,  931, 1591, 2200, 2755,
             1515, 1677, 2940,  2948,  2686,  1850,   189,  2938,  2227,  2907, 2205,  265,  432, 1951, 2832,  553,  696, 1570, 3277, 2097, 3277, 1898,
             280,  2817, 1007,  1437,   603,  1939,  3028,  2803,  1365,  2203, 1151, 1964, 1822, 1497, 1473, 2011, 1106,  371, 2577, 3312, 1624, 3009,
             1934, 2116, 1221,   289,   357,  1170,    46,  1293,   727,   326, 1769, 1735, 2751, 2372, 1333,  110, 1847,  357, 2313,  657, 2321, 1794,
             1453, 1464,  920,  2813,   713,  1891,  2096,  1381,  1484,  1871,  262, 1774, 2229, 2420, 1820, 1181, 3147, 3134, 2951, 2541, 2557, 2982,
             545,  2667, 1500,   902,  2639,  3146,   882,  1104,  2327,  2717,  255, 1888, 3015, 2154};	
						
						ntt(poly_A);
						 
						 
	return 0;
}