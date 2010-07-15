// event_table_file_reader.h

#ifndef EVENT_TABLE_FILE_READER
#define EVENT_TABLE_FILE_READER

#include "../base/sigviewer_user_types.h"

#include <QStringList>
#include <QMap>
#include <QList>

#include <set>

class QTextStream;

namespace BioSig_
{


//-----------------------------------------------------------------------------
///
/// EventTableFileReader
///
/// responsible for mapping of EventType to String (name of event)
class EventTableFileReader
{
public:
    typedef QList<uint16>::const_iterator IntIterator;
    typedef QStringList::const_iterator StringIterator;

    EventTableFileReader();
    ~EventTableFileReader();

    void setLogStream(QTextStream* log_stream);


    StringIterator getGroupIdBegin() const;
    StringIterator getGroupIdEnd() const;
    QString getEventGroupName(const QString& group_id);

    IntIterator eventTypesBegin();
    IntIterator eventTypesEnd();
    QString getEventName (EventType event_type_id) const;
    void setEventName (EventType event_type_id, QString const& name);
    void restoreEventNames ();
    std::set<EventType> getEventsOfGroup (QString const& group_id) const;
    QString getEventGroupId (EventType event_type_id) const;

    std::set<uint16> getAllEventTypes () const;
private:
    bool load(const QString& file_name);

    struct EventItem
    {
        QString name;
        QString group_id;
    };

    typedef QList<uint16> IntList;
    typedef QMap<uint16, EventItem> Int2EventItemMap;
    typedef QMap<QString, QString> String2StringMap;

    // not allowed
    EventTableFileReader(const EventTableFileReader& src);
    const EventTableFileReader& operator=(const EventTableFileReader& src);

    IntList event_types_;
    QStringList event_group_ids_;
    Int2EventItemMap event_type2name_;
    String2StringMap group_id2name_;

    QString event_file_path_;
    QTextStream* log_stream_;
};

} // namespace BioSig_

#endif
