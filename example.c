#include "vibrant_logs.h"
#include "plutonium_cs.h"

#define COMPONENT_BUTTON 1

typedef struct button
{
	float x, y;
	bool click;
	bool press;
} button;

void button_init(void *component, void *owner)
{
	button *button = component;

	button -> x = button -> y = 0;
	button -> click = button -> press = 0;
}

typedef struct entity
{
	const char *ID;
} entity;

/*
   IDEA:

   maybe we can have a util function or even switch to a core function
   that instead of takes the data straight from the user, a file path is provided.
   the file path points to a .h file where the user defines the component they wish
   to register and use in the program.

   like this:

		pluto_cs_register("../include/components/button.h");

	then pluto_cs_register(...) reads that .h file line by line, parsing
	the file and generating the data from that?

	you call the file 'button.h' and so now pluto_cs_register(...) expects
	the file to contain this template:

	'
	typedef struct button
	{
		button_data....
	} button;

	void button_init(void *component, void *owner) { ... }
	'

	Since every component requires that template, we can just take the file name
	and place it in the expected places in the file. (ex. struct 'button', void 'button'_init(...)).

	For a file called transform.h, it would look like this instead:

	'
	typedef struct transform
	{
	} transform;

	void transform_init(void *component, void *owner) { ... }
	'

	this now results in every component residing in its own file, and PlutoniumCS automates
	everything else.

	Just an idea.
*/

int main(void)
{
	vl_init();

	if(!pluto_cs_init())
	{
		return 1;
	}

	if(!pluto_cs_register(COMPONENT_BUTTON, sizeof(button), button_init))
	{
		pluto_cs_shutdown();
		return 1;
	}


	entity my_object = { "entity0" };

	button *button = pluto_cs_get_component(&my_object, COMPONENT_BUTTON);
	if(button)
		vl_log(VL_DEBUG, "button added successfully!\n");
	else
		vl_log(VL_ERROR, "Button component not found on entity0!\n");

	pluto_cs_shutdown();

	return 0;
}
