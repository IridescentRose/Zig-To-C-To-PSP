#undef linux

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__has_builtin)
#define zig_has_builtin(builtin) __has_builtin(__builtin_##builtin)
#else
#define zig_has_builtin(builtin) 0
#endif

#if defined(__has_attribute)
#define zig_has_attribute(attribute) __has_attribute(attribute)
#else
#define zig_has_attribute(attribute) 0
#endif

#if __STDC_VERSION__ >= 201112L
#define zig_threadlocal _Thread_local
#elif defined(__GNUC__)
#define zig_threadlocal __thread
#elif _MSC_VER
#define zig_threadlocal __declspec(thread)
#else
#define zig_threadlocal zig_threadlocal_unavailable
#endif

#if zig_has_attribute(naked) || defined(__GNUC__)
#define zig_naked __attribute__((naked))
#elif defined(_MSC_VER)
#define zig_naked __declspec(naked)
#else
#define zig_naked zig_naked_unavailable
#endif

#if zig_has_attribute(cold)
#define zig_cold __attribute__((cold))
#else
#define zig_cold
#endif

#if __STDC_VERSION__ >= 199901L
#define zig_restrict restrict
#elif defined(__GNUC__)
#define zig_restrict __restrict
#else
#define zig_restrict
#endif

#if __STDC_VERSION__ >= 201112L
#define zig_align(alignment) _Alignas(alignment)
#elif zig_has_attribute(aligned)
#define zig_align(alignment) __attribute__((aligned(alignment)))
#elif _MSC_VER
#else
#error the C compiler being used does not support aligning variables
#endif

#if zig_has_builtin(unreachable)
#define zig_unreachable() __builtin_unreachable()
#else
#define zig_unreachable()
#endif

#if defined(__cplusplus)
#define zig_extern extern "C"
#else
#define zig_extern extern
#endif

#if zig_has_builtin(debugtrap)
#define zig_breakpoint() __builtin_debugtrap()
#elif zig_has_builtin(trap)
#define zig_breakpoint() __builtin_trap()
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#define zig_breakpoint() __debugbreak()
#elif defined(__i386__) || defined(__x86_64__)
#define zig_breakpoint() __asm__ volatile("int $0x03");
#else
#define zig_breakpoint() raise(SIGTRAP)
#endif

#if zig_has_builtin(return_address)
#define zig_return_address() __builtin_extract_return_addr(__builtin_return_address(0))
#elif defined(_MSC_VER)
#define zig_return_address() _ReturnAddress()
#else
#define zig_return_address() 0
#endif

#if zig_has_builtin(frame_address)
#define zig_frame_address() __builtin_frame_address(0)
#else
#define zig_frame_address() 0
#endif

#if zig_has_builtin(prefetch)
#define zig_prefetch(addr, rw, locality) __builtin_prefetch(addr, rw, locality)
#else
#define zig_prefetch(addr, rw, locality)
#endif

#if zig_has_builtin(memory_size) && zig_has_builtin(memory_grow)
#define zig_wasm_memory_size(index) __builtin_wasm_memory_size(index)
#define zig_wasm_memory_grow(index, delta) __builtin_wasm_memory_grow(index, delta)
#else
#define zig_wasm_memory_size(index) zig_unimplemented()
#define zig_wasm_memory_grow(index, delta) zig_unimplemented()
#endif

#define zig_extern_c extern

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#define zig_atomic(type) _Atomic(type)
#define zig_cmpxchg_strong(obj, expected, desired, succ, fail) atomic_compare_exchange_strong_explicit(obj, &(expected), desired, succ, fail)
#define   zig_cmpxchg_weak(obj, expected, desired, succ, fail) atomic_compare_exchange_weak_explicit  (obj, &(expected), desired, succ, fail)
#define zig_atomicrmw_xchg(obj, arg, order) atomic_exchange_explicit  (obj, arg, order)
#define  zig_atomicrmw_add(obj, arg, order) atomic_fetch_add_explicit (obj, arg, order)
#define  zig_atomicrmw_sub(obj, arg, order) atomic_fetch_sub_explicit (obj, arg, order)
#define   zig_atomicrmw_or(obj, arg, order) atomic_fetch_or_explicit  (obj, arg, order)
#define  zig_atomicrmw_xor(obj, arg, order) atomic_fetch_xor_explicit (obj, arg, order)
#define  zig_atomicrmw_and(obj, arg, order) atomic_fetch_and_explicit (obj, arg, order)
#define zig_atomicrmw_nand(obj, arg, order) __atomic_fetch_nand       (obj, arg, order)
#define  zig_atomicrmw_min(obj, arg, order) __atomic_fetch_min        (obj, arg, order)
#define  zig_atomicrmw_max(obj, arg, order) __atomic_fetch_max        (obj, arg, order)
#define   zig_atomic_store(obj, arg, order) atomic_store_explicit     (obj, arg, order)
#define    zig_atomic_load(obj,      order) atomic_load_explicit      (obj,      order)
#define zig_fence(order) atomic_thread_fence(order)
#elif defined(__GNUC__)
#define memory_order_relaxed __ATOMIC_RELAXED
#define memory_order_consume __ATOMIC_CONSUME
#define memory_order_acquire __ATOMIC_ACQUIRE
#define memory_order_release __ATOMIC_RELEASE
#define memory_order_acq_rel __ATOMIC_ACQ_REL
#define memory_order_seq_cst __ATOMIC_SEQ_CST
#define zig_atomic(type) type
#define zig_cmpxchg_strong(obj, expected, desired, succ, fail) __atomic_compare_exchange_n(obj, &(expected), desired, zig_false, succ, fail)
#define   zig_cmpxchg_weak(obj, expected, desired, succ, fail) __atomic_compare_exchange_n(obj, &(expected), desired, zig_true , succ, fail)
#define zig_atomicrmw_xchg(obj, arg, order) __atomic_exchange_n(obj, arg, order)
#define  zig_atomicrmw_add(obj, arg, order) __atomic_fetch_add (obj, arg, order)
#define  zig_atomicrmw_sub(obj, arg, order) __atomic_fetch_sub (obj, arg, order)
#define   zig_atomicrmw_or(obj, arg, order) __atomic_fetch_or  (obj, arg, order)
#define  zig_atomicrmw_xor(obj, arg, order) __atomic_fetch_xor (obj, arg, order)
#define  zig_atomicrmw_and(obj, arg, order) __atomic_fetch_and (obj, arg, order)
#define zig_atomicrmw_nand(obj, arg, order) __atomic_fetch_nand(obj, arg, order)
#define  zig_atomicrmw_min(obj, arg, order) __atomic_fetch_min (obj, arg, order)
#define  zig_atomicrmw_max(obj, arg, order) __atomic_fetch_max (obj, arg, order)
#define   zig_atomic_store(obj, arg, order) __atomic_store_n   (obj, arg, order)
#define    zig_atomic_load(obj,      order) __atomic_load_n    (obj,      order)
#define zig_fence(order) __atomic_thread_fence(order)
#else
#define memory_order_relaxed 0
#define memory_order_consume 1
#define memory_order_acquire 2
#define memory_order_release 3
#define memory_order_acq_rel 4
#define memory_order_seq_cst 5
#define zig_atomic(type) type
#define zig_cmpxchg_strong(obj, expected, desired, succ, fail) zig_unimplemented()
#define   zig_cmpxchg_weak(obj, expected, desired, succ, fail) zig_unimplemented()
#define zig_atomicrmw_xchg(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_add(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_sub(obj, arg, order) zig_unimplemented()
#define   zig_atomicrmw_or(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_xor(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_and(obj, arg, order) zig_unimplemented()
#define zig_atomicrmw_nand(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_min(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_max(obj, arg, order) zig_unimplemented()
#define   zig_atomic_store(obj, arg, order) zig_unimplemented()
#define    zig_atomic_load(obj,      order) zig_unimplemented()
#define zig_fence(order) zig_unimplemented()
#endif

