// copy_event_dialog.h

#ifndef COPY_EVENT_DIALOG_H
#define COPY_EVENT_DIALOG_H

#include "base/user_types.h"

#include <QDialog>

class QListWidget;
class QPushButton;

namespace BioSig_
{

    // copy event dialog
class CopyEventDialog : public QDialog
{
    Q_OBJECT
public:
    CopyEventDialog (QWidget* parent = 0);

    void loadSettings();
    void saveSettings();

    void addSelectableChannel(uint32 channel_nr, QString const& channel_label);
    bool isSelected(uint32 channel_nr);

private:
    // not allowed
    CopyEventDialog();
    CopyEventDialog(const CopyEventDialog&);
    const CopyEventDialog& operator=(const CopyEventDialog&);

    QListWidget* channel_list_widget_;
    QPushButton* ok_button_;
    QPushButton* cancel_button_;
};

} // namespace BioSig_

#endif
