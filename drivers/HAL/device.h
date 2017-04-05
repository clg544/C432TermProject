#ifndef __DEVICE__
#define __DEVICE__

enum device_type {SERIAL, LED};

struct device {
    enum device_type type;

    /* Write a byte to the device.
     * Returns -1 on faulure.
     */
    int (*write)(char byte);

    /* Read a byte to the device */
    char (*read)();

    /* Open the device, mark it as in use.
     * Returns -1 if device already open, 0 on success.
     */
    int (*open)();

    /* Close the device, mark it as available
     * Returns -1 if device fails to close, 0 on success.
     */
    int (*close)();

    /* If multiple devices of the same type exist */
    int instance;
};

/* Initialize all peripherals and create device struct
 * instances with the appropriate function pointers set.
 */
void enumerate_devices();

/* returns the device instance
 * corresponding to the device name.
 */
struct device *get_device(int dev);


#endif
