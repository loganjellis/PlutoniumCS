#pragma once

#include <stdlib.h>
#include <string.h>

/**
  Initializes the given dynamic map.

  @note This macro expects a pointer to the dynamic map.

  The given dynamic map must have at least 4 fields:

  - A pointer representing the array of keys, called 'keys.'
  - A pointer representing the array of values, called 'values.'
  - A size_t called 'size.' This is the map's current size.
  - A size_t called 'capacity.' This is the max size of the map.

  When a dynamic map is initialized, its size is 0, its capacity
  starts out at 10, and its keys and values are automatically
  allocated for you.

  @important Make sure you call dynmaps_free(&dynamic_map) when you are
  ready to free the dynamic map's memory.
*/
#define dynmaps_init(dynamic_map) \
	do { \
		(dynamic_map) -> size = 0; \
		(dynamic_map) -> capacity = 10; \
		(dynamic_map) -> keys = calloc((dynamic_map) -> capacity * sizeof(*(dynamic_map) -> keys), sizeof(*(dynamic_map) -> keys)); \
		(dynamic_map) -> values = calloc((dynamic_map) -> capacity * sizeof(*(dynamic_map) -> values), sizeof(*(dynamic_map) -> values)); \
	} while(0)

/**
  Frees a dynamic map's memory.

  @note This macro expects a pointer to the dynamic map.
*/
#define dynmaps_free(dynamic_map) \
	do { \
		(dynamic_map) -> size = 0; \
		(dynamic_map) -> capacity = 10; \
		free((dynamic_map) -> keys); \
		free((dynamic_map) -> values); \
		(dynamic_map) -> keys = NULL; \
		(dynamic_map) -> values = NULL; \
	} while(0)

/**
  Frees a dynamic map's memory.

  @note This macro is targeted towards dynamic maps
  that use string keys. This is because string-keyed
  dynamic maps allocate memory to duplicate strings.

  @note This macro expects a pointer to the dynamic map.
*/
#define dynmaps_free_strkey(dynamic_map) \
	do { \
		for(size_t i = 0; i < (dynamic_map) -> size; ++i) \
			free((dynamic_map) -> keys[i]); \
		(dynamic_map) -> size = 0; \
		(dynamic_map) -> capacity = 10; \
		free((dynamic_map) -> keys); \
		free((dynamic_map) -> values); \
		(dynamic_map) -> keys = NULL; \
		(dynamic_map) -> values = NULL; \
	} while(0)

/**
  Obtains a value from a dynamic map
  based on a key. If the key is
  not found, the resulting value is NULL instead.

  @note This macro expects a pointer to the dynamic map.

  @important This macro compares each key in the map to the
  key given using the '=' operator, meaning if the map's
  keys are strings, this macro will not function
  properly. If using string keys, use dynmaps_get_strkey(...)
  instead.

  @note The 3rd argument should a pointer of the same type as
  the values in the map. This macro sets it to NULL when the
  key is not found in the map, and it points to the matching
  value if the key is found.
*/
#define dynmaps_get(dynamic_map, key, value) \
	do { \
		(value) = NULL; \
		for(size_t i = 0; i < (dynamic_map) -> size; ++i) \
		{ \
			if((dynamic_map) -> keys[i] == (key)) \
			{ \
				(value) = &(dynamic_map) -> values[i]; \
				break; \
			} \
		} \
	} while(0)

/**
  Obtains a value from a dynamic
  map based on a string key. If the key
  is not found, the resulting value is NULL
  instead.

  @note This macro expects a pointer to the dynamic map.

  @important This macro is used specifically when the
  map's keys are strings. This is because the macro
  will compare each string character-by-character.

  @note The 3rd argument should be a pointer of the
  same type as the values in the map. This macro
  sets it to NULL when the key is not found in the map,
  and it points to the matching value if the key is found.
*/
#define dynmaps_get_strkey(dynamic_map, key, value) \
	do { \
		(value) = NULL; \
		for(size_t i = 0; i < (dynamic_map) -> size; ++i) \
		{ \
			if(strcmp((dynamic_map) -> keys[i], (key)) == 0) \
			{ \
				(value) = &(dynamic_map) -> values[i]; \
				break; \
			} \
		} \
	} while(0)

