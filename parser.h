#ifndef parser_h
#define parser_h

typedef struct line_data { // (unsigned long long int: 18.446.744.073.709.551.615) (unsigned long int: 4.294.967.295)
	char event_time[32];
	char event_type[32];
	unsigned long int product_id;
	unsigned long long int category_id; // exemplo: 2.053.013.555.631.882.655
	char category_code[64];
	char brand[64];
	unsigned long long int price; // preço guardado em centavos
	unsigned long int user_id;
	char user_session[37];
} LineData;

typedef struct line_product {
	unsigned long int product_id;
	unsigned long long int indice;
	unsigned long long int category_id;
	char category_code[64];
	char brand[64];
	unsigned long long int price;
	int removed;
	char mandatory_newline;
} LineProduct;

typedef struct line_access {
	long long int event_timestamp;
	unsigned long long int indice;
	unsigned long int user_id;
	char event_type[32];
	unsigned long int product_id;
	char user_session[37];
	int removed;
	char mandatory_newline;
} LineAccess;

LineData* read_line(char string[]);

void print_data(LineData *data);
void testar_linhas_produto();
unsigned long long int write_data_to_binary_files(char filename[], int count);
#endif

