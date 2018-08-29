#include <stdio.h>
#include <stdarg.h>
/**
 * 使用函数指针，类似于php中的回调
 * @return 
 */

//声明一个结构体
struct MyStruct;

//定义一个全局的指针
static struct MyStruct (*p)(struct MyStruct s, int a[*]) = NULL;

//声明一个函数
static void Test(void);

//声明一个方法
static struct MyStruct Func(struct MyStruct s, int a[]);

//声明一个方法
//参数是一个函数指针的
static void callfn(struct MyStruct (*f1)(struct MyStruct s, int a[*]));

//定义一个函数类型fffff
typedef void(*fffff)(void);
//不能使用下面的方法来定义一个函数类型
//typedef void(*)(void) fffff2;

//定义一个函数类型是否可以？？
typedef void(f2)(void);
/**
 * 不定参数的函数
 * @param n
 * @param ...
 * @return 
 */
static int Foo(int n, ...){
    va_list ap;
    va_start(ap, n);
    
    int sum = 0;
    
    for(int i = 0; i < n; i++){
        sum += va_arg(ap, int);
    }
    
    va_end(ap);
    
    return sum;
}

int main(){
    
    //声明一个函数指针的
    void(*pf)(void) = Test;
    
    //执行一个函数指针的
    (*pf)();
    
    //定义一个函数指针
    fffff pf1 = Test;
    pf1();
    
    //fffff* pf2 = &pf1;
    //*pf2();
    
    //定义了一个函数实体，和上面定义的函数指针还是有区别的
    f2 pf3;
    //pf3 = Test;
    pf3();
    
    f2* pf4 = NULL;
    pf4 = Test;
    pf4();
    
    //指向指针的指针
    f2** pf5 = NULL;
    pf5 = &pf4;
    (*pf5)();
    
    int (*pFunc)(int, ...) = &Foo;
    
    int result = pFunc(2,10,20,30);
    printf("result 1 = %d \n",result);
    
    int(**pp)(int, ...) = &pFunc;
    
    result = (*pp)(5,1,2,3,4,5);
    
    printf("result 2 = %d \n",result);
    
    *pp = NULL;
    
    if(pFunc == NULL){
        puts("NULL");
    }
    
    struct MyStruct (*p1)(struct MyStruct s, int a[*]) = Func;
    callfn(p1);
    
}

struct MyStruct{
    int a;
    float f;
};

static struct MyStruct Func(struct MyStruct s, int a[]){
    printf("sum = %f\n",s.a + s.f + a[0]);
    
    return s;
}

static void Test(void){
    //全局变量p,实际上是一个函数指针
    p = Func;
    
    //字面量的结构体和数组
    p((struct MyStruct){.a = 10, .f = 0.5f},(int[]){1,2,3});
}

/**
 * 
 * @param f1
 * @param ...
 */
static void callfn(struct MyStruct (*f1)(struct MyStruct s, int a[*])){
    f1((struct MyStruct){.a = 12, .f = 10.5f},(int[]){10,2,3});
}

void pf3(void){
    printf("pf3\n");
}