/**
 * Debug macros based on http://c.learncodethehardway.org/book/ex20.html
 * Created on: Jun 24, 2016
 * 
 * @author Tepex <tepex@mail.ru>
 */

#ifndef __DBG_H__
#define __DBG_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#ifndef NO_DEBUG
	#define NO_DEBUG true
#endif

/* Extended debug information for L-function */
#ifndef DEBUG_L
	#define DEBUG_L false
#endif

/* Why the do-while loop? See: http://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing/1644898#1644898 */
#define DEBUG(M, ...) \
	do { if(!NO_DEBUG) fprintf(stderr, "[DEBUG] %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while (0)

#define LOG(T, M, ...) \
	do { fprintf(stderr, "["T"] (function %s: %s:%d: errno: %s) " M "\n", __func__, __FILE__, __LINE__, errno == 0 ? "None" : strerror(errno), ##__VA_ARGS__); errno = 0; } while(0)

#define LOG_ERR(M, ...) LOG("ERROR", M, ##__VA_ARGS__) 
#define LOG_WARN(M, ...) LOG("WARN", M, ##__VA_ARGS__)
#define LOG_INFO(M, ...) LOG("INFO", M, ##__VA_ARGS__)

#define TRY(A, M, ...) do { if(!(A)) {LOG_ERR(M, ##__VA_ARGS__); goto finally;} } while(0)
#define TRY_MEM(A) TRY(A, "Out of memory.")
#define TRY_DEBUG(A, M, ...) do { if(!(A)) {DEBUG(M, ##__VA_ARGS__); goto finally;} } while(0)
#define THROW(M, ...) {LOG_ERR(M, ##__VA_ARGS__); goto finally;}

#define CHECK(A, M, ...) do { if(!(A)) {LOG_ERR(M, ##__VA_ARGS__); return -1;} } while(0)
#define CHECK_MEM(A) CHECK(A, "Out of memory.")

#endif /* __DBG_H__ */
