#pragma once

#include <stdlib.h>
#include <string.h>

/**
  Dynamic array holding integers.
*/
typedef struct dynas_int_arr
{
	int *data;
	size_t size;
	size_t capacity;
} dynas_int_arr;
/**
  Dynamic array holding floats.
*/
typedef struct dynas_float_arr
{
	float *data;
	size_t size;
	size_t capacity;
} dynas_float_arr;
/**
  Dynamic array holding doubles.
*/
typedef struct dynas_double_arr
{
	double *data;
	size_t size;
	size_t capacity;
} dynas_double_arr;
/**
  Dynamic array holding characters.

  This dynamic array is special,
  as it can represent a string.
*/
typedef struct dynas_char_arr
{
	char *data;
	size_t size;
	size_t capacity;
} dynas_char_arr;
/**
  Dynamic array holding strings.
*/
typedef struct dynas_string_arr
{
	char **data;
	size_t size;
	size_t capacity;
} dynas_string_arr;

/**
  Initializes the given dynamic array.

  @note This macro expects a pointer to the dynamic array.

  The given dynamic array must have at least
  3 fields:

  - A pointer representing the array, called 'data.'
  - A size_t called 'size.' This is the array's current size.
  - A size_t called 'capacity.' This is the max size of the array.

  When a dynamic array is initialized, its size is 0, its capacity
  starts out at 10, and its data is automatically allocated for you.

  @important Make sure you call dynas_free(&dynamic_array) when you are
  ready to free the dynamic array's memory.
*/
#define dynas_init(dynamic_array) \
	do { \
		(dynamic_array) -> size = 0; \
		(dynamic_array) -> capacity = 10; \
		(dynamic_array) -> data = calloc((dynamic_array) -> capacity * sizeof(*(dynamic_array) -> data), sizeof(*(dynamic_array) -> data)); \
	} while(0)

/**
  Frees a dynamic array's data.

  @note This macro expects a pointer to the dynamic array.
*/
#define dynas_free(dynamic_array) \
	do { \
		(dynamic_array) -> size = 0; \
		(dynamic_array) -> capacity = 10; \
		free((dynamic_array) -> data); \
		(dynamic_array) -> data = NULL; \
	} while(0)

/**
  Obtains an item at a given index in a dynamic array.

  @note This macro expects a pointer to the dynamic array.

  Make sure the index is valid before
  calling this macro, as it does not validate the index.
*/
#define dynas_get(dynamic_array, idx) *((dynamic_array) -> data + (idx))

/**
  This macro determines if the given dynamic array needs
  to expand, and it will automatically expand the
  array if necessary.

  @note This macro expects a pointer to the dynamic array.
*/
#define dynas_expand(dynamic_array) \
	do { \
		if((dynamic_array) -> size >= (dynamic_array) -> capacity) \
		{ \
			if((dynamic_array) -> capacity * 2 <= SIZE_MAX) \
			{ \
				(dynamic_array) -> capacity *= 2; \
				(dynamic_array) -> data = realloc((dynamic_array) -> data, (dynamic_array) -> capacity * sizeof(*(dynamic_array) -> data)); \
			} \
		} \
	} while(0)

/**
  Adds an item to a dynamic array.

  @note This macro expects a pointer to the dynamic array.

  @note The dynamic array will auto-expand if necessary.
*/
#define dynas_add(dynamic_array, item) \
	do { \
		dynas_expand(dynamic_array); \
		(dynamic_array) -> data[(dynamic_array) -> size] = (item); \
		(dynamic_array) -> size++; \
	} while(0)

/**
  Adds multiple items to a dynamic array.

  @note This macro expects a pointer to the dynamic array.

  @note The 2nd argument must be the type of elements
  the array stores. For example, for an integer
  array, the arguments would look like this:
  (int_arr, int, 1, 2, 3, 4, ...).
*/
#define dynas_add_n(dynamic_array, ...) \
	do { \
		__typeof__(*((dynamic_array) -> data)) dynas_args[] = { __VA_ARGS__ }; \
		int dynas_args_len = sizeof dynas_args / sizeof(__typeof__(*((dynamic_array) -> data))); \
		for(size_t i = 0; i < dynas_args_len; ++i) \
			dynas_add(dynamic_array, dynas_args[i]); \
	} while(0)

