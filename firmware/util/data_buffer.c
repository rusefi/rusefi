/*@
 * @file	data_buffer.c
 *
 * @date Dec 8, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "data_buffer.h"
#include "main.h"
#if EFI_PROD_CODE
//#include "eficonsole.h"
#endif /* EFI_PROD_CODE */

int dbIsFull(data_buffer_s *db) {
	return db->size == DB_MAX_SIZE;
}

void dbClear(data_buffer_s *db) {
	db->size = 0;
}

void dbCopy(data_buffer_s *source, data_buffer_s *target) {
	int s = source->size;
	target->size = s;
	for (int i = 0; i < s; i++)
		target->elements[i] = source->elements[i];
}

void dbAdd(data_buffer_s *db, int value) {
	if (db->size == DB_MAX_SIZE)
		return;
	int s = db->size;
	db->elements[s] = value;
	db->size = s + 1;
}

int modp(int param) {
	return param > EF_PERIOD ? param - EF_PERIOD : param;
}

//void dbPrint(data_buffer_s *db, char *message, int withDiff) {
//	int s = db->size;
//	print("buffer [%s] size=%d\r\n", message, s);
//	int range = db->elements[s - 1] - db->elements[0];
//	print("range %d\r\n", range);
//
//	for (int i = 0; i < s; i++) {
//		print("%d: %d", i, db->elements[i]);
//		if (withDiff && i > 0) {
//			int diff = modp(db->elements[i]) - modp(db->elements[i - 1]);
//			print(" t=%d", diff);
//		}
//		print("\r\n");
//	}
//}

//void dbPrintTable(data_buffer_s *table[], char *caption[], int columns) {
//	for (int c = 0; c < columns; c++)
//		print("%7s", caption[c]);
//	print("\r\n");
//
//	for (int r = 0; r < DB_MAX_SIZE; r++) {
//		for (int c = 0; c < columns; c++) {
//			data_buffer_s *buf = table[c];
//			print("%7d", buf->elements[r]);
//		}
//		print("\r\n");
//	}
//}

