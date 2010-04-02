#ifndef EVENT_INFO_DOCKWIDGET_H
#define EVENT_INFO_DOCKWIDGET_H

#include "../base/user_types.h"
#include "../base/signal_event.h"

#include <QWidget>
#include <QSharedPointer>
#include <QLabel>
#include <QObject>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QList>
#include <QMap>
#include <QMutex>

#include <map>
#include <set>

namespace BioSig_
{

class SignalBrowserModel;
class SignalEvent;
class EventManager;
class CommandExecuter;

class EventInfoWidget : public QWidget
{
    Q_OBJECT

public:
    EventInfoWidget(QWidget* parent,
                    EventManager& event_manager,
                    CommandExecuter& command_executer,
                    QSharedPointer<SignalBrowserModel> signal_browser_model);
    virtual ~EventInfoWidget ();

public slots:
    void updateSelectedEventInfo (QSharedPointer<SignalEvent const>
                                  selected_signal_event);
    void updateShownEventTypes (std::set<uint16> shown_event_types);

private slots:
    void selfChangedType (int combo_box_index);
    void selfChangedDuration (double new_duration);

private:
    void setSelfUpdating (bool self_updating);
    bool isSelfUpdating ();


    QSharedPointer<SignalBrowserModel> signal_browser_model_;
    EventManager& event_manager_;
    CommandExecuter& command_executer_;
    std::map<uint16, QString> shown_event_types_;

    QSharedPointer<SignalEvent const> selected_signal_event_;

    QList<QWidget*> disabled_widgets_if_nothing_selected_;
    QMap<int32, QSharedPointer<SignalEvent const> > hovered_events_;


    QDoubleSpinBox* duration_spinbox_;
    QLabel* start_label_;
    QComboBox* event_type_combobox_;
    QHBoxLayout* layout_;

    QMutex self_updating_mutex_;
    bool self_updating_;
};

}

#endif // EVENT_INFO_DOCKWIDGET_H