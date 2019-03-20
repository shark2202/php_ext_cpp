#include <phpcpp.h>
#include <iostream>

#include "./php.h"

//注册模块全局变量
#include "./init.h"

#include "./backtrace7.c"

/**
 * 2019-03-19
 * by Will
 * 做一个amp的扩展，完成
 * (a)函数的调用的时间的耗时的追踪
 * (b)完成一个请求的耗时的追踪
 * (c)完成打点的开始和结束的耗时的追踪
 * 扩展到curl和mysql，mongodb，redis等等扩展的监控
 * 
 * 
 */

/**
 * 使用的地方要声明这个
 */ 
ZEND_EXTERN_MODULE_GLOBALS(mytest);

extern "C" {
    static int php_mycall_handler(zend_execute_data *execute_data);

    static int php_myreturn_handler(zend_execute_data *execute_data);
}
void myFunction();

/* {{{ */
static void php_mytest_init_globals(zend_mytest_globals *ng) {
	memset(ng, 0, sizeof(zend_mytest_globals));
} /* }}} */

/**
 * 处理handler
 * 这个是在方法调用之前触发的
 */ 
static int php_mycall_handler(zend_execute_data *execute_data){

    if(MYTEST_G(disable)){
        Php::out << "disabled:true" << std::endl;
        return ZEND_USER_OPCODE_DISPATCH;
    }
    //opline的内容
	const zend_op *opline = execute_data->opline;
    zend_execute_data *call = execute_data->call;
	zend_function *fbc = call->func;

    if (fbc->common.scope == NULL) {
        //方法的名称
        zend_string *fname = fbc->common.function_name;


        Php::out << "function name:" << fname->val << std::endl;
    }else{

        zend_string *fname = fbc->common.function_name;
        zend_class_entry *clazzname = fbc->common.scope;

        Php::out << "class name:" << clazzname->name->val << ", method name:" << fname->val << std::endl;
    }
	//taint_free_op free_op1;
	//zval *op1;
    //myFunction();

    //zend_execute_data *call = EX(call);

    //当前的函数已经执行完了跳到下一个函数
	EX(call) = call->prev_execute_data;
	EX(opline) = EX(opline) + 1;

	zend_vm_stack_free_call_frame(call);

    //不让他继续执行了
    return ZEND_USER_OPCODE_LEAVE;

    //继续执行的
	// return ZEND_USER_OPCODE_DISPATCH;
} 

/**
 * 函数返回的时候会触发的
 */ 
static int php_myreturn_handler(zend_execute_data *execute_data){
    //return 1;

    //zend_vm_handler_t zend = NULL;

    //opline的内容
	//const zend_op *opline = execute_data->opline;
    //zend_execute_data *call = execute_data->call;
    zend_execute_data *call;

    char *call_type;
    const char *function_name;

    //const char *function_name;

    call = EG(current_execute_data);

    zend_string *class_name = NULL;
	zend_function *fbc = NULL;
    zend_object *object;

    object = Z_OBJ(call->This);

    if(call->func){
        fbc = call->func;

			function_name = (fbc->common.scope && fbc->common.scope->trait_aliases) ?
				ZSTR_VAL(zend_resolve_method_name((object ? object->ce : fbc->common.scope), fbc)) :
				(fbc->common.function_name ? ZSTR_VAL(fbc->common.function_name) : NULL);


			if (object) {
				if (fbc->common.scope) {
					class_name = fbc->common.scope->name;
				} else if (object->handlers->get_class_name == std_object_handlers.get_class_name) {
					class_name = object->ce->name;
				} else {
					class_name = object->handlers->get_class_name(object);
				}

				call_type = "->";
            //如果不是对象的实例方法，那么可能是类方法或者是命名空间的方法
			} else if (fbc->common.scope) {
				class_name = fbc->common.scope->name;
				call_type = "::";
			} else {
				class_name = NULL;
				call_type = NULL;
			}
    }else{
        fbc = NULL;
        function_name = NULL;

        class_name = NULL;
		call_type = NULL;
    }


    if(class_name){//
        php_printf("class_name:%s\n",ZSTR_VAL(class_name));
    }

    if(call_type){//
        php_printf("call_type:%s\n",call_type);
    }

    if(function_name){//
        php_printf("function_name:%s\n",function_name);
    }
    // if (fbc->common.scope == NULL) {
    //     //方法的名称
    //     zend_string *fname = fbc->common.function_name;
    //     //zend_string *bar2 = zend_string_dup(fname,0);
    //     //bar2 = zend_string_init(fname->val, fname->len, 0);

    //     //Php::out << "php_myreturn_handler function name:" << fname->val << std::endl;
    //     php_printf("php_myreturn_handler function name: %s \n", ZSTR_VAL(fname));

    //     //zend_string_release(bar2);
    // }else{

    //     zend_string *fname = fbc->common.function_name;
    //     zend_class_entry *clazzname = fbc->common.scope;

    //     //Php::out << "php_myreturn_handler class name:" << clazzname->name->val << ", method name:" << fname->val << std::endl;
    //     php_printf("php_myreturn_handler class name: %s, method name: %s \n", ZSTR_VAL(clazzname->name),ZSTR_VAL(fname));
    // }

	//if (zend) {
		//return zend(execute_data);
	//}

    Php::out << "php_myreturn_handler" << std::endl;

	return ZEND_USER_OPCODE_DISPATCH;
}

