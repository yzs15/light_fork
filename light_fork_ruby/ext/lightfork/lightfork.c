#include "ruby.h"

extern int ipdos_set_mem_file_name(const char* filename);
extern int ipdos_remove_parent_mem_file();

static VALUE set_mem_file_name(VALUE self, VALUE str) {
    const char* filename = StringValueCStr(str);
    
    ipdos_set_mem_file_name(filename);

    return Qnil;
}

static VALUE remove_parent_mem_file(VALUE self) {

    ipdos_remove_parent_mem_file();

    return Qnil;
}

void Init_lightfork() {
    VALUE cLightFork = rb_define_module("LightFork");
    // VALUE cLightFork = rb_const_get(rb_cObject, rb_intern("LightFork"));
    rb_define_module_function(cLightFork, "set_mem_file_name", set_mem_file_name, 1);
    rb_define_module_function(cLightFork, "remove_parent_mem_file", remove_parent_mem_file, 0);
}