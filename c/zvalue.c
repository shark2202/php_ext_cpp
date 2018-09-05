/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
struct _zval_struct { /* 变量 信息 */ 
    zvalue_value value; /* value 值*/ 
    zend_uint refcount__gc; 
    zend_uchar type; /* 类型 */ 
    zend_uchar is_ref__gc; 
}; 

typedef union _zvalue_value { 
    long lval; /* 长整型*/ 
    double dval; /* 浮点型*/ 
    struct { char *val; int len; } str; /* 字符串的信息 */
    HashTable *ht; /* HashTable 数组*/ 
    zend_object_value obj; /* 对象的信息 */
    zend_ast *ast; /* ast ???*/
}zvalue_value;


