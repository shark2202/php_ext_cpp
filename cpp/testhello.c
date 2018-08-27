/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 基本的输入输出是定义在stdio这个库的
 */
#include <stdio.h>

/**
 * 定义一个结构体，这个结构体是空的？？？？
 */
struct _a{
    int a;
};
int main(){
    
    
    /**
     * 字符串可以分开的，最后的结果是合在一起的
     * @return 
     */
    printf("hello world!" "\n");
    
    struct _a b = {10};
    printf("b.a is %d \n",b.a);
    
    /**
     * 逐个的获取单个字符的
     * @return 
     */
    int ch;
    while( (ch = getchar()) != EOF){
        printf("input is %c" "\n",ch);
    }
    return 1;
}
