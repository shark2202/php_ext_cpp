#include "php.h"
#include "zend_types.h"
#include "ext/standard/php_string.h"
#include "Zend/zend_generators.h"
#include "zend_smart_str.h"


#define dump_max_depth 10

void append_backtrace(smart_str *trace_str TSRMLS_DC);
static void debug_print_backtrace_args(zval *arg_array TSRMLS_DC, smart_str *trace_str);
static void append_flat_zval_r(zval *expr TSRMLS_DC, smart_str *trace_str, char depth);
static void append_flat_hash(HashTable *ht TSRMLS_DC, smart_str *trace_str, char is_object, char depth);
static void debug_backtrace_get_args(zend_execute_data *call, zval *arg_array);
static int append_variable(zval *expr, smart_str *trace_str);
static char *apm_addslashes(char *str, uint length, int *new_length);

void append_backtrace(smart_str *trace_str TSRMLS_DC)
{
	/* backtrace variables */
	zend_execute_data *ptr, *skip;

	int lineno;
	const char *function_name;
	const char *filename;
	char *call_type;
	const char *include_filename = NULL;

	zval arg_array;
	zend_execute_data *call;
	zend_string *class_name = NULL;
	zend_object *object;
	zend_function *func;

	int indent = 0;

	ZVAL_UNDEF(&arg_array);//初始化一个变量
	//FIXME? ptr = EX(prev_execute_data);
	ptr = EG(current_execute_data);//获取当前允许的数据
	call = ptr;


	while (ptr) {
		class_name = NULL;
		call_type = NULL;

		ZVAL_UNDEF(&arg_array);
		ptr = zend_generator_check_placeholder_frame(ptr);//检查生成器吗？


		skip = ptr;

        // 方法为空或者code为空？
        // 检查前一个数据
        // 检查前一个opcode不是特定的code 
        // ZEND_DO_ICALL 
        // ZEND_DO_UCALL 
        // ZEND_DO_FCALL 
        // ZEND_DO_FCALL_BY_NAME 
        // ZEND_INCLUDE_OR_EVAL 对应require include eval这些
		if ((!skip->func || !ZEND_USER_CODE(skip->func->common.type)) &&
			skip->prev_execute_data &&
			skip->prev_execute_data->func &&
			ZEND_USER_CODE(skip->prev_execute_data->func->common.type) &&

			skip->prev_execute_data->opline->opcode != ZEND_DO_ICALL &&
			skip->prev_execute_data->opline->opcode != ZEND_DO_UCALL &&
			skip->prev_execute_data->opline->opcode != ZEND_DO_FCALL &&
			skip->prev_execute_data->opline->opcode != ZEND_DO_FCALL_BY_NAME &&
			skip->prev_execute_data->opline->opcode != ZEND_INCLUDE_OR_EVAL) {

			skip = skip->prev_execute_data;
		}

		if (skip->func && ZEND_USER_CODE(skip->func->common.type)) {
			filename = skip->func->op_array.filename->val;//获取文件名

			if (skip->opline->opcode == ZEND_HANDLE_EXCEPTION) {//异常处理的
				if (EG(opline_before_exception)) {
					lineno = EG(opline_before_exception)->lineno;
				} else {
					lineno = skip->func->op_array.line_end;
				}
			} else {
				lineno = skip->opline->lineno;//当前的文件的行数
			}
		}
		else {
			filename = NULL;
			lineno = 0;
		}

		/* $this may be passed into regular internal functions */
		object = Z_OBJ(call->This);

		if (call->func) {
			func = call->func;

            //下面是获取方法的名称
			function_name = (func->common.scope && func->common.scope->trait_aliases) ?
				ZSTR_VAL(zend_resolve_method_name(
					(object ? object->ce : func->common.scope), func)) :
				(func->common.function_name ?
					ZSTR_VAL(func->common.function_name) : NULL);
		} else {
			func = NULL;
			function_name = NULL;
		}

		if (function_name) {
            //如果是对象的实例方法，查询对象的类的名称信息
			if (object) {
				if (func->common.scope) {
					class_name = func->common.scope->name;
				} else if (object->handlers->get_class_name == std_object_handlers.get_class_name) {
					class_name = object->ce->name;
				} else {
					class_name = object->handlers->get_class_name(object);
				}

				call_type = "->";
            //如果不是对象的实例方法，那么可能是类方法或者是命名空间的方法
			} else if (func->common.scope) {
				class_name = func->common.scope->name;
				call_type = "::";
			} else {
				class_name = NULL;
				call_type = NULL;
			}

            //如果不是通过eval执行的，要解析方法的参数
			if (func->type != ZEND_EVAL_CODE) {
				debug_backtrace_get_args(call, &arg_array);
			}
		}
		else {
			/* i know this is kinda ugly, but i'm trying to avoid extra cycles in the main execution loop */
			zend_bool build_filename_arg = 1;

			if (!ptr->func || !ZEND_USER_CODE(ptr->func->common.type) || ptr->opline->opcode != ZEND_INCLUDE_OR_EVAL) {
				/* can happen when calling eval from a custom sapi */
				function_name = "unknown";
				build_filename_arg = 0;
			} else
			switch (ptr->opline->op2.constant) {
				case ZEND_EVAL:
					function_name = "eval";
					build_filename_arg = 0;
					break;
				case ZEND_INCLUDE:
					function_name = "include";
					break;
				case ZEND_REQUIRE:
					function_name = "require";
					break;
				case ZEND_INCLUDE_ONCE:
					function_name = "include_once";
					break;
				case ZEND_REQUIRE_ONCE:
					function_name = "require_once";
					break;
				default:
					/* this can actually happen if you use debug_backtrace() in your error_handler and
					 * you're in the top-scope */
					function_name = "unknown";
					build_filename_arg = 0;
					break;
			}

			if (build_filename_arg && include_filename) {
				array_init(&arg_array);
				add_next_index_string(&arg_array, include_filename);
			}
			call_type = NULL;
		}
		smart_str_appendc(trace_str, '#');
		smart_str_append_long(trace_str, indent);
		smart_str_appendc(trace_str, ' ');

		if (class_name) {
			smart_str_appends(trace_str, ZSTR_VAL(class_name));
			/* here, call_type is either "::" or "->" */
			smart_str_appendl(trace_str, call_type, 2);
		}
		if (function_name) {
			smart_str_appends(trace_str, function_name);
		} else {
            //如果没有查询到方法，就是在全局直接写的代码运行的
			smart_str_appendl(trace_str, "main", 4);
		}
		smart_str_appendc(trace_str, '(');
		if (Z_TYPE(arg_array) != IS_UNDEF) {
			debug_print_backtrace_args(&arg_array, trace_str);
			zval_ptr_dtor(&arg_array);
		}

        //什么情况下没有对应的文件名？
		if (filename) {
			smart_str_appendl(trace_str, ") called at [", sizeof(") called at [") - 1);
			smart_str_appends(trace_str, filename);
			smart_str_appendc(trace_str, ':');
			smart_str_append_long(trace_str, lineno);
			smart_str_appendl(trace_str, "]\n", 2);
		} else {
			zend_execute_data *prev_call = skip;
			zend_execute_data *prev = skip->prev_execute_data;

			while (prev) {
				if (prev_call &&
					prev_call->func &&
					!ZEND_USER_CODE(prev_call->func->common.type)) {
					prev = NULL;
					break;
				}
				if (prev->func && ZEND_USER_CODE(prev->func->common.type)) {
					zend_printf(") called at [%s:%d]\n", prev->func->op_array.filename->val, prev->opline->lineno);
					break;
				}
				prev_call = prev;
				prev = prev->prev_execute_data;
			}
			if (!prev) {
				smart_str_appendl(trace_str, ")\n", 2);
			}
		}
		include_filename = filename;

        //这个是往前推的
		ptr = skip->prev_execute_data;
		++indent;
		call = skip;
	}
}

