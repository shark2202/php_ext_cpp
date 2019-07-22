#include <phpcpp.h>


/**
 * 一个简单的c++方法的
 * @param  params 参数
 * @return        返回值的
 */
Php::Value example_function(Php::Parameters &params)
{
    int a = params[0];
    int b = params[1];

    return a + b;
}

/**
 * 另外一个重载的方法example_function
 */
/*void example_function()
{
     Php::out << "example output" << std::endl;
}*/

Php::Value run_test(Php::Parameters &params)
{
    // create the anonymous function
    // 创建一个匿名的函数
    Php::Function multiply_by_two([](Php::Parameters &params) -> Php::Value {

        // make sure the function was really called with at least one parameter
        if (params.empty()) return nullptr;

        // one parameter is passed to the function
        Php::Value param = params[0];

        // multiple the parameter by two
        return param * 2;
    });

    // the function now is callable
    Php::Value four = multiply_by_two(2);

    // a Php::Function object is a derived Php::Value, and its value can 
    // also be stored in a normal Php::Value object, it will then still 
    // be a callback function then
    Php::Value value = multiply_by_two;

    // the value object now also holds the function
    Php::Value six = value(3);

    // create an array
    Php::Value array;
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    array[3] = 4;

    // call the user-space function
    Php::Value result = Php::call("my_array_map", params[0], multiply_by_two);

    // @todo do something with the result variable (which now holds
    // an array with values 2, 4, 6 and 8).
    // 
    
    return result;
}

/**
 *  Switch to C context, because the Zend engine expects get get_module()
 *  to have a C style function signature
 */
extern "C" {
    /**
     *  Startup function that is automatically called by the Zend engine
     *  when PHP starts, and that should return the extension details
     *  @return void*
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // the extension object
        static Php::Extension extension("extdemo", "1.0");


/**
 * 设置参数的方法的
 *  Constructor
 *  @param  name        Name of the parameter
 *  @param  classname   Name of the class
 *  @param  nullable    Can it be null?
 *  @param  required    Is this parameter required?
 */
//ByVal(const char *name, const char *classname, bool nullable = false, bool required = true);

        //void  (*callback)(Parameters &parameters);
        //callback = example_function;
        //把cpp的方法和php的方法关联起来的
        extension.add<example_function>("php_example_function",{
            /*设置方法有两个参数的*/
            Php::ByVal("a", Php::Type::Numeric),
            Php::ByVal("b", Php::Type::Numeric),
        });


        extension.add<run_test>("run_test");


        // return the extension details
        return extension;
    }
}