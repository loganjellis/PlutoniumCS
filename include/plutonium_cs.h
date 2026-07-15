#pragma once

#ifdef _WIN32
	#ifdef PLUTONIUM_CS_EXPORTS
		#define PLUTONIUM_CS_API __declspec(dllexport)
	#else
		#define PLUTONIUM_CS_API __declspec(dllimport)
	#endif
#else
	#define PLUTONIUM_CS_API
#endif

/**
  For components, they require an init function
  in the registry so PlutoniumCS knows how to instantiate
  each one when requested. The init function should
  just return an instance of the component.

  @param component The component to initialize.
  @param owner The object pointer that is paired with the
  component; the object that owns this component.
*/
typedef void (*pluto_cs_init_fn) (void *component, void *owner);

/**
  Initializes the PlutoniumCS library.

  @return 1 on success, 0 on failure.
*/
PLUTONIUM_CS_API int pluto_cs_init(void);
/**
  Shuts down the PlutoniumCS library.
*/
PLUTONIUM_CS_API void pluto_cs_shutdown(void);

/**
  Registers component type info for the component system.

  @important You must call this function for every
  type of component required in your program.

  @param type The type of component to register. You should
  only register each type once.
  @param size_bytes The size of the component in bytes. For
  example, sizeof(my_component).
  @param init_fn The init function for the component. See
  pluto_cs_init_fn.

  @see pluto_cs_init_fn
*/
PLUTONIUM_CS_API int pluto_cs_register(int type, size_t size_bytes, pluto_cs_init_fn init_fn);

/**
  Adds a component to an object.

  @note When calling this function, the 'obj' pointer
  is expected to be handled by the user. Additionally,
  it should be a valid pointer for the entirety of the program.
  PlutoniumCS tracks objects and their components by comparing
  pointers.

  @return A valid component pointer on success, NULL on failure.
*/
PLUTONIUM_CS_API void *pluto_cs_add_component(void *obj, int type);

/**
  Returns whether or not an object owns a component
  of the specified type.
*/
PLUTONIUM_CS_API bool pluto_cs_check_component(void *obj, int type);

/**
  Returns an object's component of the matching type,
  if it is found, and returns NULL if the component
  isn't found.
*/
PLUTONIUM_CS_API void *pluto_cs_get_component(void *obj, int type);