/**
 * 在扩展中使用多线程要怎么使用? 
 */

Php::Value my_bt(void){
        smart_str trace_str = {0};
        //smart_str_appends(&trace_str, "Hello, you are using PHP version \0 5555");
        append_backtrace(&trace_str TSRMLS_CC);
        smart_str_0(&trace_str);

        if((trace_str.s) && (ZSTR_LEN(trace_str.s) > 0)){
            php_printf("bt: %s \n", ZSTR_VAL(trace_str.s));
        }else{
            php_printf("bt: %s \n","empty");
        }   
        //Php::out << "bt: " << trace_str.s->val << std::endl;

        smart_str_free(&trace_str);

        return true;
}

Php::Value mytest_disbaled(Php::Parameters &params){
    MYTEST_G(disable) = 0;
    bool disabled = params[0];
    if(disabled){
        MYTEST_G(disable) = 1;
    }

    Php::out << "disabled:" << disabled << std::endl;

    return true;
}

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


Php::Value PhpStrace(Php::Parameters &p){
    Php::out << "called" << std::endl;
}

PHPCPP_EXTENSION()
{
    //使用静态变量常驻内存的
    static Php::Extension extension(EXTERSION_NAME, EXTERSION_VERSION);

    //添加一个ini的配置信息,第三个参数是设置配置允许修改的返回
    extension.add(Php::Ini("mytest.disable", 0, Php::Ini::Place::Perdir));

    /**
     * 注册一方法在方法调用之前和之后进行标记的
     */
    extension.add<mytest_disbaled>("mytest_disbaled"); 

    extension.add<my_bt>("mybacktrace");

    //cpp中定义的方法的名称，后面是php中定义的方法的名称
    //extension.add<myFunction>("newFunction");

    //直接定义的命名空间的方法
    //extension.add<myFunction>("CppTest\\newFunction");


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
    // extension.add<myFunction>("myFunction",{
    //     Php::ByVal("a",Php::Type::Numeric),
    // });

    // extension.add<evalPHP>("evalPhp",{
    //     Php::ByVal("code",Php::Type::String),
    // });

    // extension.add<returnfunction>("returnfunction");

    // extension.add<fnInCpp>("fnInCpp");

    // extension.add<multiply_by_two>("multiply_by_two");


    // //关联cpp中的类到php的类
    // Php::Class<CppClass> class1("CppTest\\CppClassReg");

    // class1.method<&CppClass::testCall>("testcall123");

    // class1.method<&CppClass::setVal>("setVal");

    // //注册到扩展中的
    // extension.add(std::move(class1));

    //一个请求的开始
    extension.onRequest([](){
        int disabledIni = Php::ini_get("mytest.disable");
        Php::out << "disabledIni:" << disabledIni << std::endl;
        MYTEST_G(disable) = 0;
        if(disabledIni){
            MYTEST_G(disable) = 1;
        }
    });

    //一个请求的结束
    extension.onIdle( []() {
        my_bt();

        //自己实例化一个smart_str的对象
        smart_str my_str = {0};
        smart_str_appends(&my_str, "Hello, you are using PHP version \0 5555");

        smart_str_appends(&my_str, " PHP functions");
        smart_str_0(&my_str);
        
        //php_printf("retstr=%s\n", ZSTR_VAL(my_str.s));
        smart_str_free(&my_str);


        Php::out << "request end" << std::endl;
    });

    //zend_set_user_opcode_handler(ZEND_DO_FCALL, LIGHT_DO_FCALL);
    //ZEND_DO_UCALL
    extension.onStartup([](){

        ZEND_INIT_MODULE_GLOBALS(mytest, php_mytest_init_globals, NULL);

        int disabledIni = Php::ini_get("mytest.disable");
        MYTEST_G(disable) = 0;
        if(disabledIni){
            MYTEST_G(disable) = 1;
        }

        // zend_set_user_opcode_handler(ZEND_INCLUDE_OR_EVAL, php_taint_fcall_handler);
        //这个是把函数进行hook了
        zend_set_user_opcode_handler(ZEND_DO_UCALL, php_mycall_handler);
        zend_set_user_opcode_handler(ZEND_DO_FCALL, php_mycall_handler);

        zend_set_user_opcode_handler(ZEND_RETURN, php_myreturn_handler); 

        //这个要把方法进行hook
        //zend_set_user_opcode_handler(ZEND_INIT_FCALL, php_mycall_handler);
        //ZEND_RETURN
    
        //zend_set_user_opcode_handler(ZEND_RETURN, php_mycall_handler);
        //myFunction();

        // MYTEST_G(str2str) = {"hello":"ok"};
    });

    return extension;
}