/*

    $Id: biosig_writer.cpp,v 1.7 2009/02/22 12:36:46 cle1109 Exp $
    Copyright (C) Thomas Brunner  2006,2007 
              Christoph Eibel 2007,2008, 
          Clemens Brunner 2006,2007,2008  
              Alois Schloegl  2008
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


#include "biosig_writer.h"
#include "../file_handling/file_signal_writer_factory.h"

#include <QFile>
#include <QMutexLocker>
#include <QMessageBox>

#include <iostream>

namespace BioSig_
{

//-----------------------------------------------------------------------------
BioSigWriter BioSigWriter::prototype_instance_ (true);


//-----------------------------------------------------------------------------
BioSigWriter::BioSigWriter(FileFormat target_type)
 : target_type_ (target_type)
{
    file_formats_support_event_saving_.insert(GDF1);
    file_formats_support_event_saving_.insert(GDF);
}

//-----------------------------------------------------------------------------
BioSigWriter::BioSigWriter (bool)
{
    FileSignalWriterFactory::getInstance()->addPrototype(".gdf", new BioSigWriter (GDF));
}


//-----------------------------------------------------------------------------
BioSigWriter::~BioSigWriter()
{
}

//-----------------------------------------------------------------------------
FileSignalWriter* BioSigWriter::clone()
{
    return new BioSigWriter (target_type_);
}

//-----------------------------------------------------------------------------
bool BioSigWriter::supportsSavingEvents () const
{
    return file_formats_support_event_saving_.count(target_type_) > 0;
}

//-----------------------------------------------------------------------------
QString BioSigWriter::saveEventsToSignalFile (QSharedPointer<EventManager> event_manager,
                                  QString const& file_path)
{
    if (file_formats_support_event_saving_.count(target_type_) == 0)
        return QObject::tr("Can't write events to that file that file type!");

    unsigned number_events = event_manager->getNumberOfEvents ();

    HDRTYPE* header = constructHDR (0, number_events);

    QList<EventID> events = event_manager->getAllEvents();
    header = sopen (file_path.toStdString().c_str(), "r", header);
    header->EVENT.SampleRate = event_manager->getSampleRate();
    header->EVENT.N = number_events;
    header->EVENT.TYP = (typeof(header->EVENT.TYP)) realloc(header->EVENT.TYP,number_events * sizeof(typeof(*header->EVENT.TYP)));
    header->EVENT.POS = (typeof(header->EVENT.POS)) realloc(header->EVENT.POS,number_events * sizeof(typeof(*header->EVENT.POS)));
    header->EVENT.CHN = (typeof(header->EVENT.CHN)) realloc(header->EVENT.CHN,number_events * sizeof(typeof(*header->EVENT.CHN)));
    header->EVENT.DUR = (typeof(header->EVENT.DUR)) realloc(header->EVENT.DUR,number_events * sizeof(typeof(*header->EVENT.DUR)));
    for (unsigned index = 0; index < number_events; index++)
    {
        QSharedPointer<SignalEvent const> event = event_manager->getEvent(events[index]);
        if (event->getChannel() == UNDEFINED_CHANNEL)
            header->EVENT.CHN[index] = 0;
        else
            header->EVENT.CHN[index] = event->getChannel() + 1;
        header->EVENT.TYP[index] = event->getType ();
        header->EVENT.POS[index] = event->getPosition();
        header->EVENT.DUR[index] = event->getDuration();
    }

    int error = sflush_gdf_event_table (header);
    if (error)
        QMessageBox::critical(0, "Events not saved!!!", QString::number(error));

    sclose (header);
    destructHDR (header);

    return "";
}


//-----------------------------------------------------------------------------
QString BioSigWriter::save (QSharedPointer<EventManager> event_manager,
                               QString const& old_file_path,
                               QString const& file_path)
{
    HDRTYPE* read_header = sopen (old_file_path.toStdString().c_str(), "r", NULL);
    uint32 read_data_size = read_header->NS * read_header->NRec * read_header->SPR;
    biosig_data_type* read_data = new biosig_data_type[read_data_size];
    sread (read_data, 0, read_data_size, read_header);

    read_header->TYPE = target_type_;
    if (target_type_ == GDF)
        read_header->VERSION = 2;

    HDRTYPE* write_header = sopen (file_path.toStdString().c_str(), "w", read_header);
    std::cout << "write NELEM = " << swrite (read_data, read_header->NRec, write_header) << std::endl;

    delete read_data;

    sclose (write_header);
    sclose (read_header);

    destructHDR (write_header);

    if (file_formats_support_event_saving_.count(target_type_))
        saveEventsToSignalFile (event_manager, file_path);

    return "";
}

}
