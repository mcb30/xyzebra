#ifndef DLPUSB245_H
#define DLPUSB245_H

#include <stdint.h>
#include <stddef.h>

/** A DLPUSB245 device */
struct dlpusb245_device {
	/** libusb context */
	struct libusb_context *context;
	/** libusb device handle */
	struct libusb_device_handle *device;
};

extern int open_device ( struct dlpusb245_device *device );
extern void close_device ( struct dlpusb245_device *device );
extern int set_output_mask ( struct dlpusb245_device *device,
			     uint8_t output_mode, int bitmask );
extern int set_output_value ( struct dlpusb245_device *device,
			      uint8_t output_value );
extern int read_input_value ( struct dlpusb245_device *device,
			      uint8_t *input_value );
extern int write_fifo ( struct dlpusb245_device *device, uint8_t *data,
			size_t len );

#endif
