#ifndef SEALIB_MACRO_H
#define SEALIB_MACRO_H

#define SEA____NSEQ_N() \
        64, 63, 62, 61, 60, 59, 58, 57, 56, 55, \
        54, 53, 52, 51, 50, 49, 48, 47, 46, 45, \
        44, 43, 42, 41, 40, 39, 38, 37, 36, 35, \
        34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
        24, 23, 22, 21, 20, 19, 18, 17, 16, 15, \
        14, 13, 12, 11, 10,  9,  8,  7,  6,  5, \
         4,  3,  2,  1,  0

#define SEA___NARG_N(\
         _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
        _61, _62, _63, _64, N, ...) N

#define SEA___NARG_IMPL_(...) SEA___NARG_N(__VA_ARGS__)
#define SEA_PP_NARG(...)  SEA___NARG_IMPL_(__VA_ARGS__, SEA____NSEQ_N())

#define SEA___CONCAT_IMPL(a, b) a##b
#define SEA_PP_CONCAT(a, b) SEA___CONCAT_IMPL(a, b)

#define SEA___MAP0(f)
#define SEA___MAP1(f, x) f(x)
#define SEA___MAP2(f, x, ...) f(x), SEA___MAP1(f, __VA_ARGS__)
#define SEA___MAP3(f, x, ...) f(x), SEA___MAP2(f, __VA_ARGS__)
#define SEA___MAP4(f, x, ...) f(x), SEA___MAP3(f, __VA_ARGS__)
#define SEA___MAP5(f, x, ...) f(x), SEA___MAP4(f, __VA_ARGS__)
#define SEA___MAP6(f, x, ...) f(x), SEA___MAP5(f, __VA_ARGS__)
#define SEA___MAP7(f, x, ...) f(x), SEA___MAP6(f, __VA_ARGS__)
#define SEA___MAP8(f, x, ...) f(x), SEA___MAP7(f, __VA_ARGS__)
#define SEA___MAP9(f, x, ...) f(x), SEA___MAP8(f, __VA_ARGS__)
#define SEA___MAP10(f, x, ...) f(x), SEA___MAP9(f, __VA_ARGS__)
#define SEA___MAP11(f, x, ...) f(x), SEA___MAP10(f, __VA_ARGS__)
#define SEA___MAP12(f, x, ...) f(x), SEA___MAP11(f, __VA_ARGS__)
#define SEA___MAP13(f, x, ...) f(x), SEA___MAP12(f, __VA_ARGS__)
#define SEA___MAP14(f, x, ...) f(x), SEA___MAP13(f, __VA_ARGS__)
#define SEA___MAP15(f, x, ...) f(x), SEA___MAP14(f, __VA_ARGS__)
#define SEA___MAP16(f, x, ...) f(x), SEA___MAP15(f, __VA_ARGS__)
#define SEA___MAP17(f, x, ...) f(x), SEA___MAP16(f, __VA_ARGS__)
#define SEA___MAP18(f, x, ...) f(x), SEA___MAP17(f, __VA_ARGS__)
#define SEA___MAP19(f, x, ...) f(x), SEA___MAP18(f, __VA_ARGS__)
#define SEA___MAP20(f, x, ...) f(x), SEA___MAP19(f, __VA_ARGS__)
#define SEA___MAP21(f, x, ...) f(x), SEA___MAP20(f, __VA_ARGS__)
#define SEA___MAP22(f, x, ...) f(x), SEA___MAP21(f, __VA_ARGS__)
#define SEA___MAP23(f, x, ...) f(x), SEA___MAP22(f, __VA_ARGS__)
#define SEA___MAP24(f, x, ...) f(x), SEA___MAP23(f, __VA_ARGS__)
#define SEA___MAP25(f, x, ...) f(x), SEA___MAP24(f, __VA_ARGS__)
#define SEA___MAP26(f, x, ...) f(x), SEA___MAP25(f, __VA_ARGS__)
#define SEA___MAP27(f, x, ...) f(x), SEA___MAP26(f, __VA_ARGS__)
#define SEA___MAP28(f, x, ...) f(x), SEA___MAP27(f, __VA_ARGS__)
#define SEA___MAP29(f, x, ...) f(x), SEA___MAP28(f, __VA_ARGS__)
#define SEA___MAP30(f, x, ...) f(x), SEA___MAP29(f, __VA_ARGS__)
#define SEA___MAP31(f, x, ...) f(x), SEA___MAP30(f, __VA_ARGS__)
#define SEA___MAP32(f, x, ...) f(x), SEA___MAP31(f, __VA_ARGS__)
#define SEA___MAP33(f, x, ...) f(x), SEA___MAP32(f, __VA_ARGS__)
#define SEA___MAP34(f, x, ...) f(x), SEA___MAP33(f, __VA_ARGS__)
#define SEA___MAP35(f, x, ...) f(x), SEA___MAP34(f, __VA_ARGS__)
#define SEA___MAP36(f, x, ...) f(x), SEA___MAP35(f, __VA_ARGS__)
#define SEA___MAP37(f, x, ...) f(x), SEA___MAP36(f, __VA_ARGS__)
#define SEA___MAP38(f, x, ...) f(x), SEA___MAP37(f, __VA_ARGS__)
#define SEA___MAP39(f, x, ...) f(x), SEA___MAP38(f, __VA_ARGS__)
#define SEA___MAP40(f, x, ...) f(x), SEA___MAP39(f, __VA_ARGS__)
#define SEA___MAP41(f, x, ...) f(x), SEA___MAP40(f, __VA_ARGS__)
#define SEA___MAP42(f, x, ...) f(x), SEA___MAP41(f, __VA_ARGS__)
#define SEA___MAP43(f, x, ...) f(x), SEA___MAP42(f, __VA_ARGS__)
#define SEA___MAP44(f, x, ...) f(x), SEA___MAP43(f, __VA_ARGS__)
#define SEA___MAP45(f, x, ...) f(x), SEA___MAP44(f, __VA_ARGS__)
#define SEA___MAP46(f, x, ...) f(x), SEA___MAP45(f, __VA_ARGS__)
#define SEA___MAP47(f, x, ...) f(x), SEA___MAP46(f, __VA_ARGS__)
#define SEA___MAP48(f, x, ...) f(x), SEA___MAP47(f, __VA_ARGS__)
#define SEA___MAP49(f, x, ...) f(x), SEA___MAP48(f, __VA_ARGS__)
#define SEA___MAP50(f, x, ...) f(x), SEA___MAP49(f, __VA_ARGS__)
#define SEA___MAP51(f, x, ...) f(x), SEA___MAP50(f, __VA_ARGS__)
#define SEA___MAP52(f, x, ...) f(x), SEA___MAP51(f, __VA_ARGS__)
#define SEA___MAP53(f, x, ...) f(x), SEA___MAP52(f, __VA_ARGS__)
#define SEA___MAP54(f, x, ...) f(x), SEA___MAP53(f, __VA_ARGS__)
#define SEA___MAP55(f, x, ...) f(x), SEA___MAP54(f, __VA_ARGS__)
#define SEA___MAP56(f, x, ...) f(x), SEA___MAP55(f, __VA_ARGS__)
#define SEA___MAP57(f, x, ...) f(x), SEA___MAP56(f, __VA_ARGS__)
#define SEA___MAP58(f, x, ...) f(x), SEA___MAP57(f, __VA_ARGS__)
#define SEA___MAP59(f, x, ...) f(x), SEA___MAP58(f, __VA_ARGS__)
#define SEA___MAP60(f, x, ...) f(x), SEA___MAP59(f, __VA_ARGS__)
#define SEA___MAP61(f, x, ...) f(x), SEA___MAP60(f, __VA_ARGS__)
#define SEA___MAP62(f, x, ...) f(x), SEA___MAP61(f, __VA_ARGS__)
#define SEA___MAP63(f, x, ...) f(x), SEA___MAP62(f, __VA_ARGS__)
#define SEA___MAP64(f, x, ...) f(x), SEA___MAP63(f, __VA_ARGS__)

