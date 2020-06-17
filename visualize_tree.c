#include <stdio.h>
#include <stdlib.h>
#include "visualize_tree.h"
#include "gvc.h"

/* Already defined in gvc.h
#define TRUE 1
#define FALSE 0
*/

static int GLOBAL_ID = 1;

// Since I can't make heads or tails of the graphviz C library, I'm going
// to ensure each node is unique by prepending every string with an
// unique ID
int get_ID() {
	return GLOBAL_ID++;
}

// Converts the unique ID to a string
char *get_ID_str() {
	int id = get_ID();
	int length = snprintf(NULL, 0, "%d", id);
	char *str = malloc(length+1);
	snprintf(str, length+1, "%d", id);
	return str;
}

// Creates a node according to the name
Agnode_t *make_node(Agraph_t *g, char *name) {
	char *id = get_ID_str();
	char *str = malloc(strlen(id) + strlen(name) + 3);
	sprintf(str, "%s: %s", id, name);
	Agnode_t *result = agnode(g, str, TRUE);
	free(id);
	free(str);
	return result;
}

// Creates an "edge" between nodes involved with name
// Since I can't figure out how to do any labelling of edges, we're going to
// approximate it with another node inbetween that's named
void make_edge(Agraph_t *g, Agnode_t *parent, Agnode_t *child, char *name) {
	Agnode_t *inbetween = make_node(g, name);
	agedge(g, parent, inbetween, name, TRUE);
	agedge(g, inbetween, child, name, TRUE);
}

// Helper functions - See below for their instantiations
Agnode_t *visualize_decl(Agraph_t *g, struct decl *d);
Agnode_t *visualize_type(Agraph_t *g, struct type *t);
Agnode_t *visualize_param_list(Agraph_t *g, struct param_list *p);
Agnode_t *visualize_stmt(Agraph_t *g, struct stmt *s);
Agnode_t *visualize_expr(Agraph_t *g, struct expr *e);

// Creates a SVG file based on the given abstract syntax tree
// Returns number of errors whilst making graph
int visualize_tree(struct decl *d, char *outputSVG) {
	Agraph_t *g = agopen("g", Agdirected, FALSE);
    
    // Recursively go through AST and add declarations to graph
    visualize_decl(g, d);
	
	// Render graph in SVG using Graphviz
	GVC_t *gvc = gvContext(); 						// Set up a graphviz context
    gvLayout(gvc, g, "dot");						// Layout graph in DOT language
    gvRenderFilename(gvc, g, "svg", outputSVG);		// Render graph to output SVG
    gvFreeLayout(gvc, g);							// Free layout data
    agclose(g);										// Free graph structures

    // Close output file, free context, and return number of errors
    return (gvFreeContext(gvc));
}

// Add declaration to graph and recurse for children declarations
Agnode_t *visualize_decl(Agraph_t *g, struct decl *d) {
	if (d == NULL) return NULL;

	// Add decl node
	char *temp = malloc(strlen(d->symbol->name) + 8);
	sprintf(temp, "DECL \"%s\"", d->symbol->name);
	Agnode_t *decl = make_node(g, temp);
	free(temp);
	
	// Add type node(s) and edge
	Agnode_t *type = visualize_type(g, d->symbol->type);	
	make_edge(g, decl, type, "type");

	// If initialized, add expr node(s) + edge
	if (d->value != NULL) {
		Agnode_t *value = visualize_expr(g, d->value);
		make_edge(g, decl, value, "value");
	}
	
	// If a function, add stmt node(s) + edge
	if (d->symbol->type->kind == TYPE_FUNCTION && d->code != NULL) {
		Agnode_t *code = visualize_stmt(g, d->code);
		make_edge(g, decl, code, "code");
	}
	
	// Recurse
	if (d->next != NULL) {
		Agnode_t *next = visualize_decl(g, d->next);
		make_edge(g, decl, next, "next");
	}

	return decl;
}

