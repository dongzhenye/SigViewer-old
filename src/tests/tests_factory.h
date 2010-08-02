#ifndef TESTS_FACTORY_H
#define TESTS_FACTORY_H

#include "test.h"
#include "event_manager_tests.h"
#include "data_block_tests.h"
#include "editing_commands_tests.h"
#include "gui_tests.h"

#include <QSharedPointer>
#include <QList>

namespace SigViewer_
{

namespace TestsFactory_
{

//-----------------------------------------------------------------------------
QList<QSharedPointer<Tests_::Test> > allTests ()
{
    QList<QSharedPointer<Tests_::Test> > tests;
    tests.append (QSharedPointer<Tests_::Test>(new Tests_::DataBlockTests));
    tests.append (QSharedPointer<Tests_::Test>(new Tests_::EventManagerTests));
    tests.append (QSharedPointer<Tests_::Test>(new Tests_::EditingCommandsTests));
    tests.append (QSharedPointer<Tests_::Test>(new Tests_::GuiTests));

    return tests;
}

}

}

#endif // TESTS_FACTORY_H