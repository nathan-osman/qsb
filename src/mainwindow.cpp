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

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QStyle>
#include <QToolBar>

#include "mainwindow.hpp"

MainWindow::MainWindow()
{
    initUi();

    setWindowIcon(QPixmap(":/images/logo.png"));
    setWindowTitle("QSoundBoard");
    resize(1024, 768);
}

void MainWindow::open()
{
    //...
}

void MainWindow::save()
{
    //...
}

void MainWindow::saveAs()
{
    //...
}

void MainWindow::about()
{
    QMessageBox::about(
        this,
        "About QSoundBoard",
        "QSoundBoard version 1.0\nCopyright 2026 - Nathan Osman"
    );
}

void MainWindow::initUi()
{
    QAction *actionOpen = new QAction("&Open...", this);
    actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::open);

    QAction *actionSave = new QAction("&Save", this);
    actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    actionSave->setShortcut(QKeySequence::Save);
    connect(actionSave, &QAction::triggered, this, &MainWindow::save);

    QAction *actionSaveAs = new QAction("S&ave As...", this);
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);

    QAction *actionQuit = new QAction("&Quit", this);
    actionQuit->setShortcut(QKeySequence::Quit);
    connect(actionQuit, &QAction::triggered, this, &QWidget::close);

    QAction *actionAbout = new QAction("&About", this);
    connect(actionAbout, &QAction::triggered, this, &MainWindow::about);

    QMenu *menuFile = menuBar()->addMenu("&File");
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);

    QMenu *menuHelp = menuBar()->addMenu("&Help");
    menuHelp->addAction(actionAbout);

    QToolBar *toolBar = addToolBar("Main Toolbar");
    toolBar->setMovable(false);
    toolBar->addAction(actionOpen);
    toolBar->addAction(actionSave);
}