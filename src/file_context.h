#ifndef FILE_CONTEXT_H
#define FILE_CONTEXT_H

#include "file_handling/event_manager.h"
#include "file_handling/basic_header.h"
#include "file_handling/channel_manager.h"
#include "gui/signal_visualisation_model.h"
#include "base/file_states.h"

#include <QObject>
#include <QString>
#include <QSharedPointer>

namespace BioSig_
{


//-----------------------------------------------------------------------------
///
/// FileContext
///
/// exists once per opened file...
///
class FileContext : public QObject
{
    Q_OBJECT
public:
    //-------------------------------------------------------------------------
    FileContext (QString const & file_path_and_name,
                 QSharedPointer<EventManager> event_manager,
                 QSharedPointer<ChannelManager> channel_manager,
                 QSharedPointer<BasicHeader> header);

    //-------------------------------------------------------------------------
    ~FileContext ();

    //-------------------------------------------------------------------------
    void resetFilePathAndName (QString const& new_file_path_and_name);

    //-------------------------------------------------------------------------
    QString getFilePath () const;

    //-------------------------------------------------------------------------
    QString const& getFilePathAndName () const;

    //-------------------------------------------------------------------------
    QString getFileName () const;

    //-------------------------------------------------------------------------
    QSharedPointer<SignalVisualisationModel> getMainVisualisationModel ();

    //-------------------------------------------------------------------------
    void setMainVisualisationModel (QSharedPointer<SignalVisualisationModel> signal_vis_model);

    //-------------------------------------------------------------------------
    QSharedPointer<EventManager> getEventManager ();

    //-------------------------------------------------------------------------
    QSharedPointer<ChannelManager> getChannelManager ();

    //-------------------------------------------------------------------------
    QSharedPointer<BasicHeader> getHeader () {return basic_header_;}

    //-------------------------------------------------------------------------
    FileState getState () const;

signals:
    //-------------------------------------------------------------------------
    void stateChanged (FileState state);

    //-------------------------------------------------------------------------
    void fileNameChanged (QString const& file_path_and_name);

public slots:
    //-------------------------------------------------------------------------
    void setState (FileState state);

    //-------------------------------------------------------------------------
    void setAsChanged ();

private:
    //-------------------------------------------------------------------------
    // disabled
    FileContext (FileContext const&);
    FileContext& operator= (FileContext const&);

    FileState state_;
    QString file_path_and_name_;
    QSharedPointer<EventManager> event_manager_;
    QSharedPointer<ChannelManager> channel_manager_;
    QSharedPointer<BasicHeader> basic_header_;
    QSharedPointer<SignalVisualisationModel> main_signal_vis_model_;
};

} // namespace BioSig_

#endif // FILE_CONTEXT_H