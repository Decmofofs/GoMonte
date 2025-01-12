//
// Created by Decmo on 25-1-12.
//
#include "SaveSelectionDialog.h"
#include <QVBoxLayout>
#include <QDir>
#include <QMessageBox>

SaveSelectionDialog::SaveSelectionDialog(const QString & save_dir, QWidget * parent) : QDialog(parent), save_dir(save_dir) {
    setWindowTitle("选择存档");
    save_list_widget = new QListWidget(this);
    selected_file = "";
    QVBoxLayout * main_layout = new QVBoxLayout(this);
    main_layout->addWidget(save_list_widget);
    setLayout(main_layout);

    QDir save_dir_obj(save_dir);
    if (!save_dir_obj.exists()) {
        QMessageBox::warning(this, "错误", "存档目录不存在！");
        return;
    }

    QStringList file_list = save_dir_obj.entryList(QStringList() << "*.json", QDir::Files);
    for (auto file : file_list) {

        save_list_widget->addItem(file.chopped(5));
    }

    connect(save_list_widget, &QListWidget::itemDoubleClicked, this, &SaveSelectionDialog::handle_item_double_click);
}

QString SaveSelectionDialog::get_selected_file() const {
    return selected_file;
}

void SaveSelectionDialog::handle_item_double_click(QListWidgetItem * item) {
    selected_file = item->text();
    selected_file += ".json";
    qInfo() << selected_file;
    accept();
}