#include <phpcpp.h>
#include <string>
#include <iostream>
extern "C"
{
#include "php.h"
#include "php_ini.h"
#include "php_globals.h"
#include "php_main.h"

#include "zend_API.h"
#include "php_streams.h"
#include "php_network.h"

#if PHP_MAJOR_VERSION < 7
#error "only supports PHP7 or later."
#endif

#include "zend_interfaces.h"
#include "zend_exceptions.h"
#include "zend_variables.h"
#include "zend_inheritance.h"
}

// typedef void (*php_func)(INTERNAL_FUNCTION_PARAMETERS);

// #include <string>
// using namespace std;

namespace Mytest {
/**这个是一个cpp的函数
 * @param
 * @return
 */
// bool myinclude(string file)
// {
//     zend_file_handle file_handle;
//     int ret = php_stream_open_for_zend_ex(file.c_str(), &file_handle, USE_PATH | STREAM_OPEN_FOR_INCLUDE);
//     if (ret != SUCCESS)
//     {
//         return false;
//     }

//     zend_string *opened_path;
//     if (!file_handle.opened_path)
//     {
//         file_handle.opened_path = zend_string_init(file.c_str(), file.length(), 0);
//     }
//     opened_path = zend_string_copy(file_handle.opened_path);
//     zval dummy;
//     zval result;
//     zend_op_array *new_op_array;
//     ZVAL_NULL(&dummy);
//     if (zend_hash_add(&EG(included_files), opened_path, &dummy))
//     {
//         new_op_array = zend_compile_file(&file_handle, ZEND_REQUIRE);
//         zend_destroy_file_handle (&file_handle);
//     }
//     else
//     {
//         new_op_array = NULL;
//         zend_file_handle_dtor (&file_handle);
//     }
//     zend_string_release(opened_path);
//     if (!new_op_array)
//     {
//         return false;
//     }

//     ZVAL_UNDEF(&result);
//     zend_execute(new_op_array, &result);

//     destroy_op_array(new_op_array);
//     efree(new_op_array);
//     if (!EG(exception))
//     {
//         zval_ptr_dtor(&result);
//     }
//     return true;
// }


/**
 * 第一个参数是方法的名称，第二个参数是方法名称的长度
 * 第三个参数是新的方法，第四个参数是
 */
// static void php_override_func(const char *name, size_t len, php_func handler, php_func *stash)
// {
//     zend_function *func;
//     //查找方法的,方法全部存储在EG(function_table)中的
//     if ((func = zend_hash_str_find_ptr(EG(function_table), name, strlen(name))) != NULL) {
//         if (stash) {
//             *stash = func->internal_function.handler;
//         }
//         func->internal_function.handler = handler;
//     }
// }

/**拷贝一个方法到另一个方法
 * @return
 */
// Php::Value copyFn2Fn(Php::Parameters &params)
// {
//     string f1 = params[0];
//     string f2 = params[1];

//     // Php::call(f1);
//     return Php::Value value([]() {

//     });
    // Php::Function multiply_by_two([](Php::Parameters &params) -> Php::Value {

    //     // make sure the function was really called with at least one parameter
    //     if (params.empty()) return nullptr;

    //     // one parameter is passed to the function
    //     Php::Value param = params[0];

    //     // multiple the parameter by two
    //     return param * 2;
    // });
        
    //php_override_func("ini_get", sizeof("ini_get"), PHP_FN(fake_ini_get), NULL TSRMLS_CC);
    //return true;
// }

/**要包装成可以在扩展中使用的函数
 * @param
 * @return
 */
// Php::Value include22(Php::Parameters &params)
// {
//     return myinclude(params[0]);
// }

// /**定时刷新conf??
//  * @return
//  */
// Php::Value getConf(){
//     return true;
// }
}
// #include "../tt.h"

class Complex : public Php::Base
{
private:
    double r=0,i=0;

public:    
    Complex(){
    }

    virtual ~Complex(){
    }    

    Php::Value getReal(){
        return r;
    }

    Php::Value getImage(){
        return i;
    }

    /**返回this
    */
    Php::Value getThis1(){
        return this;
    }

    /**返回指针的，是特殊类型的
    */
    Php::Value getNewObject(){
        Complex *t = new Complex();

        t->r = r;
        t->i = i;

        return Php::Object("\\trComplex\\Complex",t);
    }

    /**抛出异常
    */
    Php::Value newException(){
        throw Php::Exception("newException");

        return this;
    }

    /**
    *在cpp中捕获异常
    */
    Php::Value catchException(Php::Parameters &params){
        try{    
            Php::Value fn = params[0];
            return fn();
        }catch(Php::Exception &exception){
            return "new Exception in PHP";
        }
    }

    /**
    *使用php方法
    */
    Php::Value callPhpFn(Php::Parameters &params){
        return Php::call("var_dump",params[0]);
    }


    void __construct(Php::Parameters &params){
        if(params.size() == 2){
            r = params[0];
            i = params[1];
        }else{
            r = 0;
            i = 0;
        }
    }
    
