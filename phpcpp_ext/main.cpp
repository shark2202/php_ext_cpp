#include <phpcpp.h>
#include <iostream>

//使用宏替换一下
#define PHPCPP_EXTENSION()  extern "C" { PHPCPP_EXPORT void *get_module(); } PHPCPP_EXPORT void *get_module()

//没有返回值的
void myFunction()
{
    Php::out << "example output" << std::endl;
}

void evalPHP(Php::Parameters &params)
{
    Php::eval(params[0].stringValue());
}

Php::Value fnInCpp(Php::Parameters &p)
{
    return Php::Object("ClassInPhp");
    Php::Value param = p[0];
    return param * 2;
}

Php::Value multiply_by_two(Php::Parameters &params) {

        // make sure the function was really called with at least one parameter
        if (params.empty()) return nullptr;

        // one parameter is passed to the function
        Php::Value param = params[0];

        // multiple the parameter by two
        return param * 2;
}

Php::Value returnfunction(Php::Parameters &p)
{
    // //这个一个cpp的匿名函数
    // Php::Function multiply_by_two([](Php::Parameters &params) -> Php::Value {

    //     // make sure the function was really called with at least one parameter
    //     if (params.empty()) return nullptr;

    //     //Php::call("var_dump",params);

    //     // one parameter is passed to the function
    //     Php::Value param = params[0];

    //     // multiple the parameter by two
    //     return 10;
    // });

    // create the anonymous function
    Php::Function multiply_by_two([](Php::Parameters &params) -> Php::Value {

        // make sure the function was really called with at least one parameter
        if (params.empty()) return nullptr;

        // one parameter is passed to the function
        Php::Value param = params[0];

        // multiple the parameter by two
        return param * 2;
    });

    // create an array
    Php::Value array;
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    array[3] = 4;

    // call the user-space function
    Php::Value f ("my_array_map");
    Php::Value result = f(array, multiply_by_two);

    Php::call("var_dump",result);

    return 1;

    Php::Function f1(fnInCpp);
    return Php::Object("Closure",f1);
    // return std::move(f1);

    Php::Value r;
    Php::call("var_dump",Php::Array(p[0]));

    Php::call("var_dump",multiply_by_two);

    r = Php::call("array_map",multiply_by_two,Php::Array(p[0]));

    Php::call("var_dump",r);

    return 0;
}

class CppClass: public Php::Base
{
private:
    Php::Value _val;

public:
    Php::Value testCall(Php::Parameters &p)
    {
        return 100;
    }

    Php::Value setVal(){
        //获取php空间的this对象
        Php::Value self(this);

        self["_val"] = 100;

        Php::Value r1 = self.call("testcall123");//调用php的方法
        
        Php::call("var_dump",r1);

        Php::call("var_dump",self.get("caller").isCallable());

        Php::Function multiply_by_two([]() -> Php::Value {
            Php::call("var_dump",100);
            return 100;
        });


        Php::call(multiply_by_two);
        //_val = 100;
        return self;
    }
};

PHPCPP_EXTENSION()
{
    //使用静态变量常驻内存的
    static Php::Extension extension("mytest", "1.0");

    //cpp中定义的方法的名称，后面是php中定义的方法的名称
    extension.add<myFunction>("newFunction");

    //直接定义的命名空间的方法
    extension.add<myFunction>("CppTest\\newFunction");


    /**
    * 限定参数的类型
    *   Php::Type::Null
    *   Php::Type::Numeric
    *   Php::Type::Float
    *   Php::Type::Bool
    *   Php::Type::Array
    *   Php::Type::Object
    *   Php::Type::String
    *   Php::Type::Resource
    *   Php::Type::Constant
    *   Php::Type::ConstantArray
    *   Php::Type::Callable
    * 
    *
    */
    extension.add<myFunction>("myFunction",{
        Php::ByVal("a",Php::Type::Numeric),
    });

    extension.add<evalPHP>("evalPhp",{
        Php::ByVal("code",Php::Type::String),
    });

    extension.add<returnfunction>("returnfunction");

    extension.add<fnInCpp>("fnInCpp");

    extension.add<multiply_by_two>("multiply_by_two");


    //关联cpp中的类到php的类
    Php::Class<CppClass> class1("CppTest\\CppClassReg");

    class1.method<&CppClass::testCall>("testcall123");

    class1.method<&CppClass::setVal>("setVal");

    //注册到扩展中的
    extension.add(std::move(class1));

    return extension;
}