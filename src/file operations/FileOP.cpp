//
// Created by Decmo on 25-1-9.
//
#include <FileOP.h>

FileOPWorker::FileOPWorker(QObject *parent) : QObject(parent) {

}

void FileOPWorker::load_game(const QString& file_path) {
    GameSaver saved_game;
    int success = saved_game.load_from_file(file_path);
    emit finish_load(success, saved_game);
}

void FileOPWorker::save_game(const QString& file_path, const GameSaver & saved_game) {
    int success = saved_game.save_to_file(file_path);
    emit finish_save(success);
}