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

#include "include/common.h"

#define BUFF_SIZE       1024    //buffer大小
#define SELECT_TIMEOUT  5       //select的timeout seconds

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

//定义一个全部的变量
Php::Value ClassFn1;
//std::map<Php::Value,Php::Value> classMap1;

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

/**
 * 创建一个server类的
 */
class Server : public Php::Base{

private:
    static std::map<std::string,Php::Value> _events;

public:
    /**
     * [__construct description]
     * @param  params [description]
     * @return        [description]
     */
    Php::Value __construct(Php::Parameters &params){

        return this;
    }

    /**
     * [on description]
     * @param  params [description]
     * @return        [description]
     */
    Php::Value on(Php::Parameters &params){
        return true;
    }

    Php::Value run(){
        Php::Value self(this);

        Php::Value onStart = self["onStart"];
        Php::Value onConnect = self["onConnect"];
        Php::Value onMessage = self["onMessage"];
        Php::Value onClose = self["onClose"];

        onStart();

        int SERVPORT = self["port"];

        int BACKLOG = 10;
        //int MAXDATASIZE = 1000;
        // #define SERVPORT 13000   // 服务器监听端口号
        // #define BACKLOG 10  // 最大同时连接请求数
        // #define MAXDATASIZE 1000
        

        int sock_fd,client_fd;  // sock_fd：监听socket；client_fd：数据传输socket
        int sin_size;
        struct sockaddr_in my_addr; // 本机地址信息
        struct sockaddr_in remote_addr; // 客户端地址信息
     
        if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            // perror("socket创建出错！");
            Php::error << "socket创建出错！" << std::flush;
            return false;
        }
     
