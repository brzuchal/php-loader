/*
/+----------------------------------------------------------------------+
 | PHP CLI ClassLoader                                                  |
 +----------------------------------------------------------------------+
 | Copyright (c) 2016 Michał Brzuchalski                                |
 +----------------------------------------------------------------------+
 | This source file is subject to version 3.01 of the PHP license,      |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.php.net/license/3_01.txt.                                 |
 | If you did not receive a copy of the PHP license and are unable to   |
 | obtain it through the world-wide-web, please send a note to          |
 | license@php.net so we can mail you a copy immediately.               |
 +----------------------------------------------------------------------+
 | Author: Michał Brzuchalski <michal.brzuchalski@gmail.com>            |
 +----------------------------------------------------------------------+
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "loader.h"
#include "Zend/zend_objects_API.h"
#include "Zend/zend_API.h"

typedef struct _loader_obj loader_obj;
struct _loader_obj {
	HashTable   *prefixes;
    HashTable   *fallback_prefixes;
	zend_object  std;
};
static zend_object_handlers loader_object_handlers;
static inline loader_obj *loader_obj_from_obj(zend_object *obj) {
	return (loader_obj*)((char*)(obj) - XtOffsetOf(loader_obj, std));
}
#define Z_CLOBJECT_P(zv)  loader_obj_from_obj(Z_OBJ_P((zv)))

static zend_object *psr4_loader_new(zend_class_entry *class_type) /* {{{ */
{
    loader_obj *intern = ecalloc(1, sizeof(loader_obj) + zend_object_properties_size(class_type));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);
    intern->std.handlers = &loader_object_handlers;

    return &intern->std;
} /* }}} */

/* {{{ proto void Psr4Loader::__construct()
   Argument constructor */
ZEND_METHOD(Psr4Loader, __construct)
{
}
/* }}} */

/* {{{ proto void Psr4Loader::add()
   Argument constructor */
ZEND_METHOD(Psr4Loader, add)
{
    zval *object = getThis();
    loader_obj *intern;
    zend_string *prefix;
    zend_string *path;
    zend_bool prepend = 0;
    zval tmp;

    ZEND_PARSE_PARAMETERS_START(2, 3)
        Z_PARAM_STR(prefix)
        Z_PARAM_STR(path)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(prepend)
    ZEND_PARSE_PARAMETERS_END();

    intern = Z_CLOBJECT_P(object);
    if (!intern->prefixes) {
        ALLOC_HASHTABLE(intern->prefixes);
        zend_hash_init(intern->prefixes, 2, NULL, NULL, 0);
    }
    // if ()
    ZVAL_STR(&tmp, path);
    zend_hash_str_update(intern->prefixes, ZSTR_VAL(prefix), ZSTR_LEN(prefix), &tmp);
}
/* }}} */

/* {{{ proto void Psr4Loader::getPrefixes()
   Argument constructor */
ZEND_METHOD(Psr4Loader, getPrefixes)
{
    zval *object = getThis();
    loader_obj *intern;
    zend_string *prefix;
    zval *path;

    array_init(return_value);
    intern = Z_CLOBJECT_P(object);
    if (intern->prefixes) {
        ZEND_HASH_FOREACH_STR_KEY_PTR(intern->prefixes, prefix, path) {
            add_assoc_zval_ex(return_value, ZSTR_VAL(prefix), ZSTR_LEN(prefix), path);
        } ZEND_HASH_FOREACH_END();
    }
}
/* }}} */

/* {{{ proto void Psr4Loader::loadCLass(string $className)
   Loads class using defined prefixes */
ZEND_METHOD(Psr4Loader, loadClass)
{
    zval *object = getThis();
    loader_obj *intern;
    zend_string *className;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(className)
    ZEND_PARSE_PARAMETERS_END();

    // array_init(return_value);
    // intern = Z_CLOBJECT_P(object);
    // if (intern->prefixes) {
    //     ZEND_HASH_FOREACH_STR_KEY_PTR(intern->prefixes, prefix, path) {
    //         add_assoc_zval_ex(return_value, ZSTR_VAL(prefix), ZSTR_LEN(prefix), path);
    //     } ZEND_HASH_FOREACH_END();
    // }
}
/* }}} */

PHP_MINIT_FUNCTION(loader)
{
    zend_class_entry loader_ce;
    zend_class_entry psr4_loader_ce;

    INIT_CLASS_ENTRY(loader_ce, "Loader", NULL);
    loader_ce_ptr = zend_register_internal_class(&loader_ce);
    // zend_declare_class_constant_long(loader_ce_ptr, ZEND_STRL("PSR4"), 1);
    // zend_declare_class_constant_long(loader_ce_ptr, ZEND_STRL("FILE"), 2);

    zend_function_entry loader_methods[] = {
        PHP_ABSTRACT_ME(ClassLoader, loadClass,    arginfo_loader_loadClass)
        PHP_FE_END
    };
    INIT_CLASS_ENTRY(loader_ce, "ClassLoader", loader_methods);
    loader_ce_ptr = zend_register_internal_interface(&loader_ce);

    zend_function_entry psr4_loader_methods[] = {
        PHP_ME(Psr4Loader, __construct, arginfo_psr4_loader___construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
        PHP_ME(Psr4Loader, add,         arginfo_psr4_loader_add,         ZEND_ACC_PUBLIC)
        PHP_ME(Psr4Loader, getPrefixes, arginfo_psr4_loader_getPrefixes, ZEND_ACC_PUBLIC)
        PHP_ME(Psr4Loader, loadClass,   arginfo_loader_loadClass,  ZEND_ACC_PUBLIC)
        PHP_FE_END
    };
    INIT_CLASS_ENTRY(psr4_loader_ce, "Psr4Loader", psr4_loader_methods);
    psr4_loader_ce.create_object = psr4_loader_new;
    psr4_loader_ce_ptr = zend_register_internal_class(&psr4_loader_ce);
    zend_class_implements(psr4_loader_ce_ptr, 1, loader_ce_ptr);
    memcpy(&loader_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); 
    loader_object_handlers.offset = XtOffsetOf(loader_obj, std);

    return SUCCESS;
}

PHP_RINIT_FUNCTION(loader)
{
#if defined(COMPILE_DL_LOADER) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(loader)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(loader)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "loader support", "enabled");
    php_info_print_table_row(2, "loader version", PHP_LOADER_VERSION);
    php_info_print_table_end();
}

zend_module_entry loader_module_entry = {
    STANDARD_MODULE_HEADER_EX,
    NULL,
    NULL,
    "loader",
    NULL,
    PHP_MINIT(loader),
    NULL,
    PHP_RINIT(loader),
    PHP_RSHUTDOWN(loader),
    PHP_MINFO(loader),
    PHP_LOADER_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_LOADER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(loader)
#endif
