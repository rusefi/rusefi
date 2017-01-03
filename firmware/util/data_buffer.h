/*
 * data_buffer.h
 *
 * @date Dec 8, 2012
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef DATA_BUFFER_H_
#define DATA_BUFFER_H_

#define DB_MAX_SIZE 1024

#define EF_PERIOD 100000000

typedef struct {
	int elements[DB_MAX_SIZE];
	int size;
} data_buffer_s;

int dbIsFull(data_buffer_s *db);
void dbClear(data_buffer_s *db);
void dbAdd(data_buffer_s *db, int value);
void dbCopy(data_buffer_s *source, data_buffer_s *target);
void dbPrint(data_buffer_s *db, char *message, int withDiff);
void dbPrintTable(data_buffer_s *table[], char *caption[], int columns);

#endif /* DATA_BUFFER_H_ */
