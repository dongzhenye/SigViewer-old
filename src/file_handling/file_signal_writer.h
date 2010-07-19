// file_signal_writer.h

#ifndef FILE_SIGNAL_WRITER_H
#define FILE_SIGNAL_WRITER_H

#include "file_signal_reader.h"
#include "file_context.h"
#include "event_manager.h"
#include "channel_manager.h"
#include "base/signal_event.h"



class QTextStream;

namespace BioSig_
{

// abstract signal writer
class FileSignalWriter
{
public:
    typedef FileSignalReader::SignalEventVector SignalEventVector;

    virtual ~FileSignalWriter() {}

    //-------------------------------------------------------------------------
    virtual QSharedPointer<FileSignalWriter> createInstance (QString const& file_path) = 0;

    //-------------------------------------------------------------------------
    virtual bool supportsSavingEvents () const {return false;}

    //-------------------------------------------------------------------------
    virtual QString saveEventsToSignalFile (QSharedPointer<EventManager const>,
                                            std::set<EventType> const& types) = 0;

    //-------------------------------------------------------------------------
    virtual QString save (QSharedPointer<FileContext const> file_context)
    {
        QSharedPointer<EventManager const> event_manager = file_context->getEventManager();
        return save (file_context, event_manager->getAllPossibleEventTypes ());
    }

    //-------------------------------------------------------------------------
    virtual QString save (QSharedPointer<FileContext const> file_context,
                          std::set<EventType> const& types) = 0;

protected: 
    FileSignalWriter () {}

private:
    // not allowed
    FileSignalWriter(const FileSignalWriter&);
    const FileSignalWriter& operator=(const FileSignalWriter&);
};

} // namespace BioSig_

#endif