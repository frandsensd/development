#ifndef RECORD_SORT_H_
#define RECORD_SORT_H_

/*
 * record type to sort on
 */
typedef struct {
	char *name;
	unsigned int ID;
} record_t;
/*
 * Function prototypes
 */
int read_file (char *filename, int *size, record_t *records[]);
int write_sorted (int size, record_t records[]);
void return_records (int size, record_t records[]);

void sort_name (int size, record_t records[]);
void sort_ID (int size, record_t records[]);
#endif /*RECORD_SORT_H_*/
