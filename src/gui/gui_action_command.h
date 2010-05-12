#ifndef GUI_ACTION_COMMAND_H
#define GUI_ACTION_COMMAND_H

#include "../base/application_states.h"
#include "../base/tab_states.h"
#include "../base/file_states.h"
#include "../base/exception.h"

#include "../gui/signal_visualisation_model.h"

#include <QObject>
#include <QAction>
#include <QStringList>

namespace BioSig_
{

class ActionConnector;

class GuiActionCommand : public QObject
{
    Q_OBJECT
public:
    //-------------------------------------------------------------------------
    virtual ~GuiActionCommand () {}

    //-------------------------------------------------------------------------
    QList<QAction*> getQActions ();

    //-------------------------------------------------------------------------
    QList<QString> getActionIDs () const;

    //-------------------------------------------------------------------------
    virtual void init () = 0;

public slots:
    //-------------------------------------------------------------------------
    virtual void trigger (QString const&) {}

    //-------------------------------------------------------------------------
    void updateEnablednessToApplicationState (ApplicationState state);

    //-------------------------------------------------------------------------
    void updateEnablednessToFileState (FileState state);

    //-------------------------------------------------------------------------
    void updateEnablednessToTabSelectionState (TabSelectionState state);

    //-------------------------------------------------------------------------
    void updateEnablednessToTabEditState (TabEditState state);

    //-------------------------------------------------------------------------
    QAction* getQAction (QString const& id);

protected:
    //-------------------------------------------------------------------------
    GuiActionCommand (QStringList const& action_ids);

    //-------------------------------------------------------------------------
    void resetActionTriggerSlot (QString const& action_id, const char* slot);

    //-------------------------------------------------------------------------
    void setShortcut (QString const& action_id, QKeySequence const& key_sequence);

    //-------------------------------------------------------------------------
    void setIcon (QString const& action_id, QIcon const& icon);

    //-------------------------------------------------------------------------
    virtual void applicationStateChanged () {}

    //-------------------------------------------------------------------------
    virtual void evaluateEnabledness () {}

    //-------------------------------------------------------------------------
    QSharedPointer<SignalVisualisationModel> currentVisModel ();

    //-------------------------------------------------------------------------
    ApplicationState getApplicationState () const {return app_state_;}

    //-------------------------------------------------------------------------
    FileState getFileState () const {return file_state_;}

    //-------------------------------------------------------------------------
    TabSelectionState getTabSelectionState () const {return tab_selection_state_;}

    //-------------------------------------------------------------------------
    TabEditState getTabEditState () const {return tab_edit_state_;}

    //-------------------------------------------------------------------------
    void disableIfNoEventSelected (QStringList const &actions);

    //-------------------------------------------------------------------------
    void disableIfNoFileIsOpened (QStringList const &actions);

private:
    QMap<QString, QAction*> action_map_;
    QList<ActionConnector*> connectors_;

    ApplicationState app_state_;
    FileState file_state_;
    TabSelectionState tab_selection_state_;
    TabEditState tab_edit_state_;
};

class ActionConnector : public QObject
{
    Q_OBJECT
public:
    ActionConnector (QObject* parent, QString const& name) : QObject (parent), name_ (name) {}
public slots:
    void trigger () {emit triggered (name_);}
signals:
    void triggered (QString const& name);
private:
    QString name_;
};




}


#endif // GUI_ACTION_COMMAND_H