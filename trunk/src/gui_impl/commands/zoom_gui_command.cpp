#include "zoom_gui_command.h"
#include "../gui_helper_functions.h"

#include <QInputDialog>
#include <QDebug>

namespace BioSig_
{

//-----------------------------------------------------------------------------
QString const ZoomGuiCommand::GOTO_ = "Go to...";
QString const ZoomGuiCommand::ZOOM_IN_VERTICAL_ = "Zoom In Vertical";
QString const ZoomGuiCommand::ZOOM_OUT_VERTICAL_ = "Zoom Out Vertical";
QString const ZoomGuiCommand::ZOOM_IN_HORIZONTAL_ = "Zoom In Horizontal";
QString const ZoomGuiCommand::ZOOM_OUT_HORIZONTAL_ = "Zoom Out Horizontal";
QString const ZoomGuiCommand::SCALE_X_AXIS_ = "Scale X Axis";
QString const ZoomGuiCommand::CHANNEL_PER_PAGE_ = "Channels per Page...";
QString const ZoomGuiCommand::AUTO_ZOOM_VERTICAL_ = "Auto Zoom Vertical";


QStringList const ZoomGuiCommand::ACTIONS_ = QStringList() << ZoomGuiCommand::ZOOM_IN_VERTICAL_
                                           << ZoomGuiCommand::ZOOM_OUT_VERTICAL_
                                           << ZoomGuiCommand::ZOOM_IN_HORIZONTAL_
                                           << ZoomGuiCommand::ZOOM_OUT_HORIZONTAL_
                                           << ZoomGuiCommand::GOTO_
                                           << ZoomGuiCommand::SCALE_X_AXIS_
                                           << ZoomGuiCommand::CHANNEL_PER_PAGE_;


//-----------------------------------------------------------------------------
GuiActionFactoryRegistrator ZoomGuiCommand::registrator_ ("Zooming",
                                                          QSharedPointer<ZoomGuiCommand> (new ZoomGuiCommand));



//-----------------------------------------------------------------------------
ZoomGuiCommand::ZoomGuiCommand ()
    : GuiActionCommand (ACTIONS_)
{
    // nothing to do here
}

//-----------------------------------------------------------------------------
void ZoomGuiCommand::init ()
{
    getQAction (GOTO_)->setIcon (QIcon(":/images/icons/goto.png"));
    getQAction (ZOOM_IN_VERTICAL_)->setIcon (QIcon(":/images/icons/zoom_in_vertical.png"));
    getQAction (ZOOM_OUT_VERTICAL_)->setIcon (QIcon(":/images/icons/zoom_out_vertical.png"));
    getQAction (ZOOM_IN_HORIZONTAL_)->setIcon (QIcon(":/images/icons/zoom_in_horizontal.png"));
    getQAction (ZOOM_OUT_HORIZONTAL_)->setIcon (QIcon(":/images/icons/zoom_out_horizontal.png"));

    setShortcut (ZOOM_IN_VERTICAL_, QKeySequence::ZoomIn);
    setShortcut (ZOOM_OUT_VERTICAL_, QKeySequence::ZoomOut);
    setShortcut (ZOOM_IN_HORIZONTAL_, Qt::ALT + Qt::Key_Plus);
    setShortcut (ZOOM_OUT_HORIZONTAL_, Qt::ALT + Qt::Key_Minus);

    resetActionTriggerSlot (GOTO_, SLOT(goTo()));
    resetActionTriggerSlot (ZOOM_IN_VERTICAL_, SLOT(zoomInVertical()));
    resetActionTriggerSlot (ZOOM_OUT_VERTICAL_, SLOT(zoomOutVertical()));
    resetActionTriggerSlot (ZOOM_IN_HORIZONTAL_, SLOT(zoomInHorizontal()));
    resetActionTriggerSlot (ZOOM_OUT_HORIZONTAL_, SLOT(zoomOutHorizontal()));
    resetActionTriggerSlot (SCALE_X_AXIS_, SLOT(scaleXAxis()));
    resetActionTriggerSlot (CHANNEL_PER_PAGE_, SLOT(setChannelsPerPage()));
}

//-------------------------------------------------------------------------
void ZoomGuiCommand::evaluateEnabledness ()
{
    QSharedPointer<SignalViewSettings> vis = currentSignalViewSettings ();
    QSharedPointer<SignalVisualisationModel> vis_model = currentVisModel();
    bool file_open = getApplicationState () == APP_STATE_FILE_OPEN;
    bool zoom_out_vertical_possible = false;
    bool zoom_in_vertical_possible = false;
    bool zoom_out_horizontal_possible = false;
    bool zoom_in_horizontal_possible = false;

    if (file_open && !vis.isNull())
    {
        zoom_out_vertical_possible = (vis_model->getSignalViewSettings()->getChannelHeight() * vis_model->getShownChannels().size() > vis_model->getShownHeight());
        zoom_in_vertical_possible = (static_cast<unsigned>(vis_model->getSignalViewSettings()->getChannelHeight()) < vis_model->getShownHeight());
        zoom_out_horizontal_possible = vis->getPixelsPerSample() > minPixelPerSample();
        zoom_in_horizontal_possible = vis->getPixelsPerSample() < maxPixelPerSample();
    }

    getQAction (ZOOM_OUT_VERTICAL_)->setEnabled (zoom_out_vertical_possible);
    getQAction (ZOOM_IN_VERTICAL_)->setEnabled (zoom_in_vertical_possible);
    getQAction (ZOOM_OUT_HORIZONTAL_)->setEnabled (zoom_out_horizontal_possible);
    getQAction (ZOOM_IN_HORIZONTAL_)->setEnabled (zoom_in_horizontal_possible);
    getQAction (GOTO_)->setEnabled (file_open);
    getQAction (SCALE_X_AXIS_)->setEnabled (file_open);
}


//-------------------------------------------------------------------------
void ZoomGuiCommand::goTo ()
{
    QSharedPointer<SignalVisualisationModel> vis_model =
                currentVisModel();
    QSharedPointer<ChannelManager const> channel_manager =
            vis_model->getChannelManager();
    bool ok;
    double sec = QInputDialog::getDouble (0, tr("Go to..."), tr("Second: "), 0, 0,
                             channel_manager->getDurationInSec(), 1, &ok);
    if (!ok)
        return;


    vis_model->goToSample (sec * channel_manager->getSampleRate ());
}

//-----------------------------------------------------------------------------
void ZoomGuiCommand::zoomInHorizontal ()
{
    float32 pixel_per_sample = currentSignalViewSettings()->getPixelsPerSample();
    float32 new_pixel_per_sample = std::min (pixel_per_sample * ZOOM_FACTOR_,
                                 maxPixelPerSample() );

    GuiHelper::animateProperty (currentSignalViewSettings().data(), "pixelsPerSample",
                                pixel_per_sample, new_pixel_per_sample,
                                this, SLOT(evaluateEnabledness()));
}

//-----------------------------------------------------------------------------
void ZoomGuiCommand::zoomOutHorizontal ()
{
    float32 pixel_per_sample = currentSignalViewSettings()->getPixelsPerSample();
    float32 new_pixel_per_sample = std::max (pixel_per_sample / ZOOM_FACTOR_,
                                 minPixelPerSample ());

    GuiHelper::animateProperty (currentSignalViewSettings().data(), "pixelsPerSample",
                                pixel_per_sample, new_pixel_per_sample,
                                this, SLOT(evaluateEnabledness()));
}


//-----------------------------------------------------------------------------
void ZoomGuiCommand::zoomInVertical ()
{
    unsigned channel_height = currentVisModel()->getSignalViewSettings()->getChannelHeight();
    unsigned new_channel_height = std::min<unsigned> (channel_height * ZOOM_FACTOR_,
                                    maxChannelHeight());
    GuiHelper::animateProperty (currentVisModel()->getSignalViewSettings().data(), "channelHeight",
                                channel_height, new_channel_height,
                                this, SLOT(evaluateEnabledness()));
}

//-----------------------------------------------------------------------------
void ZoomGuiCommand::zoomOutVertical ()
{
    unsigned channel_height = currentVisModel()->getSignalViewSettings()->getChannelHeight();
    unsigned new_channel_height = std::max<unsigned> (channel_height / ZOOM_FACTOR_,
                              minChannelHeight());
    GuiHelper::animateProperty (currentVisModel()->getSignalViewSettings().data(), "channelHeight",
                                channel_height, new_channel_height,
                                this, SLOT(evaluateEnabledness()));
}

//-----------------------------------------------------------------------------
void ZoomGuiCommand::autoZoomVertical ()
{
}


//-------------------------------------------------------------------------
void ZoomGuiCommand::scaleXAxis ()
{
    float32 sample_rate = currentVisModel()->getChannelManager()->getSampleRate();
    float32 pixel_per_second = currentSignalViewSettings()->getPixelsPerSample() * sample_rate;
    float32 width = currentVisModel()->getShownSignalWidth();
    float32 new_secs_per_page = QInputDialog::getDouble (0, tr("Scale X Axis"), tr("Seconds"), width / pixel_per_second, minPixelPerSample() / sample_rate, currentVisModel()->getChannelManager()->getDurationInSec());

    float32 pixel_per_sample = width / (new_secs_per_page * sample_rate);

    GuiHelper::animateProperty (currentVisModel().data(), "pixel_per_sample_",
                                currentSignalViewSettings()->getPixelsPerSample(),
                                pixel_per_sample,
                                this, SLOT(evaluateEnabledness()));
}

//-------------------------------------------------------------------------
void ZoomGuiCommand::setChannelsPerPage ()
{
    unsigned channel_height = currentVisModel()->getSignalViewSettings()->getChannelHeight();
    float viewport_height = currentVisModel()->getShownHeight();
    float channels_per_page = viewport_height / channel_height;

    bool ok = false;
    float new_channels_per_page = QInputDialog::getInt (0, tr("Vertical Zooming"),
                                                        tr("Channels per Page"),
                                                        channels_per_page, 1, currentVisModel()->getShownChannels().size(), 1, &ok);

    if (!ok)
        return;

    unsigned new_channel_height = viewport_height / new_channels_per_page;
    if (new_channel_height == channel_height)
        return;

    GuiHelper::animateProperty (currentVisModel()->getSignalViewSettings().data(), "channelHeight",
                                channel_height, new_channel_height,
                                this, SLOT(evaluateEnabledness()));
}

//-------------------------------------------------------------------------
unsigned ZoomGuiCommand::maxChannelHeight ()
{
    return currentVisModel()->getShownHeight();
}

//-------------------------------------------------------------------------
unsigned ZoomGuiCommand::minChannelHeight ()
{
    return std::max<unsigned> (20, currentVisModel()->getShownHeight() / currentVisModel()->getShownChannels().size());
}


//-------------------------------------------------------------------------
float32 ZoomGuiCommand::maxPixelPerSample ()
{
    return static_cast<float32>(currentVisModel()->getShownSignalWidth ()) / 4.0;
}


//-------------------------------------------------------------------------
float32 ZoomGuiCommand::minPixelPerSample ()
{
    return static_cast<float32>(currentVisModel()->getShownSignalWidth()) /
           static_cast<float32>(currentVisModel()->getChannelManager()->getNumberSamples ());
}



}