/**
  This macro determines the given dynamic map
  needs to expand, and it will automatically
  expand the map is necessary.

  @note This macro expects a pointer to the dynamic map.
*/
#define dynmaps_expand(dynamic_map) \
	do { \
		if((dynamic_map) -> size >= (dynamic_map) -> capacity) \
		{ \
			if((dynamic_map) -> capacity * 2 <= SIZE_MAX) \
			{ \
				(dynamic_map) -> capacity *= 2; \
				(dynamic_map) -> keys = realloc((dynamic_map) -> keys, (dynamic_map) -> capacity * sizeof(*(dynamic_map) -> keys)); \
				(dynamic_map) -> values = realloc((dynamic_map) -> values, (dynamic_map) -> capacity * sizeof(*(dynamic_map) -> values)); \
			} \
		} \
	} while(0)

/**
  Sets a key and value in the given dynamic map.
  If the key already exists in the map, its value
  is set to the value given. If the key does not
  exist in the map, the key and value
  are appended to it. If the map uses string keys,
  use dynmaps_set_strkey(...) instead.

  @note This macro expects a pointer to the dynamic map.

  @note The dynamic map will auto-expand if necessary.
*/
#define dynmaps_set(dynamic_map, key, value) \
	do { \
		dynmaps_expand(dynamic_map); \
		int dynmaps_k_idx = -1; \
		dynmaps_find_entry(dynamic_map, key, dynmaps_k_idx); \
		if(dynmaps_k_idx != -1) \
		{ \
			(dynamic_map) -> values[dynmaps_k_idx] = (value); \
		} \
		else \
		{ \
			(dynamic_map) -> keys[(dynamic_map) -> size] = (key); \
			(dynamic_map) -> values[(dynamic_map) -> size] = (value); \
			(dynamic_map) -> size++; \
		} \
	} while(0)

/**
  Sets a key and value in the given dynamic map.
  If the key already exists in the map, its value
  is set to the value given. If the key does not
  exist in the map, the key and value
  are appended to it.

  @note This macro expects a pointer to the dynamic map.

  @note The dynamic map will auto-expand if necessary.
*/
#define dynmaps_set_strkey(dynamic_map, key, value) \
	do { \
		dynmaps_expand(dynamic_map); \
		int dynmaps_k_idx = -1; \
		dynmaps_find_entry_strkey(dynamic_map, key, dynmaps_k_idx); \
		if(dynmaps_k_idx != -1) \
		{ \
			(dynamic_map) -> values[dynmaps_k_idx] = (value); \
		} \
		else \
		{ \
			(dynamic_map) -> keys[(dynamic_map) -> size] = strdup((key)); \
			(dynamic_map) -> values[(dynamic_map) -> size] = (value); \
			(dynamic_map) -> size++; \
		} \
	} while(0)

/**
  Removes a key/value entry from a dynamic map at
  a given index.

  @note This macro expects a pointer to the dynamic map.

  @note This macro does not validate the size of the map or
  the index given, so make sure the index is valid for
  the given map, otherwise undefined behavior may occur.
*/
#define dynmaps_remove_at(dynamic_map, idx) \
	do { \
		memmove((dynamic_map) -> keys + (idx), (dynamic_map) -> keys + (idx) + 1, ((dynamic_map) -> size - (idx) - 1) * sizeof(*(dynamic_map) -> keys)); \
		memmove((dynamic_map) -> values + (idx), (dynamic_map) -> values + (idx) + 1, ((dynamic_map) -> size - (idx) - 1) * sizeof(*(dynamic_map) -> values)); \
		(dynamic_map) -> size--; \
	} while(0)

