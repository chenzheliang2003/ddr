/*
 * 北太天元（Baltamatica）开发者工具箱头文件。
 *
 * 该文件包含 API 导出的宏定义。
 * 适用于任意版本的北太天元。
 *
 * 版权所有 © 2021-2024，北太振寰（重庆）科技有限公司
 */

#ifndef BALTAM_CORE_API_MACRO_H
#define BALTAM_CORE_API_MACRO_H

// 导出共享库符号的相关宏
#if defined(__WIN32__) || defined(__CYGWIN__)
    #define BALTAM_HELPER_DLL_IMPORT __declspec(dllimport)
    #define BALTAM_HELPER_DLL_EXPORT __declspec(dllexport)
    #define BALTAM_HELPER_DLL_LOCAL
#elif (defined(__GNUC__) && __GNUC__ >= 4) || defined(__clang__)
    #define BALTAM_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define BALTAM_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define BALTAM_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
    #define BALTAM_HELPER_DLL_IMPORT
    #define BALTAM_HELPER_DLL_EXPORT
    #define BALTAM_HELPER_DLL_LOCAL
#endif

#ifdef BALTAM_BUILD_DYN_LIB
    #ifdef BALTAM_DLL_EXPORTS
        #define BALTAM_API BALTAM_HELPER_DLL_EXPORT
    #else
        #define BALTAM_API BALTAM_HELPER_DLL_IMPORT
    #endif
    #define BALTAM_LOCAL BALTAM_HELPER_DLL_LOCAL
#else
    #define BALTAM_API
    #define BALTAM_LOCAL
#endif

#endif

