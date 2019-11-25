#pragma once

#include "Platform.h"
#include "Types.h"

#include "MurmurHash1.h"
#include "MurmurHash2.h"
#include "MurmurHash3.h"
#include "PMurHash.h"

#define XXH_INLINE_ALL
#include "xxhash.h"

#include "metrohash.h"
#include "cmetrohash.h"
#include "opt_cmetrohash.h"

#include "fasthash.h"
#include "jody_hash32.h"
#include "jody_hash64.h"

//----------
// These are _not_ hash functions (even though people tend to use crc32 as one...)

void BadHash(const void *key, int len, uint32_t seed, void *out);
void sumhash(const void *key, int len, uint32_t seed, void *out);
void sumhash32(const void *key, int len, uint32_t seed, void *out);

void DoNothingHash(const void *key, int len, uint32_t seed, void *out);
void NoopOAATReadHash(const void *key, int len, uint32_t seed, void *out);
void crc32(const void *key, int len, uint32_t seed, void *out);

//----------
// Cryptographic hashes (but very bad in the used range)

void md5_32(const void *key, int len, uint32_t seed, void *out);
void sha1_32a(const void *key, int len, uint32_t seed, void *out);
#if 0
void sha1_64a              ( const void * key, int len, uint32_t seed, void * out );
void sha2_32a              ( const void * key, int len, uint32_t seed, void * out );
void sha2_64a              ( const void * key, int len, uint32_t seed, void * out );
void BLAKE2_32a            ( const void * key, int len, uint32_t seed, void * out );
void BLAKE2_64a            ( const void * key, int len, uint32_t seed, void * out );
void bcrypt_64a            ( const void * key, int len, uint32_t seed, void * out );
void scrypt_64a            ( const void * key, int len, uint32_t seed, void * out );
#endif

//----------
// General purpose hashes

#ifdef __SSE2__
void hasshe2_test(const void *key, int len, uint32_t seed, void *out);
#endif
#if defined(__SSE4_2__) && defined(__x86_64__)
void crc32c_hw_test(const void *key, int len, uint32_t seed, void *out);
void crc32c_hw1_test(const void *key, int len, uint32_t seed, void *out);
void crc64c_hw_test(const void *key, int len, uint32_t seed, void *out);
void CityHashCrc64_test(const void *key, int len, uint32_t seed, void *out);
void CityHashCrc128_test(const void *key, int len, uint32_t seed, void *out);
void falkhash_test_cxx(const void *key, int len, uint32_t seed, void *out);
#endif
size_t fibonacci(const char *key, int len, uint32_t seed);
inline void fibonacci_test(const void *key, int len, uint32_t seed, void *out) {
  *(size_t *)out = fibonacci((const char *)key, len, seed);
}
size_t FNV2(const char *key, int len, size_t seed);
inline void FNV2_test(const void *key, int len, uint32_t seed, void *out) {
  *(size_t *)out = FNV2((const char *)key, len, (size_t)seed);
}
uint32_t FNV32a(const void *key, int len, uint32_t seed);
inline void FNV32a_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *)out = FNV32a((const char *)key, len, seed);
}
uint32_t FNV32a_YoshimitsuTRIAD(const char *key, int len, uint32_t seed);
inline void FNV32a_YT_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *)out = FNV32a_YoshimitsuTRIAD((const char *)key, len, seed);
}
#ifdef HAVE_INT64
uint32_t FNV1A_Totenschiff(const char *key, int len, uint32_t seed);
inline void FNV1A_Totenschiff_test(const void *key, int len, uint32_t seed,
                                   void *out) {
  *(uint32_t *)out = FNV1A_Totenschiff((const char *)key, len, seed);
}
uint32_t FNV1A_Pippip_Yurii(const char *key, int wrdlen, uint32_t seed);
inline void FNV1A_PY_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *)out = FNV1A_Pippip_Yurii((const char *)key, len, seed);
}
#endif
uint64_t FNV64a(const char *key, int len, uint64_t seed);
inline void FNV64a_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint64_t *)out = FNV64a((const char *)key, len, (uint64_t)seed);
}
uint64_t fletcher2(const char *key, int len, uint64_t seed);
inline void fletcher2_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint64_t *) out = fletcher2((const char *)key, len, (uint64_t)seed);
}
uint64_t fletcher4(const char *key, int len, uint64_t seed);
inline void fletcher4_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint64_t *) out = fletcher2((const char *)key, len, (uint64_t)seed);
}
uint32_t Bernstein(const char *key, int len, uint32_t seed);
inline void Bernstein_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = Bernstein((const char *)key, len, seed);
}
uint32_t sdbm(const char *key, int len, uint32_t hash);
inline void sdbm_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = sdbm((const char *)key, len, seed);
}
uint32_t x17(const char *key, int len, uint32_t h);
inline void x17_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = x17((const char *)key, len, seed);
}
uint32_t JenkinsOOAT(const char *key, int len, uint32_t hash);
inline void JenkinsOOAT_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = JenkinsOOAT((const char *)key, len, seed);
}
uint32_t JenkinsOOAT_perl(const char *key, int len, uint32_t hash);
inline void JenkinsOOAT_perl_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = JenkinsOOAT_perl((const char *)key, len, seed);
}
uint32_t GoodOAAT(const char *key, int len, uint32_t hash);
inline void GoodOAAT_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = GoodOAAT((const char *)key, len, seed);
}
uint32_t MicroOAAT(const char *key, int len, uint32_t hash);
inline void MicroOAAT_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = MicroOAAT((const char *)key, len, seed);
}
uint32_t SuperFastHash (const char * data, int len, int32_t hash);
inline void SuperFastHash_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t*)out = SuperFastHash((const char*)key, len, seed);
}
uint32_t lookup3(const char *key, int len, uint32_t hash);
inline void lookup3_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = lookup3((const char *)key, len, seed);
}
uint32_t MurmurOAAT(const char *key, int len, uint32_t hash);
inline void MurmurOAAT_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = MurmurOAAT((const char *)key, len, seed);
}
uint32_t Crap8(const uint8_t * key, uint32_t len, uint32_t seed);
inline void Crap8_test(const void *key, int len, uint32_t seed, void *out) {
  *(uint32_t *) out = Crap8((const uint8_t *)key, len, seed);
}

