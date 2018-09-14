/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>

int main(){

    struct sa{
        int sa;
        float sb;
    };
    
    struct sa *a = (void*)0;
    
    struct sa b = (struct sa){1,1.2};
    
    a = &b;
    
    printf("a->sa = %d\n",a->sa);
    
    a->sa = 100;
    printf("b.sa = %d\n",b.sa);
    
    return 0;
}

