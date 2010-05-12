/*

    $Id: biosig_reader.cpp,v 1.36 2009/03/03 11:57:07 cle1109 Exp $
    Copyright (C) Thomas Brunner  2005,2006,2007
    Copyright (C) Christoph Eibel 2007,2008
    Copyright (C) Clemens Brunner 2006,2007,2008
    Copyright (C) Alois Schloegl  2008,2009
    Copyright (C) Oliver Terbu    2008
    This file is part of the "SigViewer" repository
    at http://biosig.sf.net/

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "biosig_reader.h"
#include "biosig_basic_header.h"
#include "../file_handling/file_signal_reader_factory.h"

#include "../../extern/biosig.h"

#include <QTextStream>
#include <QTranslator>
#include <QMutexLocker>
#include <QDebug>
#include <QTime>
#include <QProgressDialog>
#include <QMessageBox>

#include <cmath>
#include <cassert>

using namespace std;

namespace BioSig_
{

BioSigReader BioSigReader::prototype_instance_ (true);

//-----------------------------------------------------------------------------
BioSigReader::BioSigReader() :
    basic_header_ (0),
    biosig_header_ (0),
    buffered_all_channels_ (false),
    buffered_all_events_ (false)
{
    qDebug () << "Constructed BioSigReader";
    // nothing to do here
}

//-----------------------------------------------------------------------------
BioSigReader::BioSigReader (bool) :
       basic_header_ (0),
       biosig_header_ (0),
       buffered_all_channels_ (false)
{
    qDebug () << "BioSigReader constructing prototypes";
    FileSignalReaderFactory::getInstance()->registerHandler ("gdf", QSharedPointer<BioSigReader>(new BioSigReader ()));
    FileSignalReaderFactory::getInstance()->registerHandler ("evt", QSharedPointer<BioSigReader>(new BioSigReader ()));
    FileSignalReaderFactory::getInstance()->registerHandler ("bdf", QSharedPointer<BioSigReader>(new BioSigReader ()));
    FileSignalReaderFactory::getInstance()->registerHandler ("bkr", QSharedPointer<BioSigReader>(new BioSigReader ()));
    FileSignalReaderFactory::getInstance()->registerHandler ("cnt", QSharedPointer<BioSigReader>(new BioSigReader ()));
    FileSignalReaderFactory::getInstance()->registerHandler ("edf", QSharedPointer<BioSigReader>(new BioSigReader ()));
    FileSignalReaderFactory::getInstance()->registerHandler ("eeg", QSharedPointer<BioSigReader>(new BioSigReader ()));

    FileSignalReaderFactory::getInstance()->registerHandler ("acq", QSharedPointer<BioSigReader>(new BioSigReader ()));	// Biopac
    FileSignalReaderFactory::getInstance()->registerHandler ("ahdr", QSharedPointer<BioSigReader>(new BioSigReader ()));	// BrainVision file format
    FileSignalReaderFactory::getInstance()->registerHandler ("vhdr", QSharedPointer<BioSigReader>(new BioSigReader ()));	// BrainVision file format
    FileSignalReaderFactory::getInstance()->registerHandler ("scp", QSharedPointer<BioSigReader>(new BioSigReader ()));	// SCP-ECG: EN1064, ISO 11073-91064

    FileSignalReaderFactory::getInstance()->registerDefaultHandler (QSharedPointer<BioSigReader>(new BioSigReader ()));
}


//-----------------------------------------------------------------------------
BioSigReader::~BioSigReader()
{
    doClose();
}

//-----------------------------------------------------------------------------
QSharedPointer<FileSignalReader> BioSigReader::createInstance (QString const& file_path)
{
    QSharedPointer<BioSigReader> reader (new BioSigReader);
    if (file_path.section('.', -1) == "evt")
        reader->buffered_all_channels_ = true;
    QString error = reader->open (file_path);
    if (error.size() > 0)
    {
        qDebug () << error;
        QMessageBox::critical(0, QObject::tr("Error"), error);
        return QSharedPointer<FileSignalReader> (0);
    }
    else
        return reader;
}


//-----------------------------------------------------------------------------
void BioSigReader::doClose () const
{
    if (biosig_header_)
    {
        sclose (biosig_header_);
        destructHDR (biosig_header_);
    }
    biosig_header_ = 0;
}

//-----------------------------------------------------------------------------
QSharedPointer<DataBlock const> BioSigReader::getSignalData (ChannelID channel_id,
                                       unsigned start_sample,
                                       unsigned length) const
{
    QMutexLocker lock (&mutex_);

    if (!buffered_all_channels_)
        bufferAllChannels();

    if (!channel_map_.contains(channel_id))
        return QSharedPointer<DataBlock const> (0);

    if (length == basic_header_->getNumberOfSamples() &&
        start_sample == 0)
        return channel_map_[channel_id];
    else
        return channel_map_[channel_id]->createSubBlock (start_sample, length);
}

//-----------------------------------------------------------------------------
QList<QSharedPointer<SignalEvent const> > BioSigReader::getEvents () const
{
    QMutexLocker lock (&mutex_);
    QList<QSharedPointer<SignalEvent const> > empty_list;
    if (!biosig_header_)
        return empty_list;

    if (!buffered_all_events_)
        bufferAllEvents();

    return events_;
}

//-----------------------------------------------------------------------------
QString BioSigReader::open (QString const& file_name)
{

    QMutexLocker lock (&mutex_);
    return loadFixedHeader (file_name);
}

//-----------------------------------------------------------------------------
QString BioSigReader::loadFixedHeader(const QString& file_name)
{
    QMutexLocker locker (&biosig_access_lock_);
    char *c_file_name = new char[file_name.length() + 1];
    strcpy (c_file_name, file_name.toLocal8Bit ().data());
    c_file_name[file_name.length()] = '\0';

    tzset();

    // set flags
    if(biosig_header_==NULL)
    {
        biosig_header_ = constructHDR (0,0);
        biosig_header_->FLAG.UCAL = 0;
        biosig_header_->FLAG.OVERFLOWDETECTION = 0;
    }

    biosig_header_ = sopen(c_file_name, "r", biosig_header_ );

    basic_header_ = QSharedPointer<BasicHeader>
                    (new BiosigBasicHeader (biosig_header_));

    /// waldesel: REMOVE OLD STUFF from here downwards
    ///           and move it into BiosigBasicHeader!!!

#ifdef CHOLMOD_H
    char *REREF_FILE;
    REREF_FILE = "/home/as/trunk/biosig4c++/MM63.mmm";
    REREF_FILE = NULL; 
    /*
    	TODD: 
    	REREF_FILE must be the filename of the spatial filter in MarketMatrix format 
    */

    RerefCHANNEL(biosig_header_, REREF_FILE, 1);
