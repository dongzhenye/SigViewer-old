#include "convert_file_command.h"

#include "open_file_command.h"
#include "file_handling/file_signal_writer_factory.h"
#include "application_context_impl.h"
#include "gui/progress_bar.h"

namespace SigViewer_
{

//-----------------------------------------------------------------------------
QString ConvertFileCommand::execute ()
{
    OpenFileCommand open_file_command (source_file_path_);
    QString open_error = open_file_command.execute ();
    if (open_error.size ())
        return open_error;

    QSharedPointer<FileSignalWriter> writer = FileSignalWriterFactory::getInstance()->getHandler (destination_file_path_);
    ProgressBar::instance().initAndShow (ApplicationContextImpl::getInstance()->getCurrentFileContext()->getChannelManager()->getNumberSamples() +
                              ApplicationContextImpl::getInstance()->getCurrentFileContext()->getEventManager()->getNumberOfEvents(), "Converting",
                              ApplicationContextImpl::getInstance());
    QString result = writer->save (ApplicationContextImpl::getInstance()->getCurrentFileContext());
    ProgressBar::instance().close ();

    return result;
}

}