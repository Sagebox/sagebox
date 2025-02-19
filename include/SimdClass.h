#pragma once

#include "intrin.h"
#include "xmmintrin.h"

//#define Vec256(_x) { _x,_x,_x,_x,_x,_x,_x,_x }

#define SetSimd128



class Simd128
{
public:
    static constexpr unsigned int kSimdBits = 128;
    static constexpr int kSimdBytes32 = 4;
    static constexpr int kSimdAnd32   = 3;

static constexpr __m128 Vec128(float _x) { return  __m128{ _x,_x,_x,_x }; }
static constexpr __m128i Vec128i(int _x1,  int _x2, int _x3,  int _x4) 
        { return  __m128i{ (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
                           (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4 }; }

static constexpr __m256 Vec256(float _x) { return  __m256{ _x,_x,_x,_x, _x,_x,_x,_x }; }
static constexpr __m256i Vec256i(int _x1,  int _x2, int _x3,  int _x4) 
    { return  __m256i{  (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4 }; }



   // static __forceinline __m128 Vec(float a) { return Vec128(a); }
   // static __forceinline __m128i Veci(int x1,int x2,int x3,int x4) { return Vec128i(x1,x2,x3,x4); }
    static __forceinline __m128 And(__m128 a,__m128 b)           { return _mm_and_ps(a,b); };
    static __forceinline __m128 CmpLt(__m128 a,__m128 b)         { return _mm_cmplt_ps(a,b); };
    static __forceinline __m128 CmpGt(__m128 a,__m128 b)         { return _mm_cmpgt_ps(a,b); };
    static __forceinline __m128  AndNot(__m128 a,__m128 b)       { return _mm_andnot_ps(a,b); };
    static __forceinline __m128 Or(__m128 a,__m128 b)            { return _mm_or_ps(a,b); };
    static __forceinline __m128 RoundTrunc(__m128 a)             { return _mm_round_ps(a,_MM_FROUND_TRUNC); };
    static __forceinline __m128 Sub(__m128 a,__m128 b)           { return _mm_sub_ps(a,b); };
    static __forceinline __m128 Add(__m128 a,__m128 b)           { return _mm_add_ps(a,b); };
    static __forceinline __m128 Div(__m128 a,__m128 b)           { return _mm_div_ps(a,b); };
    static __forceinline __m128 Mul(__m128 a,__m128 b)           { return _mm_mul_ps(a,b); };
    static __forceinline __m128 Xor(__m128 a,__m128 b)           { return _mm_xor_ps(a,b); };
    static __forceinline __m128 Sqrt(__m128 a)                   { return _mm_sqrt_ps(a); };
    static __forceinline __m128i Cvttps_Epi32(__m128 a)          { return _mm_cvttps_epi32(a); };
    static __forceinline __m128 Min(__m128 a,__m128 b)           { return _mm_min_ps(a,b); };
    static __forceinline __m128i Unpackhi_epi32(__m128i a,__m128i b)   { return _mm_unpackhi_epi32(a,b); };
    static __forceinline __m128i Unpacklo_epi32(__m128i a,__m128i b)   { return _mm_unpacklo_epi32(a,b); };
    static __forceinline __m128i Packs_epi32(__m128i a,__m128i b)   { return _mm_packs_epi32(a,b); };
    static __forceinline __m128i Packus_epi16(__m128i a,__m128i b)   { return _mm_packus_epi16(a,b); };
    static __forceinline __m128 Sin(__m128 a)                   { return _mm_sin_ps(a); };
    static __forceinline __m128 Cos(__m128 a)                   { return _mm_cos_ps(a); };

    static __forceinline __m128i Veci64(int iValue)             { return _mm_set1_epi64x(iValue); }
    static __forceinline __m128i Veci(int iValue)               { return _mm_set1_epi32(iValue); }
    static __forceinline __m128d Vecd(double fValue)            { return _mm_set1_pd(fValue); }
    static __forceinline __m128  Vecf(float fValue)             { return _mm_set1_ps(fValue); }
    static __forceinline __m128i Vecic(char cValue)             { return _mm_set1_epi8(cValue); };
    static __forceinline __m128i Veciuc(unsigned char ucValue)  { return _mm_set1_epi8((char) ucValue); };
    static __forceinline __m128  Load(float * fAddr)                { return _mm_load_ps(fAddr); };
    static __forceinline void    Store(float * fAddr,__m128 fValue) { return _mm_store_ps(fAddr,fValue); };
    static __forceinline __m128  Load1(float * fAddr)                { return _mm_load1_ps(fAddr); };
    static __forceinline __m128d  Load1(double * fAddr)             { return _mm_load1_pd(fAddr); };

};

class Simd256
{
public:
    static constexpr unsigned int kSimdBits     = 256;
    static constexpr int kSimdCharBytes  = 32;
    static constexpr int kSimdBytes32    = 8;
    static constexpr int kSimdBytes64    = 4;
    static constexpr int kSimdAnd32      = 7;
    static constexpr int kSimdAnd64      = 3;
    static constexpr int kSimdCharAnd    = 31;

    template<typename T>
    static constexpr const int GetSimdBytes()
    {
        return std::is_same<float,T>::value ? kSimdBytes32 : kSimdBytes64;
    }
    static constexpr __m128 Vec128(float _x) { return  __m128{ _x,_x,_x,_x }; }
static constexpr __m128i Vec128i(int _x1,  int _x2, int _x3,  int _x4) 
        { return  __m128i{ (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
                           (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4 }; }

static constexpr __m256 Vec256f(float _x) { return  __m256{ _x,_x,_x,_x, _x,_x,_x,_x }; }
static constexpr __m256 Vec256(float _x) { return  __m256{ _x,_x,_x,_x, _x,_x,_x,_x }; }
static constexpr __m256d Vec256(double _x) { return  __m256d{ _x,_x,_x,_x, }; }
static constexpr __m256d Vec256d(double _x) { return  __m256d{ _x,_x,_x,_x }; }
static constexpr __m256i Vec256i(int _x1,  int _x2, int _x3,  int _x4) 
    { return  __m256i{  (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4 }; }



 //   static __forceinline __m256 Vec(float a) { return Vec256(a); }
 //   static __forceinline __m256i Veci(int x1,int x2,int x3,int x4) { return Vec256i(x1,x2,x3,x4); }
    static __forceinline __m256 And(__m256 a,__m256 b)           { return _mm256_and_ps(a,b); };
    static __forceinline __m256 CmpLt(__m256 a,__m256 b)         { return _mm256_cmp_ps(a,b,_CMP_LT_OQ); };
    static __forceinline __m256 CmpGt(__m256 a,__m256 b)         { return _mm256_cmp_ps(a,b,_CMP_GT_OQ); };
    static __forceinline __m256  AndNot(__m256 a,__m256 b)       { return _mm256_andnot_ps(a,b); };
    static __forceinline __m256 Or(__m256 a,__m256 b)            { return _mm256_or_ps(a,b); };
    static __forceinline __m256 RoundTrunc(__m256 a)             { return _mm256_round_ps(a,_MM_FROUND_TRUNC); };
    static __forceinline __m256 Sub(__m256 a,__m256 b)           { return _mm256_sub_ps(a,b); };
    static __forceinline __m256 Add(__m256 a,__m256 b)           { return _mm256_add_ps(a,b); };
    static __forceinline __m256 Div(__m256 a,__m256 b)           { return _mm256_div_ps(a,b); };
    static __forceinline __m256 Mul(__m256 a,__m256 b)           { return _mm256_mul_ps(a,b); };
    static __forceinline __m256 Xor(__m256 a,__m256 b)           { return _mm256_xor_ps(a,b); };
    static __forceinline __m256 Sqrt(__m256 a)                   { return _mm256_sqrt_ps(a); };
    static __forceinline __m256i Cvttps_Epi32(__m256 a)          { return _mm256_cvttps_epi32(a); };
    static __forceinline __m256 Min(__m256 a,__m256 b)           { return _mm256_min_ps(a,b); };
    static __forceinline __m256i Unpackhi_epi32(__m256i a,__m256i b)   { return _mm256_unpackhi_epi32(a,b); };
    static __forceinline __m256i Unpacklo_epi32(__m256i a,__m256i b)   { return _mm256_unpacklo_epi32(a,b); };
    static __forceinline __m256i Packs_epi32(__m256i a,__m256i b)   { return _mm256_packs_epi32(a,b); };
    static __forceinline __m256i Packus_epi16(__m256i a,__m256i b)   { return _mm256_packus_epi16(a,b); };
    static __forceinline __m256 Sin(__m256 a)                   { return _mm256_sin_ps(a); };
    static __forceinline __m256 Cos(__m256 a)                   { return _mm256_cos_ps(a); };

    static __forceinline __m256i Veci64(int iValue)                 { return _mm256_set1_epi64x(iValue); }
    static __forceinline __m256i Vecii(int iValue)                  { return _mm256_set1_epi32(iValue); }
    static __forceinline __m256  Vecff(float fValue)                 { return _mm256_set1_ps(fValue); }
    static __forceinline __m256  Vecf(float fValue)                 { return _mm256_set1_ps(fValue); }
    static __forceinline __m256i Vecic(char cValue)                 { return _mm256_set1_epi8(cValue); };
    static __forceinline __m256i Veciuc(unsigned char ucValue)      { return _mm256_set1_epi8((char) ucValue); };
    static __forceinline __m256  Load(const float * fAddr)                { return _mm256_load_ps(fAddr); };
    static __forceinline void    Store(float * fAddr,__m256 fValue) { return _mm256_store_ps(fAddr,fValue); };
    
    // double functions (added as needed, so this will be incomplete for longer than float versions above)

    static __forceinline __m256d Vecfd(double fValue)                   { return _mm256_set1_pd(fValue); }
    static __forceinline __m256d Vecf(double fValue)                    { return _mm256_set1_pd(fValue); }
    static __forceinline __m256d Mul(__m256d a,__m256d b)               { return _mm256_mul_pd(a,b); };
    static __forceinline __m256d Add(__m256d a,__m256d b)               { return _mm256_add_pd(a,b); };
    static __forceinline __m256d Load(const double * fAddr)                   { return _mm256_load_pd(fAddr); };
    static __forceinline void    Store(double * fAddr,__m256d fValue)   { return _mm256_store_pd(fAddr,fValue); };



    static __forceinline __m256 CvtFloat(__m256i a)   { return _mm256_cvtepi32_ps(a); }
    static __forceinline __m256d CvtFloat(__m128i a)   { return _mm256_cvtepi32_pd(a); }

    static __forceinline __m256i CvttInt(__m256  a)   { return _mm256_cvttps_epi32 (a); }
    static __forceinline __m256i CvttInt(__m128  a)   { return _mm256_cvttps_epi64 (a); }
    static __forceinline __m256i CvttIntU(__m256  a)   { return _mm256_cvttps_epu32 (a); }
    static __forceinline __m256i CvttIntU(__m128  a)   { return _mm256_cvttps_epu64 (a); }

    static __forceinline __m256i CvtInt(__m256  a)   { return _mm256_cvtps_epi32 (a); }
    static __forceinline __m256i CvtInt(__m128  a)   { return _mm256_cvtps_epi64 (a); }
    static __forceinline __m256i CvtIntU(__m256  a)   { return _mm256_cvtps_epu32 (a); }
    static __forceinline __m256i CvtIntU(__m128  a)   { return _mm256_cvtps_epu64 (a); }

    static __forceinline __m128i CvttInt32(__m256d  a)   { return _mm256_cvttpd_epi32 (a); }

    static __forceinline __m256i Pack32s(__m256i   a, __m256i b)   { return _mm256_packs_epi32 (a,b); }
    static __forceinline __m256i Pack32us(__m256i   a, __m256i b)   { return _mm256_packus_epi32 (a,b); }
    static __forceinline __m256i Pack32s(__m256i   a)   { return _mm256_packs_epi32 (a,_mm256_castsi128_si256(_mm256_extractf128_si256(a,1))); }
    static __forceinline __m256i Pack32us(__m256i   a)   { return _mm256_packus_epi32 (a,_mm256_castsi128_si256(_mm256_extractf128_si256(a,1))); }

    static __forceinline __m256i Pack16s(__m256i   a, __m256i b)   { return _mm256_packs_epi16 (a,b); }
    static __forceinline __m256i Pack16us(__m256i   a, __m256i b)   { return _mm256_packus_epi16 (a,b); }
    static __forceinline __m256i Pack16s(__m256i   a)   { return _mm256_packs_epi16 (a,a); }
    static __forceinline __m256i Pack16us(__m256i   a)   { return _mm256_packus_epi16 (a,a); }

    template<typename inType = float>
    static __forceinline __m256i Cvtu8Int(__m128i a)    { return std::is_same<float,inType>::value ? _mm256_cvtepu8_epi32(a) : _mm256_cvtepu8_epi64(a); }
    
    template<typename inType = float>
    static __forceinline __m256i Cvtu8Int(__m128 a)     { return std::is_same<float,inType>::value ?  _mm256_cvtepu8_epi32(_mm_castps_si128(a)) :  _mm256_cvtepu8_epi64(_mm_castps_si128(a)); }
    
    template<typename inType = float>
    static __forceinline __m256i Cvtu8Int(__m128d a)    { return std::is_same<float,inType>::value ?  _mm256_cvtepu8_epi32(_mm_castpd_si128(a)) :  _mm256_cvtepu8_epi64(_mm_castpd_si128(a)) ; }




    static __forceinline __m256i Cvtu8Intd(__m128i a)    { return _mm256_cvtepu8_epi64(a); }
    static __forceinline __m256i Cvtu8Intd(__m128 a)     { return _mm256_cvtepu8_epi64(_mm_castps_si128(a)); }
    static __forceinline __m256i Cvtu8Intd(__m128d a)    { return _mm256_cvtepu8_epi64(_mm_castpd_si128(a)) ; }

    static __forceinline __m256i Cvtu8Intf(__m128i a)    { return _mm256_cvtepu8_epi32(a); }
    static __forceinline __m256i Cvtu8Intf(__m128 a)     { return _mm256_cvtepu8_epi32(_mm_castps_si128(a)); }
    static __forceinline __m256i Cvtu8Intf(__m128d a)    { return _mm256_cvtepu8_epi32(_mm_castpd_si128(a)) ; }

    template<int i>
    static __forceinline int Extract16(__m256i a) { return _mm256_extract_epi16(a,i); }

    template<int i>
    static __forceinline int Extract8(__m256i a) { return _mm256_extract_epi8(a,i); }


    #define Extractf128_si256(a,iIndex)  _mm256_extractf128_si256(a,iIndex)
};



class Simd512
{
public:
    static constexpr unsigned int kSimdBits     = 512;
    static constexpr int kSimdBytes32    = 16;
    static constexpr int kSimdAnd32      = 15;

//    static constexpr __m128 Vec128(float _x) { return  __m128{ _x,_x,_x,_x }; }
//static constexpr __m128i Vec128i(int _x1,  int _x2, int _x3,  int _x4) 
//        { return  __m128i{ (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                           (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4 }; }
//
//static constexpr __m256 Vec256(float _x) { return  __m256{ _x,_x,_x,_x, _x,_x,_x,_x }; }
//static constexpr __m256i Vec256i(int _x1,  int _x2, int _x3,  int _x4) 
//    { return  __m256i{  (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4 }; }
//
//
static constexpr __m512 Vec512(float _x) { return  __m512{ _x,_x,_x,_x, _x,_x,_x,_x, _x,_x,_x,_x, _x,_x,_x,_x }; }
//static constexpr __m512i Vec512i(int _x1,  int _x2, int _x3,  int _x4) 
//    { return  __m512i{  (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4,
//                        (char) _x1,(char) _x2,(char) _x3,(char) _x4, (char) _x1, (char) _x2, (char) _x3,(char) _x4 }; }
//


 //   static __forceinline __m256 Vec(float a) { return Vec256(a); }
 //   static __forceinline __m256i Veci(int x1,int x2,int x3,int x4) { return Vec256i(x1,x2,x3,x4); }
    static __forceinline __m512 And(__m512 a,__m512 b)           { return _mm512_and_ps(a,b); };
    static __forceinline __m512 CmpLt(__m512 a,__m512 b)         { return _mm512_castsi512_ps(_mm512_maskz_set1_epi32(_mm512_cmp_ps_mask(a,b,_CMP_LT_OQ),0xFFFFFFFF)); };
    static __forceinline __m512 CmpGt(__m512 a,__m512 b)         { return _mm512_castsi512_ps(_mm512_maskz_set1_epi32(_mm512_cmp_ps_mask(a,b,_CMP_GT_OQ),0xFFFFFFFF)); };
    static __forceinline __m512  AndNot(__m512 a,__m512 b)       { return _mm512_andnot_ps(a,b); };
    static __forceinline __m512 Or(__m512 a,__m512 b)            { return _mm512_or_ps(a,b); };
    static __forceinline __m512 RoundTrunc(__m512 a)             { return _mm512_trunc_ps(a); };
    static __forceinline __m512 Sub(__m512 a,__m512 b)           { return _mm512_sub_ps(a,b); };
    static __forceinline __m512 Add(__m512 a,__m512 b)           { return _mm512_add_ps(a,b); };
    static __forceinline __m512 Div(__m512 a,__m512 b)           { return _mm512_div_ps(a,b); };
    static __forceinline __m512 Mul(__m512 a,__m512 b)           { return _mm512_mul_ps(a,b); };
    static __forceinline __m512 Xor(__m512 a,__m512 b)           { return _mm512_xor_ps(a,b); };
    static __forceinline __m512 Sqrt(__m512 a)                   { return _mm512_sqrt_ps(a); };
    static __forceinline __m512i Cvttps_Epi32(__m512 a)          { return _mm512_cvttps_epi32(a); };
    static __forceinline __m512 Min(__m512 a,__m512 b)           { return _mm512_min_ps(a,b); };
    static __forceinline __m512i Unpackhi_epi32(__m512i a,__m512i b)   { return _mm512_unpackhi_epi32(a,b); };
    static __forceinline __m512i Unpacklo_epi32(__m512i a,__m512i b)   { return _mm512_unpacklo_epi32(a,b); };
    static __forceinline __m512i Packs_epi32(__m512i a,__m512i b)   { return _mm512_packs_epi32(a,b); };
    static __forceinline __m512i Packus_epi16(__m512i a,__m512i b)   { return _mm512_packus_epi16(a,b); };
    static __forceinline __m512 Sin(__m512 a)                   { return _mm512_sin_ps(a); };
    static __forceinline __m512 Cos(__m512 a)                   { return _mm512_cos_ps(a); };

    static __forceinline __m512i Veci64(int iValue)             { return _mm512_set1_epi64(iValue); }
    static __forceinline __m512i Vecii(int iValue)               { return _mm512_set1_epi32(iValue); }
    static __forceinline __m512d Vecd(double fValue)            { return _mm512_set1_pd(fValue); }
    static __forceinline __m512  Vecf(float fValue)             { return _mm512_set1_ps(fValue); }
    static __forceinline __m512i Vecic(char cValue)             { return _mm512_set1_epi8(cValue); };
    static __forceinline __m512i Veciuc(unsigned char ucValue)  { return _mm512_set1_epi8((char) ucValue); };
    static __forceinline __m512  Load(float * fAddr)             { return _mm512_load_ps(fAddr); };
    static __forceinline void Store(float * fAddr,__m512 fValue) { return _mm512_store_ps(fAddr,fValue); };



    #define Extractf128_si256(a,iIndex)  _mm256_extractf128_si256(a,iIndex)
};