    Php::Value class_exist_my(Php::Parameters &params){
        //zval var_value; //变量的值
        //zend_string *var_name = NULL; //变量名称
    
        zend_string *str = NULL;
        const char* content = params[0].stringValue().c_str();
        size_t size = params[0].stringValue().size();
        
        Php::out << content << std::endl;
        //var_name = zend_string_init("str", 3, 0); //设置变量名称 
        str = zend_string_init(content, size , 0);
        //str = zval_get_string(params[0]);
        //ZVAL_STR(&var_value, str); //设置变量的值
        //zend_set_local_var(var_name, &var_value, 0); //设置本地变量
        
        //struct _zend_string *_path = nullptr;
        //std::string *name = new std::string("StaticCnew");
        //_path = zend_string_init("stdClass", sizeof("stdClass") - 1, 0);
        //获得一个_zend_string
        //_path = zend_resolve_path(name->c_str(), name->size());
                
//查询方法是可以的，但是查询类不行了???
//        auto *val =  zend_hash_find(EG(function_table), str);

//在EG(class_table)中类名全部是小写的，如果包含有命令空间的就查询不出来了？？
        auto *val =  zend_hash_find(EG(class_table), str);
        
        zend_string_release(str);
        //ZVAL_NULL(&var_value);
        
        //return val;
        // check whether something was found
        if (val == nullptr) return false;
//        Php::echo(name.c_str());
        //Php::echo(name.size());
        //std::cout << name.c_str() << " " << name.size() << std::end;
        return true;
    }
};

class StaticC : public Php::Base
{
public:
    int map;
    static int objectCount;
    static std::vector<Php::Value> extArr;//这个是一个数组的
    static std::map<std::string,Php::Value> extMap;//这个是一个map的

    StaticC(){
        map = 0;
    }

    virtual ~StaticC(){
    } 

    Php::Value getSS(){
        return extMap;
    }

    Php::Value setSS(Php::Parameters &params){
        std::string key = params[0];
        extMap[key] = params[1];
        //extMap.push_back(key);
        // map ++;
        //返回一个this标识的是当前的对象的
        return this;
    }

    Php::Value classtest(Php::Parameters &params){
//        zend_string *strg;
//
//        char *key = "StaticCnew";
//        // uint  keylen = strlen(key);
//        strg = strpprintf(0, key);
//        
//        int *data = 0;
//        zend_hash_find( EG(class_table), strg, (void *)&data );
//        if( data ){
//            php_printf("value pointer:%u, value:%d\n", data, (int)*data);
//        } else{
//            php_printf("value pointer:%u\n", data);
//        }
        return false;
    }
};
    Php::Value add_new_func(Php::Parameters &params){
        Php::Function multiply_by_two([](Php::Parameters &params) -> Php::Value {
            auto fn = params[0];
            Php::out << content << std::endl;
            params.erase(params.begin()+1);
            return Php::call(fn, params);
        });

        zend_string *str = NULL;
        const char* content = params[0].stringValue().c_str();
        size_t size = params[0].stringValue().size();
        
        Php::out << content << std::endl;

        str = zend_string_init(content, size , 0);


        //在EG(class_table)中类名全部是小写的，如果包含有命令空间的就查询不出来了？？
        auto *val =  zend_hash_find(EG(function_table), str);
        
        zend_string_release(str);
        if (val == nullptr) return false;

        //return params[1](params[2]);
        return multiply_by_two;
    }

//初始静态变量的
int StaticC::objectCount = 0;
std::vector<Php::Value> StaticC::extArr = {};//空数组的
std::map<std::string,Php::Value> StaticC::extMap = {};//空map
/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        //Php::dl();
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension myext("mytest", "1.0");

        myext.onStartup([]() {
            StaticC::objectCount = 100;
            StaticC::extArr.push_back(1);
            StaticC::extMap["1"] = 100;
        });

        Php::Namespace mynamespace("trComplex");

        Php::Class<Complex> extClass("Complex");
        //extClass.method("__construct",&Complex::__construct);
        extClass.method<&Complex::__construct> ("__construct");
        extClass.method<&Complex::getReal> ("getReal");
        extClass.method<&Complex::getImage> ("getImage");

        extClass.method<&Complex::getThis1> ("this1");
        extClass.method<&Complex::getNewObject> ("this2");
        
        extClass.method<&Complex::newException> ("e1");
        extClass.method<&Complex::catchException> ("e2");

        //callPhpFn
        extClass.method<&Complex::callPhpFn> ("callPhpFn");
        
        //class_exist_my
        extClass.method<&Complex::class_exist_my> ("class_exist_my");

        //myext.add(std::move(extClass));
        mynamespace.add(std::move(extClass));

        // mynamespace.add<Mytest::getConf>("getConf");

        myext.add(std::move(mynamespace));

        // myext.add<Mytest::include22>("include22");


        Php::Class<StaticC> extClass2("StaticCnew");
        extClass2.method<&StaticC::getSS> ("getSS");
        extClass2.method<&StaticC::setSS> ("setSS");

        myext.add(std::move(extClass2));

        myext.add<add_new_func>("add_new_func");
        // return the extension
        return myext;
    }
}
