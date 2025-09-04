/*
 * 北太天元（Baltamatica）开发者工具箱头文件。
 *
 * 该文件包含使用 bex 库必要的宏定义，类型声明，函数原型声明。
 * 提供 C++ 编程接口。
 * 适用于 v3.5 版本的北太天元。
 *
 * 版权所有 © 2021-2024，北太振寰（重庆）科技有限公司
 */


#ifndef BALTAM_BEX_HPP
#define BALTAM_BEX_HPP

#include <vector>
#include <string>
#include "ba_obj/extern_obj.h"
#include "bex/bex.h"

typedef int (*bxAddCXXClass_impl_t)(const char *);
typedef void *(*bxGetExtObj_impl_t)(const bxArray*);

extern "C" {
    BALTAM_API int bxAddCXXClass_impl(const char *);
    BALTAM_API void * bxGetExtObj_impl(const bxArray*);
    BALTAM_API const void * bxGetExtObjRO_impl(const bxArray * ba);
    BALTAM_API void * bxGetExtObjRW_impl(bxArray * ba);
    BALTAM_API bxArray * bxCreateExtObj_v(const void *);
}

namespace baltam {

    BALTAM_API bxArray *bxCreateExtObj(const extern_obj_base*);
    BALTAM_API std::string bxTypeString(const bxArray*);
    BALTAM_API std::string bxAsString(const bxArray*);
    BALTAM_API std::string* bxGetStringPr(const bxArray*);
    BALTAM_API std::vector<std::string> bxGetFieldNames(const bxArray*);
    BALTAM_API std::vector<bxArray*> bxGetCellsV(const bxArray*);
    BALTAM_API std::vector<const bxArray*> bxGetCellsVRO(const bxArray*);
    BALTAM_API std::vector<bxArray*> bxGetCellsVRW(const bxArray*);

    template <class T, typename... Args>
    T * bxNewCXXObject(Args&&... args){
        T * ptr = new T{args...};
        ptr->sid = T::ID;
        return ptr;
    }

    template <class T, typename... Args>
    bxArray *bxCreateExtObj(Args&&... args){
        T * ptr = bxNewCXXObject<T>(args...);
        return bxCreateExtObj(ptr);
    }

    template <class T>
    int bxAddCXXClass(const std::string& name){
        int id = bxAddCXXClass_impl(name.c_str());
        if (id > 0) T::ID = id;
        return id;
    }

    template <class T>
    int bxAddCXXClass(const std::string& name, bxAddCXXClass_impl_t impl){
        int id = impl(name.c_str());
        if (id > 0) T::ID = id;
        return id;
    }

    template <class T>
    const T * bxGetExtObjRO(const bxArray * in){
        const void * ptr = bxGetExtObjRO_impl(in);
        if (ptr == nullptr)
            return nullptr;

        return dynamic_cast<const T*>(static_cast<const extern_obj_base*>(ptr));
    }

    template <class T>
    T * bxGetExtObjRW(bxArray * in){
        void * ptr = bxGetExtObjRW_impl(in);
        if (ptr == nullptr)
            return nullptr;

        return dynamic_cast<T*>(static_cast<extern_obj_base*>(ptr));
    }

    template <class T>
    T * bxGetExtObj(const bxArray* in){
        void * ptr = bxGetExtObj_impl(in);
        if (ptr == nullptr)
            return nullptr;

        return dynamic_cast<T*>(static_cast<extern_obj_base*>(ptr));
    }

    template <class T>
    T * bxGetExtObj(const bxArray* in, bxGetExtObj_impl_t impl){
        void * ptr = impl(in);
        if (ptr == nullptr)
            return nullptr;

        return dynamic_cast<T*>(static_cast<extern_obj_base*>(ptr));
    }

}

#ifdef BALTAM_BEX_LEGACY_GETTER_RW
#define bxGetCellsV bxGetCellsRW
#endif

#endif

