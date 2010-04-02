// file_signal_reader.h

#ifndef FILE_SIGNAL_READER_H
#define FILE_SIGNAL_READER_H

#include "basic_header.h"
#include "../base/signal_event.h"

#include <biosig.h>

#include <QVector>
#include <QPointer>
#include <QSharedPointer>

class QTextStream;
class QString;

namespace BioSig_
{

class SignalDataBlock;

// abstract signal reader
class FileSignalReader
{
public:
    typedef QVector<SignalEvent> SignalEventVector;
    typedef QVector<SignalDataBlock*> SignalDataBlockPtrVector;
    typedef SignalDataBlockPtrVector::iterator SignalDataBlockPtrIterator;

    virtual ~FileSignalReader();
    virtual FileSignalReader* clone() = 0;

    void setLogStream(QTextStream* log_stream);

    virtual QString open(const QString& file_name) = 0;
    virtual QString open(const QString& file_name, const bool overflow_detection) = 0;

    virtual void enableCaching() = 0;

    virtual bool isOpen() = 0;
    virtual void close() = 0;

    virtual void loadSignals(SignalDataBlockPtrIterator begin,
                             SignalDataBlockPtrIterator end,
                             uint32 start_record) = 0;
    virtual void loadEvents(SignalEventVector& event_vector) = 0;

    virtual QPointer<BasicHeader> getBasicHeader () = 0;

    virtual HDRTYPE* getRawHeader () {return 0;}

protected:
    FileSignalReader();

    QTextStream* log_stream_;

private:
    // not allowed
    FileSignalReader(const FileSignalReader&);
    const FileSignalReader& operator=(const FileSignalReader&);
};

} // namespace BioSig_

#endif