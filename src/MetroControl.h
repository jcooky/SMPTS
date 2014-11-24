#ifndef __METRO_CONTROL_H_
#define __METRO_CONTROL_H_

typedef struct MetroControl {

    NetworkInterface *metroControlNetworkInterface;
    FileIoInterface *fileIoInterface;
    void (*run)(struct MetroControl* self);


} MetroControl;


MetroControl* newMetroControl();

#endif // __METRO_CONTROL_H_