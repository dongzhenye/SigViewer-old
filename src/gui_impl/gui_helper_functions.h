#ifndef GUI_HELPER_FUNCTIONS_H
#define GUI_HELPER_FUNCTIONS_H

#include "../base/user_types.h"
#include "../file_handling/channel_manager.h"
#include "../gui/signal_visualisation_model.h"

#include <set>

namespace BioSig_
{

namespace GuiHelper
{

//-----------------------------------------------------------------------------
QSharedPointer<SignalEvent const> getSelectedEvent ();

//-----------------------------------------------------------------------------
EventID getSelectedEventID ();

//-----------------------------------------------------------------------------
EventType selectEventType (EventType preselected_type);

//-----------------------------------------------------------------------------
std::set<EventType> selectEventTypes (std::set<EventType> const& preselected_type,
                                      bool enable_color_editing = false);

//-----------------------------------------------------------------------------
ChannelID selectChannel (ChannelID preselected_channel);

//-----------------------------------------------------------------------------
std::set<ChannelID> selectShownChannels (ChannelID hide_channel);

//-----------------------------------------------------------------------------
std::set<ChannelID> selectChannels (QSharedPointer<ChannelManager const> channel_manager,
                                    QString const& file_name = "",
                                    QSharedPointer<SignalVisualisationModel> vis_model
                                    = QSharedPointer<SignalVisualisationModel>(0));

//-----------------------------------------------------------------------------
QString getFilePathFromSaveAsDialog (QString const& path,
                                     QString const& extensions);


}

}

#endif // GUI_HELPER_FUNCTIONS_H