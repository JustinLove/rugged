/*
 * The MIT License
 *
 * Copyright (c) 2010 Scott Chacon
 * Copyright (c) 2010 Vicent Marti
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "rugged.h"

VALUE rb_mRugged;

static VALUE rb_git_hex_to_raw(VALUE self, VALUE hex)
{
	git_oid oid;

	Check_Type(hex, T_STRING);
	git_oid_mkstr(&oid, RSTRING_PTR(hex));
	return rugged_str_ascii(oid.id, 20);
}

static VALUE rb_git_raw_to_hex(VALUE self, VALUE raw)
{
	git_oid oid;
	char out[40];

	Check_Type(raw, T_STRING);
	git_oid_mkraw(&oid, RSTRING_PTR(raw));
	git_oid_fmt(out, &oid);

	return rugged_str_new(out, 40, NULL);
}

static VALUE rb_git_type_to_string(VALUE self, VALUE type)
{
	const char *str;

	Check_Type(type, T_FIXNUM);
	git_otype t = (git_otype)FIX2INT(type);
	str = git_object_type2string(t);
	return str ? rugged_str_new2(str, NULL) : Qfalse;
}

static VALUE rb_git_string_to_type(VALUE self, VALUE string_type)
{
	Check_Type(string_type, T_STRING);
	return INT2FIX(git_object_string2type(RSTRING_PTR(string_type)));
}

void rugged_exception_check(int errorcode)
{
	if (errorcode < 0)
		rb_raise(rb_eRuntimeError, "Rugged exception: %s", git_strerror(errorcode));
}


void Init_rugged()
{
	rb_mRugged = rb_define_module("Rugged");

	rb_define_module_function(rb_mRugged, "hex_to_raw", rb_git_hex_to_raw, 1);
	rb_define_module_function(rb_mRugged, "raw_to_hex", rb_git_raw_to_hex, 1);
	rb_define_module_function(rb_mRugged, "type_to_string", rb_git_type_to_string, 1);
	rb_define_module_function(rb_mRugged, "string_to_type", rb_git_string_to_type, 1);

	Init_rugged_signature();

	Init_rugged_object();
	Init_rugged_commit();
	Init_rugged_tree();
	Init_rugged_tag();
	Init_rugged_blob();

	Init_rugged_index();
	Init_rugged_repo();
	Init_rugged_revwalk();

	Init_rugged_backend();

	/* Constants */
	rb_define_const(rb_mRugged, "SORT_NONE", INT2FIX(0));
	rb_define_const(rb_mRugged, "SORT_TOPO", INT2FIX(1));
	rb_define_const(rb_mRugged, "SORT_DATE", INT2FIX(2));
	rb_define_const(rb_mRugged, "SORT_REVERSE", INT2FIX(4));

	rb_define_const(rb_mRugged, "OBJ_ANY", INT2FIX(GIT_OBJ_ANY));
	rb_define_const(rb_mRugged, "OBJ_BAD", INT2FIX(GIT_OBJ_BAD));
	rb_define_const(rb_mRugged, "OBJ_COMMIT", INT2FIX(GIT_OBJ_COMMIT));
	rb_define_const(rb_mRugged, "OBJ_TREE", INT2FIX(GIT_OBJ_TREE));
	rb_define_const(rb_mRugged, "OBJ_BLOB", INT2FIX(GIT_OBJ_BLOB));
	rb_define_const(rb_mRugged, "OBJ_TAG", INT2FIX(GIT_OBJ_TAG));
}

