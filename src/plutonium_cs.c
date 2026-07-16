#include "vibrant_logs.h"
#include "dynamic_array_spellbook.h"
#include "plutonium_cs.h"

// array of void*
typedef struct obj_arr
{
	void **data;
	size_t size, capacity;
} obj_arr;

// info for each component TYPE in the system
typedef struct component_type_info
{
	// array of object pointers (match-to-instances)
	obj_arr objs;
	// instances of this component
	obj_arr components;
	// size of component in bytes
	size_t elem_size;
	// init function (constructor-like)
	pluto_cs_init_fn init;
	// type of component
	int type;
} component_type_info;

// array of component type info structs
typedef struct component_type_info_arr
{
	// array containing component info
	component_type_info *data;
	size_t size, capacity;
} component_type_info_arr;


static component_type_info_arr component_types = {0};
static bool init = false;


int pluto_cs_init()
{
	if(init)
	{
		vl_log(VL_ERROR, "Cannot re-initialize PlutoniumCS!\n");
		return 0;
	}

	dynas_init(&component_types);

	if(!component_types.data)
	{
		vl_log(VL_ERROR, "Failed to allocate memory for component registry!\n");
		return 0;
	}

	init = true;
	vl_log(VL_SUCCESS, "Initialized PlutoniumCS!\n");
	return 1;
}
void pluto_cs_shutdown()
{
	if(!init)
	{
		vl_log(VL_ERROR, "Cannot shutdown PlutoniumCS because it was never initialized!\n");
		return;
	}

	// free each component array for each type
	for(size_t i = 0; i < component_types.size; ++i)
	{
		// get type info at i
		component_type_info *info = &component_types.data[i];

		// free each component in the instances array:
		for(size_t j = 0; j < info -> components.size; ++j)
			free(info -> components.data[i]);

		// free the whole instances array
		dynas_free(&info -> components);
		dynas_free(&info -> objs);
	}

	// free the whole component type array
	dynas_free(&component_types);

	init = false;
	vl_log(VL_SUCCESS, "PlutoniumCS properly shut down!\n");
}

// get component type info based on a type
static component_type_info *pluto_cs_find_component_type(int type)
{
	for(size_t i = 0; i < component_types.size; ++i)
	{
		if(component_types.data[i].type == type)
			return &component_types.data[i];
	}

	return NULL;
}

int pluto_cs_register(int type, size_t size_bytes, pluto_cs_init_fn init_fn)
{
	if(!init)
	{
		vl_log(VL_ERROR, "Cannot register component, PlutoniumCS was never initialized!\n");
		return 0;
	}

	if(!init_fn)
	{
		vl_log(VL_ERROR, "Registering a component requires the init function to be a valid pointer!\n");
		return 0;
	}

	// see if type already registered
	if(pluto_cs_find_component_type(type))
	{
		vl_log(VL_ERROR, "Component type already registered: %d\n", type);
		return 0;
	}

	// type does not exist yet, so add it and track it
	component_type_info info = {
		.elem_size = size_bytes,
		.init = init_fn,
		.type = type,
	};

	// init the object arrays in this type info
	dynas_init(&info.components);
	dynas_init(&info.objs);

	// check for bad alloc
	if(!info.components.data)
	{
		vl_log(VL_ERROR, "Failed to allocate memory for component array!\n");
		return 0;
	}

	dynas_add(&component_types, info);

	// check for realloc failure
	if(!component_types.data)
	{
		vl_log(VL_ERROR, "Failed to realloc memory in component system!\n");
		return 0;
	}

	vl_log(VL_SUCCESS, "Successfully registered component type: %d\n", type);

	return 1;
}

void *pluto_cs_add_component(void *obj, int type)
{
	if(!init)
	{
		vl_log(VL_ERROR, "Cannot add component, PlutoniumCS was never initialized!\n");
		return NULL;
	}

	if(!obj)
	{
		vl_log(VL_ERROR, "Cannot add component to a null object!\n");
		return NULL;
	}

	// see if component type registered
	component_type_info *type_info = pluto_cs_find_component_type(type);
	if(!type_info)
	{
		vl_log(VL_ERROR, "Component type was never registered: %d\n", type);
		return NULL;
	}

	// see if obj already contains this component type
	if(pluto_cs_check_component(obj, type))
	{
		vl_log(VL_ERROR, "This object %p already contains this component type: %d\n", obj, type);
		return NULL;
	}

	// init instance of type
	void *component = malloc(type_info -> elem_size);
	if(!component)
	{
		vl_log(VL_ERROR, "Failed to allocate memory for component!\n");
		return NULL;
	}

	// add allocated pointer into instance array
	dynas_add(&type_info -> components, component);
	dynas_add(&type_info -> objs, obj);

	// component was successfully allocated, init it
	type_info -> init(component, obj);

	return component;
}

bool pluto_cs_check_component(void *obj, int type)
{
	if(!init)
	{
		vl_log(VL_ERROR, "Cannot check for a component, PlutoniumCS was never initialized!\n");
		return false;
	}

	if(!obj)
	{
		vl_log(VL_ERROR, "Cannot check for a component on a null object!\n");
		return false;
	}

	// if obj was never registered, automatic false
	component_type_info *info = pluto_cs_find_component_type(type);
	if(!info)
		return false;

	// see if object is paired with an instance of this component
	for(size_t i = 0; i < info -> components.size; ++i)
	{
		// match type of the current instance with pointer in the instance
		if(info -> type == type && info -> objs.data[i] == obj)
			return true;
	}

	return false;
}

void *pluto_cs_get_component(void *obj, int type)
{
	if(!init)
	{
		vl_log(VL_ERROR, "Cannot obtain component, PlutoniumCS was never initialized!\n");
		return NULL;
	}

	if(!obj)
	{
		vl_log(VL_ERROR, "Cannot obtain a component from a null object!\n");
		return NULL;
	}

	// if obj was never registered, automatic false
	component_type_info *info = pluto_cs_find_component_type(type);
	if(!info)
		return NULL;

	// find pair of obj-component
	for(size_t i = 0; i < info -> components.size; ++i)
	{
		if(info -> type == type && info -> objs.data[i] == obj)
			return info -> components.data[i];
	}

	return NULL;
}
