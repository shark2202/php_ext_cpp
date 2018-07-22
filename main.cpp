#include <phpcpp.h>

// #include "tt.cpp"
// #include <string>
// extern "C"
// {
// #include "php.h"
// #include "php_ini.h"
// #include "php_globals.h"
// #include "php_main.h"

// #include "zend_API.h"
// #include "php_streams.h"
// #include "php_network.h"

// #if PHP_MAJOR_VERSION < 7
// #error "only supports PHP7 or later."
// #endif

// #include "zend_interfaces.h"
// #include "zend_exceptions.h"
// #include "zend_variables.h"
// #include "zend_inheritance.h"
// }

// using namespace std;

//     bool include(string file)
//     {
//         zend_file_handle file_handle;
//         int ret = php_stream_open_for_zend_ex(file.c_str(), &file_handle, USE_PATH | STREAM_OPEN_FOR_INCLUDE);
//         if (ret != SUCCESS)
//         {
//             return false;
//         }

//         zend_string *opened_path;
//         if (!file_handle.opened_path)
//         {
//             file_handle.opened_path = zend_string_init(file.c_str(), file.length(), 0);
//         }
//         opened_path = zend_string_copy(file_handle.opened_path);
//         zval dummy;
//         zval result;
//         zend_op_array *new_op_array;
//         ZVAL_NULL(&dummy);
//         if (zend_hash_add(&EG(included_files), opened_path, &dummy))
//         {
//             new_op_array = zend_compile_file(&file_handle, ZEND_REQUIRE);
//             zend_destroy_file_handle (&file_handle);
//         }
//         else
//         {
//             new_op_array = NULL;
//             zend_file_handle_dtor (&file_handle);
//         }
//         zend_string_release(opened_path);
//         if (!new_op_array)
//         {
//             return false;
//         }

//         ZVAL_UNDEF(&result);
//         zend_execute(new_op_array, &result);

//         destroy_op_array(new_op_array);
//         efree(new_op_array);
//         if (!EG(exception))
//         {
//             zval_ptr_dtor(&result);
//         }
//         return true;
//     }


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
    Php::Value getThis(){
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
};

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
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension myext("mytest", "1.0");
        Php::Namespace mynamespace("trComplex");

        Php::Class<Complex> extClass("Complex");
        //extClass.method("__construct",&Complex::__construct);
        extClass.method<&Complex::__construct> ("__construct");
        extClass.method<&Complex::getReal> ("getReal");
        extClass.method<&Complex::getImage> ("getImage");

        extClass.method<&Complex::getThis> ("this1");
        extClass.method<&Complex::getNewObject> ("this2");
        
        extClass.method<&Complex::newException> ("e1");
        extClass.method<&Complex::catchException> ("e2");

        //callPhpFn
        extClass.method<&Complex::callPhpFn> ("callPhpFn");

        //myext.add(std::move(extClass));
        mynamespace.add(std::move(extClass));

        myext.add(std::move(mynamespace));

        //myext.add<myinclude>("include22");

        // return the extension
        return myext;
    }
}
