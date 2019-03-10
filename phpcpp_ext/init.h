#pragma once

ZEND_BEGIN_MODULE_GLOBALS(mytest)
    // std::map<std::string,std::strng> str2str;
    zend_bool   disable;
ZEND_END_MODULE_GLOBALS(mytest)

/**
 *  And now we're going to define a macro. This also is a uncommon architecture 
 *  from PHP to get access to a variable from the structure above.
 */
#ifdef ZTS
#define MYTEST_G(v) TSRMG(mytest_globals_id, phpcpp_globals *, v)
#else
#define MYTEST_G(v) (mytest_globals.v)
#endif

/**
 *  We're almost there, we now need to declare an instance of the
 *  structure defined above (if building for a single thread) or some
 *  sort of impossible to understand magic pointer-to-a-pointer (for
 *  multi-threading builds). We make this a static variable because
 *  this already is bad enough.
 */
ZEND_DECLARE_MODULE_GLOBALS(mytest)