void CityHash32_test(const void *key, int len, uint32_t seed, void *out);
void CityHash64noSeed_test(const void *key, int len, uint32_t seed, void *out);
void CityHash64_test(const void *key, int len, uint32_t seed, void *out);
inline void CityHash64_low_test(const void *key, int len, uint32_t seed, void *out) {
  uint64_t result;
  CityHash64_test(key, len, seed, &result);
  *(uint32_t *)out = (uint32_t)result;
}
void CityHash128_test(const void *key, int len, uint32_t seed, void *out);
// objsize: eb0-3b91: 11489 (mult. variants per len)
void FarmHash32_test       ( const void * key, int len, uint32_t seed, void * out );
// objsize: 0-eae: 3758 (mult. variants per len)
void FarmHash64_test       ( const void * key, int len, uint32_t seed, void * out );
void FarmHash64noSeed_test ( const void * key, int len, uint32_t seed, void * out );
// objsize: 44a0-4543: 163
void FarmHash128_test      ( const void * key, int len, uint32_t seed, void * out );
// objsize: 0x2c70-0x2f6a farmhash32_su_with_seed
void farmhash32_c_test     ( const void * key, int len, uint32_t seed, void * out );
// objsize: 4a20-4a82/5b0-5fd/660-1419: 3688 farmhash64_na_with_seeds
void farmhash64_c_test     ( const void * key, int len, uint32_t seed, void * out );
// objsize: 4140-48a2: 1890
void farmhash128_c_test    ( const void * key, int len, uint32_t seed, void * out );

// all 3 using the same Hash128
// objsize: 0-8ad: 2221
void SpookyHash32_test     ( const void * key, int len, uint32_t seed, void * out );
void SpookyHash64_test     ( const void * key, int len, uint32_t seed, void * out );
void SpookyHash128_test    ( const void * key, int len, uint32_t seed, void * out );

//----------
// Used internally as C++
uint32_t MurmurOAAT ( const char * key, int len, uint32_t seed );

