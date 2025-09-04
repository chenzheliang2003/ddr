/*
 * 北太天元（Baltamatica）开发者工具箱头文件。
 *
 * 该文件包含使用 bex 库必要的宏定义，类型声明，函数原型声明。
 * 适用于当前版本的北太天元。
 *
 * 版权所有 © 2021-2022，北太振寰（重庆）科技有限公司
 */

#ifndef BALTAM_EXTERN_OBJ_H
#define BALTAM_EXTERN_OBJ_H

#include <string>
#include "api_macro.h"

namespace baltam {
    /**
     * @class extern_obj_base
     * @brief 所有不属于 BALTAM 的外部对象基类。
     *
     * 当合作开发者/社区开发者使用 C++ 开发并且需要将某些对象临时托管到 BALTAM 中时，
     * 必须从这个类继承。
     *
     * 软件内核对该类对象仅定义抽象的方法，具体实现交给开发者。软件无需了解
     * @p extern_obj_base 子类的实现细节。
     */
    struct BALTAM_API extern_obj_base {
        virtual extern_obj_base* dup() const { return nullptr; }
        virtual ~extern_obj_base() {};
        virtual std::string to_string() const { return ""; }
        virtual std::string classname() const { return "extern object"; };
        int sid; ///< 整数，用于存储内核自动分配的外部类 ID
    };
}

#endif

