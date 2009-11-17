#ifndef EVENT_GRAPHICS_ITEM_H
#define EVENT_GRAPHICS_ITEM_H


#include "../base/user_types.h"

#include <QGraphicsItem>
#include <QSharedPointer>
#include <QColor>
#include <QVector>

class QMutex;

namespace BioSig_
{

class SignalBuffer;
class SignalEvent;
class SignalBrowserModel;
class SignalBrowserView;
class EventContextMenu;


class EventGraphicsItem : public QGraphicsItem
{
public:
    EventGraphicsItem(SignalBuffer& buffer, SignalBrowserModel& model,
                    SignalBrowserView* browser,
                    QSharedPointer<SignalEvent> signal_event);

    virtual ~EventGraphicsItem ();

    uint32 getId() const;

    void setSize (int32 width, int32 height);
    void startMouseMoveEnd ();
    void setSelected (bool selected);
    QSharedPointer<SignalEvent> getSignalEvent ();
    void updateColor ();

    static void displayContextMenu (QGraphicsSceneContextMenuEvent * event);

    virtual QRectF boundingRect () const;
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent * mouse_event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event);
    virtual void hoverMoveEvent (QGraphicsSceneHoverEvent * event );

protected:
    enum Action
    {
        ACTION_NONE,
        ACTION_MOVE_BEGIN,
        ACTION_MOVE_END,
        ACTION_SHIFT_TO_CHANNEL,
        ACTION_COPY_SHIFT_TO_CHANNEL,
        ACTION_SELECT
    };

    enum State
    {
        STATE_NONE,
        STATE_MOVE_BEGIN,
        STATE_MOVE_END,
        STATE_SHIFT_TO_CHANNEL
    };

    //-----------------------------------------------------------------------------
    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent * event);

    //-----------------------------------------------------------------------------
    Action getMousePressAction(QGraphicsSceneMouseEvent* e);


    SignalBrowserModel& signal_browser_model_;
    SignalBuffer& signal_buffer_;
    QColor color_;
    State state_;

    int32 height_;
    int32 width_;
    bool is_selected_;
    QSharedPointer<SignalEvent> signal_event_;
    static int move_mouse_range_;
    static QMutex event_handling_mutex_;

    static QMutex context_menu_mutex_;
    static QSharedPointer<EventContextMenu> context_menu_;

};

}
#endif // EVENT_GRAPHICS_ITEM_H