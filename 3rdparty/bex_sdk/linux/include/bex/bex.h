/*
 * 北太天元（Baltamatica）开发者工具箱头文件。
 *
 * 该文件包含使用 bex 库必要的宏定义，类型声明，函数原型声明。
 * 适用于 v4.0 版本的北太天元。
 *
 * 版权所有 © 2021-2024，北太振寰（重庆）科技有限公司
 */

#ifndef BALTAM_BEX_H
#define BALTAM_BEX_H

// 默认整数类型
#ifndef BALTAM_DEFAULT_INT
#include <stddef.h>
#define BALTAM_DEFAULT_INT ptrdiff_t
#endif

#include <stdint.h>
#include <stdbool.h>
#include "api_macro.h"
#include "bex/bx_op.h"
#include "bex/extern_fcn_type.h"
#include "ba_obj/calendar_duration_elem.h"

#ifndef BALTAM_DLL_EXPORTS
#include "bex_config.h"
#endif

// 宏展开：统一工具箱接口
#define BALTAM_PLUGIN_FCN(word) \
    void word(int nlhs, bxArray *plhs[], int nrhs, const bxArray *prhs[])


#ifdef __cplusplus
extern "C" {
#endif

// 定义数据类型
typedef BALTAM_DEFAULT_INT baInt;
typedef baInt baIndex;
typedef baInt baSize;
typedef baIndex baSparseIndex;

// 定义用于传递内核变量的 C 结构体（不透明对象）
struct __bxArray;
typedef struct __bxArray bxArray;

typedef enum {
    bxREAL = 0,
    bxCOMPLEX
} bxComplexity;

typedef enum {
    bxDENSE = 0,
    bxSPARSE
} bxSparsity;

typedef enum {
    bxUNKNOWN_CLASS = 0,
    bxINT8_CLASS = 1, // 需要从 1 开始
    bxINT16_CLASS,
    bxINT_CLASS,
    bxINT32_CLASS = bxINT_CLASS,
    bxINT64_CLASS,
    bxUINT8_CLASS,
    bxUINT16_CLASS,
    bxUINT32_CLASS,
    bxUINT64_CLASS,
    bxDOUBLE_CLASS,
    bxSINGLE_CLASS,
    bxCHAR_CLASS = 13,
    bxLOGICAL_CLASS,
    bxSTRUCT_CLASS = 16,
    bxSTRING_CLASS,
    bxOBJECT_CLASS = 20,
    bxEXTERN_CLASS = 21,
    bxVOID_CLASS = 22,
    bxCELL_CLASS = 25,
    bxTABLE_CLASS = 32,
    bxDATETIME_CLASS,
    bxDURATION_CLASS,
    bxCALENDAR_DURATION_CLASS,
    bxTIMETABLE_CLASS,
} bxClassID;

// 函数句柄类型枚举类
typedef enum {
    bxFH_UNKNOWN = -1,
    bxFH_ANONYMOUS = 0, // 匿名函数
    bxFH_VARIABLE,      // 变量
    bxFH_BUILTIN,       // 内置函数
    bxFH_MFUNCTION,     // M 函数
    bxFH_SCRIPT         // 脚本
} bxFHandleType;

// 表格类型插入模式
typedef enum {
    bxINSERT_BEFORE = 0,
    bxINSERT_AFTER
} bxInsertMode;

// 表格维度
typedef enum {
    bxTABLE_ROW = 0,
    bxTABLE_COLUMN
} bxTableDimension;

// 添加变量模式
typedef enum {
    bxNON_OVERWRITE = 0,
    bxOVERWRITE
} bxVarOpMode;

// 获取公有属性模式
typedef enum {
    bxEXCLUDE_HIDDEN = 0,
    bxINCLUDE_HIDDEN
} bxPubPropMode;

// 获取属性访问权限模式
typedef enum {
    bxGET_ACCESS = 0,
    bxSET_ACCESS
} bxPropertyAccessType;

// 访问权限
typedef enum {
    bxACCESS_UNKNOWN,
    bxACCESS_PUBLIC = 0,
    bxACCESS_PROTECTED,
    bxACCESS_PRIVATE
} bxAccessLevel;

// bex 接口函数原型
// 获取数据
BALTAM_API bxClassID bxGetClassID(const bxArray*);
BALTAM_API const char * bxTypeCStr(const bxArray*);
BALTAM_API baSize bxGetNumberOfDimensions(const bxArray*);
BALTAM_API const baSize * bxGetDimensions(const bxArray*);
BALTAM_API baSize bxGetNumberOfElements(const bxArray*);
BALTAM_API baSize bxGetM(const bxArray*);
BALTAM_API baSize bxGetN(const bxArray*);
BALTAM_API baSize bxGetStringLen(const bxArray*);
BALTAM_API baSize bxGetStringLength(const bxArray*, baIndex);
// 旧版 getter
BALTAM_API double* bxGetPr(const bxArray*);
BALTAM_API int8_t* bxGetInt8s(const bxArray*);
BALTAM_API int16_t* bxGetInt16s(const bxArray*);
BALTAM_API int32_t* bxGetInt32s(const bxArray*);
BALTAM_API int64_t* bxGetInt64s(const bxArray*);
BALTAM_API uint8_t* bxGetUInt8s(const bxArray*);
BALTAM_API uint16_t* bxGetUInt16s(const bxArray*);
BALTAM_API uint32_t* bxGetUInt32s(const bxArray*);
BALTAM_API uint64_t* bxGetUInt64s(const bxArray*);
BALTAM_API double* bxGetDoubles(const bxArray*);
BALTAM_API float* bxGetSingles(const bxArray*);
BALTAM_API void* bxGetComplexDoubles(const bxArray*);
BALTAM_API void* bxGetComplexSingles(const bxArray*);
BALTAM_API char* bxGetChars(const bxArray*);
BALTAM_API bool* bxGetLogicals(const bxArray*);
// v3 新增新版 getter，区分了读写和只读的情况
BALTAM_API const double* bxGetPrRO(const bxArray*);
BALTAM_API const int8_t* bxGetInt8sRO(const bxArray*);
BALTAM_API const int16_t* bxGetInt16sRO(const bxArray*);
BALTAM_API const int32_t* bxGetInt32sRO(const bxArray*);
BALTAM_API const int64_t* bxGetInt64sRO(const bxArray*);
BALTAM_API const uint8_t* bxGetUInt8sRO(const bxArray*);
BALTAM_API const uint16_t* bxGetUInt16sRO(const bxArray*);
BALTAM_API const uint32_t* bxGetUInt32sRO(const bxArray*);
BALTAM_API const uint64_t* bxGetUInt64sRO(const bxArray*);
BALTAM_API const double* bxGetDoublesRO(const bxArray*);
BALTAM_API const float* bxGetSinglesRO(const bxArray*);
BALTAM_API const void* bxGetComplexDoublesRO(const bxArray*);
BALTAM_API const void* bxGetComplexSinglesRO(const bxArray*);
BALTAM_API const char* bxGetCharsRO(const bxArray*);
BALTAM_API const bool* bxGetLogicalsRO(const bxArray*);
// RW
BALTAM_API double* bxGetPrRW(const bxArray*);
BALTAM_API int8_t* bxGetInt8sRW(const bxArray*);
BALTAM_API int16_t* bxGetInt16sRW(const bxArray*);
BALTAM_API int32_t* bxGetInt32sRW(const bxArray*);
BALTAM_API int64_t* bxGetInt64sRW(const bxArray*);
BALTAM_API uint8_t* bxGetUInt8sRW(const bxArray*);
BALTAM_API uint16_t* bxGetUInt16sRW(const bxArray*);
BALTAM_API uint32_t* bxGetUInt32sRW(const bxArray*);
BALTAM_API uint64_t* bxGetUInt64sRW(const bxArray*);
BALTAM_API double* bxGetDoublesRW(const bxArray*);
BALTAM_API float* bxGetSinglesRW(const bxArray*);
BALTAM_API void* bxGetComplexDoublesRW(const bxArray*);
BALTAM_API void* bxGetComplexSinglesRW(const bxArray*);
BALTAM_API char* bxGetCharsRW(const bxArray*);
BALTAM_API bool* bxGetLogicalsRW(const bxArray*);
// end of getters
BALTAM_API const char* bxGetString(const bxArray*, baIndex);
BALTAM_API const char* bxGetStringDataPr(const bxArray*);
BALTAM_API bxArray* bxGetCell(const bxArray*, baIndex);
BALTAM_API const bxArray* bxGetCellRO(const bxArray*, baIndex);
BALTAM_API bxArray* bxGetCellRW(const bxArray*, baIndex);
BALTAM_API bool bxIsInt8(const bxArray*);
BALTAM_API bool bxIsInt16(const bxArray*);
BALTAM_API bool bxIsInt32(const bxArray*);
BALTAM_API bool bxIsInt64(const bxArray*);
BALTAM_API bool bxIsUInt8(const bxArray*);
BALTAM_API bool bxIsUInt16(const bxArray*);
BALTAM_API bool bxIsUInt32(const bxArray*);
BALTAM_API bool bxIsUInt64(const bxArray*);
BALTAM_API bool bxIsDouble(const bxArray*);
BALTAM_API bool bxIsSingle(const bxArray*);
BALTAM_API bool bxIsComplex(const bxArray*);
BALTAM_API bool bxIsChar(const bxArray*);
BALTAM_API bool bxIsLogical(const bxArray*);
BALTAM_API bool bxIsString(const bxArray*);
BALTAM_API bool bxIsCell(const bxArray*);
BALTAM_API bool bxIsRealDouble(const bxArray*);
BALTAM_API bool bxIsRealSingle(const bxArray*);
BALTAM_API bool bxIsComplexDouble(const bxArray*);
BALTAM_API bool bxIsComplexSingle(const bxArray*);
// 创建和删除
BALTAM_API bxArray* bxCreateDoubleMatrix(baSize, baSize, bxComplexity);
BALTAM_API bxArray* bxCreateNumericMatrix(baSize, baSize, bxClassID, bxComplexity);
BALTAM_API bxArray* bxCreateNumericArray(baSize, const baSize *, bxClassID, bxComplexity);
BALTAM_API bxArray* bxCreateLogicalMatrix(baSize, baSize);
BALTAM_API bxArray* bxCreateLogicalArray(baSize, const baSize *);
BALTAM_API bxArray* bxCreateCharMatrixFromStrings(baSize, const char **);
BALTAM_API bxArray* bxCreateCharArray(baSize, const baSize *);
BALTAM_API bxArray* bxCreateString(const char *);
BALTAM_API bxArray* bxCreateStringObj(const char *);
BALTAM_API bxArray* bxCreateCellMatrix(baSize, baSize);
BALTAM_API bxArray* bxCreateCellArray(baSize, const baSize *);
BALTAM_API bxArray* bxCreateStringMatrix(baSize, baSize);
BALTAM_API bxArray* bxCreateStringArray(baSize, const baSize *);
BALTAM_API bxArray* bxCreateStringMatrixFromStrings(baSize, baSize, const char **);
BALTAM_API bxArray* bxCreateInt8Scalar(int8_t);
BALTAM_API bxArray* bxCreateInt16Scalar(int16_t);
BALTAM_API bxArray* bxCreateInt32Scalar(int32_t);
BALTAM_API bxArray* bxCreateInt64Scalar(int64_t);
BALTAM_API bxArray* bxCreateUInt8Scalar(uint8_t);
BALTAM_API bxArray* bxCreateUInt16Scalar(uint16_t);
BALTAM_API bxArray* bxCreateUInt32Scalar(uint32_t);
BALTAM_API bxArray* bxCreateUInt64Scalar(uint64_t);
BALTAM_API bxArray* bxCreateDoubleScalar(double);
BALTAM_API bxArray* bxCreateSingleScalar(float);
BALTAM_API bxArray* bxCreateComplexDoubleScalar(double, double);
BALTAM_API bxArray* bxCreateComplexSingleScalar(float, float);
// BALTAM_API bxArray *bxCreateCharScalar(char);
BALTAM_API bxArray* bxCreateLogicalScalar(bool);
BALTAM_API bxArray* bxCreateStringScalar(const char *);
BALTAM_API bxArray* bxDuplicateArray(const bxArray*);
BALTAM_API bxArray* bxDuplicateArrayS(const bxArray*);
BALTAM_API void bxCopyArray(const bxArray *src, bxArray *dst);
BALTAM_API void bxCopyArrayS(const bxArray *src, bxArray *dst);
BALTAM_API void bxDestroyArray(bxArray*);
// 修改数据
BALTAM_API void bxSetM(bxArray*, baSize);
BALTAM_API void bxSetN(bxArray*, baSize);
BALTAM_API void bxResize(bxArray*, baSize, baSize);
BALTAM_API void bxSetDimensions(bxArray*, const baSize *, int);
BALTAM_API void bxSetStringFromCStr(bxArray*, const char *);
BALTAM_API void bxSetCell(bxArray*, baIndex, bxArray*);
BALTAM_API void bxSetString(bxArray*, baIndex, const char *);

// 类型转化
BALTAM_API baInt bxAsInt(const bxArray* ba, int *perr);
BALTAM_API int bxAsCStr(const bxArray *ba, char *buffer, baSize size);
BALTAM_API int bxMakeArrayComplex(bxArray *ba);
BALTAM_API int bxMakeArrayReal(bxArray *ba);
BALTAM_API void bxResetArray(bxArray *ba, bxClassID id, bxComplexity c, bxSparsity s);

// 结构体
BALTAM_API bxArray* bxCreateStructMatrix(baSize, baSize, int, const char **);
BALTAM_API bxArray* bxCreateStructArray(baSize, const baSize *, int, const char **);
BALTAM_API bxArray* bxExtractStructRows(const bxArray *ba, const baIndex *row_ind, int nrow);
BALTAM_API bxArray* bxExtractStructSubBlock(const bxArray *ba, const baIndex *row_ind, int nrow, const baIndex *col_ind, int ncol);
BALTAM_API bxArray* bxGetField(const bxArray*, baIndex, const char *);
BALTAM_API const bxArray* bxGetFieldRO(const bxArray*, baIndex, const char *);
BALTAM_API bxArray* bxGetFieldRW(const bxArray*, baIndex, const char *);
BALTAM_API bxArray* bxGetFieldByNumber(const bxArray*, baIndex, int);
BALTAM_API const bxArray* bxGetFieldByNumberRO(const bxArray*, baIndex, int);
BALTAM_API bxArray* bxGetFieldByNumberRW(const bxArray*, baIndex, int);
BALTAM_API baSize bxGetNumberOfFields(const bxArray*);
BALTAM_API int bxGetFieldNumber(const bxArray*, const char *);
BALTAM_API const char* bxGetFieldNameByNumber(const bxArray*, int);
BALTAM_API bool bxIsField(const bxArray *ba, const char *fieldname);
BALTAM_API void bxSetField(bxArray*, baIndex, const char *, bxArray*);
BALTAM_API void bxSetFieldByNumber(bxArray*, baIndex, int, bxArray*);
BALTAM_API void bxAddField(bxArray*, const char *);
BALTAM_API void bxAddFieldAt(bxArray*, baIndex, const char *);
BALTAM_API void bxRenameField(bxArray*, baIndex, const char *);
BALTAM_API void bxRemoveField(bxArray*, const char *);
BALTAM_API bool bxIsStruct(const bxArray*);

// 稀疏矩阵
BALTAM_API bxArray* bxCreateSparse(baSize, baSize, baSize, bxComplexity);
BALTAM_API bxArray* bxCreateSparseNumericMatrix(baSize, baSize, baSize, bxClassID, bxComplexity);
BALTAM_API bxArray* bxCreateSparseLogicalMatrix(baSize, baSize, baSize);
BALTAM_API bool bxIsSparse(const bxArray*);
BALTAM_API bool bxIsSparseRealDouble(const bxArray*);
BALTAM_API bool bxIsSparseRealSingle(const bxArray*);
BALTAM_API bool bxIsSparseComplexDouble(const bxArray*);
BALTAM_API bool bxIsSparseComplexSingle(const bxArray*);
BALTAM_API bool bxIsSparseLogical(const bxArray*);
// 旧版 getters
BALTAM_API double* bxGetSparseDoubles(const bxArray*);
BALTAM_API float* bxGetSparseSingles(const bxArray*);
BALTAM_API void* bxGetSparseComplexDoubles(const bxArray*);
BALTAM_API void* bxGetSparseComplexSingles(const bxArray*);
BALTAM_API bool* bxGetSparseLogicals(const bxArray*);
BALTAM_API baSparseIndex* bxGetIr(const bxArray*);
BALTAM_API baSparseIndex* bxGetJc(const bxArray*);
// v3 getters
BALTAM_API const double* bxGetSparseDoublesRO(const bxArray*);
BALTAM_API const float* bxGetSparseSinglesRO(const bxArray*);
BALTAM_API const void* bxGetSparseComplexDoublesRO(const bxArray*);
BALTAM_API const void* bxGetSparseComplexSinglesRO(const bxArray*);
BALTAM_API const bool* bxGetSparseLogicalsRO(const bxArray*);
BALTAM_API const baSparseIndex* bxGetIrRO(const bxArray*);
BALTAM_API const baSparseIndex* bxGetJcRO(const bxArray*);
BALTAM_API double* bxGetSparseDoublesRW(const bxArray*);
BALTAM_API float* bxGetSparseSinglesRW(const bxArray*);
BALTAM_API void* bxGetSparseComplexDoublesRW(const bxArray*);
BALTAM_API void* bxGetSparseComplexSinglesRW(const bxArray*);
BALTAM_API bool* bxGetSparseLogicalsRW(const bxArray*);
BALTAM_API baSparseIndex* bxGetIrRW(const bxArray*);
BALTAM_API baSparseIndex* bxGetJcRW(const bxArray*);
// end of getters
BALTAM_API baSize bxGetNnz(const bxArray*);
BALTAM_API baSize bxGetNzmax(const bxArray*);
BALTAM_API void bxSetNzmax(bxArray*, baSize);
BALTAM_API void bxSparseFinalize(bxArray*);

// 函数句柄
BALTAM_API bool bxIsFunctionHandle(const bxArray*);
BALTAM_API bxFHandleType bxGetFunctionHandleType(const bxArray*);
BALTAM_API const char * bxGetFunctionHandleData(const bxArray*);

// 表格
BALTAM_API bxArray* bxCreateTable(baSize m, baSize n);
BALTAM_API bxArray* bxCreateNullTable(baSize m, baSize n);
BALTAM_API bxArray* bxCreateTableFromColumns(baSize n, const bxArray **columns);
BALTAM_API void bxResizeTableRows(bxArray *ba, baSize nrows);
BALTAM_API const bxArray* bxGetTableColumnRO(const bxArray *ba, baIndex i);
BALTAM_API bxArray* bxGetTableColumnRW(const bxArray *ba, baIndex i);
BALTAM_API const bxArray* bxGetTableColumnByNameRO(const bxArray *ba, const char *name);
BALTAM_API bxArray* bxGetTableColumnByNameRW(const bxArray *ba, const char *name);
BALTAM_API void bxSetTableColumn(bxArray *ba, baIndex i, const bxArray *column);
BALTAM_API void bxSetTableColumnByName(bxArray *ba, const char *name, const bxArray *column);
BALTAM_API void bxInsertTableColumn(bxArray *ba, baIndex i, bxInsertMode mode, const char *name, const bxArray *column);
BALTAM_API void bxInsertTableColumnByName(bxArray *ba, const char *ref, bxInsertMode mode, const char *name, const bxArray *column);
BALTAM_API void bxAppendTableColumn(bxArray *ba, const char *name, const bxArray *column);
BALTAM_API void bxRemoveTableColumn(bxArray *ba, baIndex i);
BALTAM_API void bxRemoveTableColumnByName(bxArray *ba, const char *name);
BALTAM_API void bxRenameTableColumn(bxArray *ba, baIndex i, const char *name);
BALTAM_API void bxRenameTableColumnByName(bxArray *ba, const char *old_name, const char *name);
BALTAM_API const char *bxGetTableColumnName(const bxArray *ba, baIndex i);
BALTAM_API const char *bxGetTableRowName(const bxArray *ba, baIndex i);
BALTAM_API const char *bxGetTableDimensionName(const bxArray *ba, bxTableDimension dim);
BALTAM_API void bxSetTableColumnNames(bxArray *ba, const char **names);
BALTAM_API void bxSetTableRowNames(bxArray *ba, const char **names);
BALTAM_API void bxSetTableDimensionName(bxArray *ba, bxTableDimension dim, const char *name);
BALTAM_API bool bxIsTableColumn(const bxArray *ba, const char *name);
BALTAM_API void bxRenameTableRow(bxArray *ba, baIndex i, const char * name);

// 日期时间（datetime）
BALTAM_API bxArray* bxCreateDatetimeMatrix(baSize m, baSize n);
BALTAM_API bxArray* bxCreateDatetimeArray(baSize ndim, const baSize * dim_vec);
BALTAM_API bxArray* bxCreateDatetimeScalar(double date);
BALTAM_API bxArray* bxCreateDatetimeScalarFromString(const char *date_str);
BALTAM_API const double* bxGetDatetimesRO(const bxArray *ba);
BALTAM_API double* bxGetDatetimesRW(const bxArray *ba);
BALTAM_API int bxSetDatetimeByString(bxArray *ba, baIndex ind, const char * date_str);
BALTAM_API int bxSetDatetimeFormat(bxArray *ba, const char *fmt);
BALTAM_API int bxSetTimeZone(bxArray *ba, const char *tz);
BALTAM_API void bxGetDatetimeFormat(const bxArray *ba, char *result, baSize len);
BALTAM_API void bxGetTimeZone(const bxArray *ba, char *result, baSize len);
BALTAM_API void bxGetDatetimeDisplayString(const bxArray *ba, baIndex ind, char *result, baSize len);

// 持续时间（duration）
BALTAM_API bxArray* bxCreateDurationMatrix(baSize m, baSize n);
BALTAM_API bxArray* bxCreateDurationArray(baSize ndim, const baSize * dim_vec);
BALTAM_API bxArray* bxCreateDurationScalar(double ms);
BALTAM_API const double* bxGetDurationsRO(const bxArray *ba);
BALTAM_API double* bxGetDurationsRW(const bxArray *ba);
BALTAM_API const char *bxGetDurationFormat(const bxArray *ba);
BALTAM_API bool bxSetDurationFormat(bxArray *ba, const char *fmt);
BALTAM_API baSize bxGetDurationDisplayString(const bxArray *ba, baIndex ind, char *result, baSize len);

// 日历持续时间（calendarDuration）
BALTAM_API bxArray* bxCreateCalendarDurationMatrix(baSize m, baSize n);
BALTAM_API bxArray* bxCreateCalendarDurationArray(baSize ndim, const baSize * dim_vec);
BALTAM_API bxArray* bxCreateCalendarDurationScalar(int year, int month, int day, int hour, int minute, double second);
BALTAM_API bxArray* bxCreateCalendarDurationScalarByMS(double ms);
BALTAM_API const baCalendarDurationElem* bxGetCalendarDurationsRO(const bxArray *ba);
BALTAM_API baCalendarDurationElem* bxGetCalendarDurationsRW(const bxArray *ba);
BALTAM_API const char *bxGetCalendarDurationFormat(const bxArray *ba);
BALTAM_API bool bxSetCalendarDurationFormat(bxArray *ba, const char *fmt);
BALTAM_API void bxNormalizeCalendarDuration(bxArray *ba);

// 对象（object）
BALTAM_API bool bxIsClass(const bxArray *ba, const char * classname);
BALTAM_API bool bxIsClassdef(const bxArray *ba);
BALTAM_API const char * bxGetClassName(const bxArray *ba);
BALTAM_API int bxGetInternalClassID(const bxArray *ba);
BALTAM_API const bxArray * bxGetPropertyRO(const bxArray *ba, baIndex index, const char *propname);
BALTAM_API bxArray * bxGetPropertyRW(const bxArray *ba, baIndex index, const char *propname);

BALTAM_API void bxSetProperty(bxArray *ba, baIndex index, const char *propname, const bxArray * value);
BALTAM_API baSize bxGetNumberOfPublicProperties(const bxArray *ba, bxPubPropMode mode);
BALTAM_API baSize bxGetPublicPropertyNames(const bxArray *ba, bxPubPropMode mode, const char ** names, baSize n);
BALTAM_API bxAccessLevel bxGetPropertyAccess(const bxArray *ba, const char *propname, bxPropertyAccessType type);
BALTAM_API baSize bxGetNumberOfProperties(const bxArray *ba, bxPubPropMode mode);
BALTAM_API baSize bxGetPropertyNames(const bxArray *ba, bxPubPropMode mode, const char ** names, baSize n);

// 时间表
BALTAM_API bxArray* bxCreateTimeTable(baSize m, baSize n);
BALTAM_API bxArray* bxCreateNullTimeTable(baSize m, baSize n);
BALTAM_API bxArray* bxCreateTimeTableFromColumns(baSize n, const bxArray **columns);
BALTAM_API bool bxIsTimeTableRowTimeDatetime(const bxArray *ba);
BALTAM_API bool bxIsTimeTableRowTimeDuration(const bxArray *ba);
BALTAM_API void bxResizeTimeTableRows(bxArray *ba, baSize nrows);
BALTAM_API const bxArray* bxGetTimeTableColumnRO(const bxArray *ba, baIndex ind);
BALTAM_API bxArray* bxGetTimeTableColumnRW(const bxArray *ba, baIndex ind);
BALTAM_API const bxArray* bxGetTimeTableColumnByNameRO(const bxArray *ba, const char *name);
BALTAM_API bxArray* bxGetTimeTableColumnByNameRW(const bxArray *ba, const char *name);
BALTAM_API void bxSetTimeTableColumn(bxArray *ba, baIndex ind, const bxArray *column);
BALTAM_API void bxSetTimeTableColumnByName(bxArray *ba, const char *name, const bxArray *column);
BALTAM_API void bxInsertTimeTableColumn(bxArray *ba, baIndex ind, bxInsertMode mode, const char *name, const bxArray *column);
BALTAM_API void bxInsertTimeTableColumnByName(bxArray *ba, const char *ref, bxInsertMode mode, const char *name, const bxArray *column);
BALTAM_API void bxAppendTimeTableColumn(bxArray *ba, const char *name, const bxArray *column);
BALTAM_API void bxRemoveTimeTableColumn(bxArray *ba, baIndex ind);
BALTAM_API void bxRemoveTimeTableColumnByName(bxArray *ba, const char *name);
BALTAM_API void bxRenameTimeTableColumn(bxArray *ba, baIndex ind, const char *name);
BALTAM_API void bxRenameTimeTableColumnByName(bxArray *ba, const char *old_name, const char *name);
BALTAM_API const char *bxGetTimeTableColumnName(const bxArray *ba, baIndex ind);
BALTAM_API baSize bxGetTimeTableRowTimeString(const bxArray *ba, baIndex ind, char *name, baSize len);
BALTAM_API const char *bxGetTimeTableDimensionName(const bxArray *ba, bxTableDimension dim);
BALTAM_API void bxSetTimeTableColumnNames(bxArray *ba, const char **names);
BALTAM_API void bxSetTimeTableRowTimesByDatetime(bxArray *ba, const double *rowtimes);
BALTAM_API void bxSetTimeTableRowTimesByDuration(bxArray *ba, const double *rowtimes);
BALTAM_API void bxRenameTimeTableRow(bxArray *ba, baIndex ind, double rowtime);
BALTAM_API void bxSetTimeTableDimensionName(bxArray *ba, bxTableDimension dim, const char *name);
BALTAM_API bool bxIsTimeTableColumn(const bxArray *ba, const char *name);
BALTAM_API int bxSetTimeTableFormat(bxArray *ba, const char *fmt);
BALTAM_API int bxSetTimeTableTimeZone(bxArray *ba, const char *tz);
BALTAM_API baSize bxGetTimeTableFormat(const bxArray *ba, char *result, baSize len);
BALTAM_API baSize bxGetTimeTableTimeZone(const bxArray *ba, char *result, baSize len);

// 输出与调试
BALTAM_API int bxPrintf(const char *, ...);
BALTAM_API void bxErrMsgTxt(const char *);
BALTAM_API void bxArrayToStdout(const bxArray *ba, int line_width);
BALTAM_API baSize bxArrayToCStr(const bxArray *ba, int line_width, int phase, char * buffer, baSize len);

// 辅助函数
BALTAM_API baIndex bxCalcSingleSubscript(const bxArray*, int, baIndex*);
BALTAM_API const char * bxGetBaltamaticaRoot(void);
BALTAM_API const char * bxClassIDCStr(bxClassID id);

// bex 外部数据
BALTAM_API int bxRegisterCStruct(const char *, cstruct_copy_t, cstruct_delete_t);
BALTAM_API void * bxGetCStruct(int, const bxArray*);
BALTAM_API bxArray *bxCreateCStruct(int, void *);
BALTAM_API bool bxIsExtern(const bxArray * ba);
BALTAM_API bool bxIsExternID(const bxArray * ba, int id);
BALTAM_API int bxRegisterUnaryOperator(const char *, const char *, int, bexfun_t);
BALTAM_API int bxRegisterBinaryOperator(const char *, const char *, int, int, bexfun_t);
BALTAM_API int bxRegisterTernaryOperator(const char *, const char *, int, int, int, bexfun_t);
BALTAM_API int bxRegisterUnaryOperatorID(const char *, bxOperatorID, int, bexfun_t);
BALTAM_API int bxRegisterBinaryOperatorID(const char *, bxOperatorID, int, int, bexfun_t);
BALTAM_API int bxRegisterTernaryOperatorID(const char *, bxOperatorID, int, int, int, bexfun_t);
BALTAM_API int bxRegisterOperator(const char *, const char *, int, bexfun_t);
BALTAM_API int bxRegisterOperatorID(const char *, bxOperatorID, int, bexfun_t);

// 函数重载
BALTAM_API int bxOverloadFunction(const char *, const char *, bexfun_t);
BALTAM_API void bxReturnOverloadFailure(void);
BALTAM_API void bxReturnOverloadFailureMsg(const char *);

// Baltamatica 插件函数原型
BALTAM_API int bxPluginInitLib(void*);
BALTAM_API int bxPluginInit(int, const bxArray *[]);
BALTAM_API int bxPluginFini(void);
BALTAM_API bexfun_info_t *bxPluginFunctions(void);

// BEX 文件原型
BALTAM_API void bexFunction(int, bxArray *[], int, const bxArray*[]);

// 向内核和前端的轮询函数
BALTAM_API void * bxK2FQuery(const char *, void *);
BALTAM_API void * bxF2KQuery(const char *, void *);

// 执行函数和语句
BALTAM_API int bxEvalString(const char *);
BALTAM_API int bxEvalIn(const char *ws, const char *expr, bxArray **plhs);
BALTAM_API int bxCallBaltamatica(int, bxArray *[], int, const bxArray *[], const char *);
BALTAM_API int bxCallBaltamaticaF(int, bxArray *[], int, const bxArray *[], const bxArray *);

// 工作区操作
BALTAM_API int bxAddVariable(const char *name, bxArray *value, bxVarOpMode mode);
BALTAM_API int bxRenameVariable(const char *old_name, const char *new_name, bxVarOpMode mode);
BALTAM_API void bxRemoveVariable(const char *name);
BALTAM_API void bxGetVariableNames(const char ***result, int * num);
BALTAM_API void bxFreeVariableNames(const char **result);

// bxGetXxxxs 等函数是否关联到 RW
// 从而在调用的时候会自动触发写入时复制
#ifdef BALTAM_BEX_LEGACY_GETTER_RW
#define bxGetPr bxGetPrRW
#define bxGetInt8s bxGetInt8sRW
#define bxGetInt16s bxGetInt16sRW
#define bxGetInt32s bxGetInt32sRW
#define bxGetInt64s bxGetInt64sRW
#define bxGetUInt8s bxGetUInt8sRW
#define bxGetUInt16s bxGetUInt16sRW
#define bxGetUInt32s bxGetUInt32sRW
#define bxGetUIn64s bxGetUInt64sRW
#define bxGetDoubles bxGetDoublesRW
#define bxGetSingles bxGetSinglesRW
#define bxGetComplexDoubles bxGetComplexDoublesRW
#define bxGetComplexSingles bxGetComplexSinglesRW
#define bxGetChars bxGetCharsRW
#define bxGetLogicals bxGetLogicalsRW
#define bxGetField bxGetFieldRW
#define bxGetFieldByNumber bxGetFieldByNumberRW
#define bxGetCell bxGetCellRW
#define bxGetSparseDoubles bxGetSparseDoublesRW
#define bxGetSparseSingles bxGetSparseSinglesRW
#define bxGetSparseComplexDoubles bxGetSparseComplexDoublesRW
#define bxGetSparseComplexSingles bxGetSparseComplexSinglesRW
#define bxGetSparseLogicals bxGetSparseLogicalsRW
#define bxGetIr bxGetIrRW
#define bxGetJc bxGetJcRW
#endif

// ---

#ifdef __cplusplus
}
#endif // of __cplusplus

#endif

