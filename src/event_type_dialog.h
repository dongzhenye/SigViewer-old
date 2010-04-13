// event_type_dialog.h

#ifndef EVENT_TYPE_DIALOG_H
#define EVENT_TYPE_DIALOG_H

#include "base/user_types.h"

#include <QList>
#include <qdialog.h>

class QTreeWidget;
class QPushButton;
class QTreeWidgetItem;

namespace BioSig_
{

// basic header info dialog
class EventTypeDialog : public QDialog
{
    Q_OBJECT

public:

    typedef QList<uint16> IntList;

    EventTypeDialog(const QString& caption,
                    QWidget* parent = 0);

    void loadSettings();
    void saveSettings();
    void storeColors();
    void setShownTypes(IntList& shown_type_set, const bool all = false);
    void getShownTypes(IntList& shown_type_set);
    bool isAllSelected() const;
    void selectAll();

private slots:

    void itemClicked(QTreeWidgetItem* item, int column);
    void itemChanged(QTreeWidgetItem* item ,int column);

private:

    // not allowed
    EventTypeDialog();
    EventTypeDialog(const EventTypeDialog&);
    const EventTypeDialog& operator=(const EventTypeDialog&);
    void buildTree();

    QTreeWidget* event_tree_widget_;
    QPushButton* ok_button_;
    QPushButton* cancel_button_;
};

} // namespace BioSig_



#endif