#endif
    if (biosig_header_ == NULL || serror())
    {
            sclose (biosig_header_);
            destructHDR(biosig_header_);
            biosig_header_ = 0;
            delete c_file_name;
            return "file not supported";
    }

    // (C) 2008 AS: EVENT.DUR and EVENT.CHN are optional in SOPEN, but SigViewer needs them.
    convert2to4_eventtable(biosig_header_);

    //hdr2ascii(biosig_header_,stdout,4);

    delete c_file_name;
    c_file_name = 0;

    basic_header_->setType(GetFileTypeString(biosig_header_->TYPE));

    uint16_t NS=0;  // count number of selected channels - status channels are already converted to event table
    for (uint16_t k=0; k<biosig_header_->NS; k++)
    {
        if (biosig_header_->CHANNEL[k].OnOff) NS++;
    }

#ifdef CHOLMOD_H
    if ((biosig_header_->Calib) && (biosig_header_->Calib->nrow==NS))
	NS = biosig_header_->Calib->ncol;
#endif
    basic_header_->setNumberChannels(NS);

    basic_header_->setVersion (QString::number(biosig_header_->VERSION));
    basic_header_->setNumberRecords (biosig_header_->NRec);
    basic_header_->setRecordSize (biosig_header_->SPR);
    basic_header_->setRecordsPosition (biosig_header_->HeadLen);

    basic_header_->setRecordDuration (biosig_header_->SPR / biosig_header_->SampleRate);
    basic_header_->setSampleRate (biosig_header_->SampleRate);
    basic_header_->setNumberEvents(biosig_header_->EVENT.N);
    if (biosig_header_->EVENT.SampleRate)
        basic_header_->setEventSamplerate(biosig_header_->EVENT.SampleRate);
    else
        basic_header_->setEventSamplerate(biosig_header_->SampleRate);