#if __STDC_VERSION__ >= 201112L
#define zig_noreturn _Noreturn void
#elif zig_has_attribute(noreturn) || defined(__GNUC__)
#define zig_noreturn __attribute__((noreturn)) void
#elif _MSC_VER
#define zig_noreturn __declspec(noreturn) void
#else
#define zig_noreturn void
#endif

#define zig_concat(lhs, rhs) lhs##rhs
#define zig_expand_concat(lhs, rhs) zig_concat(lhs, rhs)

#define zig_bitSizeOf(T) (CHAR_BIT * sizeof(T))

typedef void zig_void;

#if defined(__cplusplus)
typedef bool zig_bool;
#define zig_false false
#define zig_true true
#else
#if __STDC_VERSION__ >= 199901L
typedef _Bool zig_bool;
#else
typedef char zig_bool;
#endif
#define zig_false ((zig_bool)0)
#define zig_true  ((zig_bool)1)
#endif

typedef              uintptr_t zig_usize;
typedef               intptr_t zig_isize;
typedef   signed     short int zig_c_short;
typedef unsigned     short int zig_c_ushort;
typedef   signed           int zig_c_int;
typedef unsigned           int zig_c_uint;
typedef   signed      long int zig_c_long;
typedef unsigned      long int zig_c_ulong;
typedef   signed long long int zig_c_longlong;
typedef unsigned long long int zig_c_ulonglong;

typedef uint8_t  zig_u8;
typedef  int8_t  zig_i8;
typedef uint16_t zig_u16;
typedef  int16_t zig_i16;
typedef uint16_t zig_u16;
typedef  int16_t zig_i16;
typedef uint32_t zig_u32;
typedef  int32_t zig_i32;
typedef uint64_t zig_u64;
typedef  int64_t zig_i64;

#define zig_as_u8(val)  UINT8_C(val)
#define zig_as_i8(val)   INT8_C(val)
#define zig_as_u16(val) UINT16_C(val)
#define zig_as_i16(val)  INT16_C(val)
#define zig_as_u32(val) UINT32_C(val)
#define zig_as_i32(val)  INT32_C(val)
#define zig_as_u64(val) UINT64_C(val)
#define zig_as_i64(val)  INT64_C(val)

#define zig_minInt_u8  zig_as_u8(0)
#define zig_maxInt_u8   UINT8_MAX
#define zig_minInt_i8    INT8_MIN
#define zig_maxInt_i8    INT8_MAX
#define zig_minInt_u16 zig_as_u16(0)
#define zig_maxInt_u16 UINT16_MAX
#define zig_minInt_i16  INT16_MIN
#define zig_maxInt_i16  INT16_MAX
#define zig_minInt_u32 zig_as_u32(0)
#define zig_maxInt_u32 UINT32_MAX
#define zig_minInt_i32  INT32_MIN
#define zig_maxInt_i32  INT32_MAX
#define zig_minInt_u64 zig_as_u64(0)
#define zig_maxInt_u64 UINT64_MAX
#define zig_minInt_i64  INT64_MIN
#define zig_maxInt_i64  INT64_MAX

#define zig_compiler_rt_abbrev_u32  si
#define zig_compiler_rt_abbrev_i32  si
#define zig_compiler_rt_abbrev_u64  di
#define zig_compiler_rt_abbrev_i64  di
#define zig_compiler_rt_abbrev_f16  hf
#define zig_compiler_rt_abbrev_f32  sf
#define zig_compiler_rt_abbrev_f64  df

zig_extern void *memcpy (void *zig_restrict, void const *zig_restrict, zig_usize);
zig_extern void *memset (void *, int, zig_usize);

/* ==================== 8/16/32/64-bit Integer Routines ===================== */

