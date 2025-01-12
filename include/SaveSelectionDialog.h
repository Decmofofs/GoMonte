//
// Created by Decmo on 25-1-12.
//

#ifndef SAVESELECTIONDIALOG_H
#define SAVESELECTIONDIALOG_H
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
class SaveSelectionDialog : public QDialog {
    Q_OBJECT
public:
    explicit SaveSelectionDialog(const QString & save_dir, QWidget * parent = nullptr);

    QString get_selected_file() const;

public slots:
    void handle_item_double_click(QListWidgetItem * item);

private:
    QListWidget * save_list_widget;
    QString selected_file;
    QString save_dir;
    QVBoxLayout * main_layout;
};
#endif //SAVESELECTIONDIALOG_H
