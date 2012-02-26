/**
 * @file
 *
 * Target list
 *
 */

#include "targets.h"

/** Target list */
struct target targets[NUM_TARGETS];

/** Target producer counter */
uint8_t target_prod;

/** Target consumer counter */
uint8_t target_cons;
