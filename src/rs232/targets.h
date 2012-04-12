#ifndef _TARGETS_H
#define _TARGETS_H

#include <stdint.h>

/** A target */
struct target {
	/** X position */
	uint16_t x;
	/** Y position */
	uint16_t y;
};

/** Number of targets in list */
#define NUM_TARGETS 32

extern uint8_t target_prod;
extern uint8_t target_cons;
extern struct target targets[NUM_TARGETS];


static inline int targets_full ( void ) {
	return ( ( target_cons % NUM_TARGETS == target_prod % NUM_TARGETS ) && ( target_cons != target_prod ) );
}

static inline int targets_empty ( void ) {
	return ( target_cons == target_prod );
}


#endif /* _TARGETS_H */
