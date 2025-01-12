//
// Created by Decmo on 25-1-9.
//

#ifndef FILEOP_H
#define FILEOP_H
#include <GameData.h>
#include <QObject>
#include "GameSaver.h"
class FileOPWorker : public QObject {
    Q_OBJECT

public:
    explicit FileOPWorker(QObject *parent = nullptr);

signals:
    void finish_load(bool success, const GameSaver & saved_game);
    void finish_save(bool success);

public slots:
    void load_game(const QString& file_path);
    void save_game(const QString& file_path, const GameSaver & saved_game);
};

#endif //FILEOP_H