static void debug_print_backtrace_args(zval *arg_array, smart_str *trace_str)
{
	zval *tmp;
	int i = 0;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(arg_array), tmp) {
		if (i++) {
			smart_str_appendl(trace_str, ", ", 2);
		}
		append_flat_zval_r(tmp, trace_str, 0);
	} ZEND_HASH_FOREACH_END();
}

static void append_flat_zval_r(zval *expr TSRMLS_DC, smart_str *trace_str, char depth)
{
	if (depth >= dump_max_depth) {
		smart_str_appendl(trace_str, "/* [...] */", sizeof("/* [...] */") - 1);
		return;
	}

	switch (Z_TYPE_P(expr)) {
		case IS_REFERENCE:
			ZVAL_DEREF(expr);
			smart_str_appendc(trace_str, '&');
			append_flat_zval_r(expr, trace_str, depth);
			break;

		case IS_ARRAY:
			smart_str_appendc(trace_str, '[');
			if (ZEND_HASH_APPLY_PROTECTION(Z_ARRVAL_P(expr)) && ++Z_ARRVAL_P(expr)->u.v.nApplyCount>1) {
				smart_str_appendl(trace_str, " *RECURSION*", sizeof(" *RECURSION*") - 1);
				Z_ARRVAL_P(expr)->u.v.nApplyCount--;
				return;
			}
			append_flat_hash(Z_ARRVAL_P(expr) TSRMLS_CC, trace_str, 0, depth + 1);
			smart_str_appendc(trace_str, ']');
			if (ZEND_HASH_APPLY_PROTECTION(Z_ARRVAL_P(expr))) {
				Z_ARRVAL_P(expr)->u.v.nApplyCount--;
			}
			break;
		case IS_OBJECT:
		{
			HashTable *properties = NULL;
			zend_string *class_name = Z_OBJ_HANDLER_P(expr, get_class_name)(Z_OBJ_P(expr));
			smart_str_appends(trace_str, ZSTR_VAL(class_name));
			smart_str_appendl(trace_str, " Object (", sizeof(" Object (") - 1);
			zend_string_release(class_name);

			if (Z_OBJ_APPLY_COUNT_P(expr) > 0) {
				smart_str_appendl(trace_str, " *RECURSION*", sizeof(" *RECURSION*") - 1);
				return;
			}
			if (Z_OBJ_HANDLER_P(expr, get_properties)) {
				properties = Z_OBJPROP_P(expr);
			}
			if (properties) {

				Z_OBJ_INC_APPLY_COUNT_P(expr);
				append_flat_hash(properties TSRMLS_CC, trace_str, 1, depth + 1);
				Z_OBJ_DEC_APPLY_COUNT_P(expr);
			}
			smart_str_appendc(trace_str, ')');
			break;
		}
		default:
			append_variable(expr, trace_str);
			break;
	}
}