#ifdef CHOLMOD_H
    if (biosig_header_->Calib==NULL) {
#endif
    uint16_t chan=0;
    for (uint16_t channel_index = 0; channel_index < biosig_header_->NS; ++channel_index)
    if (biosig_header_->CHANNEL[channel_index].OnOff)	// show only selected channels - status channels are not shown.
    {
        SignalChannel* channel = new SignalChannel(chan++,
                                                   biosig_header_->SPR,
                                                   biosig_header_->CHANNEL[channel_index]);
        basic_header_->addChannel(channel);
    }
#ifdef CHOLMOD_H
    } else 
    for (uint16_t channel_index = 0; channel_index < biosig_header_->Calib->ncol; ++channel_index)
    {
        SignalChannel* channel = new SignalChannel(channel_index, 
                                                   biosig_header_->SPR,
                                                   biosig_header_->rerefCHANNEL[channel_index]);
        basic_header_->addChannel(channel);
    }
#endif
    return "";
}

//-----------------------------------------------------------------------------
QSharedPointer<BasicHeader> BioSigReader::getBasicHeader ()
{
    QMutexLocker lock (&mutex_);
    return basic_header_;
}

//-----------------------------------------------------------------------------
void BioSigReader::bufferAllChannels () const
{
    uint32 length = basic_header_->getNumberOfSamples ();
    biosig_data_type* read_data = new biosig_data_type[length];

    biosig_header_->FLAG.ROW_BASED_CHANNELS = 0;
    QProgressDialog progress_dialog;
    progress_dialog.setMaximum (basic_header_->getNumberChannels());
    progress_dialog.setLabelText(QObject::tr("Loading channels..."));
    progress_dialog.show ();
    QTime timer;
    timer.start();

    for (ChannelID channel_id_sub = 0; channel_id_sub <  basic_header_->getNumberChannels(); ++channel_id_sub)
        biosig_header_->CHANNEL[channel_id_sub].OnOff = 0;

    for (ChannelID channel_id = 0; channel_id < basic_header_->getNumberChannels();
         ++channel_id)
    {
        if (channel_id > 0)
            biosig_header_->CHANNEL[channel_id-1].OnOff = 0;
        biosig_header_->CHANNEL[channel_id].OnOff = 1;

        sread (read_data, 0, length / biosig_header_->SPR, biosig_header_);

        QSharedPointer<std::vector<float32> > raw_data (new std::vector<float32> (length));
//            if (biosig_header_->FLAG.ROW_BASED_CHANNELS == 1)
//            {
//                raw_data[index] = read_data[length + (index * channel_id)];
//            }
        raw_data->insert(raw_data->begin(), read_data, read_data + length);
        QSharedPointer<DataBlock const> data_block (new DataBlock (raw_data,
                                                                   basic_header_->getSampleRate()));
        channel_map_[channel_id] = data_block;
        progress_dialog.setValue(progress_dialog.value()+1);
    }
    progress_dialog.setValue(progress_dialog.maximum());
    qDebug() << "buffering time = " << timer.elapsed();

    buffered_all_channels_ = true;
    if (buffered_all_events_)
        doClose();
    delete read_data;
}

//-------------------------------------------------------------------------
void BioSigReader::bufferAllEvents () const
{
    unsigned number_events = biosig_header_->EVENT.N;
    for (unsigned index = 0; index < number_events; index++)
    {
        QSharedPointer<SignalEvent> event (new SignalEvent (biosig_header_->EVENT.POS[index],
                                                            biosig_header_->EVENT.TYP[index],
                                                            biosig_header_->EVENT.SampleRate));
        if (biosig_header_->EVENT.CHN)
        {
            if (biosig_header_->EVENT.CHN[index] == 0)
                event->setChannel (UNDEFINED_CHANNEL);
            else
                event->setChannel (biosig_header_->EVENT.CHN[index] - 1);
            event->setDuration (biosig_header_->EVENT.DUR[index]);
        }
        events_.append (event);
    }

    buffered_all_events_ = true;
    if (buffered_all_channels_)
        doClose();
}

}