#ifndef BLOCKS_VISUALISATION_MODEL_H
#define BLOCKS_VISUALISATION_MODEL_H

#include "../base/data_block.h"
#include "../abstract_browser_model.h"

#include <QSharedPointer>

#include <list>

namespace BioSig_ {

class BlocksVisualisationView;
class BlockVisualisationItemModel;

//-----------------------------------------------------------------------------
class BlocksVisualisationModel : public AbstractBrowserModel
{
public:
    BlocksVisualisationModel (BlocksVisualisationView* view,
                              float64 pixel_per_sec,
                              float64 sample_rate);

    virtual ~BlocksVisualisationModel ();

    virtual void setPixelPerXUnit (float64 pixel_per_x_unit);
    virtual float64 getPixelPerXUnit() const;

    virtual void setItemsHeight (int32 block_height);
    virtual void zoom (ZoomDimension, float) {}

    virtual std::set<ChannelID> getShownChannels () const {std::set<ChannelID> blub; return blub;}
    virtual bool setShownChannels (std::set<ChannelID> const&) {return true;}

    virtual void updateLayout ();

    //-------------------------------------------------------------------------
    /// @return the height of the signal viewport
    virtual unsigned getShownHeight () const {return 0;}


    //-------------------------------------------------------------------------
    /// see base class
    virtual unsigned getShownSignalWidth () const {return 0;}

    //-------------------------------------------------------------------------
    /// see base class
    virtual unsigned getShownPosition () const {return 0;}

    //-------------------------------------------------------------------------
    /// see base class
    virtual void goToSample (unsigned) {}

    void visualiseBlock (QSharedPointer<DataBlock> data_block,
                         QSharedPointer<DataBlock> deviation);

    unsigned getBlockHeight () const;

protected:
    virtual void modeChanged (SignalVisualisationMode) {}

private:
    BlocksVisualisationView* view_;
    float64 pixel_per_sec_;
    float64 sample_rate_;
    unsigned num_block_items_;
    unsigned block_height_;
    unsigned max_block_length_;

    std::list<BlockVisualisationItemModel*> items_;
};

}

#endif // BLOCKS_VISUALISATION_MODEL_H