// MurmurHash2
void MurmurHash2_test      ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash2A_test     ( const void * key, int len, uint32_t seed, void * out );

void siphash_test          ( const void * key, int len, uint32_t seed, void * out );
void siphash13_test        ( const void * key, int len, uint32_t seed, void * out );
void halfsiphash_test      ( const void * key, int len, uint32_t seed, void * out );

//-----------------------------------------------------------------------------
// Test harnesses for Murmur1/2

inline void MurmurHash1_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint32_t*)out = MurmurHash1(key,len,seed);
}

inline void MurmurHash2_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint32_t*)out = MurmurHash2(key,len,seed);
}

inline void MurmurHash2A_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint32_t*)out = MurmurHash2A(key,len,seed);
}

#if __WORDSIZE >= 64
inline void MurmurHash64A_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint64_t*)out = MurmurHash64A(key,len,seed);
}
#endif
#ifdef HAVE_INT64
inline void MurmurHash64B_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint64_t*)out = MurmurHash64B(key,len,seed);
}
#endif

inline void jodyhash32_test( const void * key, int len, uint32_t seed, void * out ) {
  *(uint32_t*)out = jody_block_hash32((const jodyhash32_t *)key, (jodyhash32_t) seed, (size_t) len);
}
#ifdef HAVE_INT64
inline void jodyhash64_test( const void * key, int len, uint32_t seed, void * out ) {
  *(uint64_t*)out = jody_block_hash((const jodyhash_t *)key, (jodyhash_t) seed, (size_t) len);
}
#endif

inline void xxHash32_test( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 10-104 + 3e0-5ce: 738
  *(uint32_t*)out = (uint32_t) XXH32(key, (size_t) len, (unsigned) seed);
}
#ifdef HAVE_INT64
inline void xxHash64_test( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 630-7fc + c10-1213: 1999
  *(uint64_t*)out = (uint64_t) XXH64(key, (size_t) len, (unsigned long long) seed);
}
#endif

#define restrict // oddly enough, seems to choke on this keyword
#include "xxh3.h"

#ifdef HAVE_INT64
inline void xxh3_test( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 12d0-15b8: 744
  *(uint64_t*)out = (uint64_t) XXH3_64bits_withSeed(key, (size_t) len, seed);
}
#endif

inline void xxh3low_test( const void * key, int len, uint32_t seed, void * out ) {
  (void)seed;
  // objsize 12d0-15b8: 744 + 1f50-1f5c: 756
  *(uint32_t*)out = (uint32_t) XXH3_64bits(key, (size_t) len);
}

#ifdef HAVE_INT64
inline void xxh128_test( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 1f60-2354: 1012
  *(XXH128_hash_t*)out = XXH128(key, (size_t) len, seed);
}

inline void xxh128low_test( const void * key, int len, uint32_t seed, void * out ) {
  *(uint64_t*)out = (uint64_t) (XXH128(key, (size_t) len, seed).low64);
}


