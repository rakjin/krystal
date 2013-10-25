%option noyywrap

%{

/* symbol table */

enum {
	LOOKUP = 0,
	ADD_VERB,
	ADD_NOUN
};

enum {
	WORD_TYPE_UNKNOWN,
	WORD_TYPE_VERB,
	WORD_TYPE_NOUN
};

int state;
int add_word(int type, char* word);
int lookup_word(char* word);

%}







%%

\n		{ state = LOOKUP; }
^verb	{ state = ADD_VERB; }
^noun	{ state = ADD_NOUN; }

[a-zA-Z]+ {
	if (state != LOOKUP) {
		add_word(state, yytext);
	} else {
		switch (lookup_word(yytext)) {
			case WORD_TYPE_VERB: printf("%s: verb\n", yytext); break;
			case WORD_TYPE_NOUN: printf("%s: noun\n", yytext); break;
			default: printf("%s: don't know\n", yytext); break;
		}
	}
}





%%

main ()
{
	yylex();
}

struct word {
	char* word_name;
	int word_type;
	struct word* next;
};

struct word* word_list;
extern void *malloc();

int add_word(int type, char* word)
{
	struct word* wp;
	if (lookup_word(word) != WORD_TYPE_UNKNOWN) {
		printf("!!! word %s already defined.\n", word);
		return 0;
	}

	wp = (struct word*) malloc(sizeof(struct word));
	wp->next = word_list;

	wp->word_name = (char*) malloc(strlen(word)+1);
	strcpy(wp->word_name, word);

	/* wp->word_type = type; */
	switch (type) {
		case ADD_VERB:
		wp->word_type = WORD_TYPE_VERB; break;
		case ADD_NOUN:
		wp->word_type = WORD_TYPE_NOUN; break;
		default:
		wp->word_type = WORD_TYPE_UNKNOWN; break;
	}

	word_list = wp;
	return 1;
}

int lookup_word(char* word)
{
	struct word* wp = word_list;
	for(; wp; wp = wp->next) {
		if (strcmp(wp->word_name, word) == 0) {
			return wp->word_type;
		}
	}
	return WORD_TYPE_UNKNOWN;
}