        long flag = 1;
        int pid;
        setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));

        //设置非阻塞模式的
        int flags;

        //首先获取sock的模式
        flags = fcntl(sock_fd,F_GETFL,0);
        if(flags < 0){
            Php::error << "fcntl出错！" << std::flush;
            return false;
        }
        //在原有的模式基础上添加非阻塞模式的
        if(fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) < 0){
            Php::error << "fcntl出错2！" << std::flush;
            return false;
        }
        Php::out << "设置非阻塞模式完成！"<< std::endl;

        //绑定地址的
        my_addr.sin_family=AF_INET;
        my_addr.sin_port=htons(SERVPORT);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(my_addr.sin_zero),8);
        if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
            // perror("bind出错！");
            Php::error << "bind出错！" << std::flush;
            return false;
        }

        //监听的
        if(listen(sock_fd, BACKLOG) == -1) {
            // perror("listen出错！");
            Php::error << "listen出错！" << std::flush;
            return false;
        }
        Php::out << "listen完成！"<< std::endl;

    //创建并初始化select需要的参数(这里仅监视read)，并把sock添加到fd_set中
    fd_set readfds;
    fd_set readfds_bak; //backup for readfds(由于每次select之后会更新readfds，因此需要backup)
    struct timeval timeout;
    int maxfd;
    maxfd = sock_fd;
    FD_ZERO(&readfds);
    FD_ZERO(&readfds_bak);
    FD_SET(sock_fd, &readfds_bak);

    //循环接受client请求
    int new_sock;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    char client_ip_str[INET_ADDRSTRLEN];
    int res;
    int i;
    char buffer[BUFF_SIZE];
    int recv_size;

    // int i;
    int new_maxfd;
    //int flags;

    while (true) {

        //注意select之后readfds和timeout的值都会被修改，因此每次都进行重置
        readfds = readfds_bak;
        //maxfd = updateMaxfd(readfds, maxfd);        //更新maxfd

        i = 0;
        new_maxfd = 0;

        for (i = 0; i <= maxfd; i++) {
            if (FD_ISSET(i, &readfds) && i > new_maxfd) {
                new_maxfd = i;
            }
        }
        maxfd = new_maxfd;

        timeout.tv_sec = SELECT_TIMEOUT;
        timeout.tv_usec = 0;
        printf("selecting maxfd=%d\n", maxfd);

        //select(这里没有设置writefds和errorfds，如有需要可以设置)
        res = select(maxfd + 1, &readfds, NULL, NULL, &timeout);
        if (res == -1) {
            perror("select failed");
            exit(EXIT_FAILURE);
        } else if (res == 0) {
            fprintf(stderr, "no socket ready for read within %d secs\n", SELECT_TIMEOUT);
            continue;
        }

        //检查每个socket，并进行读(如果是sock则accept)
        for (i = 0; i <= maxfd; i++) {
            if (!FD_ISSET(i, &readfds)) {
                continue;
            }
            //可读的socket
            if ( i == sock_fd) {
                //当前是server的socket，不进行读写而是accept新连接
                client_addr_len = sizeof(client_addr);
                new_sock = accept(sock_fd, (struct sockaddr *) &client_addr, &client_addr_len);
                if (new_sock == -1) {
                    perror("accept failed");
                    exit(EXIT_FAILURE);
                }
                if (!inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip_str, sizeof(client_ip_str))) {
                    perror("inet_ntop failed");
                    exit(EXIT_FAILURE);
                }
                printf("accept a client from: %s\n", client_ip_str);
                //设置new_sock为non-blocking
                //setSockNonBlock(new_sock);

                //获取原来的设置，加上非阻塞的选项
                flags = fcntl(new_sock, F_GETFL, 0);
                if (flags < 0) {
                    perror("fcntl(F_GETFL) failed");
                    exit(EXIT_FAILURE);
                }
                if (fcntl(new_sock, F_SETFL, flags | O_NONBLOCK) < 0) {
                    perror("fcntl(F_SETFL) failed");
                    exit(EXIT_FAILURE);
                }
                //把new_sock添加到select的侦听中
                if (new_sock > maxfd) {
                    maxfd = new_sock;
                }
                FD_SET(new_sock, &readfds_bak);
            } else {
                //当前是client连接的socket，可以写(read from client)
                memset(buffer, 0, sizeof(buffer));
                if ( (recv_size = recv(i, buffer, sizeof(buffer), 0)) == -1 ) {
                    perror("recv failed");
                    exit(EXIT_FAILURE);
                }
                printf("recved from new_sock=%d : %s(%d length string)\n", i, buffer, recv_size);
                //立即将收到的内容写回去，并关闭连接
                if ( send(i, buffer, recv_size, 0) == -1 ) {
                    perror("send failed");
                    exit(EXIT_FAILURE);
                }
                printf("send to new_sock=%d done\n", i);

                if(buffer == "closeConn"){
                    if ( close(i) == -1 ) {
                        perror("close failed");
                        exit(EXIT_FAILURE);
                    }
                    printf("close new_sock=%d done\n", i);
                    //将当前的socket从select的侦听中移除
                    FD_CLR(i, &readfds_bak);
                }
            }
        }
    }
        // while(true) {
        //     sin_size = sizeof(struct sockaddr_in);
        //     if((client_fd = accept(sock_fd, (struct sockaddr *)&remote_addr, (socklen_t*)&sin_size)) == -1) {
        //         perror("accept出错");
        //         Php::error << "accept出错" << std::flush;
        //         continue;
        //     }

        //     onConnect();

        //     printf("received a connection from %s:%u\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
        //     //Php::out << "regular output" << std::endl;
        //     Php::out << inet_ntoa(remote_addr.sin_addr) << std::endl;
        //     Php::out << ntohs(remote_addr.sin_port) << std::endl;

        //     pid = fork();
        //     if(pid < 0){
        //         Php::error << "fork失败！" << std::flush;
        //     }else if(pid == 0) {   // 子进程代码段
        //         if(send(client_fd, "Hello, you are connected!\n", 26, 0) == -1) {
        //             // perror("send出错！");
        //             Php::error << "send出错！" << std::flush;
        //         }
        //         close(client_fd);

        //         onClose();

        //         return false;
        //     }else{// 父进程的
        //         close(client_fd);
        //     }
        // }
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

    //读取回调函数进行执行的
    void callBack1(){
        Php::Value self(this);
        //Php::Function self["property1"] = 100;
        //从当前对象的属性上读取到匿名函数，然后执行的
         Php::Value func = self["property1"];
         func();
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
            Php::out << "" << std::endl;

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

//连接池的方案
//
//
//
class ObjectPool1 : public Php::Base{
public:
    /**
     * 保存连接的
     */
    static std::map<std::string,Php::Value> _pool;
    
    /**
     * [hashId description]
     * @param  conf [description]
     * @return      [description]
     */
    static std::string hashId(std::string conf){
        Php::out << conf << "hashId" << std::endl;
        return conf;
    }

    //从ini中获取配置进行连接的
    void init_parse(){

    }

    /**
     * [initObject description]
     * @param  params [description]
     * @return        [description]
     */
    void __construct(Php::Parameters &params){
        std::string class_name = params[0];//类名称
        Php::Object obj(params[0]);
        _pool[hashId(class_name)] = obj;

        Php::out << class_name << " __construct" << std::endl;
    }

    /**
     * [getObject description]
     * @return [description]
     */
    Php::Value getObject(Php::Parameters &params){
        std::string class_name = params[0];//类名称
        Php::Object obj = _pool[hashId(class_name)];

        Php::out << class_name << " getObject"  << std::endl;
        obj.call("init");

        return obj;
    }

    void releaseObject(Php::Parameters &params){
        std::string class_name = params[0];//类名称

        Php::out << class_name << " releaseObject"  << std::endl;
    }
};

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
            //ObjCall::onCallFn1 = params[0];
            ClassFn1 = params[0];
            return true;
        }
        
        return false;
    }
    
    Php::Value callInC1(Php::Parameters &params){
        if(ClassFn1){
            //Php::Array arg1;
            //arg1[0] = 1;
            //arg1[1] = 100;
//            return arg1;
            //直接使用() 进行调用的
            //Php::Value fn1 = classMap1.find("ObjCall::onCall1");
            Php::Value p1 = params;
            return ClassFn1(p1);
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
            return ClassFn1;
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

//std::map<Php::Value,Php::Value> classMap1 = {};
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
            
            ClassFn1 = NULL;
        });
        
        myext.onShutdown([](){
            ObjCall::onCallFn1 = NULL;
            
            //ClassFn1 = NULL;
            
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

        ASSERT(0,"err");
        
        return myext;
    }
}