inline void metrohash64_1_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 0-270: 624
  metrohash64_1((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
inline void metrohash64_2_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 270-4e3: 627
  metrohash64_2((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
inline void metrohash128_1_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 0-305: 773
  metrohash128_1((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
inline void metrohash128_2_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 310-615: 773
  metrohash128_2((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
#if defined(__SSE4_2__) && defined(__x86_64__)
inline void metrohash64crc_1_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 0-278: 632
  metrohash64crc_1((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
inline void metrohash64crc_2_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 280-4f8: 632
  metrohash64crc_2((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
inline void metrohash128crc_1_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 0-2d3: 723
  metrohash128crc_1((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
inline void metrohash128crc_2_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 2e0-5b3: 723
  metrohash128crc_2((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
#endif
inline void cmetrohash64_1_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 0-28c: 652
  cmetrohash64_1((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
inline void cmetrohash64_1_optshort_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 0-db2: 3506
  cmetrohash64_1_optshort((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
inline void cmetrohash64_2_test ( const void * key, int len, uint32_t seed, void * out ) {
  // objsize 290-51f: 655
  cmetrohash64_2((const uint8_t *)key,(uint64_t)len,seed,(uint8_t *)out);
}
#endif

inline void fasthash32_test ( const void * key, int len, uint32_t seed, void * out ) {
  *(uint32_t*)out = fasthash32(key, (size_t) len, seed);
}
#ifdef HAVE_INT64
inline void fasthash64_test ( const void * key, int len, uint32_t seed, void * out ) {
  *(uint64_t*)out = fasthash64(key, (size_t) len, (uint64_t)seed);
}
#endif

// objsize 0-778: 1912
void mum_hash_test(const void * key, int len, uint32_t seed, void * out);

inline void mum_low_test ( const void * key, int len, uint32_t seed, void * out ) {
  uint64_t result;
  mum_hash_test(key, len, seed, &result);
  *(uint32_t*)out = (uint32_t)result;
}


//-----------------------------------------------------------------------------

#define T1HA0_RUNTIME_SELECT 0
#ifdef HAVE_AESNI
# define T1HA0_AESNI_AVAILABLE 1
#else
# define T1HA0_AESNI_AVAILABLE 0
#endif
#include "t1ha.h"

inline void t1ha2_atonce_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize 0-21d: 541
  *(uint64_t*)out = t1ha2_atonce(key, len, seed);
}

inline void t1ha2_stream_test(const void * key, int len, uint32_t seed, void * out)
{
  t1ha_context_t ctx;
  // objsize 570-bf1: 1665
  t1ha2_init(&ctx, seed, 0);
  t1ha2_update(&ctx, key, len);
  *(uint64_t*)out = t1ha2_final(&ctx, NULL);
}

inline void t1ha2_atonce128_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize b50-db5: 613
  *(uint64_t*)out = t1ha2_atonce128((uint64_t*)out + 1, key, len, seed);
}

inline void t1ha2_stream128_test(const void * key, int len, uint32_t seed, void * out)
{
  t1ha_context_t ctx;
  // objsize e20-14a1: 1665
  t1ha2_init(&ctx, seed, 0);
  t1ha2_update(&ctx, key, len);
  *(uint64_t*)out = t1ha2_final(&ctx, (uint64_t*)out + 1);
}

inline void t1ha1_64le_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize 0-205: 517
  *(uint64_t*)out = t1ha1_le(key, len, seed);
}

inline void t1ha1_64be_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize 280-4ab: 555
  *(uint64_t*)out = t1ha1_be(key, len, seed);
}

inline void t1ha0_32le_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize 0-1fd: 509
  *(uint64_t*)out = t1ha0_32le(key, len, seed);
}

inline void t1ha0_32be_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize 250-465: 533
  *(uint64_t*)out = t1ha0_32be(key, len, seed);
}

#if T1HA0_AESNI_AVAILABLE
#ifndef _MSC_VER
inline void t1ha0_ia32aes_noavx_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize 0-39d: 925
  *(uint64_t*)out = t1ha0_ia32aes_noavx(key, len, seed);
}
#endif
#if defined(__AVX__)
inline void t1ha0_ia32aes_avx1_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize 0-34b: 843
  *(uint64_t*)out = t1ha0_ia32aes_avx(key, len, seed);
}
#endif /* __AVX__ */
#if defined(__AVX2__)
inline void t1ha0_ia32aes_avx2_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize 0-318: 792
  *(uint64_t*)out = t1ha0_ia32aes_avx2(key, len, seed);
}
#endif /* __AVX2__ */
#endif /* T1HA0_AESNI_AVAILABLE */

#if defined(__SSE4_2__) && defined(__x86_64__)
#include "clhash.h"
void clhash_init();
void clhash_test (const void * key, int len, uint32_t seed, void * out);
#endif

void multiply_shift (const void * key, int len, uint32_t seed, void * out);
void pair_multiply_shift (const void *key, int len, uint32_t seed, void *out);

void HighwayHash_init();
// objsize 20-a12: 2546
void HighwayHash64_test (const void * key, int len, uint32_t seed, void * out);

#ifdef HAVE_INT64
//https://github.com/wangyi-fudan/wyhash
//#define WYHASH_EVIL_FAST
#include "wyhash.h"
// objsize 19c0-1f1d: 1373
inline void wyhash_test (const void * key, int len, uint32_t seed, void * out) {
  *(uint64_t*)out = wyhash(key, (uint64_t)len, (uint64_t)seed);
}
inline void wyhash32low (const void * key, int len, uint32_t seed, void * out) {
  *(uint32_t*)out = 0xFFFFFFFF & wyhash(key, (uint64_t)len, (uint64_t)seed);
}

//https://github.com/vnmakarov/mir/blob/master/mir-hash.h
#include "mir-hash.h"
inline void mirhash_test (const void * key, int len, uint32_t seed, void * out) {
  // objsize 2950-2da8: 1112
  *(uint64_t*)out = mir_hash(key, (uint64_t)len, (uint64_t)seed);
}
inline void mirhash32low (const void * key, int len, uint32_t seed, void * out) {
  *(uint32_t*)out = 0xFFFFFFFF & mir_hash(key, (uint64_t)len, (uint64_t)seed);
}
inline void mirhashstrict_test (const void * key, int len, uint32_t seed, void * out) {
  // objsize 2950-2da8: 1112
  *(uint64_t*)out = mir_hash_strict(key, (uint64_t)len, (uint64_t)seed);
}
inline void mirhashstrict32low (const void * key, int len, uint32_t seed, void * out) {
  *(uint32_t*)out = 0xFFFFFFFF & mir_hash_strict(key, (uint64_t)len, (uint64_t)seed);
}

//TODO MSVC
#ifndef _MSC_VER
void tsip_init();
void tsip_test (const void * key, int len, uint32_t seed, void * out);
// objsize 0-207: 519
extern "C" uint64_t tsip(const unsigned char *seed, const unsigned char *m, uint64_t len);

extern "C" uint64_t seahash(const char *key, int len, uint64_t seed);
// objsize 29b0-2d17: 871
inline void seahash_test (const void *key, int len, uint32_t seed, void *out) {
  *(uint64_t*)out = seahash((const char *)key, len, (uint64_t)seed);
}
inline void seahash32low (const void *key, int len, uint32_t seed, void *out) {
  uint64_t result = seahash((const char *)key, len, (uint64_t)seed);
  *(uint32_t*)out = (uint32_t)(UINT64_C(0xffffffff) & result);
}
#endif /* !MSVC */
#endif /* HAVE_INT64 */

#include "tomcrypt.h"
#ifndef _MAIN_CPP
extern
#endif
       hash_state ltc_state;

int blake2b_init(hash_state * md, unsigned long outlen,
                 const unsigned char *key, unsigned long keylen);
inline void blake2b160_test(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  blake2b_init(&ltc_state, 20, NULL, 0);
  ltc_state.blake2b.h[0] = CONST64(0x6a09e667f3bcc908) ^ seed; // mix seed into lowest int
  blake2b_process(&ltc_state, (unsigned char *)key, len);
  blake2b_done(&ltc_state, (unsigned char *)out);
}
inline void blake2b224_test(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  blake2b_init(&ltc_state, 28, NULL, 0);
  ltc_state.blake2b.h[0] = CONST64(0x6a09e667f3bcc908) ^ seed;
  blake2b_process(&ltc_state, (unsigned char *)key, len);
  blake2b_done(&ltc_state, (unsigned char *)out);
}
inline void blake2b256_test(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  blake2b_init(&ltc_state, 32, NULL, 0);
  ltc_state.blake2b.h[0] = CONST64(0x6a09e667f3bcc908) ^ seed;
  blake2b_process(&ltc_state, (unsigned char *)key, len);
  blake2b_done(&ltc_state, (unsigned char *)out);
}
inline void blake2b256_64(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  unsigned char buf[32];
  blake2b_init(&ltc_state, 32, NULL, 0);
  ltc_state.blake2b.h[0] = CONST64(0x6a09e667f3bcc908) ^ seed;
  blake2b_process(&ltc_state, (unsigned char *)key, len);
  blake2b_done(&ltc_state, buf);
  memcpy(out, buf, 8);
}
int blake2s_init(hash_state * md, unsigned long outlen,
                 const unsigned char *key, unsigned long keylen);
inline void blake2s128_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize
  blake2s_init(&ltc_state, 16, NULL, 0);
  ltc_state.blake2s.h[0] = 0x6A09E667UL ^ seed;
  blake2s_process(&ltc_state, (unsigned char *)key, len);
  blake2s_done(&ltc_state, (unsigned char *)out);
}
inline void blake2s160_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize
  blake2s_init(&ltc_state, 20, NULL, 0);
  ltc_state.blake2s.h[0] = 0x6A09E667UL ^ seed;
  blake2s_process(&ltc_state, (unsigned char *)key, len);
  blake2s_done(&ltc_state, (unsigned char *)out);
}
inline void blake2s224_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize
  blake2s_init(&ltc_state, 28, NULL, 0);
  ltc_state.blake2s.h[0] = 0x6A09E667UL ^ seed;
  blake2s_process(&ltc_state, (unsigned char *)key, len);
  blake2s_done(&ltc_state, (unsigned char *)out);
}
inline void blake2s256_test(const void * key, int len, uint32_t seed, void * out)
{
  // objsize
  blake2s_init(&ltc_state, 32, NULL, 0);
  ltc_state.blake2s.h[0] = 0x6A09E667UL ^ seed;
  blake2s_process(&ltc_state, (unsigned char *)key, len);
  blake2s_done(&ltc_state, (unsigned char *)out);
}
inline void sha2_224(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  unsigned char buf[28];
  sha224_init(&ltc_state);
  ltc_state.sha256.state[0] = 0xc1059ed8UL ^ seed;
  sha224_process(&ltc_state, (unsigned char *)key, len);
  sha224_done(&ltc_state, (unsigned char *)out);
}
inline void sha2_224_64(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  unsigned char buf[28];
  sha224_init(&ltc_state);
  ltc_state.sha256.state[0] = 0xc1059ed8UL ^ seed;
  sha224_process(&ltc_state, (unsigned char *)key, len);
  sha224_done(&ltc_state, buf);
  memcpy(out, buf, 8);
}
inline void sha2_256(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  sha256_init(&ltc_state);
  ltc_state.sha256.state[0] = 0xc1059ed8UL ^ seed;
  sha256_process(&ltc_state, (unsigned char *)key, len);
  sha256_done(&ltc_state, (unsigned char *)out);
}
inline void sha2_256_64(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  unsigned char buf[32];
  sha256_init(&ltc_state);
  ltc_state.sha256.state[0] = 0xc1059ed8UL ^ seed;
  sha256_process(&ltc_state, (unsigned char *)key, len);
  sha256_done(&ltc_state, buf);
  memcpy(out, buf, 8);
}
inline void rmd128(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  rmd128_init(&ltc_state);
  ltc_state.rmd128.state[0] = 0x67452301UL ^ seed;
  rmd128_process(&ltc_state, (unsigned char *)key, len);
  rmd128_done(&ltc_state, (unsigned char *)out);
}
inline void rmd160(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  rmd160_init(&ltc_state);
  ltc_state.rmd160.state[0] = 0x67452301UL ^ seed;
  rmd160_process(&ltc_state, (unsigned char *)key, len);
  rmd160_done(&ltc_state, (unsigned char *)out);
}
inline void rmd256(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  rmd256_init(&ltc_state);
  ltc_state.rmd256.state[0] = 0x67452301UL ^ seed;
  rmd256_process(&ltc_state, (unsigned char *)key, len);
  rmd256_done(&ltc_state, (unsigned char *)out);
}
inline void sha3_256_64(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  unsigned char buf[32];
  sha3_256_init(&ltc_state);
  ltc_state.sha3.s[0] = CONST64(1) ^ seed;
  sha3_process(&ltc_state, (unsigned char *)key, len);
  sha3_done(&ltc_state, buf);
  memcpy(out, buf, 8);
}
inline void sha3_256(const void *key, int len, uint32_t seed, void *out)
{
  // objsize
  unsigned char buf[32];
  sha3_256_init(&ltc_state);
  ltc_state.sha3.s[0] = CONST64(1) ^ seed;
  sha3_process(&ltc_state, (unsigned char *)key, len);
  sha3_done(&ltc_state, (unsigned char *)out);
}