#define zig_maxInt(Type, bits) zig_shr_##Type(zig_maxInt_##Type, (zig_bitSizeOf(zig_##Type) - bits))
#define zig_expand_maxInt(Type, bits) zig_maxInt(Type, bits)
#define zig_minInt(Type, bits) zig_not_##Type(zig_maxInt(Type, bits), bits)
#define zig_expand_minInt(Type, bits) zig_minInt(Type, bits)

#define zig_int_operator(Type, RhsType, operation, operator) \
    static inline zig_##Type zig_##operation##_##Type(zig_##Type lhs, zig_##RhsType rhs) { \
        return lhs operator rhs; \
    }
#define zig_int_basic_operator(Type, operation, operator) \
    zig_int_operator(Type, Type, operation, operator)
#define zig_int_shift_operator(Type, operation, operator) \
    zig_int_operator(Type,   u8, operation, operator)
#define zig_int_helpers(w) \
    zig_int_basic_operator(u##w, and,  &) \
    zig_int_basic_operator(i##w, and,  &) \
    zig_int_basic_operator(u##w,  or,  |) \
    zig_int_basic_operator(i##w,  or,  |) \
    zig_int_basic_operator(u##w, xor,  ^) \
    zig_int_basic_operator(i##w, xor,  ^) \
    zig_int_shift_operator(u##w, shl, <<) \
    zig_int_shift_operator(i##w, shl, <<) \
    zig_int_shift_operator(u##w, shr, >>) \
\
    static inline zig_i##w zig_shr_i##w(zig_i##w lhs, zig_u8 rhs) { \
        zig_i##w sign_mask = lhs < zig_as_i##w(0) ? -zig_as_i##w(1) : zig_as_i##w(0); \
        return ((lhs ^ sign_mask) >> rhs) ^ sign_mask; \
    } \
\
    static inline zig_u##w zig_not_u##w(zig_u##w val, zig_u8 bits) { \
        return val ^ zig_maxInt(u##w, bits); \
    } \
\
    static inline zig_i##w zig_not_i##w(zig_i##w val, zig_u8 bits) { \
        (void)bits; \
        return ~val; \
    } \
\
    static inline zig_u##w zig_wrap_u##w(zig_u##w val, zig_u8 bits) { \
        return val & zig_maxInt(u##w, bits); \
    } \
\
    static inline zig_i##w zig_wrap_i##w(zig_i##w val, zig_u8 bits) { \
        return (val & zig_as_u##w(1) << (bits - zig_as_u8(1))) != 0 \
            ? val | zig_minInt(i##w, bits) : val & zig_maxInt(i##w, bits); \
    } \
\
    zig_int_basic_operator(u##w, div_floor, /) \
\
    static inline zig_i##w zig_div_floor_i##w(zig_i##w lhs, zig_i##w rhs) { \
        return lhs / rhs - (((lhs ^ rhs) & (lhs % rhs)) < zig_as_i##w(0)); \
    } \
\
    zig_int_basic_operator(u##w, mod, %) \
\
    static inline zig_i##w zig_mod_i##w(zig_i##w lhs, zig_i##w rhs) { \
        zig_i##w rem = lhs % rhs; \
        return rem + (((lhs ^ rhs) & rem) < zig_as_i##w(0) ? rhs : zig_as_i##w(0)); \
    }
zig_int_helpers(8)
zig_int_helpers(16)
zig_int_helpers(32)
zig_int_helpers(64)

static inline zig_bool zig_addo_u32(zig_u32 *res, zig_u32 lhs, zig_u32 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u32 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u32(full_res, bits);
    return overflow || full_res < zig_minInt(u32, bits) || full_res > zig_maxInt(u32, bits);
#else
    *res = zig_addw_u32(lhs, rhs, bits);
    return *res < lhs;
#endif
}

zig_extern zig_i32  __addosi4(zig_i32 lhs, zig_i32 rhs, zig_c_int *overflow);
static inline zig_bool zig_addo_i32(zig_i32 *res, zig_i32 lhs, zig_i32 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i32 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u32 full_res = __addosi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i32(full_res, bits);
    return overflow || full_res < zig_minInt(i32, bits) || full_res > zig_maxInt(i32, bits);
}

static inline zig_bool zig_addo_u64(zig_u64 *res, zig_u64 lhs, zig_u64 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u64 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u64(full_res, bits);
    return overflow || full_res < zig_minInt(u64, bits) || full_res > zig_maxInt(u64, bits);
#else
    *res = zig_addw_u64(lhs, rhs, bits);
    return *res < lhs;
#endif
}

zig_extern zig_i64  __addodi4(zig_i64 lhs, zig_i64 rhs, zig_c_int *overflow);
static inline zig_bool zig_addo_i64(zig_i64 *res, zig_i64 lhs, zig_i64 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i64 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u64 full_res = __addodi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i64(full_res, bits);
    return overflow || full_res < zig_minInt(i64, bits) || full_res > zig_maxInt(i64, bits);
}

static inline zig_bool zig_addo_u8(zig_u8 *res, zig_u8 lhs, zig_u8 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u8 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u8(full_res, bits);
    return overflow || full_res < zig_minInt(u8, bits) || full_res > zig_maxInt(u8, bits);
#else
    return zig_addo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_addo_i8(zig_i8 *res, zig_i8 lhs, zig_i8 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i8 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i8(full_res, bits);
    return overflow || full_res < zig_minInt(i8, bits) || full_res > zig_maxInt(i8, bits);
#else
    return zig_addo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_addo_u16(zig_u16 *res, zig_u16 lhs, zig_u16 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u16 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u16(full_res, bits);
    return overflow || full_res < zig_minInt(u16, bits) || full_res > zig_maxInt(u16, bits);
#else
    return zig_addo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_addo_i16(zig_i16 *res, zig_i16 lhs, zig_i16 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i16 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i16(full_res, bits);
    return overflow || full_res < zig_minInt(i16, bits) || full_res > zig_maxInt(i16, bits);
#else
    return zig_addo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_subo_u32(zig_u32 *res, zig_u32 lhs, zig_u32 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u32 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u32(full_res, bits);
    return overflow || full_res < zig_minInt(u32, bits) || full_res > zig_maxInt(u32, bits);
#else
    *res = zig_subw_u32(lhs, rhs, bits);
    return *res > lhs;
#endif
}

zig_extern zig_i32  __subosi4(zig_i32 lhs, zig_i32 rhs, zig_c_int *overflow);
static inline zig_bool zig_subo_i32(zig_i32 *res, zig_i32 lhs, zig_i32 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i32 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u32 full_res = __subosi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i32(full_res, bits);
    return overflow || full_res < zig_minInt(i32, bits) || full_res > zig_maxInt(i32, bits);
}

static inline zig_bool zig_subo_u64(zig_u64 *res, zig_u64 lhs, zig_u64 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u64 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u64(full_res, bits);
    return overflow || full_res < zig_minInt(u64, bits) || full_res > zig_maxInt(u64, bits);
#else
    *res = zig_subw_u64(lhs, rhs, bits);
    return *res > lhs;
#endif
}

zig_extern zig_i64  __subodi4(zig_i64 lhs, zig_i64 rhs, zig_c_int *overflow);
static inline zig_bool zig_subo_i64(zig_i64 *res, zig_i64 lhs, zig_i64 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i64 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u64 full_res = __subodi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i64(full_res, bits);
    return overflow || full_res < zig_minInt(i64, bits) || full_res > zig_maxInt(i64, bits);
}

static inline zig_bool zig_subo_u8(zig_u8 *res, zig_u8 lhs, zig_u8 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u8 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u8(full_res, bits);
    return overflow || full_res < zig_minInt(u8, bits) || full_res > zig_maxInt(u8, bits);
#else
    return zig_subo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_subo_i8(zig_i8 *res, zig_i8 lhs, zig_i8 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i8 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i8(full_res, bits);
    return overflow || full_res < zig_minInt(i8, bits) || full_res > zig_maxInt(i8, bits);
#else
    return zig_subo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_subo_u16(zig_u16 *res, zig_u16 lhs, zig_u16 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u16 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u16(full_res, bits);
    return overflow || full_res < zig_minInt(u16, bits) || full_res > zig_maxInt(u16, bits);
#else
    return zig_subo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_subo_i16(zig_i16 *res, zig_i16 lhs, zig_i16 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i16 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i16(full_res, bits);
    return overflow || full_res < zig_minInt(i16, bits) || full_res > zig_maxInt(i16, bits);
#else
    return zig_subo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_mulo_u32(zig_u32 *res, zig_u32 lhs, zig_u32 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u32 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u32(full_res, bits);
    return overflow || full_res < zig_minInt(u32, bits) || full_res > zig_maxInt(u32, bits);
#else
    *res = zig_mulw_u32(lhs, rhs, bits);
    return rhs != zig_as_u32(0) && lhs > zig_maxInt(u32, bits) / rhs;
#endif
}

zig_extern zig_i32  __mulosi4(zig_i32 lhs, zig_i32 rhs, zig_c_int *overflow);
static inline zig_bool zig_mulo_i32(zig_i32 *res, zig_i32 lhs, zig_i32 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i32 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u32 full_res = __mulosi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i32(full_res, bits);
    return overflow || full_res < zig_minInt(i32, bits) || full_res > zig_maxInt(i32, bits);
}

static inline zig_bool zig_mulo_u64(zig_u64 *res, zig_u64 lhs, zig_u64 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u64 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u64(full_res, bits);
    return overflow || full_res < zig_minInt(u64, bits) || full_res > zig_maxInt(u64, bits);
#else
    *res = zig_mulw_u64(lhs, rhs, bits);
    return rhs != zig_as_u64(0) && lhs > zig_maxInt(u64, bits) / rhs;
#endif
}

zig_extern zig_i64  __mulodi4(zig_i64 lhs, zig_i64 rhs, zig_c_int *overflow);
static inline zig_bool zig_mulo_i64(zig_i64 *res, zig_i64 lhs, zig_i64 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i64 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u64 full_res = __mulodi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i64(full_res, bits);
    return overflow || full_res < zig_minInt(i64, bits) || full_res > zig_maxInt(i64, bits);
}

static inline zig_bool zig_mulo_u8(zig_u8 *res, zig_u8 lhs, zig_u8 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u8 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u8(full_res, bits);
    return overflow || full_res < zig_minInt(u8, bits) || full_res > zig_maxInt(u8, bits);
#else
    return zig_mulo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_mulo_i8(zig_i8 *res, zig_i8 lhs, zig_i8 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i8 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i8(full_res, bits);
    return overflow || full_res < zig_minInt(i8, bits) || full_res > zig_maxInt(i8, bits);
#else
    return zig_mulo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_mulo_u16(zig_u16 *res, zig_u16 lhs, zig_u16 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u16 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u16(full_res, bits);
    return overflow || full_res < zig_minInt(u16, bits) || full_res > zig_maxInt(u16, bits);
#else
    return zig_mulo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_mulo_i16(zig_i16 *res, zig_i16 lhs, zig_i16 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i16 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i16(full_res, bits);
    return overflow || full_res < zig_minInt(i16, bits) || full_res > zig_maxInt(i16, bits);
#else
    return zig_mulo_i32(res, lhs, rhs, bits);
#endif
}

#define zig_int_builtins(w) \
    static inline zig_u##w zig_shlw_u##w(zig_u##w lhs, zig_u8 rhs, zig_u8 bits) { \
        return zig_wrap_u##w(zig_shl_u##w(lhs, rhs), bits); \
    } \
\
    static inline zig_i##w zig_shlw_i##w(zig_i##w lhs, zig_u8 rhs, zig_u8 bits) { \
        return zig_wrap_i##w((zig_i##w)zig_shl_u##w((zig_u##w)lhs, (zig_u##w)rhs), bits); \
    } \
\
    static inline zig_u##w zig_addw_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        return zig_wrap_u##w(lhs + rhs, bits); \
    } \
\
    static inline zig_i##w zig_addw_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        return zig_wrap_i##w((zig_i##w)((zig_u##w)lhs + (zig_u##w)rhs), bits); \
    } \
\
    static inline zig_u##w zig_subw_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        return zig_wrap_u##w(lhs - rhs, bits); \
    } \
\
    static inline zig_i##w zig_subw_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        return zig_wrap_i##w((zig_i##w)((zig_u##w)lhs - (zig_u##w)rhs), bits); \
    } \
\
    static inline zig_u##w zig_mulw_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        return zig_wrap_u##w(lhs * rhs, bits); \
    } \
\
    static inline zig_i##w zig_mulw_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        return zig_wrap_i##w((zig_i##w)((zig_u##w)lhs * (zig_u##w)rhs), bits); \
    } \
\
    static inline zig_bool zig_shlo_u##w(zig_u##w *res, zig_u##w lhs, zig_u8 rhs, zig_u8 bits) { \
        *res = zig_shlw_u##w(lhs, rhs, bits); \
        return (lhs & zig_maxInt_u##w << (bits - rhs)) != zig_as_u##w(0); \
    } \
\
    static inline zig_bool zig_shlo_i##w(zig_i##w *res, zig_i##w lhs, zig_u8 rhs, zig_u8 bits) { \
        *res = zig_shlw_i##w(lhs, rhs, bits); \
        zig_i##w mask = (zig_i##w)(zig_maxInt_u##w << (bits - rhs - 1)); \
        return (lhs & mask) != zig_as_i##w(0) && (lhs & mask) != mask; \
    } \
\
    static inline zig_u##w zig_shls_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        zig_u##w res; \
        if (rhs >= bits) return lhs != zig_as_u##w(0) ? zig_maxInt(u##w, bits) : lhs; \
        return zig_shlo_u##w(&res, lhs, (zig_u8)rhs, bits) ? zig_maxInt(u##w, bits) : res; \
    } \
\
    static inline zig_i##w zig_shls_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        zig_i##w res; \
        if ((zig_u##w)rhs < (zig_u##w)bits && !zig_shlo_i##w(&res, lhs, rhs, bits)) return res; \
        return lhs < zig_as_i##w(0) ? zig_minInt(i##w, bits) : zig_maxInt(i##w, bits); \
    } \
\
    static inline zig_u##w zig_adds_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        zig_u##w res; \
        return zig_addo_u##w(&res, lhs, rhs, bits) ? zig_maxInt(u##w, bits) : res; \
    } \
\
    static inline zig_i##w zig_adds_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        zig_i##w res; \
        if (!zig_addo_i##w(&res, lhs, rhs, bits)) return res; \
        return res >= zig_as_i##w(0) ? zig_minInt(i##w, bits) : zig_maxInt(i##w, bits); \
    } \
\
    static inline zig_u##w zig_subs_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        zig_u##w res; \
        return zig_subo_u##w(&res, lhs, rhs, bits) ? zig_minInt(u##w, bits) : res; \
    } \
\
    static inline zig_i##w zig_subs_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        zig_i##w res; \
        if (!zig_subo_i##w(&res, lhs, rhs, bits)) return res; \
        return res >= zig_as_i##w(0) ? zig_minInt(i##w, bits) : zig_maxInt(i##w, bits); \
    } \
\
    static inline zig_u##w zig_muls_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        zig_u##w res; \
        return zig_mulo_u##w(&res, lhs, rhs, bits) ? zig_maxInt(u##w, bits) : res; \
    } \
\
    static inline zig_i##w zig_muls_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        zig_i##w res; \
        if (!zig_mulo_i##w(&res, lhs, rhs, bits)) return res; \
        return (lhs ^ rhs) < zig_as_i##w(0) ? zig_minInt(i##w, bits) : zig_maxInt(i##w, bits); \
    }
zig_int_builtins(8)
zig_int_builtins(16)
zig_int_builtins(32)
zig_int_builtins(64)

#define zig_builtin8(name, val) __builtin_##name(val)
typedef zig_c_uint zig_Builtin8;

#define zig_builtin16(name, val) __builtin_##name(val)
typedef zig_c_uint zig_Builtin16;

#if INT_MIN <= INT32_MIN
#define zig_builtin32(name, val) __builtin_##name(val)
typedef zig_c_uint zig_Builtin32;
#elif LONG_MIN <= INT32_MIN
#define zig_builtin32(name, val) __builtin_##name##l(val)
typedef zig_c_ulong zig_Builtin32;
#endif

#if INT_MIN <= INT64_MIN
#define zig_builtin64(name, val) __builtin_##name(val)
typedef zig_c_uint zig_Builtin64;
#elif LONG_MIN <= INT64_MIN
#define zig_builtin64(name, val) __builtin_##name##l(val)
typedef zig_c_ulong zig_Builtin64;
#elif LLONG_MIN <= INT64_MIN
#define zig_builtin64(name, val) __builtin_##name##ll(val)
typedef zig_c_ulonglong zig_Builtin64;
#endif

#if zig_has_builtin(clz)
#define zig_builtin_clz(w) \
    static inline zig_u8 zig_clz_u##w(zig_u##w val, zig_u8 bits) { \
        if (val == 0) return bits; \
        return zig_builtin##w(clz, val) - (zig_bitSizeOf(zig_Builtin##w) - bits); \
    } \
\
    static inline zig_u8 zig_clz_i##w(zig_i##w val, zig_u8 bits) { \
        return zig_clz_u##w((zig_u##w)val, bits); \
    }
zig_builtin_clz(8)
zig_builtin_clz(16)
zig_builtin_clz(32)
zig_builtin_clz(64)
#endif

#if zig_has_builtin(ctz)
#define zig_builtin_ctz(w) \
    static inline zig_u8 zig_ctz_u##w(zig_u##w val, zig_u8 bits) { \
        if (val == 0) return bits; \
        return zig_builtin##w(ctz, val); \
    } \
\
    static inline zig_u8 zig_ctz_i##w(zig_i##w val, zig_u8 bits) { \
        return zig_ctz_u##w((zig_u##w)val, bits); \
    }
zig_builtin_ctz(8)
zig_builtin_ctz(16)
zig_builtin_ctz(32)
zig_builtin_ctz(64)
#endif

#if zig_has_builtin(popcount)
#define zig_builtin_popcount(w) \
    static inline zig_u8 zig_popcount_u##w(zig_u##w val, zig_u8 bits) { \
        (void)bits; \
        return zig_builtin##w(popcount, val); \
    } \
\
    static inline zig_u8 zig_popcount_i##w(zig_i##w val, zig_u8 bits) { \
        \
        return zig_popcount_u##w((zig_u##w)val, bits); \
    }
zig_builtin_popcount(8)
zig_builtin_popcount(16)
zig_builtin_popcount(32)
zig_builtin_popcount(64)
#endif

static inline zig_u8 zig_byte_swap_u8(zig_u8 val, zig_u8 bits) {
    return zig_wrap_u8(val >> (8 - bits), bits);
}

static inline zig_i8 zig_byte_swap_i8(zig_i8 val, zig_u8 bits) {
    return zig_wrap_i8((zig_i8)zig_byte_swap_u8((zig_u8)val, bits), bits);
}

static inline zig_u16 zig_byte_swap_u16(zig_u16 val, zig_u8 bits) {
    zig_u16 full_res;
#if zig_has_builtin(bswap16)
    full_res = __builtin_bswap16(val);
#else
    full_res = (zig_u16)zig_byte_swap_u8((zig_u8)(val >>  0)) <<  8 |
               (zig_u16)zig_byte_swap_u8((zig_u8)(val >>  8)) >>  0;
#endif
    return zig_wrap_u16(full_res >> (16 - bits), bits);
}

static inline zig_i16 zig_byte_swap_i16(zig_i16 val, zig_u8 bits) {
    return zig_wrap_i16((zig_i16)zig_byte_swap_u16((zig_u16)val, bits), bits);
}

static inline zig_u32 zig_byte_swap_u32(zig_u32 val, zig_u8 bits) {
    zig_u32 full_res;
#if zig_has_builtin(bswap32)
    full_res = __builtin_bswap32(val);
#else
    full_res = (zig_u32)zig_byte_swap_u16((zig_u16)(val >>  0)) << 16 |
               (zig_u32)zig_byte_swap_u16((zig_u16)(val >> 16)) >>  0;
#endif
    return zig_wrap_u32(full_res >> (32 - bits), bits);
}

static inline zig_i32 zig_byte_swap_i32(zig_i32 val, zig_u8 bits) {
    return zig_wrap_i32((zig_i32)zig_byte_swap_u32((zig_u32)val, bits), bits);
}

static inline zig_u64 zig_byte_swap_u64(zig_u64 val, zig_u8 bits) {
    zig_u64 full_res;
#if zig_has_builtin(bswap64)
    full_res = __builtin_bswap64(val);
#else
    full_res = (zig_u64)zig_byte_swap_u32((zig_u32)(val >>  0)) << 32 |
               (zig_u64)zig_byte_swap_u32((zig_u32)(val >> 32)) >>  0;
#endif
    return zig_wrap_u64(full_res >> (64 - bits), bits);
}

static inline zig_i64 zig_byte_swap_i64(zig_i64 val, zig_u8 bits) {
    return zig_wrap_i64((zig_i64)zig_byte_swap_u64((zig_u64)val, bits), bits);
}

static inline zig_u8 zig_bit_reverse_u8(zig_u8 val, zig_u8 bits) {
    zig_u8 full_res;
#if zig_has_builtin(bitreverse8)
    full_res = __builtin_bitreverse8(val);
#else
    static zig_u8 const lut[0x10] = {
        0b0000, 0b1000, 0b0100, 0b1100,
        0b0010, 0b1010, 0b0110, 0b1110,
        0b0001, 0b1001, 0b0101, 0b1101,
        0b0011, 0b1011, 0b0111, 0b1111,
    };
    full_res = lut[val >> 0 & 0xF] << 4 | lut[val >> 4 & 0xF] << 0;
#endif
    return zig_wrap_u8(full_res >> (8 - bits), bits);
}

static inline zig_i8 zig_bit_reverse_i8(zig_i8 val, zig_u8 bits) {
    return zig_wrap_i8((zig_i8)zig_bit_reverse_u8((zig_u8)val, bits), bits);
}

static inline zig_u16 zig_bit_reverse_u16(zig_u16 val, zig_u8 bits) {
    zig_u16 full_res;
#if zig_has_builtin(bitreverse16)
    full_res = __builtin_bitreverse16(val);
#else
    full_res = (zig_u16)zig_bit_reverse_u8((zig_u8)(val >>  0), 8) <<  8 |
               (zig_u16)zig_bit_reverse_u8((zig_u8)(val >>  8), 8) >>  0;
#endif
    return zig_wrap_u16(full_res >> (16 - bits), bits);
}

static inline zig_i16 zig_bit_reverse_i16(zig_i16 val, zig_u8 bits) {
    return zig_wrap_i16((zig_i16)zig_bit_reverse_u16((zig_u16)val, bits), bits);
}

static inline zig_u32 zig_bit_reverse_u32(zig_u32 val, zig_u8 bits) {
    zig_u32 full_res;
#if zig_has_builtin(bitreverse32)
    full_res = __builtin_bitreverse32(val);
#else
    full_res = (zig_u32)zig_bit_reverse_u16((zig_u16)(val >>  0), 16) << 16 |
               (zig_u32)zig_bit_reverse_u16((zig_u16)(val >> 16), 16) >>  0;
#endif
    return zig_wrap_u32(full_res >> (32 - bits), bits);
}

static inline zig_i32 zig_bit_reverse_i32(zig_i32 val, zig_u8 bits) {
    return zig_wrap_i32((zig_i32)zig_bit_reverse_u32((zig_u32)val, bits), bits);
}

static inline zig_u64 zig_bit_reverse_u64(zig_u64 val, zig_u8 bits) {
    zig_u64 full_res;
#if zig_has_builtin(bitreverse64)
    full_res = __builtin_bitreverse64(val);
#else
    full_res = (zig_u64)zig_bit_reverse_u32((zig_u32)(val >>  0), 32) << 32 |
               (zig_u64)zig_bit_reverse_u32((zig_u32)(val >> 32), 32) >>  0;
#endif
    return zig_wrap_u64(full_res >> (64 - bits), bits);
}

static inline zig_i64 zig_bit_reverse_i64(zig_i64 val, zig_u8 bits) {
    return zig_wrap_i64((zig_i64)zig_bit_reverse_u64((zig_u64)val, bits), bits);
}

/* ========================= Floating Point Support ========================= */

#define zig_has_f16 1
#define zig_bitSizeOf_f16 16
#define zig_libc_name_f16(name) __##name##h
#define zig_as_special_f16(sign, name, arg, repr) sign zig_as_f16(__builtin_##name, )(arg)
#if FLT_MANT_DIG == 11
typedef float zig_f16;
#define zig_as_f16(fp, repr) fp##f
#elif DBL_MANT_DIG == 11
typedef double zig_f16;
#define zig_as_f16(fp, repr) fp
#elif LDBL_MANT_DIG == 11
#define zig_bitSizeOf_c_longdouble 16
typedef long double zig_f16;
#define zig_as_f16(fp, repr) fp##l
#elif FLT16_MANT_DIG == 11
typedef _Float16 zig_f16;
#define zig_as_f16(fp, repr) fp##f16
#elif defined(__SIZEOF_FP16__)
typedef __fp16 zig_f16;
#define zig_as_f16(fp, repr) fp##f16
#else
#undef zig_has_f16
#define zig_has_f16 0
#define zig_repr_f16 i16
typedef zig_i16 zig_f16;
#define zig_as_f16(fp, repr) repr
#undef zig_as_special_f16
#define zig_as_special_f16(sign, name, arg, repr) repr
#endif

#define zig_has_f32 1
#define zig_bitSizeOf_f32 32
#define zig_libc_name_f32(name) name##f
#define zig_as_special_f32(sign, name, arg, repr) sign zig_as_f32(__builtin_##name, )(arg)
#if FLT_MANT_DIG == 24
typedef float zig_f32;
#define zig_as_f32(fp, repr) fp##f
#elif DBL_MANT_DIG == 24
typedef double zig_f32;
#define zig_as_f32(fp, repr) fp
#elif LDBL_MANT_DIG == 24
#define zig_bitSizeOf_c_longdouble 32
typedef long double zig_f32;
#define zig_as_f32(fp, repr) fp##l
#elif FLT32_MANT_DIG == 24
typedef _Float32 zig_f32;
#define zig_as_f32(fp, repr) fp##f32
#else
#undef zig_has_f32
#define zig_has_f32 0
#define zig_repr_f32 i32
typedef zig_i32 zig_f32;
#define zig_as_f32(fp, repr) repr
#undef zig_as_special_f32
#define zig_as_special_f32(sign, name, arg, repr) repr
#endif

#define zig_has_f64 1
#define zig_bitSizeOf_f64 64
#define zig_libc_name_f64(name) name
#define zig_as_special_f64(sign, name, arg, repr) sign zig_as_f64(__builtin_##name, )(arg)
#if FLT_MANT_DIG == 53
typedef float zig_f64;
#define zig_as_f64(fp, repr) fp##f
#elif DBL_MANT_DIG == 53
typedef double zig_f64;
#define zig_as_f64(fp, repr) fp
#elif LDBL_MANT_DIG == 53
#define zig_bitSizeOf_c_longdouble 64
typedef long double zig_f64;
#define zig_as_f64(fp, repr) fp##l
#elif FLT64_MANT_DIG == 53
typedef _Float64 zig_f64;
#define zig_as_f64(fp, repr) fp##f64
#elif FLT32X_MANT_DIG == 53
typedef _Float32x zig_f64;
#define zig_as_f64(fp, repr) fp##f32x
#else
#undef zig_has_f64
#define zig_has_f64 0
#define zig_repr_f64 i64
typedef zig_i64 zig_f64;
#define zig_as_f64(fp, repr) repr
#undef zig_as_special_f64
#define zig_as_special_f64(sign, name, arg, repr) repr
#endif

#define zig_has_f80 1
#define zig_bitSizeOf_f80 80
#define zig_libc_name_f80(name) __##name##x
#define zig_as_special_f80(sign, name, arg, repr) sign zig_as_f80(__builtin_##name, )(arg)
#if FLT_MANT_DIG == 64
typedef float zig_f80;
#define zig_as_f80(fp, repr) fp##f
#elif DBL_MANT_DIG == 64
typedef double zig_f80;
#define zig_as_f80(fp, repr) fp
#elif LDBL_MANT_DIG == 64
#define zig_bitSizeOf_c_longdouble 80
typedef long double zig_f80;
#define zig_as_f80(fp, repr) fp##l
#elif FLT80_MANT_DIG == 64
typedef _Float80 zig_f80;
#define zig_as_f80(fp, repr) fp##f80
#elif FLT64X_MANT_DIG == 64
typedef _Float64x zig_f80;
#define zig_as_f80(fp, repr) fp##f64x
#elif defined(__SIZEOF_FLOAT80__)
typedef __float80 zig_f80;
#define zig_as_f80(fp, repr) fp##l
#else
#undef zig_has_f80
#define zig_has_f80 0
#define zig_as_f80(fp, repr) repr
#undef zig_as_special_f80
#define zig_as_special_f80(sign, name, arg, repr) repr
#endif

#define zig_has_c_longdouble 1
typedef long double zig_c_longdouble;
#define zig_as_c_longdouble(fp, repr) fp##l
#define zig_libc_name_c_longdouble(name) name##l
#define zig_as_special_c_longdouble(sign, name, arg, repr) sign __builtin_##name##l(arg)

#define zig_convert_builtin(ResType, operation, ArgType, version) \
    zig_extern zig_##ResType zig_expand_concat(zig_expand_concat(zig_expand_concat(__##operation, \
        zig_compiler_rt_abbrev_##ArgType), zig_compiler_rt_abbrev_##ResType), version)(zig_##ArgType);
zig_convert_builtin(f16,  trunc,  f32,  2)
zig_convert_builtin(f16,  trunc,  f64,  2)
zig_convert_builtin(f32,  extend, f16,  2)
zig_convert_builtin(f32,  trunc,  f64,  2)
zig_convert_builtin(f64,  extend, f16,  2)
zig_convert_builtin(f64,  extend, f32,  2)

#define zig_float_negate_builtin_0(Type) \
    static inline zig_##Type zig_neg_##Type(zig_##Type arg) { \
        return zig_expand_concat(zig_xor_, zig_repr_##Type)(arg, zig_expand_minInt(zig_repr_##Type, zig_bitSizeOf_##Type)); \
    }
#define zig_float_negate_builtin_1(Type) \
    static inline zig_##Type zig_neg_##Type(zig_##Type arg) { \
        return -arg; \
    }

#define zig_float_less_builtin_0(Type, operation) \
    zig_extern zig_i8 zig_expand_concat(zig_expand_concat(__##operation, \
        zig_compiler_rt_abbrev_##Type), 2)(zig_##Type, zig_##Type); \
    static inline zig_i8 zig_##operation##_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return (zig_i8)zig_expand_concat(zig_expand_concat(__##operation, zig_compiler_rt_abbrev_##Type), 2)(lhs, rhs); \
    }
#define zig_float_less_builtin_1(Type, operation) \
    static inline zig_i8 zig_##operation##_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return (zig_i8)(!(lhs <= rhs) - (lhs < rhs)); \
    }

#define zig_float_greater_builtin_0(Type, operation) \
    zig_float_less_builtin_0(Type, operation)
#define zig_float_greater_builtin_1(Type, operation) \
    static inline zig_i8 zig_##operation##_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return (zig_i8)((lhs > rhs) - !(lhs >= rhs)); \
    }

#define zig_float_binary_builtin_0(Type, operation, operator) \
    zig_extern zig_##Type zig_expand_concat(zig_expand_concat(__##operation, \
        zig_compiler_rt_abbrev_##Type), 3)(zig_##Type, zig_##Type); \
    static inline zig_##Type zig_##operation##_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return zig_expand_concat(zig_expand_concat(__##operation, zig_compiler_rt_abbrev_##Type), 3)(lhs, rhs); \
    }
