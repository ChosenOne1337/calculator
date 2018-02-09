#ifndef IDENTIFIERS_H
#define IDENTIFIERS_H

int is_var_declaration(char *expr);
char *extract_var(char *expr);
void add_var(char *expr);
char *get_var_value(char *varName);
void print_var_list();
void destroy_var_list();

#endif // IDENTIFIERS_H