// Add type to graph
Agnode_t *visualize_type(Agraph_t *g, struct type *t) {
	if (t == NULL) return NULL;
	
	Agnode_t *type = NULL;
	Agnode_t *subtype, *params;
	switch (t->kind) {
		case TYPE_VOID:
			type = make_node(g, "TYPE_VOID");
			break;
		case TYPE_BOOLEAN:
			type = make_node(g, "TYPE_BOOLEAN");
			break;
		case TYPE_CHAR:
			type = make_node(g, "TYPE_CHAR");
			break;
		case TYPE_INTEGER:
			type = make_node(g, "TYPE_INTEGER");
			break;
		case TYPE_STRING:
			type = make_node(g, "TYPE_STRING");
			break;
		case TYPE_ARRAY:
			type = make_node(g, "TYPE_ARRAY");
			
			// Add type node for array content
			subtype = visualize_type(g, t->subtype);
			make_edge(g, type, subtype, "subtype");
			break;
		case TYPE_FUNCTION:
			type = make_node(g, "TYPE_FUNCTION");
			
			// Add type node for return type
			subtype = visualize_type(g, t->subtype);
			make_edge(g, type, subtype, "subtype");
			
			// Add param list node for args
			if (t->params != NULL) {
				params = visualize_param_list(g, t->params);
				make_edge(g, type, params, "params");
			}
			break;
		default:
			break;
	}

	return type;
}

// Add parameter list to graph
Agnode_t *visualize_param_list(Agraph_t *g, struct param_list *p) {
	if (p == NULL) return NULL;

	// Add param node
	Agnode_t *param = make_node(g, "PARAM");
	
	// Add name + edge
	char *temp = strdup(p->symbol->name);
	Agnode_t *name = make_node(g, temp);					
	free(temp);	
	make_edge(g, param, name, "name");
	
	// Add type + edge
	Agnode_t *type = visualize_type(g, p->symbol->type);
	make_edge(g, param, type, "type");
	
	// Recurse to next param
	if (p->next != NULL) {
		Agnode_t *next = visualize_param_list(g, p->next);
		make_edge(g, param, next, "next");
	}
	
	return param;
}

// Add statement to graph and recurse for children statements
Agnode_t *visualize_stmt(Agraph_t *g, struct stmt *s) {
	if (s == NULL) return NULL;
	
	// Add nodes + edges for each kind of statement
	Agnode_t *stmt = NULL;
	Agnode_t *decl, *init_expr, *expr, *next_expr, *body, *else_body;
	switch (s->kind) {
		case STMT_DECL:
			stmt = make_node(g, "STMT_DECL");
			decl = visualize_decl(g, s->decl);
			make_edge(g, stmt, decl, "decl");
			break;
		case STMT_EXPR:
			stmt = make_node(g, "STMT_EXPR");
			expr = visualize_expr(g, s->expr);
			make_edge(g, stmt, expr, "expr");
			break;
		case STMT_IF_ELSE:
			stmt = make_node(g, "STMT_IF_ELSE");
			expr = visualize_expr(g, s->expr);
			make_edge(g, stmt, expr, "expr");
			if (s->body != NULL) {
				body = visualize_stmt(g, s->body);
				make_edge(g, stmt, body, "body");
			}
			if (s->else_body != NULL) {
				else_body = visualize_stmt(g, s->else_body);
				make_edge(g, stmt, else_body, "else_body");
			}
			break;
		case STMT_FOR:
			stmt = make_node(g, "STMT_FOR");
			if (s->init_expr != NULL) {
				init_expr = visualize_expr(g, s->init_expr);
				make_edge(g, stmt, init_expr, "init_expr");
			}
			if (s->expr != NULL) {
				expr = visualize_expr(g, s->expr);
				make_edge(g, stmt, expr, "expr");
			}
			if (s->next_expr != NULL) {
				next_expr = visualize_expr(g, s->next_expr);
				make_edge(g, stmt, next_expr, "next_expr");
			}
			if (s->body != NULL) {
				body = visualize_stmt(g, s->body);
				make_edge(g, stmt, body, "body");
			}
			break;
		case STMT_PRINT:
			stmt = make_node(g, "STMT_PRINT");
			expr = visualize_expr(g, s->expr);
			make_edge(g, stmt, expr, "expr");
			break;
		case STMT_RETURN:
			stmt = make_node(g, "STMT_RETURN");
			expr = visualize_expr(g, s->expr);
			make_edge(g, stmt, expr, "expr");
			break;
		case STMT_BLOCK:
			stmt = make_node(g, "STMT_BLOCK");
			if (s->body != NULL) {
				body = visualize_stmt(g, s->body);
				make_edge(g, stmt, body, "body");
			}
			break;
		default:
			break;
	}
	
	// Recurse
	if (s->next != NULL) {
		Agnode_t *next = visualize_stmt(g, s->next);
		make_edge(g, stmt, next, "next");
	}
	
	return stmt;
}

