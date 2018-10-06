#ifndef PHP_LOADER_H
#define PHP_LOADER_H

#include "php.h"
#include "main/php.h"
#include "ext/standard/info.h"

extern zend_module_entry loader_module_entry;

zend_class_entry *loader_ce_ptr;
zend_class_entry *psr4_loader_ce_ptr;
zend_class_entry *psr0_loader_ce_ptr;

// ClassLoader::loadClass
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_loader_loadClass, 0, 2, IS_VOID, NULL)
    ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, scope, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

// Psr4Loader::__construct
ZEND_BEGIN_ARG_INFO_EX(arginfo_psr4_loader___construct, 0, 0, 0)
    // ZEND_ARG_TYPE_INFO(0, config, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

// Psr4Loader::add
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_psr4_loader_add, 0, 2, IS_VOID, NULL)
    ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, prepend, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

// Psr4Loader::set
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_psr4_loader_set, 0, 2, IS_VOID, NULL)
    ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, prepend, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

// Psr4Loader::add
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_psr4_loader_getPrefixes, 0, 0, IS_ARRAY, NULL)
ZEND_END_ARG_INFO()

// Psr0Loader::__construct
ZEND_BEGIN_ARG_INFO_EX(arginfo_psr0_loader___construct, 0, 0, 0)
    // ZEND_ARG_TYPE_INFO(0, config, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

// Psr0Loader::add
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_psr0_loader_add, 0, 2, IS_VOID, NULL)
    ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, prepend, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

// Psr0Loader::set
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_psr0_loader_set, 0, 2, IS_VOID, NULL)
    ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, prepend, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define phpext_loader_ptr &loader_module_entry

/* Replace with version number for your extension */
#define PHP_LOADER_VERSION "0.0.1"

#ifdef PHP_WIN32
#  define PHP_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#  define PHP_API __attribute__ ((visibility("default")))
#else
#  define PHP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

// ZEND_BEGIN_MODULE_GLOBALS(loader)
// zend_fcall_info        user_compare_fci;
// zend_fcall_info_cache  user_compare_fci_cache;
// ZEND_END_MODULE_GLOBALS(loader)

#ifdef ZTS
#define DSG(v) TSRMG(loader_globals_id, zend_loader_globals *, v)
#else
#define DSG(v) (loader_globals.v)
#endif

// ZEND_EXTERN_MODULE_GLOBALS(loader);

#if defined(ZTS) && defined(COMPILE_DL_loader)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif