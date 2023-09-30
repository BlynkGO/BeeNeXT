/**
 * @file mem.h
 *
 */

#ifndef _MEM_H
#define _MEM_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stddef.h>

/*********************
 *      DEFINES
 *********************/
#define MEM_DEBUG         0

#ifndef MEM_BUF_MAX_NUM
#define MEM_BUF_MAX_NUM    16
#endif

#ifndef MEM_ATTR
#define MEM_ATTR
#endif

#ifndef MEM_SIZE
#define MEM_SIZE    (32U * 1024U)
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**
 * Heap information structure.
 */
typedef struct
{
    uint32_t total_size; /**< Total heap size */
    uint32_t free_cnt;
    uint32_t free_size; /**< Size of available memory */
    uint32_t free_biggest_size;
    uint32_t used_cnt;
    uint8_t used_pct; /**< Percentage used */
    uint8_t frag_pct; /**< Amount of fragmentation */
} bgo_mem_monitor_t;

typedef struct {
    void * p;
    uint16_t size;
    uint8_t used    :1;
} bgo_mem_buf_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initiaize the dyn_mem module (work memory and other variables)
 */
void bgo_mem_init(void);

/**
 * Allocate a memory dynamically
 * @param size size of the memory to allocate in bytes
 * @return pointer to the allocated memory
 */
void * bgo_mem_alloc(size_t size);

/**
 * Free an allocated data
 * @param data pointer to an allocated memory
 */
void bgo_mem_free(const void * data);

/**
 * Reallocate a memory with a new size. The old content will be kept.
 * @param data pointer to an allocated memory.
 * Its content will be copied to the new memory block and freed
 * @param new_size the desired new size in byte
 * @return pointer to the new memory
 */
void * bgo_mem_realloc(void * data_p, size_t new_size);

/**
 * Join the adjacent free memory blocks
 */
void bgo_mem_defrag(void);

/**
 * Give information about the work memory of dynamic allocation
 * @param mon_p pointer to a dm_mon_p variable,
 *              the result of the analysis will be stored here
 */
void bgo_mem_monitor(bgo_mem_monitor_t * mon_p);

/**
 * Give the size of an allocated memory
 * @param data pointer to an allocated memory
 * @return the size of data memory in bytes
 */
uint32_t bgo_mem_get_size(const void * data);

/**
 * Get a temporal buffer with the given size.
 * @param size the required size
 */
void * bgo_mem_buf_get(uint32_t size);

/**
 * Release a memory buffer
 * @param p buffer to release
 */
void bgo_mem_buf_release(void * p);

/**
 * Free all memory buffers
 */
void bgo_mem_buf_free_all(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*_MEM_H*/
