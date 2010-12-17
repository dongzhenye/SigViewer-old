#ifndef EVENT_TABLE_WIDGET_H
#define EVENT_TABLE_WIDGET_H


#include "ui_event_table_widget.h"
#include "file_handling/event_manager.h"
#include "file_handling/channel_manager.h"

#include <QWidget>

namespace SigViewer_
{

class EventTableWidget : public QWidget
{
    Q_OBJECT

public:
    EventTableWidget (QSharedPointer<EventManager> event_manager,
                      QSharedPointer<ChannelManager const> channel_manager,
                      QWidget *parent = 0);
    virtual ~EventTableWidget ();

private slots:
    void addToTable (QSharedPointer<SignalEvent const> event);

private:
    void buildTable ();

    static int const ID_INDEX_ = 0;
    static int const POSITION_INDEX_ = 1;
    static int const DURATION_INDEX_ = 2;
    static int const CHANNEL_INDEX_ = 3;
    static int const TYPE_INDEX_ = 4;

    int precision_;

    Ui::EventTableWidget ui_;
    QSharedPointer<EventManager> event_manager_;
    QSharedPointer<ChannelManager const> channel_manager_;
};

}

#endif // EVENT_TABLE_WIDGET_H