#define zig_float_binary_builtin_1(Type, operation, operator) \
    static inline zig_##Type zig_##operation##_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return lhs operator rhs; \
    }

#define zig_float_builtins(Type) \
    zig_convert_builtin(i32,  fix,     Type, ) \
    zig_convert_builtin(u32,  fixuns,  Type, ) \
    zig_convert_builtin(i64,  fix,     Type, ) \
    zig_convert_builtin(u64,  fixuns,  Type, ) \
    zig_convert_builtin(Type, float,   i32,  ) \
    zig_convert_builtin(Type, floatun, u32,  ) \
    zig_convert_builtin(Type, float,   i64,  ) \
    zig_convert_builtin(Type, floatun, u64,  ) \
    zig_expand_concat(zig_float_negate_builtin_,  zig_has_##Type)(Type) \
    zig_expand_concat(zig_float_less_builtin_,    zig_has_##Type)(Type, cmp) \
    zig_expand_concat(zig_float_less_builtin_,    zig_has_##Type)(Type, ne) \
    zig_expand_concat(zig_float_less_builtin_,    zig_has_##Type)(Type, eq) \
    zig_expand_concat(zig_float_less_builtin_,    zig_has_##Type)(Type, lt) \
    zig_expand_concat(zig_float_less_builtin_,    zig_has_##Type)(Type, le) \
    zig_expand_concat(zig_float_greater_builtin_, zig_has_##Type)(Type, gt) \
    zig_expand_concat(zig_float_greater_builtin_, zig_has_##Type)(Type, ge) \
    zig_expand_concat(zig_float_binary_builtin_,  zig_has_##Type)(Type, add, +) \
    zig_expand_concat(zig_float_binary_builtin_,  zig_has_##Type)(Type, sub, -) \
    zig_expand_concat(zig_float_binary_builtin_,  zig_has_##Type)(Type, mul, *) \
    zig_expand_concat(zig_float_binary_builtin_,  zig_has_##Type)(Type, div, /) \
    zig_extern zig_##Type zig_libc_name_##Type(sqrt)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(sin)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(cos)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(tan)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(exp)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(exp2)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(log)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(log2)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(log10)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(fabs)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(floor)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(ceil)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(round)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(trunc)(zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(fmod)(zig_##Type, zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(fmin)(zig_##Type, zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(fmax)(zig_##Type, zig_##Type); \
    zig_extern zig_##Type zig_libc_name_##Type(fma)(zig_##Type, zig_##Type, zig_##Type); \
\
    static inline zig_##Type zig_div_trunc_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return zig_libc_name_##Type(trunc)(zig_div_##Type(lhs, rhs)); \
    } \
\
    static inline zig_##Type zig_div_floor_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return zig_libc_name_##Type(floor)(zig_div_##Type(lhs, rhs)); \
    } \
\
    static inline zig_##Type zig_mod_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return zig_sub_##Type(lhs, zig_mul_##Type(zig_div_floor_##Type(lhs, rhs), rhs)); \
    }
zig_float_builtins(f16)
zig_float_builtins(f32)
zig_float_builtins(f64)
zig_float_builtins(c_longdouble)