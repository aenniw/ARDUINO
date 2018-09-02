#ifndef ARDUINO_PROJECTS_ROOT_SERVICE_H
#define ARDUINO_PROJECTS_ROOT_SERVICE_H

typedef enum
{
    UNCALIBRATED,   // nothing calibrated
    SEMICALIBRATED, // bottom calibrated
    CALIBRATED      // bottom-top calibrated
} Calibration;

typedef struct
{
    Calibration calibration;
    unsigned int position, preset_1, preset_2, preset_3, end_stop;
} Table_Data;

class Service
{
  public:
    virtual void cycle() = 0;
};

#endif //ARDUINO_PROJECTS_ROOT_SERVICE_H
