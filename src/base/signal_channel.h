#ifndef SIGNAL_CHANNEL_H
#define SIGNAL_CHANNEL_H

#include "sigviewer_user_types.h"

#include <QString>
#include <QMutex>

namespace SigViewer_
{

//-----------------------------------------------------------------------------
/// @class SignalChannel
/// @brief data about a signal channel
class SignalChannel
{
public:
    //-------------------------------------------------------------------------
    SignalChannel (unsigned number,
                   QString const& label,
                   QString const& phys_y_dimension_label = "");

    //-------------------------------------------------------------------------
    QString typeString() const;
    uint32 getNumber() const;
    const QString& getLabel() const;
    float64 getLowpass() const;
    float64 getHighpass() const;
    bool getNotch() const;
    const QString& getPhysicalDim() const;
    float64 getPhysicalMaximum() const;
    float64 getDigitalMaximum() const;
    float64 getPhysicalMinimum() const;
    float64 getDigitalMinimum() const;

private:
    // from GDF format
    enum Type
    {
        CHAR    = 0,
        INT8    = 1,
        UINT8   = 2,
        INT16   = 3,
        UINT16  = 4,
        INT32   = 5,
        UINT32  = 6,
        INT64   = 7,
        UINT64   = 8,
        FLOAT32 = 16,
        FLOAT64 = 17,
        FLOAT128 = 18,

        // add N to code
        BITN    = 255,
        UBITN   = 511
    };

    mutable QMutex mutex_;
    
    uint32 number_;
    QString label_;
    QString phys_y_dimension_label_;
    uint16  physical_dimcode_;
    float64 physical_maximum_;
    float64 digital_maximum_;
    float64 physical_minimum_;
    float64 digital_minimum_;
    uint16_t data_type_;
    float64 lowpass_;
    float64 highpass_;
    bool notch_;
};

} // namespace SigViewer_

#endif