/**
  Determines the given map contains a specific key.
  For maps that use string keys, use dynmaps_find_entry_strkey(...)
  instead.

  @note This macro expects a pointer to the dynamic map.

  @note The 3rd argument should be a signed integer
  value indicating whether or not the key was found
  in the map. If 'result' is -1, the key was not found.
  If 'result' is not -1, it is equal to the index
  the key was found at in the map.
*/
#define dynmaps_find_entry(dynamic_map, key, result) \
	do { \
		(result) = -1; \
		for(size_t i = 0; i < (dynamic_map) -> size; ++i) \
		{ \
			if((dynamic_map) -> keys[i] == (key)) \
			{ \
				(result) = i; \
				break; \
			} \
		} \
	} while(0)
/**
  Determines the given map contains a specific string key.

  @note This macro expects a pointer to the dynamic map.

  @note The 3rd argument should be a signed integer
  value indicating whether or not the key was found
  in the map. If 'result' is -1, the key was not found.
  If 'result' is not -1, it is equal to the index
  the key was found at in the map.
*/
#define dynmaps_find_entry_strkey(dynamic_map, key, result) \
	do { \
		(result) = -1; \
		for(size_t i = 0; i < (dynamic_map) -> size; ++i) \
		{ \
			if(strcmp((dynamic_map) -> keys[i], (key)) == 0)\
			{ \
				(result) = i; \
				break; \
			} \
		} \
	} while(0)

/**
  Removes an entry from a map based on a specific key.
  For maps that use string keys, use dynmaps_remove_strkey_result(...)
  instead.

  @note This macro expects a pointer to the dynamic map.

  @note The 3rd argument should be a signed integer
  value indicating whether or not the key/value entry
  was removed successfully. If 'result' is -1, the key
  was not found, and therefore the entry was not removed.
  If 'result' is 1, the entry was found and removed.
*/
#define dynmaps_remove_result(dynamic_map, key, result) \
	do { \
		(result) = -1; \
		int dynmaps_entry_idx = -1; \
		dynmaps_find_entry(dynamic_map, key, dynmaps_entry_idx); \
		if(dynmaps_entry_idx != -1) \
		{ \
			dynmaps_remove_at(dynamic_map, dynmaps_entry_idx); \
			(result) = 1; \
		} \
	} while(0)

/**
  Removes an entry from a map based on a specific key.
  For maps that use string keys, use dynmaps_remove_strkey(...)
  instead.

  @note This macro expects a pointer to the dynamic map.
*/
#define dynmaps_remove(dynamic_map, key) \
	do { \
		int dynmaps_entry_idx = -1; \
		dynmaps_remove_result(dynamic_map, key, dynmaps_entry_idx); \
	} while(0)

/**
  Removes an entry from a map based on a specific string key.

  @note This macro expects a pointer to the dynamic map.

  @note The 3rd argument should be a signed integer
  value indicating whether or not the key/value entry
  was removed successfully. If 'result' is -1, the key
  was not found, and therefore the entry was not removed.
  If 'result' is 1, the entry was found and removed.
*/
#define dynmaps_remove_strkey_result(dynamic_map, key, result) \
	do { \
		(result) = -1; \
		int dynmaps_entry_idx = -1; \
		dynmaps_find_entry_strkey(dynamic_map, key, dynmaps_entry_idx); \
		if(dynmaps_entry_idx != -1) \
		{ \
			dynmaps_remove_at(dynamic_map, dynmaps_entry_idx); \
			(result) = 1; \
		} \
	} while(0)

/**
  Removes an entry from a map based on a specific string key.

  @note This macro expects a pointer to the dynamic map.
*/
#define dynmaps_remove_strkey(dynamic_map, key) \
	do { \
		int dynmaps_entry_idx = -1; \
		dynmaps_remove_strkey_result(dynamic_map, key, dynmaps_entry_idx); \
	} while(0)

/**
  Clears the given dynamic map. Clearing a map results in its
  size being 0.

  @note This macro expects a pointer to the dynamic map.
*/
#define dynmaps_clear(dynamic_map) \
	do { \
		for(size_t i = 0; i < (dynamic_map) -> size; ++i) \
		{ \
			(dynamic_map) -> keys[i] = 0; \
			(dynamic_map) -> values[i] = 0; \
		} \
		(dynamic_map) -> size = 0; \
	} while(0)