#define SEA___GET_MAP_MACRO(\
     _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10,\
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,\
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,\
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,\
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,\
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,\
    _61, _62, _63, _64, NAME, ...) NAME

#define SEA_MAP(f, ...) \
  SEA___GET_MAP_MACRO(__VA_ARGS__, \
    SEA___MAP64, SEA___MAP63, SEA___MAP62, SEA___MAP61, SEA___MAP60, \
    SEA___MAP59, SEA___MAP58, SEA___MAP57, SEA___MAP56, SEA___MAP55, \
    SEA___MAP54, SEA___MAP53, SEA___MAP52, SEA___MAP51, SEA___MAP50, \
    SEA___MAP49, SEA___MAP48, SEA___MAP47, SEA___MAP46, SEA___MAP45, \
    SEA___MAP44, SEA___MAP43, SEA___MAP42, SEA___MAP41, SEA___MAP40, \
    SEA___MAP39, SEA___MAP38, SEA___MAP37, SEA___MAP36, SEA___MAP35, \
    SEA___MAP34, SEA___MAP33, SEA___MAP32, SEA___MAP31, SEA___MAP30, \
    SEA___MAP29, SEA___MAP28, SEA___MAP27, SEA___MAP26, SEA___MAP25, \
    SEA___MAP24, SEA___MAP23, SEA___MAP22, SEA___MAP21, SEA___MAP20, \
    SEA___MAP19, SEA___MAP18, SEA___MAP17, SEA___MAP16, SEA___MAP15, \
    SEA___MAP14, SEA___MAP13, SEA___MAP12, SEA___MAP11, SEA___MAP10, \
    SEA___MAP9, SEA___MAP8, SEA___MAP7, SEA___MAP6, SEA___MAP5, SEA___MAP4, \
    SEA___MAP3, SEA___MAP2, SEA___MAP1, SEA___MAP0)(f, __VA_ARGS__)

#endif //SEALIB_MACRO_H