static void append_flat_hash(HashTable *ht TSRMLS_DC, smart_str *trace_str, char is_object, char depth)
{
	int i = 0;

	zval *tmp;
	zend_string *string_key;
	zend_ulong num_key;

	ZEND_HASH_FOREACH_KEY_VAL_IND(ht, num_key, string_key, tmp) {
		if (depth >= dump_max_depth) {
			smart_str_appendl(trace_str, "/* [...] */", sizeof("/* [...] */") - 1);
			return;
		}

		if (i++ > 0) {
			smart_str_appendl(trace_str, ", ", 2);
		}
		smart_str_appendc(trace_str, '[');
		if (string_key) {
			smart_str_appendl(trace_str, ZSTR_VAL(string_key), ZSTR_LEN(string_key));
		} else {
			smart_str_append_long(trace_str, num_key);
		}

		smart_str_appendl(trace_str, "] => ", 5);
		append_flat_zval_r(tmp, trace_str, depth);
	} ZEND_HASH_FOREACH_END();
}

static int append_variable(zval *expr, smart_str *trace_str)
{
	zval expr_copy;
	int use_copy;
	char is_string = 0;
	char * temp;
	int new_len;

	if (Z_TYPE_P(expr) == IS_STRING) {
		smart_str_appendc(trace_str, '"');
		is_string = 1;
	}

	use_copy = zend_make_printable_zval(expr, &expr_copy);
	if (use_copy) {
		expr = &expr_copy;
	}
	if (Z_STRLEN_P(expr) == 0) { /* optimize away empty strings */
		if (is_string) {
			smart_str_appendc(trace_str, '"');
		}
		if (use_copy) {
			zval_dtor(expr);
		}
		return 0;
	}

	if (is_string) {
		temp = apm_addslashes(Z_STRVAL_P(expr), Z_STRLEN_P(expr), &new_len);
		smart_str_appendl(trace_str, temp, new_len);
		smart_str_appendc(trace_str, '"');
		if (temp) {
			efree(temp);
		}
	} else {
		smart_str_appendl(trace_str, Z_STRVAL_P(expr), Z_STRLEN_P(expr));
	}

	if (use_copy) {
		zval_dtor(expr);
	}
	return Z_STRLEN_P(expr);
}

static void debug_backtrace_get_args(zend_execute_data *call, zval *arg_array)
{
	uint32_t num_args = ZEND_CALL_NUM_ARGS(call);

	array_init_size(arg_array, num_args);
	if (num_args) {
		uint32_t i = 0;
		zval *p = ZEND_CALL_ARG(call, 1);

		zend_hash_real_init(Z_ARRVAL_P(arg_array), 1);
		ZEND_HASH_FILL_PACKED(Z_ARRVAL_P(arg_array)) {
			if (call->func->type == ZEND_USER_FUNCTION) {
				uint32_t first_extra_arg = call->func->op_array.num_args;

				if (ZEND_CALL_NUM_ARGS(call) > first_extra_arg) {
					while (i < first_extra_arg) {
						if (Z_OPT_REFCOUNTED_P(p)) Z_ADDREF_P(p);
						ZEND_HASH_FILL_ADD(p);
						zend_hash_next_index_insert_new(Z_ARRVAL_P(arg_array), p);
						p++;
						i++;
					}
					p = ZEND_CALL_VAR_NUM(call, call->func->op_array.last_var + call->func->op_array.T);
				}
			}

			while (i < num_args) {
				if (Z_OPT_REFCOUNTED_P(p)) Z_ADDREF_P(p);
				ZEND_HASH_FILL_ADD(p);
				p++;
				i++;
			}
		} ZEND_HASH_FILL_END();
	}
}

static char *apm_addslashes(char *str, uint length, int *new_length)
{
	/* maximum string length, worst case situation */
	char *new_str;
	char *source, *target;
	char *end;
	int local_new_length;

	if (!new_length) {
		new_length = &local_new_length;
	}

	if (!str) {
		*new_length = 0;
		return str;
	}
	new_str = (char *) safe_emalloc(2, (length ? length : (length = strlen(str))), 1);
	source = str;
	end = source + length;
	target = new_str;

	while (source < end) {
		switch (*source) {
			case '\0':
				*target++ = '\\';
				*target++ = '0';
				break;
			case '\"':
			case '\\':
				*target++ = '\\';
				/* break is missing *intentionally* */
			default:
				*target++ = *source;
				break;
		}

		source++;
	}

	*target = 0;
	*new_length = target - new_str;
	return (char *) erealloc(new_str, *new_length + 1);
}
