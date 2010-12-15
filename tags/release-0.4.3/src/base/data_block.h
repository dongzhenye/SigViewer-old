#ifndef DATA_BLOCK_H
#define DATA_BLOCK_H

#include "sigviewer_user_types.h"

#include <list>
#include <vector>
#include <string>

#include <QSharedPointer>

namespace SigViewer_ {

//-------------------------------------------------------------------------
/// DataBlock
///
/// basic class to store channel data and sections of it
class DataBlock
{
public:
    //-------------------------------------------------------------------------
    DataBlock ();

    //-------------------------------------------------------------------------
    /// @param sample_rate_per_unit as a data_block must not represent data which
    ///                             is associated to time, the sample_rate is given
    ///                             in "per unit" (e.g. "s" or "hz", etc.)
    DataBlock (QSharedPointer<std::vector<float32> > data, float32 sample_rate_per_unit);

    //-------------------------------------------------------------------------
    DataBlock (DataBlock const &src);

    //-------------------------------------------------------------------------
    QSharedPointer<DataBlock> createSubBlock (uint32 start, uint32 length) const;

    //-------------------------------------------------------------------------
    float32 const& operator[] (uint32 index) const;

    //-------------------------------------------------------------------------
    /// length of the block
    uint32 size () const;

    //-------------------------------------------------------------------------
    std::string getLabel () const;

    //-------------------------------------------------------------------------
    void setLabel (std::string const &label);

    //-------------------------------------------------------------------------
    /// sets the x-unit label (e.g. "hz" or "s")
    void setXUnitLabel (std::string const &unit_label);

    //-------------------------------------------------------------------------
    std::string getXUnitLabel () const;

    //-------------------------------------------------------------------------
    /// sets the y-unit label (e.g. "hz" or "s")
    void setYUnitLabel (std::string const &unit_label);

    //-------------------------------------------------------------------------
    std::string getYUnitLabel () const;


    //-------------------------------------------------------------------------
    float32 getSampleRatePerUnit () const;


    //-------------------------------------------------------------------------
    float32 getMin () const;

    //-------------------------------------------------------------------------
    float32 getMax () const;

    //-------------------------------------------------------------------------
    // DataBlock getBandpassFilteredBlock (float32 lower_hz_boundary, float32 upper_hz_boundary) const;

    //-------------------------------------------------------------------------
    QSharedPointer<DataBlock const> createPowerSpectrum () const;

    //-------------------------------------------------------------------------
    static DataBlock calculateMean (std::list<QSharedPointer<DataBlock const> > const &data_blocks);

    //-------------------------------------------------------------------------
    static QSharedPointer<DataBlock> calculateStandardDeviation (std::list<QSharedPointer<DataBlock const> > const &data_blocks);

    //-------------------------------------------------------------------------
    static QSharedPointer<DataBlock> calculateStandardDeviation (std::list<QSharedPointer<DataBlock const> > const &data_blocks,
                                                 DataBlock const &means);


private:

    //-------------------------------------------------------------------------
    static QSharedPointer<DataBlock> calculateStandardDeviationImpl (std::list<QSharedPointer<DataBlock const> > const &data_blocks,
                                                                 DataBlock const &means);

    //-------------------------------------------------------------------------
    // static unsigned getLengthOfSmallestBlock (std::list<DataBlock> const &data_blocks);

    QSharedPointer<std::vector<float32> > data_;
    uint32 length_;
    uint32 start_index_;

    float32 sample_rate_per_unit_;
    std::string label_;
    std::string x_unit_label_;
    std::string y_unit_label_;
};

}

#endif // DATA_BLOCK_H