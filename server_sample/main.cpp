#include <phpcpp.h>
#include <iostream>


class SampleServer : public Php::Base
{
private:
    /**
     *  The initial value
     *  @var    int
     */
    int port = -1;

    int threadNum = 2;

    int workerNum =2;

public:
    /**
     *  C++ constructor and destructor
     */
    SampleServer(Php::Parames){
        this.
    };

    virtual ~SampleServer() = default;

    Php::Value start() { 
        return "start";
    }

    Php::Value on() {
        return "start";
    }

    Php::Value send(){
        return "send";
    }

    Php::Value addTimer(){
        return "addTimer";
    }

    Php::Value delTimer(){
        return "delTimer";
    }
};

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
        static Php::Extension extension("sample_server", "1.0");

        // add the example function so that it can be called from PHP scripts
        //extension.add("example_function", example_function);


        Php::Class<SampleServer> PHPClass("sample_server");
        
        //注册方法的
        PHPClass.method<&SampleServer::start>("start");//void 不需要参数
        PHPClass.method<&SampleServer::on>("on");//event_name callback
        PHPClass.method<&SampleServer::send>("send");//fd data
        PHPClass.method<&SampleServer::addTimer>("addTimer");// time once callback param 
        PHPClass.method<&SampleServer::delTimer>("delTimer"); //td

        //注册属性的
        PHPClass.property("port", -1, Php::Public);
        PHPClass.property("threadNum", 2, Php::Public);
        PHPClass.property("workerNum", 2, Php::Public);

        PHPClass.property("onReceive", 0, Php::Public);
        PHPClass.property("onClose", 0, Php::Public);
        PHPClass.property("onConnect", 0, Php::Public);
        PHPClass.property("onStart", 0, Php::Public);


        // add the class to the extension
        extension.add(std::move(PHPClass));


        // return the extension details
        return extension;
    }
}