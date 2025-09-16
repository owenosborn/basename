#include "m_pd.h"
#include <string.h>
#include <stdlib.h>

static t_class *basename_class;

typedef struct _basename {
    t_object x_obj;
    t_outlet *x_outlet;
} t_basename;

void basename_symbol(t_basename *x, t_symbol *s)
{
    // Convert symbol to C string
    char *input_string = s->s_name;
    
    // Allocate memory for output string (for now, same size as input)
    int input_length = strlen(input_string);
    char *output_string = (char *)malloc(input_length + 1);
    
    if (output_string == NULL) {
        pd_error(x, "basename: memory allocation failed");
        return;
    }
    
    // Find the last occurrence of '/' to get the filename
    char *filename = strrchr(input_string, '/');
    
    if (filename != NULL) {
        filename++; // Move past the '/'
    } else {
        // No '/' found, use the entire string
        filename = input_string;
    }
    
    // Check if filename exists
    int filename_length = strlen(filename);
    if (filename_length > 0) {
        // Keep the entire filename (including first 2 characters)
        strcpy(output_string, filename);
    } else {
        // If filename is empty, output empty string
        output_string[0] = '\0';
    }
    
    // Convert back to symbol and output
    t_symbol *output_symbol = gensym(output_string);
    outlet_symbol(x->x_outlet, output_symbol);
    
    // Clean up
    free(output_string);
}

void *basename_new(void)
{
    t_basename *x = (t_basename *)pd_new(basename_class);
    
    // Create outlet for symbol output
    x->x_outlet = outlet_new(&x->x_obj, &s_symbol);
    
    return (void *)x;
}

void basename_setup(void)
{
    basename_class = class_new(gensym("basename"),
        (t_newmethod)basename_new,
        0, sizeof(t_basename),
        CLASS_DEFAULT, 0);
    
    // Set up symbol method for inlet
    class_addsymbol(basename_class, basename_symbol);
}
