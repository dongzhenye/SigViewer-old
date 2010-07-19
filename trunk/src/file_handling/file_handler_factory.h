#ifndef FILE_HANDLER_FACTORY_H
#define FILE_HANDLER_FACTORY_H

#include "../base/exception.h"

#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QDebug>

#include <map>

namespace BioSig_
{

//-----------------------------------------------------------------------------
/// FileHandlerFactory
/// generic file handler factory for storing readers or writers or whatever :)
template<typename FileHandlerType>
class FileHandlerFactory
{
public:
    //-------------------------------------------------------------------------
    bool registerHandler (QString const& file_ending,
                          QSharedPointer<FileHandlerType> file_handler);

    //-------------------------------------------------------------------------
    void registerDefaultHandler (QSharedPointer<FileHandlerType> file_handler);

    //-------------------------------------------------------------------------
    QSharedPointer<FileHandlerType> getHandler (QString const& file_path);

    //-------------------------------------------------------------------------
    QStringList getAllFileEndingsWithWildcards () const;

protected :
    FileHandlerFactory () {}
    virtual ~FileHandlerFactory () {}

private:
    // not allowed
    FileHandlerFactory (const FileHandlerFactory& src);
    const FileHandlerFactory& operator=(const FileHandlerFactory& src);

    std::map<QString, QSharedPointer<FileHandlerType> > handler_map_;
    QSharedPointer<FileHandlerType> default_handler_;
    QStringList wildcard_file_endings_;
};

//-------------------------------------------------------------------------
template<typename FileHandlerType>
bool FileHandlerFactory<FileHandlerType>::registerHandler (QString const& file_ending,
                                                           QSharedPointer<FileHandlerType> file_handler)
{
    if (handler_map_.count (file_ending))
        return false;

    handler_map_[file_ending] = file_handler;
    qDebug () << "FileHandlerFactory<FileHandlerType>::registerHandler: file_ending = " << file_ending;
    wildcard_file_endings_ << QString("*.") + file_ending;
    return true;
}

//-------------------------------------------------------------------------
template<typename FileHandlerType>
void FileHandlerFactory<FileHandlerType>::registerDefaultHandler (QSharedPointer<FileHandlerType> file_handler)
{
    qDebug () << "FileHandlerFactory<FileHandlerType>::registerDefaultHandler";
    default_handler_ = file_handler;
}

//-------------------------------------------------------------------------
template<typename FileHandlerType>
QSharedPointer<FileHandlerType> FileHandlerFactory<FileHandlerType>::getHandler (QString const& file_path)
{
    QString file_ending = file_path.section('.', -1);
    qDebug () << "FACTORY " << file_ending;
    if (handler_map_.count(file_ending))
        return handler_map_[file_ending]->createInstance (file_path);
    else if (!default_handler_.isNull())
        return default_handler_->createInstance (file_path);
    else
        return QSharedPointer<FileHandlerType> (0);
}

//-------------------------------------------------------------------------
template<typename FileHandlerType>
QStringList FileHandlerFactory<FileHandlerType>::getAllFileEndingsWithWildcards () const
{
    return wildcard_file_endings_;
}

} // namespace BioSig_

#endif