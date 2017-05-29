#include "precompiled.hpp"
#include "uberbuffer.hpp"
#include "frostbite/bson.hpp"
#include "meawindow.hpp"

MEAWindow::MEAWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(800,600);
    fileTreeRoot_ = new QTreeWidgetItem(QStringList() << "ROOT");

    fileTree_ = new QTreeWidget;

    fileTree_->setColumnCount(2);

    fileTree_->insertTopLevelItem(0, fileTreeRoot_);

    fileTreeRoot_->addChild(new QTreeWidgetItem(QStringList() << "subroot" << "ss"));
    fileTreeRoot_->setExpanded(true);

    auto fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("Open directory", this, &MEAWindow::open_directory);
    fileMenu->addAction("Open .toc file", this, &MEAWindow::open_file);
    fileMenu->addAction("Open .sb file", this, &MEAWindow::open_sbfile);

    fileMenu->addSeparator();
    fileMenu->addAction("&Quit", [](){
        qApp->quit();
    });

    auto debugMenu = menuBar()->addMenu("&Debug");
    debugMenu->addAction("Save Json dump", this, &MEAWindow::save_json);

    auto helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("&About Qt", [](){
        qApp->aboutQt();
    });

    setStatusBar(new QStatusBar());
    statusBar()->addWidget(new QProgressBar());


    fileTree_->resize(200, height());

    layout_ = new QGridLayout;
    layout_->addWidget(fileTree_);
    setLayout(layout_);
}

QString MEAWindow::detect_meadir()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\BioWare\\Mass Effect Andromeda", QSettings::NativeFormat);
    return settings.value("Install Dir").toString();
}

void MEAWindow::open_directory()
{
//    QString filename = QFileDialog::getOpenFileName(this, "Open .toc file", "F:/Origin Games/Mass Effect Andromeda", QString(), nullptr, QFileDialog::ReadOnly);
    QString meadir = detect_meadir();
    if (meadir.isEmpty()) {
       meadir = QFileDialog::getExistingDirectory(this, "Select Mass Effect: Andromeda directory");
    }

    QString filename = meadir + "\\Data\\layout.toc";

//    QByteArray f = Frostbite::decrypt_file(filename);

//    filename = filename + ".out";

//    QFile out(filename);
//    if (out.open(QIODevice::WriteOnly) ) {
//        out.write(f);
//    }

//    qDebug() << "Filesize: " << f.length();
//    UberBuffer fileBuffer(&f);
    Frostbite::TocParser tp(filename);
    tp.parse();
    auto json = tp.getBson()->getJson();
    QJsonDocument doc;

    if(json.isArray()) {
        doc.setArray(json.toArray());
    } else {
        doc.setObject(json.toObject());
    }
    QFile json_out("dump.json");
    json_out.open(QIODevice::WriteOnly);
    json_out.write(doc.toJson());
}

void MEAWindow::open_file()
{
    QString meadir = detect_meadir();
    QString filename = QFileDialog::getOpenFileName(this, "Open .toc file", meadir);

    Frostbite::TocParser tp(filename);
    tp.parse();
    auto json = tp.getBson()->getJson();
    QJsonDocument doc;

    if(json.isArray()) {
        doc.setArray(json.toArray());
    } else {
        doc.setObject(json.toObject());
    }

    QFile json_out(filename + ".json");
    json_out.open(QIODevice::WriteOnly);
    json_out.write(doc.toJson());
}

void MEAWindow::open_sbfile()
{
    QString meadir = detect_meadir();
    QString filename = QFileDialog::getOpenFileName(this, "Open .toc file", meadir);

    Frostbite::SbParser tp(filename);
    tp.parse();
    tp.getBson()->getJson();

    auto json = tp.getBson()->getJson();

    QJsonDocument doc;
    if(json.isArray()) {
        doc.setArray(json.toArray());
    } else {
        doc.setObject(json.toObject());
    }
    QFile json_out(filename+ ".json");
    json_out.open(QIODevice::WriteOnly);
    json_out.write(doc.toJson());
}

void MEAWindow::save_json()
{
//    QJsonDocument doc(obj);

//    QFile json_out("dump.json");
//    json_out.open(QIODevice::WriteOnly);
//    json_out.write(doc.toJson());
}