/**
  Quickly adds multiple chars to a char array by using
  strings instead of individual chars.

  If you were to add multiple chars to a char array,
  it would look like this:
  dynas_add_n(&char_arr, 'H', 'e', 'l', 'l', 'o', '!');

  This macro makes it easier to add multiple chars
  to a char array by taking in a string instead.
  The macro then adds each char in the input string
  automatically. This means you can instead do:
  dynas_add_chars(&char_arr, "Hello!");

  @note This macro expects a pointer to the dynamic array.
*/
#define dynas_add_chars(dynamic_array, str) \
	do { \
		for(size_t i = 0; i < strlen((str)); ++i) \
		{ \
			dynas_add(dynamic_array, *((str) + i)); \
		} \
	} while(0)

/**
  Inserts an item into a dynamic array at a given index.
  After insertion, the item will be at the exact index given.

  @note This macro expects a pointer to the dynamic array.

  @note You can use this macro to append a value
  to the array if the given index is equal
  to the size of the array. However, if the
  index is invalid (greater than the size
  of the array), then this macro will
  result in undefined behavior.

  @important The order of arguments should be:
  dynamic_array, index, item.
*/
#define dynas_insert(dynamic_array, idx, item) \
	do { \
		dynas_expand(dynamic_array); \
		memmove((dynamic_array) -> data + (idx) + 1, (dynamic_array) -> data + (idx), ((dynamic_array) -> size - (idx)) * sizeof(*(dynamic_array) -> data)); \
		(dynamic_array) -> data[(idx)] = (item); \
		(dynamic_array) -> size++; \
	} while(0)

/**
  Sets a specific item at a specific index in a dynamic array.

  @note This macro expects a pointer to the dynamic array.

  @note This macro does not validate the size of the array
  or the index given, so make sure the index is
  valid for the given array, otherwise undefined
  behavior may occur.

  @important The order of arguments should be:
  dynamic_array, index, item.
*/
#define dynas_set(dynamic_array, idx, item) \
	do { \
		(dynamic_array) -> data[(idx)] = (item); \
	} while(0)

/**
  Replaces a specific item at a specific index in a dynamic array.

  @note This macro expects a pointer to the dynamic array.

  @note This macro does not validate the size of the
  array or the index given, so make sure the index
  is valid for the given array, otherwise
  undefined behavior may occur.
*/
#define dynas_replace_at(dynamic_array, idx, item) \
	do { \
		dynas_remove_at(dynamic_array, idx); \
		dynas_insert(dynamic_array, idx, item); \
	} while(0)

/**
  Replaces a specific item in a dynamic array
  based on equality.

  @note This macro expects a pointer to the dynamic array.

  @note This macro will search for a matching value in the
  array, and will replace the item with the replace value
  if it's found. Due to this, this macro will only ever
  replace the first occurrence of the given value. The 3rd
  argument should be a signed integer value indicating whether
  or not the item was found in the array and replaced.
  If 'result' is -1, the item was not found, and therefore
  not replaced. If 'result' is not -1, it will be equal
  to the index the item was found at.
*/
#define dynas_replace_item_result(dynamic_array, target_item, new_item, result) \
	do { \
		int dynas_item_idx = -1; \
		dynas_find_item(dynamic_array, target_item, dynas_item_idx); \
		if(dynas_item_idx != -1) \
		{ \
			dynas_replace_at(dynamic_array, dynas_item_idx, new_item); \
		} \
		(result) = dynas_item_idx; \
	} while(0)

/**
  Replaces a specific item in a dynamic array
  based on equality.

  @note This macro expects a pointer to the dynamic array.

  @note This macro will search for a matching value in the
  array, and will replace the item with the replace value
  if it's found. Due to this, this macro will only ever
  replace the first occurrence of the given value.
*/
#define dynas_replace_item(dynamic_array, target_item, new_item) \
	do { \
		int dynas_item_idx = -1; \
		dynas_replace_item_result(dynamic_array, target_item, new_item, dynas_item_idx); \
	} while(0)

