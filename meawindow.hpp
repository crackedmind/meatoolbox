#pragma once

class MEAWindow : public QMainWindow
{
    Q_OBJECT
    QTreeWidget *fileTree_;
    QTreeWidgetItem *fileTreeRoot_;
    QGridLayout *layout_;
public:
    explicit MEAWindow(QWidget *parent = nullptr);

    QString detect_meadir();
signals:

public slots:
    void open_directory();
    void open_file();
    void open_sbfile();
    void save_json();
};
