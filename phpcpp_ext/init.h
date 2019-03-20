#pragma once

#define EXTERSION_NAME "mytest"
#define EXTERSION_VERSION "0.01"

/* 实际上是自定义了一个结构体 */
ZEND_BEGIN_MODULE_GLOBALS(mytest)
    // std::map<std::string,std::strng> str2str;
    zend_bool   disable;
ZEND_END_MODULE_GLOBALS(mytest)

/**
 *  下面是自定义的
 */
#ifdef ZTS
#define MYTEST_G(v) TSRMG(mytest_globals_id, zend_mytest_globals *, v)
#else
#define MYTEST_G(v) (mytest_globals.v)
#endif

//ZEND_MODULE_GLOBALS_ACCESSOR 改成用zend提供的接口来处理
//#define MYTEST_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mytest,v)


/**
 *  We're almost there, we now need to declare an instance of the
 *  structure defined above (if building for a single thread) or some
 *  sort of impossible to understand magic pointer-to-a-pointer (for
 *  multi-threading builds). We make this a static variable because
 *  this already is bad enough.
 */
ZEND_DECLARE_MODULE_GLOBALS(mytest)