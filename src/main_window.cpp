/*  main_window.cpp

    Copyright (C) Thomas Brunner
                  Clemens Brunner
    		      Christoph Eibel   
    		      Alois Schloegl
    		      Oliver Terbu
    This file is part of the "SigViewer" repository
    at http://biosig.sf.net/

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

// main_window.cpp

#include "main_window.h"
#include "application_context.h"
#include "gui/gui_action_factory.h"
#include "gui_impl/open_file_gui_command.h"

#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>
#include <QValidator>
#include <QStringList>
#include <QActionGroup>
#include <QCloseEvent>
#include <QToolBar>
#include <QMenuBar>
#include <QLineEdit>
#include <QTextStream>
#include <QStatusBar>
#include <QLabel>
#include <QSettings>

#include <QUrl>

namespace BioSig_
{

//----------------------------------------------------------------------------
MainWindow::MainWindow (QSharedPointer<MainWindowModelImpl> model)
 : QMainWindow(0),
   model_ (model)
{
    setWindowTitle (tr("SigViewer"));
    setAcceptDrops (true);
    setWindowIcon(QIcon(":images/sigviewer16.png"));
    initStatusBar();
    initToolBars();
    initMenus();
    resize (800, 600);
}

//-----------------------------------------------------------------------------
void MainWindow::initStatusBar()
{
    QStatusBar* status_bar = statusBar();
    status_bar->showMessage(tr("Ready"));

    status_bar_signal_length_label_ = new QLabel(tr("Length: %1 s").arg("----.-"));
    status_bar_nr_channels_label_ = new QLabel(tr("Channels: %1").arg("---"));
    status_bar_signal_length_label_->setAlignment(Qt::AlignHCenter);
    status_bar_nr_channels_label_->setAlignment(Qt::AlignHCenter);
    status_bar_signal_length_label_->setMinimumWidth(status_bar_signal_length_label_->sizeHint().width() + 10);
    status_bar_nr_channels_label_->setMinimumWidth(status_bar_nr_channels_label_->sizeHint().width() + 10);
    status_bar->addPermanentWidget(status_bar_signal_length_label_);
    status_bar->addPermanentWidget(status_bar_nr_channels_label_);
}

//-----------------------------------------------------------------------------
void MainWindow::initToolBars()
{
    view_toolbar_views_menu_ = new QMenu (tr("Toolbars"), this);

    file_toolbar_ = addToolBar(tr("File"));
    view_toolbar_views_menu_->addAction (file_toolbar_->toggleViewAction());
    file_toolbar_->addAction (action("Open..."));
    file_toolbar_->addAction (action("Save"));
    file_toolbar_->addAction (action("Info..."));
    file_toolbar_->addAction (action("Undo"));
    file_toolbar_->addAction (action("Redo"));
    file_toolbar_->addAction (action("Close"));

    mouse_mode_toolbar_ = addToolBar(tr("Mode"));
    view_toolbar_views_menu_->addAction (mouse_mode_toolbar_->toggleViewAction());
    mouse_mode_toolbar_->addActions (GuiActionFactory::getInstance()->getQActions("Mouse Modes"));

    edit_toolbar_ = addToolBar(tr("Edit"));
    view_toolbar_views_menu_->addAction (edit_toolbar_->toggleViewAction());

    view_toolbar_ = addToolBar(tr("View"));
    view_toolbar_views_menu_->addAction (view_toolbar_->toggleViewAction());
    view_toolbar_->addAction(GuiActionFactory::getInstance()->getQAction("Events..."));
    view_toolbar_->addAction(GuiActionFactory::getInstance()->getQAction("Channels..."));
    view_toolbar_->addAction(action("Auto Scale All"));

    navigation_toolbar_ = addToolBar(tr("Navigation"));
    view_toolbar_views_menu_->addAction (navigation_toolbar_->toggleViewAction());
    navigation_toolbar_->setIconSize(QSize(22, 22));
    navigation_toolbar_->addActions (GuiActionFactory::getInstance()->getQActions("Zooming"));

    view_toolbar_views_menu_->addSeparator ();
    toggle_all_toolbars_ = new QAction (tr("Hide all Toolbars"), this);
    connect (toggle_all_toolbars_, SIGNAL(triggered()), SLOT(toggleAllToolbars()));
    toggle_all_toolbars_->setData (true);
    view_toolbar_views_menu_->addAction (toggle_all_toolbars_);
}

//-------------------------------------------------------------------
void MainWindow::toggleStatusBar (bool visible)
{
    statusBar()->setVisible (visible);
}

//-------------------------------------------------------------------
void MainWindow::toggleAllToolbars ()
{
    if (toggle_all_toolbars_->data().toBool())
    {
        toggle_all_toolbars_->setData (false);
        toggle_all_toolbars_->setText(tr("Show all Toolbars"));
        foreach (QAction* toggle_action, view_toolbar_views_menu_->actions())
            if (toggle_action->isCheckable() && toggle_action->isChecked())
                toggle_action->trigger ();
    }
    else
    {
        toggle_all_toolbars_->setData (true);
        toggle_all_toolbars_->setText(tr("Hide all Toolbars"));
        foreach (QAction* toggle_action, view_toolbar_views_menu_->actions())
            if (toggle_action->isCheckable() && !toggle_action->isChecked())
                toggle_action->trigger ();
    }
}



//-----------------------------------------------------------------------------
void MainWindow::initMenus()
{
    file_recent_files_menu_ = new QMenu(tr("Open &Recent"), this);
    connect(file_recent_files_menu_, SIGNAL(aboutToShow()),
            model_.data(), SLOT(recentFileMenuAboutToShow()));
    connect(file_recent_files_menu_, SIGNAL(triggered(QAction*)),
            model_.data(), SLOT(recentFileActivated(QAction*)));

    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(action("Open..."));
    file_menu_->addMenu (file_recent_files_menu_);
    file_menu_->addAction (action("Save"));
    file_menu_->addAction (action("Save as..."));
    file_menu_->addSeparator ();
    file_menu_->addAction (action("Export to GDF..."));
    file_menu_->addAction (action("Export Events..."));
    file_menu_->addAction (action("Import Events..."));
    file_menu_->addSeparator ();
    file_menu_->addAction (action("Info..."));
    file_menu_->addSeparator ();
    file_menu_->addAction (action("Close"));
    file_menu_->addSeparator ();
    file_menu_->addAction (action("Exit"));

    edit_menu_ = menuBar()->addMenu(tr("&Edit"));
    edit_menu_->addAction (action("Undo"));
    edit_menu_->addAction (action("Redo"));
    edit_menu_->addSeparator ();
    edit_menu_->addAction (action("To all Channels"));
    edit_menu_->addAction (action("Copy to Channels..."));
    edit_menu_->addAction (action("Delete"));
    edit_menu_->addAction (action("Change Channel..."));
    edit_menu_->addAction (action("Change Type..."));
    edit_menu_->addSeparator ();
    edit_menu_->addAction (action("Insert Over"));
    edit_menu_->addSeparator ();
    edit_menu_->addAction (action("Event Table..."));

    mouse_mode_menu_ = menuBar()->addMenu(tr("&Mode"));
    mouse_mode_menu_->addActions (GuiActionFactory::getInstance()->getQActions("Mouse Modes"));


    QAction* toggle_status_bar = new QAction (tr("Statusbar"), this);
    toggle_status_bar->setCheckable (true);
    toggle_status_bar->setChecked (true);
    connect (toggle_status_bar, SIGNAL(toggled(bool)), this, SLOT(toggleStatusBar(bool)));

    view_menu_ = menuBar()->addMenu(tr("&View"));
    view_menu_->addMenu (view_toolbar_views_menu_);
    view_menu_->addAction(toggle_status_bar);
    view_menu_->addSeparator();
    view_menu_->addAction(action("Channels..."));
    view_menu_->addAction(action("Events..."));
    view_menu_->addSeparator();
    view_menu_->addAction(action("Zoom In Vertical"));
    view_menu_->addAction(action("Zoom Out Vertical"));
    view_menu_->addAction(action("Zoom In Horizontal"));
    view_menu_->addAction(action("Zoom Out Horizontal"));
    view_menu_->addSeparator();
    view_menu_->addAction(action("Go to..."));
    view_menu_->addSeparator();
    view_menu_->addAction(action("Goto and Select Next Event"));
    view_menu_->addAction(action("Goto and Select Previous Event"));
    view_menu_->addSeparator();
    view_menu_->addAction(action("Fit View to Selected Event"));
    view_menu_->addAction(action("Hide Events of other Type"));
    view_menu_->addAction(action("Show all Events"));

    tools_menu_ = menuBar()->addMenu(tr("&Tools"));
    tools_menu_->addActions(GuiActionFactory::getInstance()->getQActions("Signal Processing"));

    options_menu_ = menuBar()->addMenu(tr("&Options"));
    options_menu_->addAction (action("Set Event Creation Type..."));

    help_menu_ = menuBar()->addMenu(tr("&Help"));
    help_menu_->addAction (action("About"));
}

//-----------------------------------------------------------------------------
void MainWindow::closeEvent (QCloseEvent* event)
{
    GuiActionFactory::getInstance()->getQAction("Exit")->trigger();
    event->ignore ();
}

//-----------------------------------------------------------------------------
void MainWindow::dropEvent (QDropEvent* event)
{
    if (event->mimeData()->hasText())
    {
        QUrl url (event->mimeData()->text());
        event->acceptProposedAction();
        OpenFileGuiCommand::openFile (url.path());
    } else if (event->mimeData()->hasUrls())
    {
        QUrl url (event->mimeData()->urls().first().toLocalFile());
        event->acceptProposedAction();
        OpenFileGuiCommand::openFile (url.path());
    }
}

//-----------------------------------------------------------------------------
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText() || event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

//-----------------------------------------------------------------------------
bool MainWindow::showFileCloseDialog(const QString& file_name)
{
    int res;
    res =  QMessageBox::question(this, tr("Really close?"),
                tr("Changes in '%1' are not saved!!").arg(file_name) + "\n" +
                tr("Really close?"), tr("Yes"),  tr("No"));
    return (res == 0);
}

//-----------------------------------------------------------------------------
void MainWindow::setRecentFiles(const QStringList& recent_file_list)
{
    file_recent_files_menu_->clear();
    for (QStringList::const_iterator it = recent_file_list.begin();
         it != recent_file_list.end();
         it++)
    {
        file_recent_files_menu_->addAction(*it);
    }
}

//-----------------------------------------------------------------------------
void MainWindow::setStatusBarSignalLength(float64 length)
{
    QString tmp = length < 0 ? "----.-" : QString("%1").arg(length, 0, 'f', 1);
    status_bar_signal_length_label_->setText(tr("Length: %1 s").arg(tmp));
    status_bar_signal_length_label_->setMinimumWidth(status_bar_signal_length_label_->sizeHint().width() + 10);
}

//-----------------------------------------------------------------------------
void MainWindow::setStatusBarNrChannels(int32 nr_channels)
{
    QString tmp = nr_channels < 0 ? "---" : QString("%1").arg(nr_channels, 3);
    status_bar_nr_channels_label_->setText(tr("Channels: %1").arg(tmp));
    status_bar_nr_channels_label_->setMinimumWidth(status_bar_nr_channels_label_->sizeHint().width() + 10);
}

//-----------------------------------------------------------------------------
QAction* MainWindow::action (QString const& action_id)
{
    return GuiActionFactory::getInstance()->getQAction (action_id);
}

} // namespace BioSig_
