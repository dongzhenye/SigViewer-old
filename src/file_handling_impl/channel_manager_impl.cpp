#include "channel_manager_impl.h"

#include <cmath>

namespace BioSig_
{

//-----------------------------------------------------------------------------
ChannelManagerImpl::ChannelManagerImpl (QSharedPointer<FileSignalReader> file_signal_reader)
    : reader_ (file_signal_reader)
{

}

//-------------------------------------------------------------------------
std::set<ChannelID> ChannelManagerImpl::getChannels () const
{
    std::set<ChannelID> channels;
    for (ChannelID id = 0; id < reader_->getBasicHeader()->getNumberChannels(); ++id)
        channels.insert(id);
    return channels;
}

//-----------------------------------------------------------------------------
uint32 ChannelManagerImpl::getNumberChannels () const
{
    return reader_->getBasicHeader()->getNumberChannels();
}

//-------------------------------------------------------------------------
QString ChannelManagerImpl::getChannelLabel (ChannelID id) const
{
    return reader_->getBasicHeader()->getChannel (id).getLabel();
}


//-----------------------------------------------------------------------------
QSharedPointer<DataBlock const> ChannelManagerImpl::getData (ChannelID id, unsigned start_pos, unsigned length) const
{
    return reader_->getSignalData(id, start_pos, length);
}

//-----------------------------------------------------------------------------
float32 ChannelManagerImpl::getDurationInSec () const
{
    return reader_->getBasicHeader()->getNumberOfSamples() /
           reader_->getBasicHeader()->getSampleRate();
}


//-----------------------------------------------------------------------------
uint32 ChannelManagerImpl::getNumberSamples () const
{
    return reader_->getBasicHeader()->getNumberOfSamples();
}

//-----------------------------------------------------------------------------
float32 ChannelManagerImpl::getSampleRate () const
{
    return reader_->getBasicHeader()->getSampleRate();
}

//-----------------------------------------------------------------------------
float64 ChannelManagerImpl::getMinValue (ChannelID channel_id) const
{
    QSharedPointer<DataBlock const> channel_data = reader_->getSignalData (channel_id, 0,
                                                    reader_->getBasicHeader()->getNumberOfSamples());

    return channel_data->getMin();
}

//-----------------------------------------------------------------------------
float64 ChannelManagerImpl::getMaxValue (ChannelID channel_id) const
{
    QSharedPointer<DataBlock const> channel_data = reader_->getSignalData (channel_id, 0,
                                                    reader_->getBasicHeader()->getNumberOfSamples());

    return channel_data->getMax();
}



}