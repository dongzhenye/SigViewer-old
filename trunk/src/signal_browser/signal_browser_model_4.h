// signal_browser_model.h
#ifndef SIGNAL_BROWSER_MODEL_H_q4
#define SIGNAL_BROWSER_MODEL_H_q4

#include "../file_context.h"
#include "../tab_context.h"
#include "../file_handling/channel_manager.h"
#include "../file_handling/event_manager.h"
#include "../gui/signal_visualisation_modes.h"
#include "../gui/signal_visualisation_model.h"
#include "../gui/color_manager.h"
#include "event_graphics_item.h"

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QPointF>

#include <map>
#include <vector>
#include <set>

namespace BioSig_
{

class SignalBrowserView;
class SignalGraphicsItem;

//-----------------------------------------------------------------------------
/// SignalBrowserModel
class SignalBrowserModel : public SignalVisualisationModel
{
    Q_OBJECT
public:
    //-------------------------------------------------------------------------
    SignalBrowserModel(QSharedPointer<EventManager> event_manager,
                       QSharedPointer<ChannelManager> channel_manager,
                       QSharedPointer<TabContext> tab_context,
                       QSharedPointer<ColorManager const> color_manager);

    //-------------------------------------------------------------------------
    virtual ~SignalBrowserModel() {}

    //-------------------------------------------------------------------------
    virtual void update ();

    //-------------------------------------------------------------------------
    virtual void scaleChannel (ChannelID id, float32 lower_value, float32 upper_value);

    //-------------------------------------------------------------------------
    virtual void scaleChannel (ChannelID id);

    //-------------------------------------------------------------------------
    virtual QSharedPointer<ChannelManager const> getChannelManager () const;

    //-------------------------------------------------------------------------
    virtual QSharedPointer<EventManager const> getEventManager () const;

    //-------------------------------------------------------------------------
    /// see base class
    virtual unsigned getShownHeight () const;

    //-------------------------------------------------------------------------
    /// see base class
    virtual unsigned getShownSignalWidth () const;

    //-------------------------------------------------------------------------
    /// see base class
    virtual unsigned getShownPosition () const;

    //-------------------------------------------------------------------------
    /// see base class
    virtual void goToSample (unsigned sample);

    //-------------------------------------------------------------------------
    /// see base class
    virtual EventID getSelectedEvent () const;

    void setSignalBrowserView(SignalBrowserView* signal_browser_view);
    void loadSettings();
    void saveSettings();

    //settings
    std::map<std::string, bool> getHideableWidgetsVisibilities () const;
    void setHideableWidgetsVisibilities (std::map<std::string, bool> const &widgets_visiblities);
    void setXGridVisible(bool visible);
    bool getGridVisible () const;
    void setYGridVisible(bool visible);

    virtual void setShownChannels (std::set<ChannelID> const& shown_channels);
    virtual std::set<ChannelID> getShownChannels () const;
    uint32 getNumberShownChannels() const;
    int32 getYPosOfChannel (uint32 channel_nr) const;

    void zoomInAll();
    void zoomOutAll();

    int32 getSignalSpacing();
    /*    int32 getPreferedXGirdPixelIntervall();
*/
    int32 getPreferedYGirdPixelIntervall();
    float64 getXGridPixelIntervall();

    EventGraphicsItem* getSelectedEventItem();
    void updateEventItems ();

public slots:
    //-------------------------------------------------------------------------
    /// adds the given event
    virtual void addEventItem (QSharedPointer<SignalEvent const> event);

    //-------------------------------------------------------------------------
    /// removes the given event
    virtual void removeEventItem (EventID id);


    void updateEvent (EventID id);

    virtual void selectEvent (EventID id);
    void unselectEvent ();

signals:
    void eventSelected (QSharedPointer<SignalEvent const> selected_event);
    void signalSpacingChanged (unsigned signal_spacing);

protected:
    virtual void shownEventTypesChangedImpl ();

private:
    //-------------------------------------------------------------------------
    void addChannel (ChannelID channel_nr);

    //-------------------------------------------------------------------------
    void removeChannel (ChannelID channel_nr);

    //-------------------------------------------------------------------------
    static uint8 const NAVIGATION_Z = 1;
    static uint8 const X_GRID_Z = 2;
    static uint8 const CHANNEL_SEPARATOR_Z = 3;
    static uint8 const SIGNAL_Z = 4;
    static uint8 const EVENT_Z = 5;

    QSharedPointer<ChannelManager> channel_manager_;
    QSharedPointer<EventManager> event_manager_;
    QSharedPointer<TabContext> tab_context_;
    QSharedPointer<ColorManager const> color_manager_;
    SignalBrowserView* signal_browser_view_;

    typedef std::map<int32, SignalGraphicsItem*> Int2SignalGraphicsItemPtrMap;
    typedef std::map<int32, EventGraphicsItem*> Int2EventGraphicsItemPtrMap;

    typedef std::map<int32, int32> Int2IntMap;

    void updateEventItemsImpl ();

    // items
    Int2SignalGraphicsItemPtrMap channel2signal_item_;
    Int2EventGraphicsItemPtrMap id2event_item_;

    Int2IntMap channel2y_pos_;
    EventGraphicsItem* selected_event_item_;

    int32 signal_spacing_;
    int32 prefered_x_grid_pixel_intervall_;
    int32 prefered_y_grid_pixel_intervall_;
    float64 x_grid_pixel_intervall_;

    bool show_y_grid_;
    bool show_x_grid_;
};

} // namespace PortingToQT4_

#endif