// Add expression to graph and recurse for children expressions
Agnode_t *visualize_expr(Agraph_t *g, struct expr *e) {
	if (e == NULL) return NULL;

	// Add node for each kind of expression
	Agnode_t *expr = NULL;
	Agnode_t *name, *integer_value, *string_literal;
	int length;
	char *str, *temp;
	switch (e->kind) {
		case EXPR_IDENTIFIER:
			expr = make_node(g, "EXPR_IDENTIFIER");
			temp = strdup(e->name);
			name = make_node(g, temp);
			free(temp);
			make_edge(g, expr, name, "name");
			break;
		case EXPR_INTEGER_LITERAL:
			expr = make_node(g, "EXPR_INTEGER_LITERAL");
			
			// Convert integer to string
			length = snprintf(NULL, 0, "%d", e->integer_value);
			str = malloc(length+1);
			snprintf(str, length+1, "%d", e->integer_value);
			integer_value = make_node(g, str);
			make_edge(g, expr, integer_value, "integer_value");
			free(str);
			break;
		case EXPR_CHAR_LITERAL:
			expr = make_node(g, "EXPR_CHAR_LITERAL");
			
			// Convert integer to char
			length = snprintf(NULL, 0, "%c", e->integer_value);
			str = malloc(length+1);
			snprintf(str, length+1, "%c", e->integer_value);
			integer_value = make_node(g, str);
			make_edge(g, expr, integer_value, "integer_value");
			free(str);
			break;
		case EXPR_STRING_LITERAL:
			expr = make_node(g, "EXPR_STRING_LITERAL");
			temp = strdup(e->string_literal);
			string_literal = make_node(g, temp);
			free(temp);
			make_edge(g, expr, string_literal, "string_literal");
			break;
		case EXPR_BOOLEAN_LITERAL:
			expr = make_node(g, "EXPR_BOOLEAN_LITERAL");
			if (e->integer_value == 0)
				integer_value = make_node(g, "false");
			else
				integer_value = make_node(g, "true");
			make_edge(g, expr, integer_value, "integer_value");
			break;
		case EXPR_SUBSCRIPT:
			expr = make_node(g, "EXPR_SUBSCRIPT");
			break;
		case EXPR_CALL:
			expr = make_node(g, "EXPR_CALL");
			break;
		case EXPR_ARG:
			expr = make_node(g, "EXPR_ARG");
			break;
		case EXPR_ASSIGN:
			expr = make_node(g, "EXPR_ASSIGN");
			break;
		case EXPR_ADD:
			expr = make_node(g, "EXPR_ADD");
			break;
		case EXPR_SUB:
			expr = make_node(g, "EXPR_SUB");
			break;
		case EXPR_MUL:
			expr = make_node(g, "EXPR_MUL");
			break;
		case EXPR_DIV:
			expr = make_node(g, "EXPR_DIV");
			break;
		case EXPR_MOD:
			expr = make_node(g, "EXPR_MOD");
			break;
		case EXPR_EXP:
			expr = make_node(g, "EXPR_EXP");
			break;
		case EXPR_AND:
			expr = make_node(g, "EXPR_AND");
			break;
		case EXPR_OR:
			expr = make_node(g, "EXPR_OR");
			break;
		case EXPR_LT:
			expr = make_node(g, "EXPR_LT");
			break;
		case EXPR_LT_EQ:
			expr = make_node(g, "EXPR_LT_EQ");
			break;
		case EXPR_GT:
			expr = make_node(g, "EXPR_GT");
			break;
		case EXPR_GT_EQ:
			expr = make_node(g, "EXPR_GT_EQ");
			break;
		case EXPR_IS_EQ:
			expr = make_node(g, "EXPR_IS_EQ");
			break;
		case EXPR_IS_NEQ:
			expr = make_node(g, "EXPR_IS_NEQ");
			break;
		case EXPR_NOT:
			expr = make_node(g, "EXPR_NOT");
			break;
		case EXPR_INC:
			expr = make_node(g, "EXPR_INC");
			break;
		case EXPR_DEC:
			expr = make_node(g, "EXPR_DEC");
			break;
		default:
			break;
	}
	
	// Recurse through the two leafs
	if (e->left != NULL) {
		Agnode_t *left = visualize_expr(g, e->left);
		make_edge(g, expr, left, "left");
	}
	if (e->right != NULL) {
		Agnode_t *right = visualize_expr(g, e->right);
		make_edge(g, expr, right, "right");
	}
	
	return expr;
}
