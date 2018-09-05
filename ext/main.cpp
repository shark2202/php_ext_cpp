#include <phpcpp.h>
#include <string>
#include <iostream>

//下面是socket的库
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>


/**
 * 注册回调函数的
 */
class ObjCall : public Php::Base{
public:
    Php::Value onCallFn;
    
    static Php::Value onCallFn1;
    /**
     * 注册一个回调的方法
     * @param params
     * @return 
     */
    Php::Value onCall(Php::Parameters &params){
        if(params.size() > 0){
            this->onCallFn = params[0];
            
            ObjCall::onCallFn1 = params[1];
            return true;
        }
        
        return false;
    }
    
    Php::Value onCall1(Php::Parameters &params){
        if(params.size() > 0){
            ObjCall::onCallFn1 = params[0];
            return true;
        }
        
        return false;
    }
    
    Php::Value callInC1(Php::Parameters &params){
        if(ObjCall::onCallFn1){
            //Php::Array arg1;
            //arg1[0] = 1;
            //arg1[1] = 100;
//            return arg1;
            //直接使用() 进行调用的
            return ObjCall::onCallFn1(params);
            //return Php::call("")
        }
        
        return NULL;
    }
    
    Php::Value callInC(Php::Parameters &params){
        if(this->onCallFn){
            //Php::Array arg1;
            //arg1[0] = 1;
            //arg1[1] = 100;
//            return arg1;
            //直接使用() 进行调用的
            return this->onCallFn(params);
            //return Php::call("")
        }
        
        return NULL;
    }
    
    
    Php::Value getOnCall(Php::Parameters &params){
        if(params[0] == 1){
            return ObjCall::onCallFn1;
        }
        return this->onCallFn;
    }
};

//初始静态变量的
int StaticC::objectCount = 0;
std::vector<Php::Value> StaticC::extArr = {};//空数组的
std::map<std::string,Php::Value> StaticC::extMap = {};//空map

std::map<std::string,Php::Value> ObjectPool1::_pool = {};

Php::Value ObjCall::onCallFn1 = NULL;
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
            //StaticC::objectCount = 100;
            //StaticC::extArr.push_back(1);
            //StaticC::extMap["1"] = 100;
            ObjCall::onCallFn1 = NULL;
        });
        
        myext.onShutdown([](){
            ObjCall::onCallFn1 = NULL;
        });
//
//        Php::Namespace mynamespace("trComplex");
//
//        Php::Class<Complex> extClass("Complex");
//        //extClass.method("__construct",&Complex::__construct);
//        extClass.method<&Complex::__construct> ("__construct");
//        extClass.method<&Complex::getReal> ("getReal");
//        extClass.method<&Complex::getImage> ("getImage");
//
//        extClass.method<&Complex::getThis1> ("this1");
//        extClass.method<&Complex::getNewObject> ("this2");
//        
//        extClass.method<&Complex::newException> ("e1");
//        extClass.method<&Complex::catchException> ("e2");
//
//        //callPhpFn
//        extClass.method<&Complex::callPhpFn> ("callPhpFn");
//        
//        //class_exist_my
//        extClass.method<&Complex::class_exist_my> ("class_exist_my");
//
//        //myext.add(std::move(extClass));
//        mynamespace.add(std::move(extClass));
//
//        // mynamespace.add<Mytest::getConf>("getConf");
//
//        myext.add(std::move(mynamespace));
//
//        // myext.add<Mytest::include22>("include22");
//
//
//        Php::Class<StaticC> extClass2("StaticCnew");
//        extClass2.method<&StaticC::getSS> ("getSS");
//        extClass2.method<&StaticC::setSS> ("setSS");
//
//
//        extClass2.method<&StaticC::callBack1> ("callBack1");
//
//        extClass2.property("property1", "", Php::Public);
//
//
//        myext.add(std::move(extClass2));
//
//        myext.add<add_new_func>("add_new_func");
//
//        Php::Class<Server> myTcpServer("myTcpServer");
//        myTcpServer.method<&Server::__construct>("__construct");
//        myTcpServer.method<&Server::run>("run");
//
//        myTcpServer.property("port", "13000", Php::Public);
//
//        myTcpServer.property("onStart", "", Php::Public);//启动的时候
//        myTcpServer.property("onConnect", "", Php::Public);//连接的时候
//        myTcpServer.property("onMessage", "", Php::Public);//消息的时候
//        myTcpServer.property("onClose", "", Php::Public);//关闭的时候
//
//        myext.add(std::move(myTcpServer));
//
//        Php::Class<ObjectPool1> myObjectPool("ObjectPool1");
//        myObjectPool.method<&ObjectPool1::__construct>("__construct");
//        myObjectPool.method<&ObjectPool1::getObject>("getObject");
//        myObjectPool.method<&ObjectPool1::releaseObject>("releaseObject");
//
//        myext.add(std::move(myObjectPool));
        // return the extension
        
        Php::Class<ObjCall> myObjCall("ObjCall123");
        //myObjCall.method<&ObjCall::__construct>("__construct");
        myObjCall.method<&ObjCall::onCall>("onCall");//onCall1
        myObjCall.method<&ObjCall::onCall1>("onCall1");
        
        myObjCall.method<&ObjCall::callInC1>("callInC1");
        myObjCall.method<&ObjCall::callInC>("callInC");
        
        myObjCall.method<&ObjCall::getOnCall>("getOnCall");
        
        myext.add(std::move(myObjCall));
        
        return myext;
    }
}
