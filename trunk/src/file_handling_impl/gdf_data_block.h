#ifndef GDF_DATA_BLOCK_H
#define GDF_DATA_BLOCK_H

#include "base/data_block.h"

#include "GDF/Reader.h"

#include <QVector>

namespace SigViewer_
{

//-------------------------------------------------------------------------------------------------
class GDFDataBlock : public DataBlock
{
public:
    //---------------------------------------------------------------------------------------------
    GDFDataBlock (gdf::Reader* reader, ChannelID channel, int downsampling_factor);

    //---------------------------------------------------------------------------------------------
    virtual ~GDFDataBlock ();

    //---------------------------------------------------------------------------------------------
    virtual QSharedPointer<DataBlock> createSubBlock (uint32 start, uint32 length) const;

    //---------------------------------------------------------------------------------------------
    virtual float32 const& operator[] (uint32 index) const;

    //-------------------------------------------------------------------------
    virtual float32 getMin () const;

    //-------------------------------------------------------------------------
    virtual float32 getMax () const;

private:
    //---------------------------------------------------------------------------------------------
    GDFDataBlock (GDFDataBlock const& src, unsigned start_sample, unsigned length);

    //---------------------------------------------------------------------------------------------
    Q_DISABLE_COPY (GDFDataBlock);

    //---------------------------------------------------------------------------------------------
    mutable float32 current_value_;
    gdf::Reader* reader_;
    ChannelID channel_;
    unsigned start_sample_;
    int downsampling_factor_;
    mutable float32 current_min_;
    mutable float32 current_max_;
};

} // namespace SigViewer_

#endif // GDF_DATA_BLOCK_H