/**
  Removes an item from a dynamic array at a given index.

  @note This macro expects a pointer to the dynamic array.

  @note This macro does not validate the size of the
  array or the index given, so make sure the
  index is valid for the given array, otherwise
  undefined behavior may occur.
*/
#define dynas_remove_at(dynamic_array, idx) \
	do { \
		memmove((dynamic_array) -> data + (idx), (dynamic_array) -> data + (idx) + 1, ((dynamic_array) -> size - (idx) - 1) * sizeof(*(dynamic_array) -> data)); \
		(dynamic_array) -> size--; \
	} while(0)

/**
  Removes an item from a dynamic array based
  on equality.

  @note This macro expects a pointer to the dynamic array.

  @note This macro will search for a matching
  value in the array, and will remove the item
  if it's found. Due to this, this macro
  will only ever remove the first occurrence
  of the given value. The 3rd argument
  should be a signed integer value indicating whether
  or not the item was found in the array and removed.
  If 'result' is -1, the item was not found, and therefore
  not removed. If 'result' is not -1, it will be equal
  to the index the item was found at before it was removed.
*/
#define dynas_remove_item_result(dynamic_array, item, result) \
	do { \
		int dynas_item_idx = -1; \
		dynas_find_item(dynamic_array, item, dynas_item_idx); \
		if(dynas_item_idx != -1) \
		{ \
			dynas_remove_at(dynamic_array, dynas_item_idx); \
		} \
		(result) = dynas_item_idx; \
	} while(0)
/**
  Removes an item from a dynamic array based
  on equality.

  @note This macro expects a pointer to the dynamic array.

  @note This macro will search for a matching
  value in the array, and will remove the item
  if it's found. Due to this, this macro
  will only ever remove the first occurrence
  of the given value. To see whether or not the
  item was removed, use dynas_remove_item_result(...).
*/
#define dynas_remove_item(dynamic_array, item) \
	do { \
		int dynas_item_dx = -1; \
		dynas_remove_item_result(dynamic_array, item, dynas_item_idx); \
	} while(0)

/**
  Determines if the given array contains a specific item.

  @note This macro expects a pointer to the dynamic array.

  @note The 3rd argument should be a signed integer value
  indicating whether or not the item was found in the array.
  If 'result' is -1, the item was not found. If 'result'
  is not -1, it is equal to the index the item was found at.
*/
#define dynas_find_item(dynamic_array, item, result) \
	do { \
		(result) = -1; \
		for(size_t i = 0; i < (dynamic_array) -> size; ++i) \
		{ \
			if((dynamic_array) -> data[i] == (item)) \
			{ \
				(result) = i; \
				break; \
			} \
		} \
	} while(0)

/**
  Clears the given dynamic array. Clearing the array
  results in its size being 0.

  @note This macro expects a pointer to the dynamic array.
*/
#define dynas_clear(dynamic_array) \
	do { \
		for(size_t i = 0; i < (dynamic_array) -> size; ++i) \
		{ \
			(dynamic_array) -> data[i] = 0; \
		} \
		(dynamic_array) -> size = 0; \
	} while(0)

/**
  Creates a new dynamic array based on a sub-region of
  another dynamic array.

  @note This macro expects a pointer to the target dynamic
  array, as well as the resulting dynamic array.

  @note 'start_idx' is inclusive, but 'end_idx' is exclusive.
  Additionally, 'start_idx' should always be less than 'end_idx' or
  undefined behavior may occur. To copy the full target array
  into the resulting array, 'start_idx' should be 0, and 'end_idx'
  should be the size of 'target_dynamic_array.'

  @note This macro does not validate the size of the
  array or the index given, so make sure the index
  is valid for the given array, otherwise undefined
  behavior may occur.

  @note This macro expects the resulting dynamic array to be
  initialized but empty.
*/
#define dynas_get_sub_arr(target_dynamic_array, start_idx, end_idx, result_dynamic_array) \
	do { \
		size_t dynas_ptr_range = (end_idx) - (start_idx); \
		size_t dynas_ptr_range_bytes = dynas_ptr_range * sizeof(*((target_dynamic_array) -> data)); \
		memmove((result_dynamic_array) -> data, (target_dynamic_array) -> data + (start_idx), dynas_ptr_range_bytes); \
		(result_dynamic_array) -> size += dynas_ptr_range; \
	} while(0)
