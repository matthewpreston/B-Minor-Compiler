#ifndef TYPE_H
#define TYPE_H

typedef union {
	struct type *type;
	struct param_list *param_list;
	struct decl *decl;
	struct stmt *stmt;
	struct expr *expr;
	int ival;
	char cval;
	const char *sval;
} type_;
#define YYSTYPE type_

#endif /* TYPE_H */

