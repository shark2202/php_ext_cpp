
##获取php的头文件的地址,以及其他的配置信息
PHP_BIN = "/usr/local/php/bin/php"
PHP_CONFIG_BIN = "/usr/local/php/bin/php-config"


PHP_BIN = "/usr/local/php7/bin/php"
PHP_CONFIG_BIN = "/usr/local/php7/bin/php-config"


###``这个是执行shell命令的,返回的是shell的输出的
PHP_INCLUDE = `${PHP_CONFIG_BIN} --includes`
PHP_LIBS = `${PHP_CONFIG_BIN} --libs`
PHP_LDFLAGS = `${PHP_CONFIG_BIN} --ldflags`
PHP_INCLUDE_DIR = `${PHP_CONFIG_BIN} --include-dir`
PHP_EXTENSION_DIR = `${PHP_CONFIG_BIN} --extension-dir`

PHPCPP_INCLUDE = -I../phpcpp2.2.0 

NAME				=	extdemo
INI_DIR				=	/usr/local/php/etc/
EXTENSION_DIR		=	${PHP_EXTENSION_DIR}

EXTENSION 			=	${NAME}.so
INI 				=	${NAME}.ini
COMPILER			=	g++
LINKER				=	g++

COMPILER_FLAGS		=	-Wall -g -c -O0 -std=c++11 -fpic ${PHP_INCLUDE} -I${PHP_INCLUDE_DIR} ${PHPCPP_INCLUDE}  -o 
#COMPILER_FLAGS		=	-Wall -DDEBUG -g -c -O0 -std=c++11 -fpic ${PHP_INCLUDE} -I${PHP_INCLUDE_DIR} -o 
LINKER_FLAGS		=	-shared 
LINKER_DEPENDENCIES	=	-lphpcpp -L../phpcpp2.2.0/
#LINKER_DEPENDENCIES =  ../phpcpp2.2.0/libphpcpp.a.2.2.0 

##重新设置删除,拷贝,创建目录的命令
RM					=	rm -f
CP					=	sudo cp -f
MKDIR				=	mkdir -p


#### wildcard 这个就是可以展开通配符的
SOURCES				=	$(wildcard *.cpp)

##替换引用实际上是使用patsubst扩展函数的缩写 
## 就是把所有的cpp都改成o文件的
OBJECTS				=	$(SOURCES:%.cpp=%.o)


all:					${OBJECTS} ${EXTENSION}


## 链接生成.so文件的
${EXTENSION}:			${OBJECTS}
						${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

## 编译的
${OBJECTS}:
						${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

install:		
						${CP} ${EXTENSION} ${EXTENSION_DIR}
						#${CP} ${INI} ${INI_DIR}

clean:
						${RM} ${EXTENSION} ${OBJECTS}
#cmake 的 ADD_DEFINITIONS 
#是设置编译的宏参数的
#
#
#
#
#
#
#
#
#
#