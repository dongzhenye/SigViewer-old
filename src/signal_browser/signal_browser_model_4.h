// signal_browser_model.h
#ifndef SIGNAL_BROWSER_MODEL_H_q4
#define SIGNAL_BROWSER_MODEL_H_q4

#include "../abstract_browser_model.h"
#include "../file_context.h"
#include "../tab_context.h"
#include "../file_handling/channel_manager.h"
#include "../file_handling/event_manager.h"
#include "../gui/signal_visualisation_modes.h"
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
class SignalBrowserModel : public AbstractBrowserModel
{
    Q_OBJECT
public:
    SignalBrowserModel(QSharedPointer<EventManager> event_manager,
                       QSharedPointer<ChannelManager> channel_manager,
                       QSharedPointer<TabContext> tab_context);
    virtual ~SignalBrowserModel();

    virtual void setPixelPerXUnit (float64 pixel_per_sec);

    virtual void setItemsHeight (int32 height);

    virtual void updateLayout ();

    virtual void zoom (ZoomDimension dimension, float factor);

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
    void setAutoZoomBehaviour (ScaleMode auto_zoom_type);
    ScaleMode getAutoZoomBehaviour () const;

    virtual bool setShownChannels (std::set<ChannelID> const& shown_channels);
    virtual std::set<ChannelID> getShownChannels () const;
    std::map<uint32, QString> getShownChannelsWithLabels () const;
    uint32 getNumberShownChannels() const;
    int32 getYPosOfChannel (uint32 channel_nr) const;

    void zoomInAll();
    void zoomOutAll();

    void autoScaleAll();

    int32 getSignalHeight();
    int32 getSignalSpacing();
    int32 getVisibleWidth();
    /*    int32 getPreferedXGirdPixelIntervall();
*/
    int32 getPreferedYGirdPixelIntervall();
    float64 getXGridPixelIntervall();

    // events
    std::set<uint16> getDisplayedEventTypes () const;

    EventGraphicsItem* getSelectedEventItem();
    QSharedPointer<SignalEvent const> getSelectedSignalEvent();
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
    void signalHeightChanged (uint32 signal_height);
    void signalSpacingChanged (unsigned signal_spacing);

protected:
    virtual void modeChanged (SignalVisualisationMode mode);
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
    QSharedPointer<FileContext> file_context_;
    QSharedPointer<TabContext> tab_context_;
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

    // parameters
    float64 pixel_per_sec_;

    int32 signal_height_;
    int32 signal_spacing_;
    int32 prefered_x_grid_pixel_intervall_;
    int32 prefered_y_grid_pixel_intervall_;
    float64 x_grid_pixel_intervall_;

    bool show_y_grid_;
    bool show_x_grid_;
    ScaleMode auto_zoom_type_;
};

} // namespace PortingToQT4_

#endif

