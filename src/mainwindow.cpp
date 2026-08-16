/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QToolBar>

#include "mainwindow.hpp"
#include "pixmaps.hpp"

MainWindow::MainWindow()
    : mActionSave("&Save")
{
    initUi();
    initDocument();

    setWindowIcon(QPixmap(":/images/logo.png"));
    resize(1024, 768);
}

void MainWindow::onNew()
{
    if (cannotClose()) {
        return;
    }

    mWidgetCategories->deleteLater();
    initDocument();
}

void MainWindow::onOpen()
{
    if (cannotClose()) {
        return;
    }

    QString filename = QFileDialog::getOpenFileName(
        this,
        "Open",
        QString(),
        "QSoundBoard projects (*.qsb)"
    );
    if (!filename.isNull()) {
        if (loadDocument(filename)) {
            setFilename(filename);
        }
    }
}

void MainWindow::onSave()
{
    if (mFilename.isNull()) {
        onSaveAs();
        return;
    }

    if (saveDocument(mFilename)) {
        mActionSave.setEnabled(false);
    }
}

void MainWindow::onSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save As",
        QString(),
        "QSoundBoard projects (*.qsb)"
    );
    if (!filename.isNull()) {
        if (saveDocument(filename)) {
            mActionSave.setEnabled(false);
            setFilename(filename);
        }
    }
}

void MainWindow::onAbout()
{
    QMessageBox::about(
        this,
        "About QSoundBoard",
        "QSoundBoard version 1.0\nCopyright 2026 - Nathan Osman"
        );
}

void MainWindow::onCategoryRemove()
{
    if (QMessageBox::question(
            this,
            "Confirm",
            "Are you sure you want to delete this category and all of its clips?"
        ) == QMessageBox::No) {
        return;
    }

    Category *category = dynamic_cast<Category *>(sender());
    mLayoutCategories->removeWidget(category);
    category->deleteLater();
    onMarkDirty();
}

void MainWindow::onNewCategory()
{
    newCategory();
    onMarkDirty();
}

void MainWindow::onMarkDirty()
{
    mActionSave.setEnabled(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (cannotClose()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::initUi()
{
    QAction *actionNew = new QAction("&New", this);
    actionNew->setIcon(getPixmapNew());
    actionNew->setShortcut(QKeySequence::New);
    connect(actionNew, &QAction::triggered, this, &MainWindow::onNew);

    QAction *actionOpen = new QAction("&Open...", this);
    actionOpen->setIcon(getPixmapOpen());
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onOpen);

    mActionSave.setIcon(getPixmapSave());
    mActionSave.setShortcut(QKeySequence::Save);
    connect(&mActionSave, &QAction::triggered, this, &MainWindow::onSave);

    QAction *actionSaveAs = new QAction("S&ave As...", this);
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAs);

    QAction *actionQuit = new QAction("&Quit", this);
    actionQuit->setShortcut(QKeySequence::Quit);
    connect(actionQuit, &QAction::triggered, this, &QWidget::close);

    QAction *actionAbout = new QAction("&About", this);
    connect(actionAbout, &QAction::triggered, this, &MainWindow::onAbout);

    QMenu *menuFile = menuBar()->addMenu("&File");
    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addAction(&mActionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);

    QMenu *menuHelp = menuBar()->addMenu("&Help");
    menuHelp->addAction(actionAbout);

    QToolBar *toolBar = addToolBar("Main Toolbar");
    toolBar->setMovable(false);
    toolBar->addAction(actionNew);
    toolBar->addAction(actionOpen);
    toolBar->addAction(&mActionSave);

    QPushButton *buttonNew = new QPushButton("New Category", this);
    buttonNew->setIcon(getPixmapNew());
    connect(buttonNew, &QAbstractButton::clicked, this, &MainWindow::onNewCategory);

    mLayoutMain.addWidget(buttonNew);
    mLayoutMain.addStretch(1);

    QWidget *widget = new QWidget(this);
    widget->setLayout(&mLayoutMain);
    setCentralWidget(widget);
}

void MainWindow::initDocument()
{
    mWidgetCategories = new QWidget(this);
    mWidgetCategories->setAutoFillBackground(true);

    mLayoutCategories = new QHBoxLayout(mWidgetCategories);
    mLayoutCategories->setSpacing(8);

    mLayoutMain.insertWidget(0, mWidgetCategories);

    mActionSave.setEnabled(false);
    setFilename(QString());
}

bool MainWindow::loadDocument(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(
            this,
            "Error",
            QString("Error: %1").arg(file.errorString())
        );
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(
            this,
            "Error",
            QString("Error: %1").arg(error.errorString())
        );
        return false;
    }

    mWidgetCategories->deleteLater();
    initDocument();

    QJsonObject object = document.object();
    const QJsonArray categories = object.value("categories").toArray();
    for (auto i : categories) {
        newCategory()->deserialize(i.toObject());
    }

    return true;
}

bool MainWindow::saveDocument(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(
            this,
            "Error",
            QString("Error: %1").arg(file.errorString())
            );
        return false;
    }

    QJsonObject object;
    object.insert("version", 1);

    QJsonArray categories;
    for (int i = 0; i < mLayoutCategories->count(); i++) {
        auto category = dynamic_cast<Category *>(mLayoutCategories->itemAt(i)->widget());
        categories.append(category->serialize());
    }
    object.insert("categories", categories);

    QJsonDocument document(object);
    QByteArray data = document.toJson(QJsonDocument::Indented);
    file.write(data);
    file.close();

    return true;
}

Category *MainWindow::newCategory()
{
    Category *category = new Category(&mManager, this);
    connect(category, &Category::remove, this, &MainWindow::onCategoryRemove);
    connect(category, &Category::markDirty, this, &MainWindow::onMarkDirty);
    mLayoutCategories->addWidget(category);
    return category;
}

bool MainWindow::cannotClose()
{
    return mActionSave.isEnabled() && QMessageBox::question(
       this,
       "Confirm",
       "You have unsaved changes; continue?"
    ) == QMessageBox::No;
}

void MainWindow::setFilename(const QString &filename)
{
    mFilename = filename;

    if (mFilename.isNull()) {
        setWindowTitle("QSoundBar");
    } else {
        setWindowTitle(tr("QSoundBar - %1").arg(filename));
    }
}
